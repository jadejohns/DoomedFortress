#include "fort_game.h"
#include "fort_art.h"

//Обьявляем cтруктуры:


MENU menu[]=  //--Меню
{
	"NEW GAME",33,16,28,
	"ABOUT",55,16,30,
	"EXIT",77,16,30
};

PLAYER player; //--Игрок
EXGATE exgate[]={69,18,0,13,'E'}; //--Ворота выхода
EXKEY exkey[]={91,18,13,0,5}; //--Ключ от ворот выхда
RED_GATE red_gate[]={7,7,12,5,'R'}; //--Красные ворота
RED_GKEY red_gkey[]={6,12,12,0,21}; //--Ключ от красных ворот
GREEN_GATE green_gate[]={70,18,10,2,'G'}; //--Зеленые ворота
GREEN_GKEY green_gkey[]={25,18,10,0,244}; //--Ключ от зеленых ворот
PORTAL portal[1]; //--Портал в один конец
HOLLOW_PORTAL hollow_portal[]={95,23,21,18,3,15,3,15,18,29}; //--Сквозной порталы
GOLD gold[20]; //--Золотые монеты
HTRAP htrap[10]; //--Горизонтальная ловушка
VTRAP vtrap[10]; //--Вертикальная ловушка
RTRAP rtrap[4]; //--Шагающая ловушка

char ESC=27; //--Код клавиши ESCAPE

//Коды символов (Стоило бы их включить в #DEFINE но руки пока не дошли)
//Цифровые коды по положению в кодировке CP866 (! КОДИРОКУ НЕ МЕНЯТЬ !)
char wall_c=219;
char gold_c=249;
char exgate_c='E';
char exkey_c=5;
char red_gkey_c=21;
char red_gate_c='R';
char green_gkey_c=244;
char green_gate_c='G';
char htrap_c=177;
char vtrap_c='M';
char rtrap_c='D';
char portal_in_c=27;
char portal_out_c='X';
char hollow_portal_in_c=18;
char hollow_portal_out_c=29;

//Проверка, запущен ли игровой процесс
int game_running=0;

//Вывод заготовленного меню
void menu_output()
{
	int i;
	brick_wall(); //--Рисуем кирпичную стенку
	make_frame(25,8,88,20,30,"| THE DOOMED FORTRESS V. 0.2a |"); //--Рисуем окно с названием игры
	//Пишем текст в окне
	gotoxy(28,10);printf("Welcome to the Doomed Fortress! Choose thy way, adventurer!");
	gotoxy(45,12);printf("Use A and D to navigate!");
	
	//Выводим меню из массива
	for (i=0;i<=2;i++)
	{
		gotoxy(menu[i].x,menu[i].y);
		set_color(menu[i].col);
		printf("%s",menu[i].name);
		
	}
}

//Прокрутчик меню (Заглушка для вывода анимации, с указателем для удобвства прокручивания через выбор)
void menu_switcher(MENU * menu)
{
	gotoxy(menu->x,menu->y);
	set_color(menu->col);
	printf("%s",menu->name);
	set_color(30);
}

//Инициализируем главное меню
void menu_init()
{
	char code; //--Коды клавиш
	int ch=0; //--Параметр выбора
	int idx=sizeof(menu)/sizeof(menu[0]);idx--; //--Индексируем сюда всё меню
	menu_output(); //--Выводим заранее заготовленное меню
	
	//Крутим меню
	while(1){
		if (kbhit()){
			//Проверка нажатия клавиши и запись кода клавиши в переменную
    		while(kbhit() && (code=getch()));
			switch(code){
				//Прокручиваем меню налево со сменой цвета выбранного меню.
				case 'a':case 'A':
					menu[ch].col=30;menu_switcher(&menu[ch]);
					ch==0 ? ch=idx : ch--;
					menu[ch].col=28;menu_switcher(&menu[ch]);
					break;
				//Прокручиваем меню направо со сменой цвета выбранного меню
				case 'd':case 'D':
					menu[ch].col=30;menu_switcher(&menu[ch]);
					ch==idx ? ch=0 : ch++;
					menu[ch].col=28;menu_switcher(&menu[ch]);
					break;
				//По нажатию Enter (Код 13)
				case 13:
					switch(ch){
						//Запускаем пункт 1 (НОВАЯ ИГРА)
						case 0:
							clr_scr();
							game_start();
							clr_scr();return;break;
						//Запускам пункт 2 (О ПРОГРАММЕ)
						case 1:
							about_init();break;
						case 2://Выход из игры
							clr_scr();return;break;
					}
					break;
				default:break;
			}
		}
	}
}

//Инициализируем окно информации
void about_init()
{	
	make_frame(32,5,82,25,94,"| ABOUT THE DOOMED FORTRESS |"); //--Рисуем окно "О Программе"
	//Печатаем тект в окне
	text_color(11,5);gotoxy(35,7);printf("The Doomed Fortress is a maze game. You need");
	gotoxy(35,9);printf("to get to the exit avoiding traps and enemies!");
	gotoxy(35,11);printf("May the luck be with you!  B E W A R E!!!");
	text_color(10,5);
	gotoxy(35,14);printf("This game was written as final task within");
	gotoxy(35,16);printf("C Programming Course! :) But I used some   ");
	gotoxy(35,18);printf("WinAPI functions.");
	text_color(15,5);
	gotoxy(35,20);printf("Coded by:");
	gotoxy(50,20);printf("Sergey Kuznetsov");
	text_color(14,5);
	//Закрываем меню по нажатию клавиши
	gotoxy(43,23);printf("<PRESS ANY KEY TO CONTINUE>");
	getch();
	//Обновление  меню
	clr_scr();
	menu_output();
}


//Инициализируем уровень
void level1_init()
{
	char c;
	int ERR;
	FILE *level1;
	if(level1=fopen("LEVEL_1.LVL", "rt")) //--Открываем файл уровня
	{
	ERR=0;
	brick_wall(); //--Печатаем кирпичную стенку
	gotoxy(0,3);text_color(4,0); //--Задаем параметры отрисовки уровня
		//Выдираем из уровня символы...
		if (level1) {
    		while ((c = getc(level1)) != EOF)
    		//..и печаем их в консоль
    		putchar(c);
    		text_color(5,8);
    		gotoxy(35,2);printf("THE DOOMED FORTRESS: LEVEL -  1");
//    		gotoxy(0,3);text_color(4,0);
			}
		fclose(level1);
		//Если файла нет, то игра не работает и извинятеся
	}else{ERR=1; clr_scr(); printf("LEVEL FILE IS NOT FOUND... GAME WILL NOT WORK... SORRY :((((\n");system("pause");exit(1);}
	//Выставляем элементы уровня (Выходы, золото, ключи, враги и т.д)
    red_gkey_init();
    red_gate_init();
    green_gkey_init();
    green_gate_init();
    portal_init();
    hollow_portal_init();
    gold_init();
    exgate_init();
    exkey_init();
    htrap_init();
    vtrap_init();
    rtrap_init();
    //Выводим окно для информации
    make_frame(100,3,116,26,30,"");
}

//Функция движения игрока и столкновения его с другими объектами
void move_char(char c)
{
    gotoxy(player.x,player.y); //--Передвигаем курсор по координатам игрока
	printf(" "); //--Вставляем пробел
		
		//Проверка нажатия клавиши
        switch(c)
        {
            case 'w':player.y--;player.dir=c;player.moves++; //--Если нажали W идем наврех, выставляем направление, добавляем шаг в переменную шагов игрока
            break;
            case 's':player.y++;player.dir=c;player.moves++; //--Если нажали S идем вниз выставляем направление, добавляем шаг в переменную шагов игрока
            break;
            case 'a':player.x--;player.dir=c;player.moves++; //--Если нажали A идем налево, выставляем направление, добавляем шаг в переменную шагов игрока
            break;
            case 'd':player.x++;player.dir=c;player.moves++; //--Если нажали D идем направо, выставляем направление, добавляем шаг в переменную шагов игрока
            break;
        }
 
 		//Столкновение игрока с преградами
        if(char_read(player.x,player.y)==wall_c || char_read(player.x,player.y)==portal_out_c) //--Если с координат игрока считали символ стены или выхода из портала
        {
        	//Проверка нажатия клавиши
            switch(c)
            {
            	//Если нажали WASD, то никуда не идем
                case 'w':player.y++; 
                break;
                case 's':player.y--;
                break;
                case 'a':player.x++;
                break;
                case 'd':player.x--;
                break;
            }
        }
    
  		//Столкновение игрока с красными воротами без ключа
		if (char_read(player.x,player.y)==red_gate_c && player.red_keyholder==0) //--Если с координат игрока считали символ красных ворота и у игрока нет ключа
        {	
        	//Если нажали WASD, то никуда не идем
			switch(c)
            {
                case 'w':player.y++;
                break;
                case 's':player.y--;
                break;
                case 'a':player.x++;
                break;
                case 'd':player.x--;
                break;
            }
    	}
    
    	//Столкновение игрока с красными воротами при наличии ключа
        if (char_read(player.x,player.y)==red_gate_c &&  player.red_keyholder==1) //--Если с координат игрока считали символ красных ворот и у игрока есть ключ
        {
        	//Позволяем игроку пройти через ворота
			red_gate_open();
		}
		
		//Столкновение игрока с зелеными воротами без ключа
		if (char_read(player.x,player.y)==green_gate_c && player.green_keyholder==0) //--Если с координат игрока считали символ зеленых ворота и у игрока нет ключа
        {	
        	//Если нажали WASD, то никуда не идем
			switch(c)
            {
                case 'w':player.y++;
                break;
                case 's':player.y--;
                break;
                case 'a':player.x++;
                break;
                case 'd':player.x--;
                break;
            }
    	}
    	
    	//Столкновение игрока с красными воротами при наличии ключа
        if (char_read(player.x,player.y)==green_gate_c &&  player.green_keyholder==1) //--Если с координат игрока считали символ зеленых ворот и у игрока есть ключ
        {
        	//Позволяем игроку пройти через ворота
			green_gate_open();
		}
		
		
		if (char_read(player.x,player.y)==red_gkey_c) //--Если с координат игрока считали символ красного ключа 
		{	
			got_red_gkey(); //--Подбираем красный ключ
		}
		
		//Тоже самое с зеленым ключем))
		if (char_read(player.x,player.y)==green_gkey_c)
        
		{		
			got_green_gkey();
		}
		
		if (char_read(player.x,player.y)==red_gate_c)
		
		{
				player.red_keyholder--;
				
		}
		
		if (char_read(player.x,player.y)==exgate_c && player.ex_keyholder==0) //--Если с координат игрока считали символ ворот выхода и у игрока нет ключа
        {	
        	//Если нажали WASD, то никуда не идем
			switch(c)
            {
                case 'w':player.y++;
                break;
                case 's':player.y--;
                break;
                case 'a':player.x++;
                break;
                case 'd':player.x--;
                break;
            }
    	}
    	
    	if (char_read(player.x,player.y)==exkey_c) //--Если с координат игрока считали символ ключа выхода
        {	
        	got_exkey(); //--Подбираем ключ
    	}
    	
    	
    	if (char_read(player.x,player.y)==exgate_c && player.ex_keyholder==1) //--Если с координат игрока считали символ ворот выхода и у игрока eсть ключа
        {	
     	  	level_complete();//--Завершаем уровень
    	}
					
		//Игрок подбирает золото
		if (char_read(player.x,player.y)==gold_c)
		{
			player.goldvalue+=100;

		}		
		
		//Игрок входит в первый портал	 
        if (char_read(player.x,player.y)==portal[0].inc) //--Если с координат игрока считали символ первого портала
		{
				player.x=portal[0].outx-1;player.y=portal[0].outy; //--Переместить координаты игрока в координаты выхода из первого портала -1 шаг по X (чтобы его не затереть)
        		gotoxy(player.x,player.y); //--Переходим к координатам игрока
     	}
     	
		//Игрок входит во второй портал		
     	if (char_read(player.x,player.y)==portal[1].inc) //--Если с координат игрока считали символ второго портала
		{
				player.x=portal[1].outx-1;player.y=portal[1].outy; //--Переместить координаты игрока в координаты выхода из первого портала -1 шаг по X (чтобы его не затереть)
        		gotoxy(player.x,player.y); //--Переходим к координатам игрока
     	}
    	
		//Игрок входит в сквозной портал	
		if (char_read(player.x,player.y)==hollow_portal->inc)
		{
			player.x=hollow_portal->outx;player.y=hollow_portal->outy+1;
        	gotoxy(player.x,player.y);
        
		}
		//Игрок входим в сквозной портал
		if (char_read(player.x,player.y)==hollow_portal->outc)
		{
			player.x=hollow_portal->inx;player.y=hollow_portal->iny-1;
        	gotoxy(player.x,player.y);
        	putchar(16);
        
		}
		
		
		//Cтолкновение с врагами с позиции игрока 
		if (char_read(player.x,player.y)==vtrap_c || char_read(player.x,player.y)==htrap_c || char_read(player.x,player.y)==rtrap_c)
		{
			player_died();
		}
		
		//Выводим символ игрока
        gotoxy(player.x,player.y);
        switch(player.dir)
        {
            case 'w':putchar(30);
            break;
            case 's':putchar(31);
            break;
            case 'a':putchar(17);
            break;
            case 'd':putchar(16);
            break;

        }
    		//Возвращаем параметры вывода игрока
        	text_color(11,0);
	
}

//Начинаем игру
void game_start()
{
	fflush(stdin); //--Обнуляем всё (JUST IN CASE)
	char c = '0';
	game_running=1; //--Запуск игрового процесса
	player.moves=0; //--Шаги игрока (Будут выводиться в окне смерти или победы)
	player.x=1; //--Начальные координты игрока по X
	player.y=7; //--Начальные координты игрока по Y
	player.c=16; //--Начальны символ игрока (залитая стрелка направо)
	player.goldvalue=0; //--Начальное количество золота
	player.ex_keyholder=0; //--Есть ли у игрока ключ от выхода (0 - нет, 1 - да)
	player.red_keyholder=0;
	player.green_keyholder=0;
	gotoxy(0,0); //--Перемещаем курсор в позицию 0,0 по XY
	level1_init(); //--Загружаем уровень
	text_color(11,0); //--Цвет для игрока
	gotoxy(player.x,player.y); //--Переходим в начальные координаты игрока по XY
    putchar(player.c); //--Выводим символ игрока
    info_output(); //--Выводим информацию
    
	//Пока запущен игровой процесс или не нажата клавиша ESC
    while(game_running==1)
    {		
			usleep(50000); //--Задержка движения (TODO: добавить программируюемую задержку и выставить для игры параметр сложности)
    	    htrap_moving(); //--Запускаем горизонтальные ловушки
    	    vtrap_moving(); //--Запускаем вертикальные ловушки
    	    rtrap_moving(); //--Запускаем рандомные ловушки
    	    
			//Если нажали клавишу, то
        	if (_kbhit()==1)
        	   { 
			   		c=getch(); //--Ловим нажатую клавишу
        	    	move_char(c); //--Двигаем статичные объекты
    				info_output(); //--Обновляем информаицю
    	 	   }
    	//Если ничего не нажали, обнуляем с 	  
    	else c='0';
    		//Если нажали ESC
        	if(c==ESC) 
				{
					game_running=0; //--Останавливаем игровой процесс
					clr_scr(); //--Очистка экрана
					menu_init(); //--Выводим меню
				}
	}
}

//Подготавливаем окно информации
void info_output()
{	
	//Выставляем цвет
	set_color(30);
	//Печатаем координаты игрока по XY
	gotoxy(103,5);printf("COORDINATES:");
	gotoxy(105,6);printf("X:%d Y:%d",player.x,player.y-3);printf(" "); //--Для расстановки юнитов и как заглушка для редактора карт (в планах) вместо y-3 использовать просто y
	gotoxy(106,8);printf("HELP:");
	gotoxy(102,9);printf("WASD - MOVING");
	gotoxy(103,10);printf("ESC - EXIT");
	//Печатаем легенду -- Ловушки
	gotoxy(105,12);printf("LEGEND:");
	gotoxy(102,13);text_color(11,1);putchar(htrap_c);
	gotoxy(103,13);text_color(10,1);putchar(vtrap_c);
	gotoxy(104,13);text_color(12,1);putchar(rtrap_c);
	gotoxy(106,13);set_color(30);printf("- TRAPS");
	//Ворота
	gotoxy(102,14);text_color(12,5);putchar(red_gate_c);
	gotoxy(103,14);text_color(10,2);putchar(green_gate_c);
	gotoxy(106,14);set_color(30);printf("- GATES");
	//Ключи от ворот
	gotoxy(102,15);text_color(12,1);putchar(red_gkey_c);
	gotoxy(103,15);text_color(10,1);putchar(green_gkey_c);
	gotoxy(105,15);set_color(30);printf("- G. KEYS");
	//Портал	
	gotoxy(102,16);text_color(13,15);putchar(portal_in_c);gotoxy(103,16);putchar(portal_out_c);
	gotoxy(105,16);set_color(30);printf("- PORTAL");
	//Сквозной портал
	gotoxy(102,17);text_color(1,15);putchar(hollow_portal_in_c);gotoxy(103,17);putchar(hollow_portal_out_c);
	gotoxy(105,17);set_color(30);printf("- 2W PORTAL");
	//Выход и ключ
	gotoxy(102,18);text_color(0,13);putchar(exgate_c);gotoxy(103,18);text_color(13,1);putchar(exkey_c);
	gotoxy(105,18);set_color(30);printf("- EXIT/KEY");
	//Золото
	gotoxy(102,19);putchar(gold_c);gotoxy(104,19);printf("- GOLD");
	//Выводим количество золота у игрока
	gotoxy(105,21);printf("GOLD: %d",player.goldvalue);
	//Выводим ключи, которые есть у игрока
	gotoxy(106,23);printf("KEYS:");
	//Печатаем информацию о взятом ключе
	//Возвращаем цвет персонажа
	text_color(11,0);
}

//Завершаем уровень
void level_complete()
{
	game_running=0;
	clr_scr();
	brick_wall();
	make_frame(32,5,82,22,206,"YOU HAVE ESCAPED!");
	gotoxy(34,10);
	printf("BRAVE HERO! YOU HAVE ESCAPED AFTER %d STEP(S)",player.moves);
	gotoxy(45,12);
	printf("WITH %d GOLD IN POCKETS",player.goldvalue);
	getch();
	//Задержка окна
	sleep(1);
	clr_scr();
	menu_init();
}

//Выводим выход с уровня
void exgate_init()
{
	gotoxy(exgate->x,exgate->y);
	text_color(exgate->txtColor,exgate->BckColor);
	putchar(exgate->c);
}

//Выводим ключ от ворот
void exkey_init()
{
	gotoxy(exkey->x,exkey->y);
	text_color(exkey->txtColor,exkey->BckColor);
	putchar(exkey->c);
}

//Выводим золото
void gold_init()
{
	int i=0;
	gold[0].x=6;gold[0].y=4;gold[0].txtColor=14;gold[0].BckColor=0;gold[0].c=gold_c;
	gold[1].x=6;gold[1].y=10;gold[1].txtColor=14;gold[1].BckColor=0;gold[1].c=gold_c;
	gold[2].x=4;gold[2].y=15;gold[2].txtColor=14;gold[2].BckColor=0;gold[2].c=gold_c;
	gold[3].x=10;gold[3].y=8;gold[3].txtColor=14;gold[3].BckColor=0;gold[3].c=gold_c;
	gold[4].x=91;gold[4].y=23;gold[4].txtColor=14;gold[4].BckColor=0;gold[4].c=gold_c;
	gold[5].x=92;gold[5].y=23;gold[5].txtColor=14;gold[5].BckColor=0;gold[5].c=gold_c;
	gold[6].x=85;gold[6].y=25;gold[6].txtColor=14;gold[6].BckColor=0;gold[6].c=gold_c;
	gold[7].x=86;gold[7].y=25;gold[7].txtColor=14;gold[7].BckColor=0;gold[7].c=gold_c;
	gold[8].x=87;gold[8].y=25;gold[8].txtColor=14;gold[8].BckColor=0;gold[8].c=gold_c;
	gold[9].x=77;gold[9].y=23;gold[9].txtColor=14;gold[9].BckColor=0;gold[9].c=gold_c;
	gold[10].x=70;gold[10].y=20;gold[10].txtColor=14;gold[10].BckColor=0;gold[10].c=gold_c;
	gold[11].x=69;gold[11].y=21;gold[11].txtColor=14;gold[11].BckColor=0;gold[11].c=gold_c;
	gold[12].x=73;gold[12].y=18;gold[12].txtColor=14;gold[12].BckColor=0;gold[12].c=gold_c;
	gold[13].x=79;gold[13].y=21;gold[13].txtColor=14;gold[13].BckColor=0;gold[13].c=gold_c;
	gold[14].x=89;gold[14].y=19;gold[14].txtColor=14;gold[14].BckColor=0;gold[14].c=gold_c;
//	gold[15].x=91;gold[15].y=18;gold[15].txtColor=14;gold[15].BckColor=0;gold[15].c=gold_c;
	gold[15].x=23;gold[15].y=18;gold[15].txtColor=14;gold[15].BckColor=0;gold[15].c=gold_c;
	gold[16].x=25;gold[16].y=22;gold[17].txtColor=14;gold[16].BckColor=0;gold[16].c=gold_c;
	gold[17].x=27;gold[17].y=23;gold[17].txtColor=14;gold[17].BckColor=0;gold[17].c=gold_c;
	gold[18].x=31;gold[18].y=18;gold[18].txtColor=14;gold[18].BckColor=0;gold[18].c=gold_c;
	gold[19].x=26;gold[19].y=19;gold[19].txtColor=14;gold[19].BckColor=0;gold[19].c=gold_c;	
		
		for(i=0;i<19;i++)
		{
		gotoxy(gold[i].x,gold[i].y);
		text_color(gold[i].txtColor,gold[i].BckColor);
		putchar(gold[i].c);
		}
	
}
	
////Печатаем попытку выйти без ключа
//void need_exkey()
//{
//	set_color(30);
//    gotoxy(107,20);printf("   ");
//	gotoxy(104,20);printf("           ");
//	gotoxy(107,20);printf("YOU");
//	gotoxy(104,21);printf("NEED A KEY!");
//	text_color(11,0); 
//}

//Игрок взял ключ
void got_exkey()
{
	text_color(13,1);
	gotoxy(107,24);
	player.ex_keyholder++;
	putchar(exkey_c);
}

//Игрок открыл ворота выхода
void exgate_open()
{
	set_color(30);
	gotoxy(107,24);
	player.ex_keyholder--;
	printf(" ");
	text_color(11,0);
}

//Игрок подобрал ключ от красной клетки
void got_red_gkey()
{
	text_color(12,1);
	gotoxy(103,24);
	player.red_keyholder++;
	putchar(red_gkey_c);
	text_color(11,0);
}

//Игрок открыл красную клетку
void red_gate_open()
{
	set_color(30);
	gotoxy(103,24);
	player.red_keyholder--;
	printf(" ");
	text_color(11,0);
}

//Игрок взял зеленый ключ
void got_green_gkey()
{
	text_color(10,1);
	gotoxy(105,24);
	player.green_keyholder++;
	putchar(green_gkey_c);
	text_color(11,0);
}

//Игрок открыл зеленые ворота
void green_gate_open()
{
	set_color(30);
	gotoxy(105,24);
	player.green_keyholder--;
	printf(" ");
	text_color(11,0);
}
	
//Выводим красные ворота
void red_gate_init()
{
	gotoxy(red_gate->x,red_gate->y);
	text_color(red_gate->txtColor,red_gate->BckColor);
	putchar(red_gate->c);
}

//Выводим красный ключ
void red_gkey_init()
{
	gotoxy(red_gkey->x,red_gkey->y);
	text_color(red_gkey->txtColor,red_gkey->BckColor);
	putchar(red_gkey->c);
}

//Выводим зеленые ворота
void green_gate_init()
{
	gotoxy(green_gate->x,green_gate->y);
	text_color(green_gate->txtColor,green_gate->BckColor);
	putchar(green_gate->c);
}

//Выводим зеленый ключ
void green_gkey_init()
{
	gotoxy(green_gkey->x,green_gkey->y);
	text_color(green_gkey->txtColor,green_gkey->BckColor);
	putchar(green_gkey->c);
}

//Выводим портал(ы)
void portal_init()
{
	int i=0;
	portal[0].inx=1;portal[0].iny=4;
	portal[0].outx=95;portal[0].outy=25;
	portal[0].intxtColor=13;portal[0].inBckColor=15;
	portal[0].outtxtColor=13;portal[0].outBckColor=15;
	portal[0].inc=27;portal[0].outc=88;
	
//	portal[1].inx=69;portal[1].iny=18;
//	portal[1].outx=35;portal[1].outy=12;
//	portal[1].intxtColor=0;portal[1].inBckColor=14;
//	portal[1].outtxtColor=0;portal[1].outBckColor=14;
//	portal[1].inc=24;portal[1].outc=149;

	
		for(i=0;i<1;i++)
		{
			gotoxy(portal[i].inx,portal[i].iny);
			text_color(portal[i].intxtColor,portal[i].inBckColor);
			putchar(portal[i].inc);
			gotoxy(portal[i].outx,portal[i].outy);
			text_color(portal[i].outtxtColor,portal[i].outBckColor);
			putchar(portal[i].outc);
		}
}

//Выводим сквозной портал
void hollow_portal_init()
{
	gotoxy(hollow_portal->inx,hollow_portal->iny); //--Вывод первого входа
	text_color(hollow_portal->intxtColor,hollow_portal->inBckColor);
	putchar(hollow_portal->inc);
	gotoxy(hollow_portal->outx,hollow_portal->outy); //--Вывод второго входа
	text_color(hollow_portal->outtxtColor,hollow_portal->outBckColor);
	putchar(hollow_portal->outc);

}

//Выводим горизонтальные ловушки
void htrap_init()
{
	int i=0;
	htrap[0].x=72;htrap[0].y=23;htrap[0].txtColor=10;htrap[0].BckColor=0;htrap[0].c=htrap_c;htrap[0].bcheck=0;
	htrap[1].x=4;htrap[1].y=14;htrap[1].txtColor=11;htrap[1].BckColor=0;htrap[1].c=htrap_c;htrap[1].bcheck=0;
	htrap[2].x=80;htrap[2].y=24;htrap[2].txtColor=12;htrap[2].BckColor=0;htrap[2].c=htrap_c;htrap[2].bcheck=1;
	htrap[3].x=94;htrap[3].y=25;htrap[3].txtColor=13;htrap[3].BckColor=0;htrap[3].c=htrap_c;htrap[3].bcheck=0;
	htrap[4].x=21;htrap[4].y=20;htrap[4].txtColor=14;htrap[4].BckColor=0;htrap[4].c=htrap_c;htrap[4].bcheck=0;
	htrap[5].x=25;htrap[5].y=22;htrap[5].txtColor=11;htrap[5].BckColor=0;htrap[5].c=htrap_c;htrap[5].bcheck=0;
	htrap[6].x=30;htrap[6].y=25;htrap[6].txtColor=15;htrap[6].BckColor=0;htrap[6].c=htrap_c;htrap[6].bcheck=0;

	
		for(i=0;i<=6;i++)
		{
			gotoxy(htrap[i].x,htrap[i].y);
			text_color(htrap[i].txtColor,htrap[i].BckColor);
			putchar(htrap[i].c);
		}
}

//Выводим вертикальные ловушки
void vtrap_init()
{
	int i=0;
	
	vtrap[0].x=5;vtrap[0].y=9;vtrap[0].txtColor=10;vtrap[0].BckColor=0;vtrap[0].c=vtrap_c;vtrap[0].bcheck=1;
	vtrap[1].x=73;vtrap[1].y=19;vtrap[1].txtColor=10;vtrap[1].BckColor=0;vtrap[1].c=vtrap_c;vtrap[1].bcheck=1;
	vtrap[2].x=76;vtrap[2].y=21;vtrap[2].txtColor=10;vtrap[2].BckColor=0;vtrap[2].c=vtrap_c;vtrap[2].bcheck=1;
	vtrap[3].x=79;vtrap[3].y=18;vtrap[3].txtColor=10;vtrap[3].BckColor=0;vtrap[3].c=vtrap_c;vtrap[3].bcheck=1;
	vtrap[4].x=30;vtrap[4].y=25;vtrap[4].txtColor=10;vtrap[4].BckColor=0;vtrap[4].c=vtrap_c;vtrap[4].bcheck=1;
	
	
		for(i=0;i<=4;i++)
			{
				gotoxy(vtrap[i].x,vtrap[i].y);
				text_color(vtrap[i].txtColor,vtrap[i].BckColor);
				putchar(vtrap[i].c);
			}
}

//Функция движения горизонтальных ловушек
void htrap_moving()
{
	int i;

    	for (i=0;i<=6;i++)
        	{
		
        	gotoxy(htrap[i].x,htrap[i].y);printf(" ");
		if(htrap[i].bcheck==1)
        {
        	//Проверка столкновений
            if(char_read(htrap[i].x-1,htrap[i].y)==wall_c || char_read(htrap[i].x-1,htrap[i].y)==portal_out_c 
			|| char_read(htrap[i].x-1,htrap[i].y)==gold_c || char_read(htrap[i].x-1,htrap[i].y)==vtrap_c || 
			char_read(htrap[i].x-1,htrap[i].y)==rtrap_c)
            {
                htrap[i].bcheck=0;
            }
            else
               htrap[i].x--;
        }
        else
        {
            if(char_read(htrap[i].x+1,htrap[i].y)==wall_c || 
			char_read(htrap[i].x+1,htrap[i].y)==portal_out_c || 
			char_read(htrap[i].x+1,htrap[i].y)==gold_c || char_read(htrap[i].x-1,htrap[i].y)==vtrap_c
			|| char_read(htrap[i].x+1,htrap[i].y)==rtrap_c)
			{
			htrap[i].bcheck=1;
        	}    
			else
                htrap[i].x++;
        }
      if (htrap[i].x==player.x && htrap[i].y==player.y)
        {

     	  	player_died();
		}
        	text_color(htrap[i].txtColor,htrap[i].BckColor);
			gotoxy(htrap[i].x,htrap[i].y);
			putchar(htrap[i].c);
			text_color(11,0);
}
    	            		
}

//Функция движения вертикальных ловушек
void vtrap_moving()
{
	int i;

    	for (i=0;i<=4;i++)
        	{
		
        	gotoxy(vtrap[i].x,vtrap[i].y);printf(" ");
        if(vtrap[i].bcheck==1)
        {
        	//Проверка столкновений
            if(char_read(vtrap[i].x,vtrap[i].y-1)==wall_c || 
			char_read(vtrap[i].x,vtrap[i].y-1)==gold->c || 
			char_read(vtrap[i].x,vtrap[i].y-1)==rtrap->c ||
			char_read(vtrap[i].x,vtrap[i].y-1)==htrap->c  )  
            {
                vtrap[i].bcheck=0;
            }
            else
               vtrap[i].y--;
        }
        else
        {
            if(char_read(vtrap[i].x,vtrap[i].y+1)==wall_c || 
			char_read(vtrap[i].x,vtrap[i].y+1)==gold_c || 
			char_read(vtrap[i].x,vtrap[i].y+1)==rtrap_c||
			char_read(vtrap[i].x,vtrap[i].y+1)==htrap_c  ) vtrap[i].bcheck=1;
            else
                vtrap[i].y++;
        }
      if (vtrap[i].x==player.x && vtrap[i].y==player.y)
        {	
        	player_died();
		}
        	text_color(vtrap[i].txtColor,vtrap[i].BckColor);
			gotoxy(vtrap[i].x,vtrap[i].y);
			putchar(vtrap[i].c);
			text_color(11,0);
    }
	            		
}

//Выводим шагающие ловушки
void rtrap_init()
{
	int i=0;
	
	rtrap[0].x=8;rtrap[0].y=4;
	rtrap[0].minx=8;rtrap[0].miny=4;
	rtrap[0].maxx=10;rtrap[0].maxy=5;
	rtrap[0].speed=35;
	rtrap[0].txtColor=15;rtrap[0].BckColor=0;
	rtrap[0].c=rtrap_c;
		
	rtrap[1].x=86;rtrap[1].y=20;
	rtrap[1].minx=82;rtrap[1].miny=1;
	rtrap[1].maxx=90;rtrap[1].maxy=25;
	rtrap[1].speed=20;
	rtrap[1].txtColor=9;rtrap[1].BckColor=0;
	rtrap[1].c=rtrap_c;
	
	rtrap[2].x=94;rtrap[2].y=19;
	rtrap[2].minx=91;rtrap[2].miny=18;
	rtrap[2].maxx=94;rtrap[2].maxy=20;
	rtrap[2].speed=10;
	rtrap[2].txtColor=12;rtrap[2].BckColor=0;
	rtrap[2].c=rtrap_c;
	
	rtrap[3].x=31;rtrap[3].y=24;
	rtrap[3].minx=25;rtrap[3].miny=21;
	rtrap[3].maxx=31;rtrap[3].maxy=25;
	rtrap[3].speed=5;
	rtrap[3].txtColor=14;rtrap[3].BckColor=0;
	rtrap[3].c=rtrap_c;
//
//	rtrap[4].x=26;rtrap[4].y=23;
//	rtrap[4].minx=21;rtrap[4].miny=19;
//	rtrap[4].maxx=31;rtrap[4].maxy=25;
//	rtrap[4].speed=8;
//	rtrap[4].txtColor=14;rtrap[4].BckColor=0;
//	rtrap[4].c=rtrap_c;
	
		for(i=0;i<=3;i++)
			{
				gotoxy(rtrap[i].x,rtrap[i].y);
				text_color(rtrap[i].txtColor,rtrap[i].BckColor);
				putchar(rtrap[i].c);
			}
}

//Функция движения шагающих ловушек
void rtrap_moving()
{

	int i,r;
    for (i=0; i<=3; i++)
    {
	gotoxy(rtrap[i].x,rtrap[i].y);
    printf(" ");
    rtrap[i].movecheck=0;
        while (rtrap[i].movecheck==0)
                {
                	//Cкорость движенния
                    r=rand()%rtrap[i].speed;
                    if ((r==1) && (rtrap[i].y>rtrap[i].miny)) 
                    {
                        rtrap[i].y-=1;
                        rtrap[i].movecheck=1;
                        //Проверка столкновений
                        if (char_read(rtrap[i].x,rtrap[i].y) == wall_c || char_read(rtrap[i].x,rtrap[i].y) == gold_c 
						|| char_read(rtrap[i].x,rtrap[i].y) == htrap_c || char_read(rtrap[i].x,rtrap[i].y) == vtrap_c || char_read(rtrap[i].x,rtrap[i].y) == exkey_c)
               				 {
								rtrap[i].y+=1;
								rtrap[i].movecheck=0;						
							 }
                    } 
                    if ((r==2) && (rtrap[i].y<rtrap[i].maxy)) 
                    	{
                        rtrap[i].y+=1;
                        rtrap[i].movecheck=1;
                        
                        if (char_read(rtrap[i].x,rtrap[i].y) == wall_c || char_read(rtrap[i].x,rtrap[i].y) == gold_c 
						|| char_read(rtrap[i].x,rtrap[i].y) == htrap_c || char_read(rtrap[i].x,rtrap[i].y) == vtrap_c || char_read(rtrap[i].x,rtrap[i].y) == exkey_c)
               					 {
								rtrap[i].y-=1;
								rtrap[i].movecheck=0;						
								 }
                	    }
                    if ((r==3) && (rtrap[i].x>rtrap[i].minx))
                    {
                        rtrap[i].x-=1;
                        rtrap[i].movecheck=1;
                        
                        if (char_read(rtrap[i].x,rtrap[i].y) == wall_c || char_read(rtrap[i].x,rtrap[i].y) == gold_c 
						|| char_read(rtrap[i].x,rtrap[i].y) == htrap_c || char_read(rtrap[i].x,rtrap[i].y) == vtrap_c || char_read(rtrap[i].x,rtrap[i].y) == exkey_c)
                        			{
                        				rtrap[i].x+=1;
                        				rtrap[i].movecheck=0;
									}
                    } 
                    if ((r==4) && (rtrap[i].x<rtrap[i].maxx))
                    {
                        rtrap[i].x+=1;
                        rtrap[i].movecheck=1;
                        
                        if (char_read(rtrap[i].x,rtrap[i].y) == wall_c || char_read(rtrap[i].x,rtrap[i].y) == gold_c 
						|| char_read(rtrap[i].x,rtrap[i].y) == htrap_c || char_read(rtrap[i].x,rtrap[i].y) == vtrap_c || char_read(rtrap[i].x,rtrap[i].y) == exkey_c)
                        			{
                        				rtrap[i].x-=1;
                        				rtrap[i].movecheck=0;
									}
                    }
                  
                        rtrap[i].movecheck=1;
                  
                }
                
             if (rtrap[i].x==player.x && rtrap[i].y==player.y)
        		{
        			player_died();
				}
				
                gotoxy(rtrap[i].x,rtrap[i].y);
                text_color(rtrap[i].txtColor,rtrap[i].BckColor);
                putchar(rtrap[i].c);
                text_color(11,0);
            }
        }

//Игрок убит
void player_died()
{
	game_running=0;
	clr_scr();
	brick_wall();
	make_frame(32,5,82,22,14,"YOU HAVE DIED!");
	gotoxy(34,10);
	printf("BRAVE HERO! YOU HAVE PERISHED AFTER %d STEP(S)",player.moves);
	gotoxy(45,12);
	printf("WITH %d GOLD IN POCKETS",player.goldvalue);
	sleep(1);
	gotoxy(43,18);
	printf("< PRESS ANY KEY TO CONTINE >");
	getch();
	sleep(1);
	clr_scr();
	menu_init();
}    
