#include "self_menu.h"

//格式化字符串
void LCD_Sprintf(uint8_t Line,char *format,...)
{
    char String[21];    //缓冲区存储格式化后的字符串
    va_list arg;
    //设置不定变量
    va_start(arg,format);
    //绑定不定变量
    vsprintf(String,format,arg);
    //将不定变量组合致string参数列表
    va_end(arg);
    //结束不定变量定义
    LCD_DisplayStringLine(Line,(u8*)String);
    //绑定LCD显示函数
}