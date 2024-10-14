#include<graphics.h>
#include<iostream.h>
#include<stdio.h>
#include<stdlib.h>
#include<dos.h>
#include<time.h>
#include<math.h>
#include<conio.h>
#include<bios.h>
#include<ctype.h>

union REGS i,o;
char const max='X';
char const min='O';
char playGrid[3][3];
char player;

void arrayinitialize();


void initmouse()
{
   i.x.ax = 0;
   int86(0X33,&i,&o);
}

void showmouseptr()
{
   i.x.ax = 1;
   int86(0X33,&i,&o);
}

void hidemouseptr()
{
	i.x.ax=2;
	int86(0x33,&i,&o);
}

void getmousepos(int *button, int *x, int *y)
{
   i.x.ax = 3;
   int86(0X33,&i,&o);

   *button = o.x.bx;
   *x = o.x.cx;
   *y = o.x.dx;
}


class minimax
{
    private:
	char player;

    public:
	minimax()  //initializing the array; only needed in first node...
	{
		player=max;
		arrayinitialize();
	}

	void setplayer(char x)
	{
		player=x;
	}

	int check()
	{
		int count=0;
		for(int i=0;i<3;i++)
			for(int j=0;j<3;j++)
				if(playGrid[i][j]=='z')count++;

		return count;
	}

//checks if the game is at state where the next move would lead min or max player to win state...
// and provide the position where the move should be made either to win or to defend...
//attack simply means to search for the move that leads to win
//and defend is straightforward..
	int defendOrAttack(char p1,int *retval)
	{
		int flag;
		flag=DOA(p1,retval);//check for win position
		if(flag==1) return 1;
		else{
			if(p1==max)flag=DOA(min,retval);//no win then check for loss
			else flag=DOA(max,retval);//no win then check for loss
			if(flag==1) return 2;
		}
		return 0;//return value of 0 means no defend or attack, 1 means defend , 2 means attack..
	}

//DOA() is called by defendOrAttack() for actual checking,
//DOA() checks for the player passed in the arguement
//so both defend and offend check can be done,
//by switching player for a game state
	int DOA(char p1,int *retval)
	{

		char p2;
		if(p1==max)p2=min;
		else p2=max;
		int i,j;
		int stat=0;
		//row check
		for(i=0;i<3;i++){
			for(j=0;j<3;j++){
				if(playGrid[i][j]==p1){stat++; }
				else if(playGrid[i][j]==p2){ stat--; }
				else{ retval[1]=j; }
			}
			if(stat==2){ retval[0]=i; return 1;}
			stat=0;
		}
		stat=0;
		//column check
		for(i=0;i<3;i++){
			for(j=0;j<3;j++){
				if(playGrid[j][i]==p1){ ++stat; }
				else if(playGrid[j][i]==p2){ --stat; }
				else retval[0]=j;
			}
			if(stat==2){ retval[1]=i; return 1;}
			stat=0;
		}
		stat=0;
	    //diagonal check, top-left to bottom-right
		for(i=0;i<3;i++){
			if(playGrid[i][i]==p1){ ++stat;}
			else if(playGrid[i][i]==p2){ --stat;}
			else{ retval[0]=retval[1]=i; }//i.e. if playGrid[x][y]==0
		}
		if(stat==2){return 1;}
		stat=0;
		//diagonal check, top-right to bottom-left
		for(i=0;i<3;i++){
			if(playGrid[i][2-i]==p1){ ++stat;}
			else if(playGrid[i][2-i]==p2){ --stat;}
			else{ retval[0]=i;retval[1]=2-i; }//i.e. if playGrid[x][y]==0
		}
		if(stat==2){return 1;}

		return 0;
	}

	char inttochar(int i)
	{
		switch(i){
			case 0: return '0';
			case 1: return '1';
			case 2: return '2';
			case 3: return '3';
			case 4: return '4';
			case 5: return '5';
			case 6: return '6';
			case 7: return '7';
			case 8: return '8';
			default: return '9';
		}
	}

	void show(){
		int i,j;
		for(i=0;i<3;i++){
			for(j=0;j<3;j++){
				cout<<playGrid[i][j]<<" ";
			}
			cout<<endl;
		}
	}

	char find(char p,int *retval)
	{
		return search(p,retval);
	}

	char search(char p, int *retval)
	{
		char r='z';
		int stat=defendOrAttack(p,retval);
		int i,j;
		int counter=0;
		int flag=check();

		if(stat==1){return p;}//if win position is found, irrelevent to current game player, return
		else if(stat==2){//if defend position is found,  " " "
			i=retval[0];
			j=retval[1];
			playGrid[i][j]=p;
			if(p==max)r=search(min,retval);
			else r=search(max,retval);
			playGrid[i][j]='z';
			retval[0]=i;
			retval[1]=j;
			if(player==r) return r;//if win position of player was found then return player else return 'z'
			else return 'z';
		}
		else{ //if no defend or attack,search for every possible place

			i=random(3);
			j=random(3);
			do{
				if(playGrid[i][j]=='z'){
					if(flag!=1){
						playGrid[i][j]=p;
						if(p==max)r=search(min,retval);
						else r=search(max,retval);
						playGrid[i][j]='z';
						retval[0]=i;
						retval[1]=j;
						if(player==r) return r;//if win position of player was found then return player else return 'z'
					}
				}
				j=(j+1)%3;
				if(j==0)i=(i+1)%3;
				++counter;
			}while(counter<9);
		}
		return 'z';
	}

};

void initialdraw();
int draw(int, int);
void draw1(int,int);
int checker();
void p2mode();
void compsTurn(minimax,int *,int *);
void cvc();
void p1mode();

int main()
{
	randomize();
	int gd=DETECT,gm;
	initgraph(&gd,&gm,"/tc/bgi");
	back:
	int x=43;
	int b,y=0;
	settextstyle(1,HORIZ_DIR,4);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	setcolor(BLUE);
	setlinestyle(SOLID_LINE,0xffff,3);
	rectangle(getmaxx()/2-80,getmaxy()/2-60,getmaxx()/2+80,getmaxy()/2-40);
	rectangle(getmaxx()/2-80,getmaxy()/2-30,getmaxx()/2+80,getmaxy()/2-10);
	rectangle(getmaxx()/2-80,getmaxy()/2,getmaxx()/2+80,getmaxy()/2+20);
	rectangle(getmaxx()/2-80,getmaxy()/2+30,getmaxx()/2+80,getmaxy()/2+50);
	setfillstyle(SOLID_FILL,RED);
	for(int i=0;i<4;i++)
	{
		floodfill(getmaxx()/2,getmaxy()/2-x,BLUE);
		x-=30;
	}
	for(int j=0;j<8;j++){
		rectangle(y+1,0,y+100,getmaxy());
		setcolor(j+4);
		y+=100;
	}
	rectangle(getmaxx()/2-80,getmaxy()/2-60,getmaxx()/2+80,getmaxy()/2-40);
	rectangle(getmaxx()/2-80,getmaxy()/2-30,getmaxx()/2+80,getmaxy()/2-10);
	rectangle(getmaxx()/2-80,getmaxy()/2,getmaxx()/2+80,getmaxy()/2+20);
	rectangle(getmaxx()/2-80,getmaxy()/2+30,getmaxx()/2+80,getmaxy()/2+50);
	setcolor(j+2);
	outtextxy(getmaxx()/2,getmaxy()/2-100,"TIC-TAC-TOE");
	settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
	outtextxy(getmaxx()/2,getmaxy()/2-50,"COMPUTER-V-HUMAN");
	outtextxy(getmaxx()/2,getmaxy()/2-20,"2 PLAYER MODE");
	outtextxy(getmaxx()/2,getmaxy()/2+10,"COMPUTER-V-COMPUTER");
	outtextxy(getmaxx()/2,getmaxy()/2+40,"EXIT");
	initmouse();
	showmouseptr();
	again:
	getmousepos(&b,&x,&y);
	if(b==1){
		b=-1;
		if(abs(x-getmaxx()/2)<=80){
			if((y-getmaxy()/2)<=(-40)&&(y-getmaxy()/2)>=(-60)){
				hidemouseptr();
				cleardevice();
				p1mode();
				hidemouseptr();
				cleardevice();
				goto back;
			}
			else if((y-getmaxy()/2)<=(-10)&&(y-getmaxy()/2)>=(-30)){
				hidemouseptr();
				cleardevice();
				p2mode();
				hidemouseptr();
				cleardevice();
				goto back;
			}
			else if(y<=(getmaxy()/2+20)&&y>=getmaxy()/2){
				hidemouseptr();
				cleardevice();
				cvc();
				hidemouseptr();
				cleardevice();
				goto back;;
			}
			else if(y<=(getmaxy()/2+50)&&y>=(getmaxy()/2+30)){goto end;}
			else goto again;
		}
	}
	goto again;
	end:
       //	getch();
	return 1;
}




//p2mode();


void arrayinitialize()
{
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			playGrid[i][j]='z';
}

void initialdraw()
{

	int x,y;
	x=getmaxx()/2;
	y=getmaxy()/2;

	//initmouse();
	setlinestyle(SOLID_LINE,0xffff,3);
	rectangle(x-20,y-20,x+20,y+20);
	line(x-20,y-20,x-60,y-20);
	line(x-20,y-20,x-20,y-60);
	line(x+20,y+20,x+60,y+20);
	line(x+20,y+20,x+20,y+60);
	line(x-20,y+20,x-60,y+20);
	line(x-20,y+20,x-20,y+60);
	line(x-20,y-20,x+60,y-20);
	line(x+20,y-20,x+20,y-60);

	setcolor(BLUE);
	outtextxy(getmaxx()-40,getmaxy()-10,"EXIT");

	showmouseptr();


}

int draw(int x, int y)//fills by taking mouse co-ordinates
{
	hidemouseptr();
	setcolor(WHITE);
	int c,d,x1,x2,y1,y2;
	c=getmaxx()/2;
	d=getmaxy()/2;

	int i,j;
	if(abs(x-c)<20){ x1=c-10; x2= c+10; i=2; }
	else if(abs(x-c+40)<20){ x1=c-50; x2=c-30;i=3; }
	else if(abs(x-c-40)<20){ x1=c+30; x2=c+50; i=1;}
	else return 0;
	if(abs(y-d)<20){ y1=d-10; y2= d+10;j=2; }
	else if(abs(y-d+40)<20){ y1=d-50; y2=d-30;j=3; }
	else if(abs(y-d-40)<20){ y1=d+30; y2=d+50; j=1;}
	else  return 0;
	if(playGrid[i-1][j-1]=='z')playGrid[i-1][j-1]=player;
	else return 0;
	if(player==max)
	{
		line(x1,y1,x2,y2);
		line(x2,y1,x1,y2);
	}
	else if(player==min){
		circle(x1+10,y1+10,10);
	}
	showmouseptr();
	return 1;
}

void draw1(int x, int y)//fills by taking 3*3 matrix co-ordinates
{
	hidemouseptr();
	setcolor(WHITE);
	int c,d,x1,x2,y1,y2;
	c=getmaxx()/2;
	d=getmaxy()/2;
	if(x==1){ x1=c-10; x2= c+10;}
	else if(x==2){ x1=c-50; x2=c-30;}
	else if(x==0){ x1=c+30; x2=c+50;}
	else ;

	if(y==1){ y1=d-10; y2= d+10;}
	else if(y==2){ y1=d-50; y2=d-30;}
	else if(y==0){ y1=d+30; y2=d+50;}
	else ;
	if(player==max)
	{
		line(x1,y1,x2,y2);
		line(x2,y1,x1,y2);
	}
	else{
		circle(x1+10,y1+10,10);
	}
	showmouseptr();
}

int checker()
{
	setcolor(GREEN);
	char text[100];
	int stat=0,stat1=0,stat2=0;
	int i,j;
	for(i=0;i<3;i++)     //row check;
	{	for(j=0;j<3;j++)
		{
			if(playGrid[i][j]==max)stat1++;
			if(playGrid[i][j]==min)stat2++;
		}

		if(stat1==3||stat2==3) goto move;
		stat1=0; stat2=0;
	}
	for(j=0;j<3;j++)   //column check;
	{	for(i=0;i<3;i++)
		{
			if(playGrid[i][j]!='z')stat++;
			if(playGrid[i][j]==max)stat1++;
			if(playGrid[i][j]==min)stat2++;
		}

		if(stat1==3||stat2==3) goto move;
		stat1=0; stat2=0;
	}
	j=0;
	for(i=0;i<3;i++)   //diagonal check;
	{
		if(playGrid[i][j]==max)stat1++;
		if(playGrid[i][j]==min)stat2++;
		j++;
	}

	j=2;
	if(stat1==3||stat2==3)goto move;
	stat1=0; stat2=0;
	for(i=0;i<3;i++)     //diagonal check;
	{
		if(playGrid[i][j]==max)stat1++;
		if(playGrid[i][j]==min)stat2++;
		j--;
	}
	move:
	if(stat1==3||stat2==3){
		sprintf(text,"player %c won",player);
		outtextxy(getmaxx()/2,getmaxy()-40,text);

		return 1;
	}
	if(stat==9){ outtextxy(getmaxx()/2,getmaxy()-40,"ITS A TIE"); return 1;}

	return 0;
}


void p2mode()
{
	int x,y,b,c,d,key;
	char array[100];
	int stat;
	initialdraw();
	arrayinitialize();
	player=min;
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
	settextjustify(LEFT_TEXT,CENTER_TEXT);
	setviewport(0,0,getmaxx(),40,0);
	clearviewport();
	int col=getbkcolor();
	for(int i=0;i<5;i++){
		if(i%3==0){
			setcolor(col);
			outtextxy(280,20,"LOADING......");
			setcolor(GREEN);
			outtextxy(280,20,"LOADING..");
		}
		else if(i%3==1){
			setcolor(col);
			outtextxy(280,20,"LOADING..");
			setcolor(GREEN);
			outtextxy(280,20,"LOADING....");
		}
		else{
			setcolor(col);
			outtextxy(280,20,"LOADING....");
			setcolor(GREEN);
			outtextxy(280,20,"LOADING......");
		}
	}

	settextjustify(CENTER_TEXT,CENTER_TEXT);
	setcolor(GREEN);
	clearviewport();
	outtextxy(350,30,"READY");
	delay(500);
	clearviewport();

	do{
		if(player==max)player=min;  //switching players
		else player=max;
		setviewport(0,0,getmaxx(),50,0);
		clearviewport();
		setcolor(GREEN);
		sprintf(array,"Player %c's turn",player);
		outtextxy(350,30,array);
		setviewport(0,0,getmaxx(),getmaxy(),0);
		again:
		getpos:
		getmousepos(&b,&x,&y);
		if(b!=1)goto getpos;

		b=-1;
		if(abs(x-getmaxx())<60 && abs(y-getmaxy())<16){//when you click on exit
			setcolor(BLUE);
			outtextxy(getmaxx()/2,getmaxy()/2,"TERMINATING");
			delay(500);
			return;
		}
		if(y<(getmaxy()/2-60)) goto getpos;
		else if(y>(getmaxy()/2+60)) goto getpos;
		else if(x>(getmaxx()/2+60)) goto getpos;
		else if(x<(getmaxx()/2-60)) goto getpos;
		else{
			hidemouseptr();
			stat=draw(x,y);
			showmouseptr();
			if(stat==0)goto getpos;
		}
		stat=checker();
		if(stat==1){break;}
	}while(bioskey(1)!=0x11b);

	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	outtextxy(getmaxx()/2,getmaxy()-70,"CLICK ON EXIT TO GO BACK TO MENU");
	getposL:
	b=-1;
	getmousepos(&b,&x,&y);
	if(b!=1)goto getposL;
	if(abs(x-getmaxx())<60 && abs(y-getmaxy())<16);//when you click on exit
	else goto getposL;
	end:
	delay(500);
     //	closegraph();
}



void compsTurn(minimax comp,int *xi,int *yj)
{
	int x,y;
	int retval[2];
	int array[2];
	int flag=comp.check();
	if(flag>7){//if less than 3 moves made so far|| less than 3 places filled..
		do{
			x=random(3);
			y=random(3);
		 }while(playGrid[x][y]!='z');
		 playGrid[x][y]=player;
	}
	else{//if 2 or more moves have been made
		if(flag==1){//if only one place remains to be filled..
			for(x=0;x<3;x++){
				for(y=0;y<3;y++){
					if(playGrid[x][y]=='z') break;
				}
				if(playGrid[x][y]=='z') break;
			}
			playGrid[x][y]=player;
		}
		else{//if more than one place remains
			array[0]=comp.find(player,retval);//search for place that leads to win state for current player
			if(player==max)array[1]=min;
			else array[1]=max;
			if(array[0]==player){//if yes
				x=retval[0];
				y=retval[1];
				playGrid[x][y]=player;
			}
			else{//if no places leads to win state for current player
				if(player==max)comp.setplayer(min);
				else comp.setplayer(max);
				if(comp.DOA(array[1],retval)==1){//check if the player have to defend
					//if yes
					x=retval[0];
					y=retval[1];
					playGrid[x][y]=player;
				}
				else{//if no
					//little change required//antisearch
					x=random(3);
					y=random(3);
					while(playGrid[x][y]!='z'){
						y=(y+1)%3;
						if(y==0)x=(x+1)%3;
					}
					playGrid[x][y]=player;
				}
			}
		}
	}
	*xi=x;
	*yj=y;
}

void cvc()
{
	int b,x,y;
	char array[20];
	minimax comp;
	initialdraw();
	player=min;
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
	settextjustify(LEFT_TEXT,CENTER_TEXT);
	setviewport(0,0,getmaxx(),40,0);
	clearviewport();
	int col=getbkcolor();
	for(int i=0;i<10;i++){
		if(i%3==0){
			setcolor(col);
			outtextxy(280,20,"LOADING......");
			setcolor(GREEN);
			outtextxy(280,20,"LOADING..");
		}
		else if(i%3==1){
			setcolor(col);
			outtextxy(280,20,"LOADING..");
			setcolor(GREEN);
			outtextxy(280,20,"LOADING....");
		}
		else{
			setcolor(col);
			outtextxy(280,20,"LOADING....");
			setcolor(GREEN);
			outtextxy(280,20,"LOADING......");
		}
	}

	settextjustify(CENTER_TEXT,CENTER_TEXT);
	setcolor(GREEN);
	clearviewport();
	outtextxy(350,30,"READY");
	delay(500);
	clearviewport();
	do{
		if(player==max)player=min;  //switching players
		else player=max;
		comp.setplayer(player);//setting player in minimax object comp
		setviewport(0,0,getmaxx(),80,0);
		clearviewport();
		setviewport(0,0,getmaxx(),getmaxy(),0);
		setcolor(GREEN);
		sprintf(array,"Computer %c's turn",player);
		outtextxy(350,30,array);
		sprintf(array,"%c is thinking",player);
		outtextxy(350,60,array);

		compsTurn(comp,&x,&y);

//the two following for statements are for delay effect to show computer is thinking
//and at the same time checks for mouse event, if user clicked on exit
		for(clock_t start=clock(),end=clock();((end-start)/CLK_TCK)<1;end=clock())
		{
			getmousepos(&b,&i,&col);
			if(b==1){
				b=-1;
				if(abs(i-getmaxx())<60 && abs(col-getmaxy())<16){
					setcolor(BLUE);
					outtextxy(getmaxx()/2,getmaxy()/2,"TERMINATING");
					delay(1000);
					return;//when you click on exit
				}
			}
		}

		draw1(x,y);//to prompt the move made in graphics


		for(start=clock(),end=clock();((end-start)/CLK_TCK)<1;end=clock())
		{
			getmousepos(&b,&x,&y);
			if(b==1){
				b=-1;
				if(abs(x-getmaxx())<60 && abs(y-getmaxy())<16){
					setcolor(BLUE);
					outtextxy(getmaxx()/2,getmaxy()/2,"TERMINATING");
					delay(1000);
					return;//when you click on exit
				}
			}
		}

		if(checker()) break;

	}while(1);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	outtextxy(getmaxx()/2,getmaxy()-70,"CLICK ON EXIT TO GO BACK TO MENU");
	getposL:
	b=-1;
	getmousepos(&b,&x,&y);
	if(b!=1)goto getposL;
	if(abs(x-getmaxx())<60 && abs(y-getmaxy())<16);//when you click on exit
	else goto getposL;
	delay(500);
}

char prompt()
{
	delay(500);
	int b,x,y;
	setcolor(BLUE);
	setlinestyle(SOLID_LINE,0xffff,3);
	rectangle(getmaxx()/2-100,getmaxy()/2-60,getmaxx()/2-20,getmaxy()/2-40);
	rectangle(getmaxx()/2+100,getmaxy()/2-60,getmaxx()/2+20,getmaxy()/2-40);
	setfillstyle(SOLID_FILL,DARKGRAY);
	floodfill(getmaxx()/2-30,getmaxy()/2-30,BLUE);
	floodfill(getmaxx()/2+60,getmaxy()/2-30,BLUE);

	setcolor(GREEN);
	outtextxy(getmaxx()/2,getmaxy()/2-100,"Choose: player X || player O");
	outtextxy(getmaxx()/2-60,getmaxy()/2-50,"Player X");
	outtextxy(getmaxx()/2+60,getmaxy()/2-50,"Player 0");
	showmouseptr();
	get:
	do{
		getmousepos(&b,&x,&y);
	}while(b!=1);
	b=-1;
	if((y-getmaxy()/2)<=(-40)&&(y-getmaxy()/2)>=(-60)){
		if(!(abs(x-getmaxx()/2)<=20)){
			if(abs(x-getmaxx()/2)<=100){
				if(x<getmaxx()/2){ hidemouseptr();outtextxy(getmaxx()/2,getmaxy()/2,"You Chose X");delay(1000); return max;}
				if(x>getmaxx()/2){ hidemouseptr();outtextxy(getmaxx()/2,getmaxy()/2,"You Chose O");delay(1000); return min;}
			}
		}
	}
	else goto get;

}

void p1mode()
{
	int b,x,y;
	char array[20];
	minimax comp;
	char turn=min;
	char flag='F';
	if(prompt()==max)player=min;
	else player=max;
	cleardevice();
	initialdraw();
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
	settextjustify(LEFT_TEXT,CENTER_TEXT);
	setviewport(0,0,getmaxx(),60,0);
	clearviewport();
	int col=getbkcolor();
	for(int i=0;i<10;i++){
		if(i%3==0){
			setcolor(col);
			outtextxy(280,20,"LOADING......");
			setcolor(GREEN);
			outtextxy(280,20,"LOADING..");
		}
		else if(i%3==1){
			setcolor(col);
			outtextxy(280,20,"LOADING..");
			setcolor(GREEN);
			outtextxy(280,20,"LOADING....");
		}
		else{
			setcolor(col);
			outtextxy(280,20,"LOADING....");
			setcolor(GREEN);
			outtextxy(280,20,"LOADING......");
		}
	}

	settextjustify(CENTER_TEXT,CENTER_TEXT);
	setcolor(GREEN);
	clearviewport();
	outtextxy(350,30,"READY");
	delay(500);
	clearviewport();
	do{
		if(turn==max)turn=min;  //switching players
		else turn=max;
		comp.setplayer(player);//setting player in minimax object comp
		setviewport(0,0,getmaxx(),100,0);
		clearviewport();
		setcolor(GREEN);
		if(turn==player){
			sprintf(array,"Computer's turn",player);
			outtextxy(350,30,array);
			sprintf(array,"Computer is thinking",player);
			outtextxy(350,60,array);

			compsTurn(comp,&x,&y);

			delay(1000);
			draw1(x,y);
		}
		else{
			flag='T';
			player=turn;
			sprintf(array,"Your turn",player);
			outtextxy(350,30,array);
			sprintf(array,"Make a move",player);
			outtextxy(350,60,array);

			getpos:
			getmousepos(&b,&x,&y);
			if(b!=1)goto getpos;

			b=-1;
			if(abs(x-getmaxx())<60 && abs(y-getmaxy())<16){//when you click on exit
				setcolor(BLUE);
				outtextxy(getmaxx()/2,getmaxy()/2,"TERMINATING");
				delay(500);
				return;
				}
			if(y<(getmaxy()/2-60)) goto getpos;
			else if(y>(getmaxy()/2+60)) goto getpos;
			else if(x>(getmaxx()/2+60)) goto getpos;
			else if(x<(getmaxx()/2-60)) goto getpos;
			else{
				hidemouseptr();
				x=draw(x,y);
				showmouseptr();
				if(x==0)goto getpos;
			}
		}

		if(checker()) break;
		else delay(1000);

		if(flag=='T'){
			if(turn==max)player=min;
			else player=max;
			flag='F';
		}
	}while(1);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	outtextxy(getmaxx()/2,getmaxy()-70,"CLICK ON EXIT TO GO BACK TO MENU");
	getposL:
	b=-1;
	getmousepos(&b,&x,&y);
	if(b!=1)goto getposL;
	if(abs(x-getmaxx())<60 && abs(y-getmaxy())<16);//when you click on exit
	else goto getposL;
	delay(500);
}