#include<conio.h>
#include<stdio.h>
#include<dos.h>
#include<stdlib.h>
#include<graphics.h>
#include<process.h>

union REGS p,o;

void initmouse()
{
   p.x.ax = 0;
   int86(0X33,&p,&o);
}

void showmouseptr()
{
   p.x.ax = 1;
   int86(0X33,&p,&o);
}

void hidemouseptr()
{
	p.x.ax=2;
	int86(0x33,&p,&o);
}

void getmousepos(int *button, int *x, int *y)
{
   p.x.ax = 3;
   int86(0X33,&p,&o);

   *button = o.x.bx;
   *x = o.x.cx;
   *y = o.x.dx;
}

void draw()//this draws the initial environment...//for 2-lane
{
	setbkcolor(9);
	rectangle(getmaxx()/2-5,0,getmaxx()/2+5,getmaxy());
	line(getmaxx()/2-85,0,getmaxx()/2-85,getmaxy());
	line(getmaxx()/2+85,0,getmaxx()/2+85,getmaxy());

	setviewport(getmaxx()/2+86,0,getmaxx()/2+175,getmaxy(),0);
	clearviewport();

	setviewport(0,0,getmaxx(),getmaxy(),0);
	setfillstyle(SOLID_FILL, 8);
	floodfill(1,1,getmaxcolor());
	setfillstyle(SOLID_FILL, 1);
	floodfill(getmaxx(),getmaxy(),getmaxcolor());
	setfillstyle(9, 15);
	floodfill(getmaxx()/2,getmaxy()/2,getmaxcolor());
}

void draw1()//this draws the initial environment...//for 3-lane
{
	setviewport(0,0,getmaxx(),getmaxy(),0);
	setbkcolor(9);
	rectangle(getmaxx()/2-5,0,getmaxx()/2+5,getmaxy());
	line(getmaxx()/2-85,0,getmaxx()/2-85,getmaxy());
	line(getmaxx()/2+85,0,getmaxx()/2+85,getmaxy());
	line(getmaxx()/2+175,0,getmaxx()/2+175,getmaxy());
	rectangle(getmaxx()/2+85,0,getmaxx()/2+95,getmaxy());
	setfillstyle(SOLID_FILL, 8);
	floodfill(1,1,getmaxcolor());
	setfillstyle(SOLID_FILL, 1);
	floodfill(getmaxx(),getmaxy(),getmaxcolor());
	setfillstyle(9, 15);
	floodfill(getmaxx()/2,getmaxy()/2,getmaxcolor());
	setfillstyle(9, 15);
	floodfill(getmaxx()/2+90,getmaxy()/2,getmaxcolor());
	setfillstyle(SOLID_FILL, 9);
	floodfill(getmaxx()/2+100,getmaxy()/2,getmaxcolor());
}

void car()//this draws player box..
{
	setviewport(getmaxx()/2-65,getmaxy()-65,getmaxx()/2-60,getmaxy()-25,0);
	rectangle(0,0,30,30);
	setfillstyle(11,15);
	floodfill(1,1,getmaxcolor());
}

void score() //to print the score and level of the game...
{
	char msg[20];
	sprintf(msg,"SCORE: 00");
	setviewport(5,0,120,80,0);
	clearviewport();
	outtextxy(10,20,msg);
	sprintf(msg,"LEVEL: 0");
	outtextxy(10,40,msg);
	sprintf(msg,"Time: 0:0");
	outtextxy(10,60,msg);
}

//gane nebu
void menu()
{
	char mg[20];
	setcolor(15);
	rectangle(9,299,111,321);
	rectangle(9,329,111,351);
	rectangle(9,359,111,381);
	sprintf(mg,"Select A Game:");
	outtextxy(10,280,mg);
	setcolor(12);
	sprintf(mg,"2-lane way");
	outtextxy(20,307,mg);
	sprintf(mg,"3-lane way");
	outtextxy(20,337,mg);
	sprintf(mg,"EXIT");
	outtextxy(45,367,mg);
	setviewport(0,0,getmaxx(),getmaxy(),0);
	setcolor(15);
}

main()
{
	int gd=DETECT,gm;
	int a,b,c,rem=0;//rem is used as flag in line 183 and 185
	char d='r'; //initializing variable with dummy value...used to control do-while loop

	//initializing graphics
	initgraph(&gd,&gm,"/tc/bgi ");

	//printing control configuration
	char mg[20];
	setviewport(0,80,400,getmaxy(),0);
	sprintf(mg,"CONTROLLER DESCRIPTION:");
	outtextxy(20,20,mg);
	sprintf(mg,"-press w to move UP");
	outtextxy(20,40,mg);
	sprintf(mg,"-press s to move DOWN");
	outtextxy(20,50,mg);
	sprintf(mg,"-press a to move LEFT");
	outtextxy(20,60,mg);
	sprintf(mg,"-press d to move RIGHT");
	outtextxy(20,70,mg);
	sprintf(mg,"-press q to QUIT");
	outtextxy(20,80,mg);
	sprintf(mg,"-press p to PAUSE");
	outtextxy(20,90,mg);
	sprintf(mg,"while moving tap the keys");
	outtextxy(20,120,mg);
	sprintf(mg,"instead of keep pressing");
	outtextxy(10,130,mg);
	setviewport(0,0,getmaxx(),getmaxy(),0);
//drawing initial GUI
	menu();
	draw();
	score();
	car();
//initializing mouse
	initmouse();
	showmouseptr();

	do{
	//initially d is set a dummy variable, if user presses 'q' it means terminate
		if(kbhit()) d=getch();
	//getting mouse position and button clicked
		getmousepos(&a,&b,&c);
	//a contains information about mouse button clicked
	//b contains x-pos of mouse
	//c contains y-pos of mouse

	//if left mouse button is clicked
		if(a==1){
			a=-1;
	//if x-pos of mouse matches where game options are displayed
			if(abs(b-60)<=30){
	//if y-pos of mouse also matches where one of the game options are displayed
				if(c>299&&c<321){spawnl(P_OVERLAY, "car5.exe", NULL); }
				else if(c>329&&c<351){spawnl(P_OVERLAY, "car6.exe", NULL);}
				else if(c>359&&c<381) exit(0); //instead of using exit(), assigning d with 'q' also does same thing
			}
		}

	//if left mouse button isn't clicked but mouse hovers over the menu
		if(abs(b-60)<=30){
			if(c>299&&c<321){  if(rem!=0){ hidemouseptr(); draw(); showmouseptr(); rem=0; }}

			else if(c>329&&c<351){ if(rem!=1){ hidemouseptr(); draw1(); showmouseptr(); rem=1; }}
		}

	}while(d!='q'); //if user presses 'q', end loop
	return 0;
}