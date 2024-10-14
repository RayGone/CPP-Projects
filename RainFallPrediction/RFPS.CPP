#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <iostream.h>
#include <process.h>



void welcomeimagecopier()
{
	FILE *fp;
	int color;
	if((fp=fopen("welcome","w"))==NULL){
		outtext("file cannot be opened");
		return;
	}
	else{
		for(int i=0;i<getmaxy();i++){
			for(int j=0;j<getmaxx();j++){
				color=getpixel(j,i);
				fwrite(&color, sizeof(color), 1, fp);
			}
		}
	}
}

void welcomeimagepaste()
{
	FILE *fp;
	int color;
	if((fp=fopen("welcome","r"))==NULL){
		outtext("file cannot be opened");
		return;
	}
	else{
		for(int i=0;i<getmaxy();i++){
			for(int j=0;j<getmaxx();j++){
				fread(&color, sizeof(color), 1, fp);
				putpixel(j,i,color);
			}
		}
	}
}

void analyseimagepaste()
{
	FILE *fp;
	int color;
	int midx=getmaxx()/2;
	int midy=getmaxy()/2;
	if((fp=fopen("analyse","r"))==NULL){
		outtext("file cannot be opened");
		return;
	}
	else{
		for(int i=0;i<getmaxy();i++){
			for(int j=0;j<getmaxx();j++){
			    if(abs(j-midx)<=200 && abs(i-midy)<=150){
				fread(&color, sizeof(color), 1, fp);
				putpixel(j,i,color);
			    }
			    else fread(&color, sizeof(color), 1, fp);
			}
		}
	}
}



void rain()
{
	int x[100];
	int y[100];
	int flag=0;
	randomize();
	for(int i=0;i<100;i++){
		x[i]=random(getmaxx()-10);
		y[i]=random(getmaxy()-10);
	}
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	settextstyle(1,HORIZ_DIR,2);
	i=0;
	while(1){
		if(i<120){
			setcolor(getmaxcolor());
			outtextxy(getmaxx()/2,getmaxy()/2-50-i*2,"WELCOME TO RAINFALL PREDICTION SYSTEM");
		}
		for(int j=0;j<5;j++)
		{
		    putpixel(random(getmaxx()-10),random(getmaxy()-10),random(10));

		    setcolor(WHITE);

		}
		{
		//formula of rain co-ordinate: (x-2*i,x-5*i,x-2*(i+1),x-5*(i+1));
			if(flag==0){
				for(j=0;j<100;j++){
					flag=8;
					setcolor(BLACK);
					line(x[j]-2*flag,y[j]-5*flag,x[j]-2*(flag+1),y[j]-5*(flag+1));
				}
				if(i%3==0){
						for(int i=0;i<100;i++){
							x[i]=random(getmaxx()-10);
							y[i]=random(getmaxy()-10);
						}
				}
				for(j=0;j<100;j++){
					setcolor(WHITE);
					flag=0;
					line(x[j]-2*flag,y[j]-5*flag,x[j]-2*(flag+1),y[j]-5*(flag+1));
				}
				flag=4;
			}
			if(flag==4){
				for(j=0;j<100;j++){
					setcolor(BLACK);
					flag=0;
					line(x[j]-2*flag,y[j]-5*flag,x[j]-2*(flag+1),y[j]-5*(flag+1));
					setcolor(WHITE);
					flag=4;
					line(x[j]-2*flag,y[j]-5*flag,x[j]-2*(flag+1),y[j]-5*(flag+1));
				}
				flag=8;
			}
			if(flag==8){
				for(j=0;j<100;j++){
					setcolor(BLACK);
					flag=4;
					line(x[j]-2*flag,y[j]-5*flag,x[j]-2*(flag+1),y[j]-5*(flag+1));
					setcolor(WHITE);
					flag=8;
					line(x[j]-2*flag,y[j]-5*flag,x[j]-2*(flag+1),y[j]-5*(flag+1));
				}
				flag=0;
			}
		}

		if(i<120){
			delay(10);
			setcolor(BLACK);
			outtextxy(getmaxx()/2,getmaxy()/2-50-i*2,"WELCOME TO RAINFALL PREDICTION SYSTEM");
			i++;
		}
		else break;
		if(kbhit()){
			if(getch()=='q') break;
		}
	}
	setcolor(getmaxcolor());
	outtextxy(getmaxx()/2,getmaxy()/2-50-i*2,"WELCOME TO RAINFALL PREDICTION SYSTEM");

 //	setcolor(BLACK);
 //	outtextxy(getmaxx()/2,getmaxy()/2-50-80*2,"WELCOME TO RAINFALL PREDICTION SYSTEM");
 //	imagecopier();
	setcolor(BLUE);
	outtextxy(getmaxx()/2,getmaxy()/2,"Press any key");
	getch();
}



main()
{
	int gd=DETECT, gm;
	initgraph(&gd,&gm,"/tc/bgi");
	delay(1000);
	rain();
	closegraph();
//	delay(3000);
	spawnl(P_OVERLAY,"sample.exe",NULL);


}