import serial
import time
import matplotlib.font_manager as fm
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque
font_name = "SimHei" # 或者 'Microsoft YaHei'

# 步骤 2: 设置字体属性
plt.rcParams['font.sans-serif'] = [font_name, 'Arial'] # 允许 Matplotlib 查找这些字体
plt.rcParams['axes.unicode_minus'] = False # 解决负号显示问题
# --- 1. 配置常量 (与发送端保持一致) ---
COM_PORT = 'COM9'  # <<< 请修改为您的实际串口号！
BAUD_RATE = 115200  # <<< 请根据您的模块设置！
CCD_PIXEL_COUNT = 128
DATA_BODY_LEN = CCD_PIXEL_COUNT  # 128个 uint8 字节
FRAME_HEADER = b'\xAA\x55'
HEADER_LEN = 2
LEN_FIELD_LEN = 2
CHECKSUM_LEN = 1
FRAME_END_LEN = 2
TOTAL_FRAME_LEN_EXPECTED = HEADER_LEN + LEN_FIELD_LEN + DATA_BODY_LEN + CHECKSUM_LEN + FRAME_END_LEN

# --- 2. 全局变量 ---
# 使用 deque 作为高效的接收缓冲区
rx_buffer = deque()
pixel_values = np.zeros(CCD_PIXEL_COUNT, dtype=np.uint8)

# --- 3. 初始化串口和绘图 ---
try:
    ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=0.05)  # 降低超时时间，提高响应速度
    print(f"成功打开串口: {COM_PORT} @ {BAUD_RATE}")
except serial.SerialException as e:
    print(f"无法打开串口 {COM_PORT}: {e}")
    exit()


# 配置 Matplotlib 实时绘图
plt.style.use('dark_background')
fig, ax = plt.subplots(figsize=(8, 6))
line, = ax.plot(np.arange(CCD_PIXEL_COUNT), pixel_values, color='#00FFCC', linewidth=2)
ax.set_title("CCD 线性扫描实时波形", color='white')
ax.set_xlabel("像素索引")
ax.set_ylabel("像素值 (0 - 255)")
ax.set_ylim(0, 255)  # 8位数据的范围
ax.grid(True, alpha=0.3)
fig.tight_layout()


# 简单的校验和函数 (与C端保持一致)
def calculate_checksum(data):
    """计算累加和的最低8位"""
    return sum(data) & 0xFF


# --- 4. 数据解析函数 ---
def process_data(new_data):
    """将新数据添加到缓冲区，并尝试解析完整的帧。"""
    global pixel_values

    # 将新数据添加到缓冲区
    rx_buffer.extend(new_data)

    while len(rx_buffer) >= TOTAL_FRAME_LEN_EXPECTED:

        # A. 查找帧头 AA 55
        try:
            # 查找帧头在 deque 中的位置
            header_index = -1
            for i in range(len(rx_buffer) - 1):
                if rx_buffer[i] == FRAME_HEADER[0] and rx_buffer[i + 1] == FRAME_HEADER[1]:
                    header_index = i
                    break
        except IndexError:
            # 缓冲区太短，无法继续查找
            break

        if header_index == -1:
            # 没有找到帧头，清空缓冲区等待下一帧
            rx_buffer.clear()
            break

        if header_index > 0:
            # 找到帧头，但前面有无效数据，移除无效数据
            for _ in range(header_index):
                rx_buffer.popleft()

        # 此时帧头在索引 0

        # B. 验证长度域 (只需要检查它是否是 0x80 0x00，因为我们是固定长度)
        # 长度域位于索引 2 和 3
        if rx_buffer[2] != 0x80 or rx_buffer[3] != 0x00:
            print(f"警告: 长度域异常 ({hex(rx_buffer[2])} {hex(rx_buffer[3])})，丢弃帧头。")
            rx_buffer.popleft()  # 丢弃第一个字节，继续查找
            continue

        # C. 提取数据体和校验和

        # 提取 128 字节的数据体
        data_body = bytes([rx_buffer[i] for i in range(4, 4 + DATA_BODY_LEN)])

        # 提取校验和 (位于索引 4 + 128 = 132)
        received_checksum = rx_buffer[4 + DATA_BODY_LEN]

        # 提取帧尾
        # frame_end = rx_buffer[TOTAL_FRAME_LEN_EXPECTED - 2: TOTAL_FRAME_LEN_EXPECTED]

        # D. 校验和验证
        calculated_checksum = calculate_checksum(data_body)

        if calculated_checksum == received_checksum:
            # 校验成功：解析像素值

            # 由于是 uint8，无需字节序转换，直接转换成 numpy 数组
            new_pixels = np.frombuffer(data_body, dtype=np.uint8)

            if len(new_pixels) == CCD_PIXEL_COUNT:
                # 1. 定义映射值
                BACKGROUND_Y = 150  # 对应 0 (背景)
                FOREGROUND_Y = 50  # 对应 1 (赛道)
                if np.max(new_pixels) > 1 and np.max(new_pixels) <= 255:
                    # 接收到的已经是映射后的数据 (50/150 或原始波形)
                    pixel_values[:] = new_pixels[:]
                else:
                    # 接收到的是纯粹的 0 或 1 二值化数据，需要映射：
                    # 映射逻辑：将所有等于 1 的像素点设置为 50，其余设置为 150
                    mapped_pixels = np.where(new_pixels == 1, FOREGROUND_Y, BACKGROUND_Y)
                    pixel_values[:] = mapped_pixels[:]  # 更新全局数组
                # print(f"帧解析成功! Checksum: {hex(received_checksum)}")
            else:
                print("错误: 像素数量不匹配。")

        else:
            print(f"校验失败! 接收CS: {hex(received_checksum)}, 计算CS: {hex(calculated_checksum)}")

        # 移除已处理的完整帧 (135 字节)
        for _ in range(TOTAL_FRAME_LEN_EXPECTED):
            rx_buffer.popleft()


# --- 5. 实时更新函数 (Matplotlib Animation) ---
def update_plot(frame):
    """由 Matplotlib 循环调用，用于串口读取和重绘图形。"""
    # 1. 从串口读取数据 (非阻塞)
    if ser.is_open and ser.in_waiting:
        try:
            new_data = ser.read(ser.in_waiting)
            process_data(new_data)
        except serial.SerialException as e:
            print(f"串口读取错误: {e}")

    # 2. 更新绘图数据
    line.set_ydata(pixel_values)

    # 3. 标题显示实时状态
    ax.set_title(f"CCD 实时波形 | Max: {np.max(pixel_values)} | Min: {np.min(pixel_values)}")

    return line,


# --- 6. 运行程序 ---
try:
    # 启动 Matplotlib 动画 (interval=100ms 对应 10Hz)
    ani = animation.FuncAnimation(fig, update_plot, interval=100, blit=True)

    print("开始实时监测... (关闭窗口停止)")
    plt.show()

except KeyboardInterrupt:
    print("\n程序被中断。")
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("串口已关闭。")