//Работа c консолью Windows
#ifndef CONSOLE_OUTPUT_H
#define CONSOLE_OUTPUT_H

//Подключаем библиотеки
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

//Определяем цвета (взято с MSDN)
#define Black 0
#define Blue 1
#define Green 2
#define Cyan 3
#define Red 4
#define Magenta 5
#define Yellow 6
#define White 7
#define Gray 8
#define LightBlue 9
#define LightGreen 10
#define LightCyan 11
#define LightRed 12
#define LightMagenta 13
#define LightYellow 14
#define BrightWhite 15

/*
Для версии Windows ниже 8.1 НЕ ВКЛЮЧАТЬ ДЕФОЛТОМ РАСТРОВЫЙ ШРИФТ 
(Столкновения по read_char с растровыми шрифтами в старой версии консоли Windows (от XP до 2008R2) не работают
*/

//Инициализация окна
void win_init(int width, int height)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD coord;
    coord.X = width;
    coord.Y = height;

    SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = height - 1;
    Rect.Right = width - 1;

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      
    SetConsoleScreenBufferSize(hConsole, coord);           
    SetConsoleWindowInfo(hConsole, TRUE, &Rect);  

	CONSOLE_CURSOR_INFO info;
	info.bVisible = 0;
	info.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &info);
	
	SetConsoleTitle("THE DOOMED FORTRESS V. 0.01a");

}

//Положение курсора
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x; coord.Y = y;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, coord);
}

//Инициализируем работу с цветом в консоли
void set_color(WORD color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole,color);
	return;
}

//Установка отдельно цвета цвета текста и фона, чтобы не считать
void text_color (int txtColor,int bckColor)
{
	int color=16*bckColor+txtColor;
	set_color(color);
}

//Очистка экрана (стащил с MSDN)
void clr_scr()
{
	COORD coordScreen={0,0};
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hConsole, coordScreen);
}

//Функция рисования окантовки окна (заглушка)
void frame(unsigned x1,unsigned y1,unsigned x2,unsigned y2,char txt[])
{  
		//без этих параметров отрисовывалась неправильно
		unsigned i,j,k; 
		k=(x2-x1);                       
		j=k/8;                          
		j-=1;
		//Верхний левый угол  
		gotoxy(x1,y1);
		putchar(0xC9);
		//Верхний правый угол
		gotoxy(x2,y1);
		putchar(0xBB);
		//printf("»");
		//Нижний левый угол      
		gotoxy(x1,y2);
		putchar(0xC8);
		//Нижний правый угол      
		gotoxy(x2,y2);
		putchar(0xBC);
			//Рисуем горизонтальные границы  
			for (i=x1+1;i<x2;i++)
			   {
				  gotoxy(i,y1);
				  putchar(0xCD);    
				  gotoxy(i,y2);
				  putchar(0xCD);    
			   }
			//Рисуем вертикальные границы
			for (i=y1+1;i<y2;i++)
			   {
				  gotoxy(x1,i);
				  putchar(0xBA);      
				  gotoxy(x2,i);
				  putchar(0xBA);
			   }
			//Текст окошка
				  gotoxy(x1+j,y1);
				  printf(txt); 

	
}

//Заливка рамки цветом (заглушка)
void frame_fill(unsigned x1,unsigned y1,unsigned x2,unsigned y2,unsigned col)
{
	int x,y;
	//Определяем цвет
	set_color(col);  
	//Заливка по Y                     
	for (y=y1;y<y2;y++)                    
		{	//Заливка по X
			for (x=x1;x<x2;x++)               
				{
				  //По всем координатам вставляем символ пробела		
				  gotoxy(x,y);
				  printf(" ");     
				}
		}
}

//Готовая процедура рисовки окна 
void make_frame(unsigned x1,unsigned y1,unsigned x2,unsigned y2,unsigned col,char txt_[])
{
	frame_fill(x1,y1,x2,y2,col);
	frame(x1,y1,x2,y2,txt_);
}

//Хватаем символ в положении курсора (взято с WINAPI.H)
char cursor_char_get()
{
    char c = 0;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hconsole != INVALID_HANDLE_VALUE &&
        GetConsoleScreenBufferInfo(hconsole,&csbi))
    {
        DWORD read = 0;
        if (!ReadConsoleOutputCharacterA(hconsole,&c,1,
            csbi.dwCursorPosition,&read) || read != 1
            )
            c = 0;
    }
    return c;
}

//Cчитываем символ в положении курсора
char char_read(int x,int y)
{
    gotoxy(x,y);
    char cr = cursor_char_get();
    return cr;
}

#endif
