#include "self_menu.h"

#define M_MENU_DISPLAY_MAX_LINES 4
#define S_MENU_DISPLAY_MAX_LINES 2

MenuNode_t *CurrentMenu=NULL;   //全局变量，指向当前菜单指针
MenuNode_t *CurrentRoot = NULL; // 当前正在显示的菜单列表的起点（根节点）

void Menu_Display(void);

MenuNode_t MenuNode_CCD = {
    "1.CCD_scan", 
     Menu_CCD_process
     
};

MenuNode_t MenuNode_LED1 = {
    "2.open_led1", 
     LED1_process
};

MenuNode_t MenuNode_LED2 = {
    "3.open_led2", 
     LED2_process
};

MenuNode_t MenuNode_LED2_2 = {
    "1.open_led2_2", 
     LED2_process
};

MenuNode_t Sub = {
    "sub", 
     NULL
};

MenuNode_t Back = {
    "Back", 
    NULL
};


//MenuNode_t *FixedMenuOrder[]={
//    &MenuNode_CCD,
//    &MenuNode_LED1,
//    &Sub
//};

////子菜单结构体
//MenuNode_t *FixedMenuOrder2[]={
//    &MenuNode_LED2,
//    &Back
//};

void Menu_Init(void)
{
    //建立双向环形连接
// 1. 设置主菜单链表 (CCD <-> LED1 <-> LED2 <-> CCD)
    MenuNode_CCD.prev = &Sub; 
    MenuNode_CCD.next = &MenuNode_LED1;
    
    MenuNode_LED1.prev = &MenuNode_CCD;
    MenuNode_LED1.next = &MenuNode_LED2; // 修复：指向 sub
    
    MenuNode_LED2.prev = &MenuNode_LED1; // 修复：指向 LED1
    MenuNode_LED2.next = &Sub;  // 修复：指向 CCD (环形闭合)

    Sub.prev=&MenuNode_LED2;
    Sub.next=&MenuNode_CCD;

    // 设置当前菜单为起点
    CurrentMenu = &MenuNode_CCD;
    CurrentRoot=&MenuNode_CCD;
    
    Sub.childmenu = &MenuNode_LED2_2; // 子菜单

    Menu_Display();
}

void Sub_Menu_Link_Init(void)
{
    // 假设子菜单列表只有 Back 一项，形成自环
    MenuNode_LED2_2.prev=&Back;
    MenuNode_LED2_2.next=&Back;

    Back.prev = &MenuNode_LED2_2;
    Back.next = &MenuNode_LED2_2;

    // 子菜单入口：让 Sub 节点指向 Back 节点
    Sub.childmenu = &MenuNode_LED2_2;
    
    // Back 节点的父菜单入口：指向主菜单的起点
    Back.fathermenu = &MenuNode_CCD; 

    Menu_Display();
}
// 移动到下一个菜单项 (例如 KEY1)
void Menu_Move_Next(void)
{
    if (CurrentMenu != NULL) {
        CurrentMenu = CurrentMenu->next;
        Menu_Display(); // 移动后立即刷新显示
    }
}

// 移动到上一个菜单项 (例如 KEY2)
void Menu_Move_Prev(void) 
{
    if (CurrentMenu != NULL) {
        CurrentMenu = CurrentMenu->prev;
        Menu_Display(); // 移动后立即刷新显示
    }
}

// 菜单项的确认/执行动作 (例如 KEY3)
void Menu_Select_Action(void)
{
    MenuNode_t *old_root;

    
    if (CurrentMenu->childmenu!=NULL)
    {
        old_root=CurrentMenu;   //记录旧菜单节点 (用于返回)
        CurrentRoot=CurrentMenu->childmenu;
        CurrentMenu=CurrentRoot;        //当前目录变为当前根目录
       Menu_Display();
        return; //没有return直接执行Back
        
    }
    else if (CurrentMenu->fathermenu != NULL) {
            CurrentRoot = CurrentMenu->fathermenu; // 切换回父菜单的根节点
            CurrentMenu = CurrentRoot;              // 选中父菜单的第一个项
            Menu_Display();
            return; // 完成操作，退出函数
        }
    else if (CurrentMenu != NULL && CurrentMenu->action_func != NULL) {
       // 执行当前菜单项绑定的功 能函数
        CurrentMenu->action_func();
        // 动作执行完毕后，可以停顿一下，或者回到菜单
        // Menu_Display(); 
    }
}


static MenuNode_t *old_current_root;
// 菜单显示函数 (更新 LCD)
void Menu_Display(void)
{
    const unsigned int START_LINE=2;
    
    MenuNode_t *temp_node;
    const char *prefix;
    unsigned int current_line;
    unsigned int i;
    unsigned int menu_num;
    
   
    //从currentRoot开始遍历需求

    temp_node=CurrentRoot;
    current_line=START_LINE;
    
    if(temp_node==&MenuNode_CCD)
    {
       LCD_PrintfLine(1,".......Menu....");
       menu_num=M_MENU_DISPLAY_MAX_LINES;
    }
    
    else if(temp_node==&MenuNode_LED2_2)
    {
        
        menu_num=S_MENU_DISPLAY_MAX_LINES;
        LCD_PrintfLine(1,".....S.Menu....");
        
    }
    
    if(old_current_root!=CurrentRoot)
    {
        LCD_CLS(1);
    }
    
    
        for(i=0;i<menu_num;i++)
        {
            current_line=START_LINE+i;
            // // 如果当前行超过了我们预留的显示空间，就停止
            // temp_node=FixedMenuOrder[i];
            if (current_line > (START_LINE + M_MENU_DISPLAY_MAX_LINES - 1)) {
                break;
            }
            
            prefix = (temp_node == CurrentMenu) ? "-> " : "   ";

            LCD_PrintfLine(current_line,"%s%s",prefix,temp_node->menu_name);

            temp_node = temp_node->next;
        }
        
        old_current_root=CurrentRoot;

}

