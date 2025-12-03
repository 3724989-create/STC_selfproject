import serial
import time
import matplotlib.font_manager as fm
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

# 解决中文显示问题
font_name = "SimHei"
plt.rcParams['font.sans-serif'] = [font_name, 'Arial']
plt.rcParams['axes.unicode_minus'] = False

# --- 1. 配置常量 (与发送端保持一致) ---
COM_PORT = 'COM9'  # <<< 请修改为您的实际串口号！
BAUD_RATE = 115200  # <<< 请根据您的模块设置！

CCD_PIXEL_COUNT = 128
DATA_BODY_LEN = CCD_PIXEL_COUNT  # 假设发送端已经将 uint16 压缩或截断为 128个 uint8 字节

FRAME_HEADER = b'\xAA\x55'
HEADER_LEN = 2
LEN_FIELD_LEN = 2
CHECKSUM_LEN = 1
FRAME_END = b'\r\n'  # 假设帧尾是两个 0
#FRAME_END = b'\0D\0A'  # 假设帧尾是两个 0
FRAME_END_LEN = len(FRAME_END)

# 计算完整的帧长度
TOTAL_FRAME_LEN_EXPECTED = HEADER_LEN + LEN_FIELD_LEN + DATA_BODY_LEN + CHECKSUM_LEN + FRAME_END_LEN

# --- 2. 全局变量 ---
# 使用 deque 作为高效的接收缓冲区
rx_buffer = deque()
pixel_values = np.zeros(CCD_PIXEL_COUNT, dtype=np.uint8)

# --- 3. 初始化串口和绘图 ---
try:
    ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=0.05)
    print(f"成功打开串口: {COM_PORT} @ {BAUD_RATE}")
except serial.SerialException as e:
    print(f"无法打开串口 {COM_PORT}: {e}")
    # exit() # 在某些环境中，如果串口打开失败，不退出可以方便调试

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

    # 循环尝试解析所有完整的帧
    while len(rx_buffer) >= TOTAL_FRAME_LEN_EXPECTED:

        # --- A. 查找帧头 AA 55 ---
        header_index = -1
        # 优化：只在缓冲区的前 TOTAL_FRAME_LEN_EXPECTED - 1 长度内查找，避免遍历整个 deque
        max_search = min(len(rx_buffer) - 1, TOTAL_FRAME_LEN_EXPECTED * 2)
        for i in range(max_search):
            if rx_buffer[i] == FRAME_HEADER[0] and rx_buffer[i + 1] == FRAME_HEADER[1]:
                header_index = i
                break

        if header_index == -1:
            # 缓冲区数据不足或无帧头，清空并等待
            if len(rx_buffer) > TOTAL_FRAME_LEN_EXPECTED * 2:  # 缓冲区过大，说明数据混乱，清空
                rx_buffer.clear()
            break

        if header_index > 0:
            # 找到帧头，但前面有无效数据，移除无效数据
            for _ in range(header_index):
                rx_buffer.popleft()

        # 此时帧头在索引 0

        # --- B. 验证长度域 (位于索引 2 和 3) ---
        # 假设长度域是 128 (0x80)
        # 典型 Little-Endian 格式: 0x80 0x00
        if rx_buffer[2] != 0x80 or rx_buffer[3] != 0x00:
            print(f"警告: 长度域异常 ({hex(rx_buffer[2])} {hex(rx_buffer[3])})，丢弃帧头。")
            rx_buffer.popleft()  # 丢弃第一个字节，继续查找
            continue

        # --- C. 提取数据体、校验和和帧尾 ---

        # 将 deque 转换为列表切片，再转换为 bytes (比原先的列表推导式高效)
        full_frame_list = list(rx_buffer)

        # 提取 128 字节的数据体 (从索引 4 开始)
        data_start_index = HEADER_LEN + LEN_FIELD_LEN
        data_body_bytes = bytes(full_frame_list[data_start_index: data_start_index + DATA_BODY_LEN])

        # 提取校验和 (位于数据体之后)
        checksum_index = data_start_index + DATA_BODY_LEN
        received_checksum = full_frame_list[checksum_index]

        # 提取帧尾 (位于校验和之后)
        frame_end_received = bytes(
            full_frame_list[checksum_index + CHECKSUM_LEN: checksum_index + CHECKSUM_LEN + FRAME_END_LEN])

        # --- D. 校验和和帧尾验证 ---
        calculated_checksum = calculate_checksum(data_body_bytes)

        if calculated_checksum == received_checksum and frame_end_received == FRAME_END:
            # 校验成功：解析像素值

            # 将 bytes 转换成 numpy 数组 (dtype=np.uint8)
            new_pixels = np.frombuffer(data_body_bytes, dtype=np.uint8)

            if len(new_pixels) == CCD_PIXEL_COUNT:
                pixel_values[:] = new_pixels[:]  # 更新全局数组
                # print(f"帧解析成功! Checksum: {hex(received_checksum)}")
            else:
                # 理论上不会发生，因为我们已经验证了总长度
                print("错误: 像素数量不匹配。")

        else:
            print(
                f"校验失败! 接收CS: {hex(received_checksum)}, 计算CS: {hex(calculated_checksum)}. 帧尾:{frame_end_received}")

        # --- E. 移除已处理的完整帧 ---
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