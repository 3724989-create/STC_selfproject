#ifndef _SELF_MENU_H_
#define _SELF_MENU_H_

#include "bsp.h"

typedef struct MenuNode
{
    const char *menu_name;           //菜单栏名称
    void (*action_func)(void);  //按下确认执行的函数
    struct MenuNode *next;
    struct MenuNode *prev;
    //多级菜单选项
    struct MenuNode *childmenu;
    struct MenuNode *fathermenu;
    
    
}MenuNode_t;

void Menu_Display(void);
void sub_Menu_Init(void);
void Menu_Select_Action(void);
void Menu_Move_Prev(void);
void Menu_Move_Next(void);
void Menu_Init(void);
void Sub_Menu_Link_Init(void);

#endif // DEBUG