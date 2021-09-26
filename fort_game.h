#ifndef FORT_GAME_H
#define FORT_GAME_H

//���������� ���������
typedef struct MENU MENU;
typedef struct PLAYER PLAYER; 
typedef struct EXGATE EXGATE;
typedef struct EXKEY EXKEY;
typedef struct RED_GATE RED_GATE;
typedef struct RED_GKEY RED_GKEY;
typedef struct GREEN_GATE GREEN_GATE;
typedef struct GREEN_GKEY GREEN_GKEY;
typedef struct YELLOW_GATE YELLOW_GATE;
typedef struct YELLOW_GKEY YELLOW_GKEY;
typedef struct GOLD GOLD;
typedef struct HTRAP HTRAP;
typedef struct VTRAP VTRAP;
typedef struct RTRAP RTRAP;
typedef struct PORTAL PORTAL;
typedef struct HOLLOW_PORTAL HOLLOW_PORTAL;


/*---------------------
��������� ���������
---------------------*/
struct MENU //--����
{	
	char name[30];
	int x,y,col;
};

struct PLAYER //--�����
{
    int x,y; //--���������� �� XY
    int ex_keyholder; //--�������� ������� ����� ��� ������ � ������
    int red_keyholder; //--�������� ������� ����� �� ������� �����
	int green_keyholder; //--�������� ������� ����� �� ������� �����
    int goldvalue; //--������� ������� ������
    int moves; //--���������� ��������� �����
	char dir;//--����������� ������
	char c; //--������ ������

};

struct GOLD //--������
{
	int  x,y; //--���������� �� XY
	int txtColor,BckColor; //--��������� ����� � ����
	char c; //--C����� ������
};

struct EXGATE //--������ ������ � ������
{
	int x,y; //--���������� �� XY
	int txtColor,BckColor; //--��������� ����� � ����
	char c; //--C����� �����
};

struct EXKEY //--���� ��� ������ � ������
{	
	int x,y; //--���������� �� XY
	int txtColor,BckColor; //--��������� ����� � ����
	char c; //--������ �����
};

struct RED_GATE //--������� ������
{	
	int x,y; //--���������� �� XY
	int txtColor,BckColor; //--��������� ����� � ����
	char c; //--������ ������� �����
};

struct RED_GKEY //--���� �� ������� �����
{	
	int x,y; //--���������� �� XY
	int txtColor,BckColor; //--��������� ����� � ����
	char c; //--������ �����
};

struct GREEN_GATE //--������� ������
{	
	int x,y; //--���������� �� XY
	int txtColor,BckColor; //--��������� ����� � ����
	char c; //--������ ������� �����
};

struct GREEN_GKEY //--���� �� ������� �����
{	
	int x,y; //--���������� �� XY
	int txtColor,BckColor; //--��������� ����� � ����
	char c; //--������ �����
};

struct HTRAP //--�������������� �������
{
	int x,y;	//--���������� �� XY
	int bcheck; //--�������� �������
	int txtColor,BckColor; //--��������� ����� � ����
	char c; //-- ������
};

struct VTRAP //--������������ �������
{
	int x,y;	//--���������� �� XY
	int bcheck; //--�������� �������
	int txtColor,BckColor; //--��������� ����� � ����
	char c; //-- ������
};

struct RTRAP //--�������� ����������� �������
{
	int x,y; //--��������� �� XY
	int minx,miny; //--����������� ����� 
	int maxx,maxy; //--������������ �����
	int speed; //--��������
	int movecheck; //--�������� ��������
	int txtColor,BckColor; //--��������� ����� � ����
	char c; //--������
};

struct PORTAL //--������ � ���� �����
{
	int inx,iny; //--���������� ����� �� XY
	int outx,outy;//--���������� ������ �� XY
	int intxtColor,inBckColor; //--��������� ����� � ���� ��� �����
	int outtxtColor,outBckColor; //--��������� ����� � ���� ��� ������
	char inc,outc; //--������� ����� � ������
};

struct HOLLOW_PORTAL //--�������� ������ (������ ��� ������)
{
	int inx,iny; //--��������� ����� �� XY
	int outx,outy;//--���������� ������ �� XY
	int intxtColor,inBckColor; //--��������� ����� � ���� �����
	int outtxtColor,outBckColor; //--��������� ����� � ���� ������
	char inc,outc; //--������ ����� � ������		
};

/*---------------------
��������� ����� �������
---------------------*/

void menu_init(void);	//--������������� ����
void menu_switcher(MENU *);	//--��������� ����
void menu_output(void);	//--���������������� ����� ����
void about_init(void); //--����� ���� "� ���������"
void level1_init(void); //--������������� ������ �� �����
void move_char(char); //--�������� ������ � ������������ �� ���������� ���������
void info_output(void); //--����� ���������� ������� ����������
void game_start(void); //--������ ����
void level_complete(void); //--���������� ������ � �������
void player_died(void); //--���������� ������ � ����������

/*--------------------------------
��������� ������� ������� ��������
--------------------------------*/

void exgate_init(void); //--����� ����� ������
void exkey_init(void); //--����� ����� �� ������
void got_exkey(void); //--������ ����� ������
//void need_exkey(void);//--����� ������� ����� ����
void exgate_open(void); //--������������� ����� �� ������
void red_gate_init(void); //--����� ������� �����
void red_gkey_init(void); //--����� �������� �����
void got_red_gkey(void);  //--������ �������� �����
void red_gate_open(void); //--�������� ������� �����
void green_gate_init(void); //--����� ������� �����
void green_gkey_init(void); //--����� �������� �����
void got_green_gkey(void); //--������ �������� �����
void green_gate_open(void); //--�������� ������� �����
void gold_init(void); //--����� ������
void portal_init(void); //--����� ��������
void hollow_portal_init(void); //--����� ��������� �������
void htrap_init(void); //--����� �������������� �������
void htrap_moving(void); //--�������� � ������������� �������������� �������
void vtrap_init(void); //--����� ������������ �������
void vtrap_moving(void); //--�������� � ������������ ������������ �������
void rtrap_init(void); //--����� ��������� �������
void rtrap_moving(void); //--�������� � ������������ ��������� �������

#endif

