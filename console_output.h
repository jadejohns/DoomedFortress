//������ c �������� Windows
#ifndef CONSOLE_OUTPUT_H
#define CONSOLE_OUTPUT_H

//���������� ����������
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

//���������� ����� (����� � MSDN)
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
��� ������ Windows ���� 8.1 �� �������� �������� ��������� ����� 
(������������ �� read_char � ���������� �������� � ������ ������ ������� Windows (�� XP �� 2008R2) �� ��������
*/

//������������� ����
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

//��������� �������
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x; coord.Y = y;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, coord);
}

//�������������� ������ � ������ � �������
void set_color(WORD color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole,color);
	return;
}

//��������� �������� ����� ����� ������ � ����, ����� �� �������
void text_color (int txtColor,int bckColor)
{
	int color=16*bckColor+txtColor;
	set_color(color);
}

//������� ������ (������ � MSDN)
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

//������� ��������� ��������� ���� (��������)
void frame(unsigned x1,unsigned y1,unsigned x2,unsigned y2,char txt[])
{  
		//��� ���� ���������� �������������� �����������
		unsigned i,j,k; 
		k=(x2-x1);                       
		j=k/8;                          
		j-=1;
		//������� ����� ����  
		gotoxy(x1,y1);
		putchar(0xC9);
		//������� ������ ����
		gotoxy(x2,y1);
		putchar(0xBB);
		//printf("�");
		//������ ����� ����      
		gotoxy(x1,y2);
		putchar(0xC8);
		//������ ������ ����      
		gotoxy(x2,y2);
		putchar(0xBC);
			//������ �������������� �������  
			for (i=x1+1;i<x2;i++)
			   {
				  gotoxy(i,y1);
				  putchar(0xCD);    
				  gotoxy(i,y2);
				  putchar(0xCD);    
			   }
			//������ ������������ �������
			for (i=y1+1;i<y2;i++)
			   {
				  gotoxy(x1,i);
				  putchar(0xBA);      
				  gotoxy(x2,i);
				  putchar(0xBA);
			   }
			//����� ������
				  gotoxy(x1+j,y1);
				  printf(txt); 

	
}

//������� ����� ������ (��������)
void frame_fill(unsigned x1,unsigned y1,unsigned x2,unsigned y2,unsigned col)
{
	int x,y;
	//���������� ����
	set_color(col);  
	//������� �� Y                     
	for (y=y1;y<y2;y++)                    
		{	//������� �� X
			for (x=x1;x<x2;x++)               
				{
				  //�� ���� ����������� ��������� ������ �������		
				  gotoxy(x,y);
				  printf(" ");     
				}
		}
}

//������� ��������� ������� ���� 
void make_frame(unsigned x1,unsigned y1,unsigned x2,unsigned y2,unsigned col,char txt_[])
{
	frame_fill(x1,y1,x2,y2,col);
	frame(x1,y1,x2,y2,txt_);
}

//������� ������ � ��������� ������� (����� � WINAPI.H)
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

//C�������� ������ � ��������� �������
char char_read(int x,int y)
{
    gotoxy(x,y);
    char cr = cursor_char_get();
    return cr;
}

#endif
