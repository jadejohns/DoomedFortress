#ifndef FORT_GAME_H
#define FORT_GAME_H

//Определяем структуры
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
Объявляем структуры
---------------------*/
struct MENU //--Меню
{	
	char name[30];
	int x,y,col;
};

struct PLAYER //--Игрок
{
    int x,y; //--Координаты по XY
    int ex_keyholder; //--Проверка наличия ключа для выхода с уровня
    int red_keyholder; //--Проверка наличия ключа от красных ворот
	int green_keyholder; //--Проверка наличия ключа от зеленых ворот
    int goldvalue; //--Сколько собрано золота
    int moves; //--Количество сделанных шагов
	char dir;//--Направление игрока
	char c; //--Символ игрока

};

struct GOLD //--Золото
{
	int  x,y; //--Координаты по XY
	int txtColor,BckColor; //--Параметры цвета и фона
	char c; //--Cимвол золота
};

struct EXGATE //--Ворота выхода с уровня
{
	int x,y; //--Координаты по XY
	int txtColor,BckColor; //--Параметры цвета и фона
	char c; //--Cимвол ворот
};

struct EXKEY //--Ключ для выхода с уровна
{	
	int x,y; //--Координаты по XY
	int txtColor,BckColor; //--Параметры цвета и фона
	char c; //--Символ ключа
};

struct RED_GATE //--Красные ворота
{	
	int x,y; //--Координаты по XY
	int txtColor,BckColor; //--Параметры цвета и фона
	char c; //--Символ красных ворот
};

struct RED_GKEY //--Ключ от красных ворот
{	
	int x,y; //--Координаты по XY
	int txtColor,BckColor; //--Параметры цвета и фона
	char c; //--Символ ключа
};

struct GREEN_GATE //--Зеленые ворота
{	
	int x,y; //--Координаты по XY
	int txtColor,BckColor; //--Параметры цвета и фона
	char c; //--Символ зеленых ворот
};

struct GREEN_GKEY //--Ключ от зеленых ворот
{	
	int x,y; //--Координаты по XY
	int txtColor,BckColor; //--Параметры цвета и фона
	char c; //--Символ ключа
};

struct HTRAP //--Горизонтальная ловушка
{
	int x,y;	//--Координаты по XY
	int bcheck; //--Проверка границы
	int txtColor,BckColor; //--Параметры цвета и фона
	char c; //-- Символ
};

struct VTRAP //--Вертикальная ловушка
{
	int x,y;	//--Координаты по XY
	int bcheck; //--Проверка границы
	int txtColor,BckColor; //--Параметры цвета и фона
	char c; //-- Символ
};

struct RTRAP //--Рандомно двигающаяся ловушка
{
	int x,y; //--Координты по XY
	int minx,miny; //--Минимальный порог 
	int maxx,maxy; //--Максимальный порог
	int speed; //--Скорость
	int movecheck; //--Проверка движения
	int txtColor,BckColor; //--Параметры цвета и фона
	char c; //--Символ
};

struct PORTAL //--Портал в один конец
{
	int inx,iny; //--Координаты входа по XY
	int outx,outy;//--Координаты выхода по XY
	int intxtColor,inBckColor; //--Параметры цвета и фона для входа
	int outtxtColor,outBckColor; //--Параметры цвета и фона для выхода
	char inc,outc; //--Символы входа и выхода
};

struct HOLLOW_PORTAL //--Сквозной портал (только для игрока)
{
	int inx,iny; //--Координты входа по XY
	int outx,outy;//--Координаты выхода по XY
	int intxtColor,inBckColor; //--Параметры цвета и фона входа
	int outtxtColor,outBckColor; //--Параметры цвета и фона выхода
	char inc,outc; //--Симвоы входа и выхода		
};

/*---------------------
Объявляем общие функции
---------------------*/

void menu_init(void);	//--Инициализация меню
void menu_switcher(MENU *);	//--Прокрутка меню
void menu_output(void);	//--Подготовительный вывод меню
void about_init(void); //--Вывод окна "О Программе"
void level1_init(void); //--Инициализация уровня из файла
void move_char(char); //--Движение игрока и столкновение со статичными объектами
void info_output(void); //--Вывод информации игровой информации
void game_start(void); //--Запуск игры
void level_complete(void); //--Завершение уровня с победой
void player_died(void); //--Завершение уровня с проигрышем

/*--------------------------------
Объявляем функции игровых объектов
--------------------------------*/

void exgate_init(void); //--Вывод ворот выхода
void exkey_init(void); //--Вывод ключа от выхода
void got_exkey(void); //--Подбор ключа выхода
//void need_exkey(void);//--Вывод надписи НУЖЕН КЛЮЧ
void exgate_open(void); //--Использование ключа от выхода
void red_gate_init(void); //--Вывод красных ворот
void red_gkey_init(void); //--Вывод красного ключа
void got_red_gkey(void);  //--Подбор красного ключа
void red_gate_open(void); //--Открытие красных ворот
void green_gate_init(void); //--Вывод зеленых ворот
void green_gkey_init(void); //--Вывод зеленого ключа
void got_green_gkey(void); //--Подбор зеленого ключа
void green_gate_open(void); //--Открытие красных ворот
void gold_init(void); //--Вывод золота
void portal_init(void); //--Вывод порталов
void hollow_portal_init(void); //--Вывод сквозного портала
void htrap_init(void); //--Вывод горизонтальных ловушек
void htrap_moving(void); //--Движение и столконовения горизонтальных ловушек
void vtrap_init(void); //--Вывод вертикальных ловушек
void vtrap_moving(void); //--Движение и столкновения вертикальных ловушек
void rtrap_init(void); //--Вывод рандомных ловушек
void rtrap_moving(void); //--Движение и столкновения рандомных ловушек

#endif

