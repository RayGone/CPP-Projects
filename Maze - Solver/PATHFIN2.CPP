//to increase the difficulty of game decrease the value of k in if statement
//in line 194 which is initially if(k==50)...


#include<iostream.h>
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<time.h>
#include<dos.h>

int x,y;
int xp,yp;//position of player on map(i.e. 2d array state)
int xc,yc;//position of player on map(i.e. scene on monitor)
int state[20][20];
int endOfFile;//to know we've reached end of file...

void drawb(int i, int j)
{
	rectangle(i-10,j-10,i+10,j+10);
}


void drawg(int i, int j)
{
	circle(i,j,5);
}

void drawst(int i, int j)
{
	circle(i,j,1);
	circle(i,j,2);
	circle(i,j,3);
	circle(i,j,4);
}

int loader(FILE *fp)
{
	char c;
	int bufyp=yp;
	if(yp<4){
		outtext("YOU'VE BEEN CRUSHED!!!!!!");
	}
	setviewport(xc-5,yc-5,xc+5,yc+5,1);
	clearviewport();

	for(int i=0;i<20;i++)
		for(int j=0;j<20;j++)
		{
			if(i<16)
				state[i][j]=state[i+4][j];

			else
				state[i][j]=1;

			if(state[i][j]==2){ yp=i; xp=j;}
		}

	if((c=getc(fp))==9) c=getc(fp);
	if(c=='\n') c=getc(fp);
	if(c==EOF) endOfFile=1;

	i=16;j=0;

	if(endOfFile!=1){

		while(c!='9')
		{
			if(c=='x') { state[i][j]=1; j++; }
			if(c=='o') { state[i][j]=0; j++; }
			if(c=='g') { state[i][j]=3; j++; }
			c=getc(fp);
			if(j==20){ j=0; i++; }

		}
	}

	setviewport(getmaxx()/2-200,getmaxy()/2-200,getmaxx()/2+200,getmaxy()/2+200,1);
	clearviewport();

	setviewport(0,0,getmaxx(),getmaxy(),1);

	rectangle(getmaxx()/2-200,getmaxy()/2-200,getmaxx()/2+200,getmaxy()/2+200);

	x=getmaxx()/2-200+10;
	y=getmaxy()/2+200-10;

	for(i=0;i<20;i++)
	{
		for(j=0;j<20;j++)
		{
			if(state[i][j]==0) x+=20;
			if(state[i][j]==1){ drawb(x,y); x+=20; }
			if(state[i][j]==2){ drawst(x,y); xc=x; yc=y; x+=20; }
			if(state[i][j]==3){ drawg(x,y); x+=20; }
		}
		x=getmaxx()/2-200+10;
		y-=20;
	}
	if(bufyp<4) return 1;
	return 0;
}


main()
{
	/*the game window is of 400pixel square...
		so maxx and maxy is 200 taken as radius....
		this helps get same result in any monitor of any resolution
	  */
	int const maxxy=200;
	endOfFile=0;

	clock_t start,end;

	FILE *fp;
	fp=fopen("envdef.txt","r");

	char c;
	char msg[40];

	int gd=DETECT, gm;

	initgraph( &gd, &gm, "/tc/bgi");

	x=getmaxx()/2-maxxy+10;
	y=getmaxy()/2+maxxy-10;

	int i,j;
	i=j=0;
	delay(1000);

	outtextxy(0,100,"CONTROLLER: ");
	outtextxy(0,110," w up");
	outtextxy(0,120," s down");
	outtextxy(0,130," a left");
	outtextxy(0,140," d right");
	outtextxy(0,150," q quit");

	outtextxy(getmaxx()/2-180,getmaxy()/2+80,"don't have CAPS LOCK ON");
	drawst(getmaxx()/2-50,getmaxy()/2-100);
	outtextxy(getmaxx()/2-40,getmaxy()/2-100,"THIS IS YOU");
	drawg(getmaxx()/2-50,getmaxy()/2-80);
	outtextxy(getmaxx()/2-40,getmaxy()/2-80,"THIS IS DESTINATION");
	outtextxy(getmaxx()/2-180,getmaxy()/2-60,"DESTINATION IS NOT SEEN INITIALLY");
	outtextxy(getmaxx()/2-180,getmaxy()/2-40,"YOU HAVE TO MOVE THROUGH THE RIGHT PATH");
	outtextxy(getmaxx()/2-180,getmaxy()/2-20,"TO THE DESTINATION");
	outtextxy(getmaxx()/2-180,getmaxy()/2,"MAZE WILL MOVE EXACTLY EVERY 5 SECONDS");
	outtextxy(getmaxx()/2-180,getmaxy()/2+20,"SINCE GAME STARTS: \"DON'T GET CRUSHED\"");

	outtextxy(getmaxx()/2-50,getmaxy()/2+40,"PRESS ANY KEY TO START...");
	getch();

	setviewport(getmaxx()/2-maxxy,getmaxy()/2-maxxy,getmaxx()/2+maxxy,getmaxy()/2+maxxy,1);
	clearviewport();
	setviewport(0,0,getmaxx(),getmaxy(),1);

	rectangle(getmaxx()/2-maxxy,getmaxy()/2-maxxy,getmaxx()/2+maxxy,getmaxy()/2+maxxy);

	while((c=getc(fp))!='9')
	{
		if(c=='x'){
			drawb(x,y);
			x+=20;
			state[i][j]=1;
			j++;
		}
		else if(c=='s'){
			drawst(x,y);
			xc=x;
			yc=y;
			x+=20;
			state[i][j]=2;
			xp=j;
			yp=i;
			j++;

		}
		else if(c=='g'){
			drawg(x,y);
			x+=20;
			state[i][j]=3;
			j++;
		}
		else if(c=='\n'){ y-=20; x=getmaxx()/2-maxxy+10; i++; j=0;}
		else{	x+=20; state[i][j]=0; j++; }
	}
	int k;
	float k2;
	start=clock();
	do{

		c='r';//cleaning fo next input...and avoiding anomaly if there's no input

		end=clock();
		k2=(end-start)/CLK_TCK;
		k=(k2*10);
		if(k==50){ k=loader(fp); if(k==1)c='q'; start=end; }

		if(kbhit())//listening to keyboard input...
			c=getch();

		if(c=='w'){
			if(state[yp+1][xp]==3){ outtext("YOU'VE DONE IT!!!!!"); c='q';}

		    if(yp+1!=20){
			if(state[yp+1][xp]!=1){
				state[yp][xp]=0;
				state[yp+1][xp]=2;
				yp++;
				setviewport(xc-5,yc-5,xc+5,yc+5,1);
				clearviewport();
				yc-=20;
				setviewport(0,0,getmaxx(),getmaxy(),1);
				drawst(xc,yc);
			}
		    }
		}

		else if(c=='s'){
			if(state[yp-1][xp]==3){ outtext("YOU'VE DONE IT!!!!!"); c='q';}

		    if(yp!=0){
			if(state[yp-1][xp]!=1){
				state[yp][xp]=0;
				state[yp-1][xp]=2;
				yp--;
				setviewport(xc-5,yc-5,xc+5,yc+5,1);
				clearviewport();
				yc+=20;
				setviewport(0,0,getmaxx(),getmaxy(),1);
				drawst(xc,yc);
			}
		    }
		}

		else if(c=='a'){
			if(state[yp][xp-1]==3){ outtext("YOU'VE DONE IT!!!!!"); c='q';}
		    if(xp!=0){
			if(state[yp][xp-1]!=1){
				state[yp][xp]=0;
				state[yp][xp-1]=2;
				xp--;
				setviewport(xc-5,yc-5,xc+5,yc+5,1);
				clearviewport();
				xc-=20;
				setviewport(0,0,getmaxx(),getmaxy(),1);
				drawst(xc,yc);
			}
		    }
		}

		else if(c=='d'){
			if(state[yp][xp+1]==3){ outtext("YOU'VE DONE IT!!!!!"); c='q';}
		    if(xp+1!=20){
			if(state[yp][xp+1]!=1){
				state[yp][xp]=0;
				state[yp][xp+1]=2;
				xp++;
				setviewport(xc-5,yc-5,xc+5,yc+5,1);
				clearviewport();
				xc+=20;
				setviewport(0,0,getmaxx(),getmaxy(),1);
				drawst(xc,yc);
			}
		    }
		}
	}while(c!='q');
	outtextxy(getmaxx()/2-100,20,"press ENTER to terminate.....");
	cin.ignore();
}