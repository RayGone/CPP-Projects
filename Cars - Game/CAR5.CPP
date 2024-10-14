#include<conio.h>
#include<stdio.h>
#include<dos.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<graphics.h>
#include<process.h>

int xa,ya;//center co-ordinate of the player, supposedly a car;
int counter;//new car is to define only after 10 counts...for keeping distance purpose..
int last;//to know where the last car came out from,,left or right lane...//points to carrs[100];
int on[10];//this determines which cars object is active at the moment...
int carrs[30]; //state of cars
int n;//score...
int t;//value of delay for controlling the speed of the game...
int tt;//to show the elapsed time of the game...
int level;//there are total 5 levels of the game...determined by the speed...
int change;//for the purpose of changing the states in state definer....


class cars
{
	int x, y;//position of the car...
	int l;//lane the car is driving//left (1) or right (0)

	public:
	cars(){	y=20; l=3;}//constructor initializing y value...

	void lane(int x){ l=x;carlane();}  //argument x determines the lane the car is on
	void carlane(){  //giving x value to the object as per the corresponding
	// l (lane) value from carrs[];
		if(l==1)x=getmaxx()/2-40;
		else x=getmaxx()/2+40;
	}

	void pos() //same as car() for player...
	{
		setviewport(x-20,y-20,x+20,y+20,0);
		rectangle(0,0,40,40);
		setfillstyle(11,15);
		floodfill(1,1,getmaxcolor());
	}
	void incrementer()
	{
		y=y+4;//moving cars 4 pixel per round...
	}
//gives motion to car objects
	void move()
	{
		setviewport(x-20,y-20,x+20,y+20,0);
		clearviewport();
		incrementer();
		pos();
	}

	//checks if player crashed with car object
	int crashcheck()
	{
		char msg[26];
		if(xa==x&&abs(ya-y)<40) return 1;
		else return 0;
	}

	int check(){ //end of road check;;
		if(y>getmaxy()-30) return 1;
		else return 0;
	}

	void reloader()//if a car reaches end of road,,its y value is reloaded...
	{
		setviewport(x-20,y-20,x+20,y+20,0);
		clearviewport();
		y=20;
	}

};


void score() //to print the score and level of the game...
{
	char msg[20];
	sprintf(msg,"SCORE: %d",n);
	setviewport(5,0,100,80,0);
	clearviewport();
	outtextxy(10,20,msg);
	sprintf(msg,"LEVEL: %d",level);
	outtextxy(10,40,msg);
}
//these three function are defined here because they are used in objectdefiner...
//and since object definer was defined before these initially, the program didn't work//
//so i did this...
void draw();
void car();
void statedefiner();

void end()//if the car crashes this function is envoked for the termination...
{
	char ch;
	setviewport(0,0,getmaxx(),getmaxy(),0);
	outtextxy(getmaxx()/2+150,getmaxy()/2,"YOU CRASHED.......");
	outtextxy(getmaxx()/2+120,getmaxy()/2+20,"press q to terminate");
	delay(1200);
	bac:
	ch=getch();
	if(ch=='q') spawnl(P_OVERLAY,"carindex.exe",NULL);
	else goto bac;
}

void objectdefiner(cars *obj[])  //all the good things happens here...
{
	static int m; //keeps track of first inactive object...
	int k,j=0;
	int stat=0;
	for(int i=0;i<10;i++) if(on[i]!=0){ stat=1; break;}
	if(stat==0) m=0; //should be true only at starting of game...
	i=m;//reloading m...description given 5 lines below;
	if(counter==0){
		while(on[i]!=0){ i=(i+1)%10; }//searching for not active object to make it active..
		if(carrs[last]!=2){
			on[i]=1; m=(i+1)%10; //m=i+1 makes the searching of the not active
				//object from next position from last in next loop...
			obj[i]->lane(carrs[last]);
		}

		last=(last+1)%30;//counting through total states;
	    //next phase of states;
		if(last==0)statedefiner();
	}
	for(i=0;i<10;i++){
		if(on[i]==1){
			obj[i]->move();
			j=obj[i]->crashcheck();
		}
		if(j==1) {end();}
		k=obj[i]->check();
		if(k==1){
			obj[i]->reloader();
			on[i]=0;
			n++;
			if(n==20){t-=20;level=1;} //1st phase..
			else if(n==50){t-=10;level=2;}//2nd phase..
			else if(n==90){t-=8;level=3;}//3rd phase..
			else if(n==120){t-=6;level=4;}//4th pahse...
			else if(n==140){t-=6;level=5;;}//5th and final phase..
			score();
		}
		k=0;
	}

	counter=(counter+1)%12;/* the delay in the main is calling
		this objectdefiner() function 20 times per second and
i.e.delay(50)//	increment of y-cordinate of car is 5 pixels per call
initially///	and the length of the car is 40 pixels..thus 12 call
		results to 60 pixels increment, so it gives us 20 pixel
		distance between every subsequent car...80 pixel if differnt
		lane......*/
}

void draw()//this draws the initial environment...
{
	setbkcolor(9);
	rectangle(getmaxx()/2-5,0,getmaxx()/2+5,getmaxy());
	line(getmaxx()/2-85,0,getmaxx()/2-85,getmaxy());
	line(getmaxx()/2+85,0,getmaxx()/2+85,getmaxy());
	setfillstyle(SOLID_FILL, 8);
	floodfill(1,1,getmaxcolor());
	setfillstyle(SOLID_FILL, 1);
	floodfill(getmaxx(),getmaxy(),getmaxcolor());
	setfillstyle(SLASH_FILL, 15);
	floodfill(getmaxx()/2,getmaxy()/2,getmaxcolor());
}

void car()//this draws player car..
{
	setviewport(xa-15,ya-15,xa+20,ya+15,0);
	rectangle(0,0,30,30);
	setfillstyle(11,15);
	floodfill(1,1,getmaxcolor());
}

int value(char dir) //movement of the player...
{
	if(dir=='a') return getmaxx()/2-40;
	if(dir=='d') return getmaxx()/2+40;
	if(dir=='w') if(ya>15) return ya-5;
	if(dir=='s') if(ya<(getmaxy()-15)) return ya+5;
	return 0;
}

void statedefiner() //0;-left lane, 1;-right lane, 2:- no car
{
	if(change==2){
	carrs[0]=2;  carrs[1]=0;  carrs[2]=2;  carrs[3]=1;  carrs[4]=2;
	carrs[5]=1;  carrs[6]=2;  carrs[7]=0;  carrs[8]=2;  carrs[9]=1;
	carrs[10]=2; carrs[11]=0; carrs[12]=0; carrs[13]=2; carrs[14]=1;
	carrs[15]=2; carrs[16]=0; carrs[17]=2; carrs[18]=1; carrs[19]=2;
	carrs[20]=0; carrs[21]=2;
	carrs[22]=1; carrs[23]=2; carrs[24]=1; carrs[25]=2; carrs[26]=0;
	carrs[27]=0; carrs[28]=2; carrs[29]=1; change=0;}

	if(change==1){
	carrs[0]=2; carrs[1]=1; carrs[2]=2; carrs[3]=0; carrs[4]=2; carrs[5]=1;
	carrs[6]=1; carrs[7]=2; carrs[8]=0; carrs[9]=2; carrs[10]=1; carrs[11]=1;
	carrs[12]=2; carrs[13]=0; carrs[14]=2; carrs[15]=1; carrs[16]=2;
	carrs[17]=0;  carrs[18]=2; carrs[19]=1;carrs[20]=2;carrs[20]=0; carrs[21]=2;
	carrs[22]=1; carrs[23]=2; carrs[24]=0; carrs[25]=2; carrs[26]=1;
	carrs[27]=1; carrs[28]=2; carrs[29]=0;  change=0;}

	if(change==0){carrs[0]=2; carrs[1]=0;
	carrs[2]=2; carrs[3]=1; carrs[4]=2; carrs[5]=0; carrs[6]=2; carrs[7]=1;
	carrs[8]=1; carrs[9]=2; carrs[10]=0; carrs[11]=0; carrs[12]=2; carrs[13]=1;
	carrs[14]=2; carrs[15]=0; carrs[16]=0; carrs[17]=2; carrs[18]=1; carrs[19]=1;
	carrs[20]=2; carrs[21]=0; carrs[22]=2; carrs[23]=1; carrs[24]=2; carrs[25]=0;
	carrs[26]=2; carrs[27]=1; carrs[28]=2; carrs[29]=0;change=2;}

}

main()
{
	int gd=DETECT,gm;
	clock_t start, end;
	start=clock();
	t=50;
	char c='a',d=c,mg[40];
	counter=0;//global variable, described at begining//used in objectdefiner() as others
	last=0;//bcoz (last+1)%100=0,,global variable described at begining..
	n=level=change=0;//initially score and level is 0, so is change;global variable described at begining..
			//n and level used in score()...
	cars *obj[10];
	for(int i=0;i<10;i++){ on[i]=0; obj[i]=new cars; }
	statedefiner();//initiates the car out timings....
	initgraph(&gd,&gm,"/tc/bgi ");
	restart:
	clearviewport();
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
	draw();
	score();
	ya=getmaxy()-120;
	xa=getmaxx()/2-40;
	delay(1500);
	car();
	int sec,min=0,ss=0;
	delay(150);

   do{
	delay(t);//speed of the game...

	if(kbhit())	c=getch();

	if(c=='p'){
		setviewport(420,50,getmaxx()-20,90,0);
		clearviewport();
		sprintf(mg,"!!PAUSED......");
		outtextxy(5,10,mg);
		sprintf(mg,"press r to continue");
		outtextxy(5,20,mg);
		back:
		c=getch();
		if(c!='r') goto back;
		else clearviewport();
	}

	if(c!='q'&&c!='a'&&c!='d'&&c!='w'&&c!='s') goto ahead;

	if(d==c)goto ahead;

	if(c=='a'||c=='d'){
		setviewport(xa-20,ya-20,xa+20,ya+20,0);
		clearviewport();
		xa=value(c);
		car();
		d=c;
	}

	if(c=='w'||c=='s'){
		setviewport(xa-20,ya-20,xa+20,ya+20,0);
		clearviewport();
		ya=value(c);
		car();
		c='r';
	}

	ahead:
	objectdefiner(obj);
	end=clock();
	tt=(end-start)/CLK_TCK;
	if(tt%30==0) ;
	sec=tt%60;
	setviewport(5,50,100,70,0);
	clearviewport();
	sprintf(mg,"TIME: %d:%d",min,sec);
	outtextxy(5,10,mg);
	if(sec==59&&ss!=sec)min++;
	ss=sec;

   }while(c!='q');
   spawnl(P_OVERLAY,"carindex.exe",NULL);
   return 0;
}