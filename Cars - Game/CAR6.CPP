#include<conio.h>
#include<stdio.h>
#include<dos.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<graphics.h>
#include<process.h>

int xa,ya;//center co-ordinate of the player, supposedly a car;
int counter;//new car is to be defined only after 10 counts...for keeping distance purpose..
int last;//to know where the last car came out from,,left or right lane...//points to carrs[100];
int on[12];//this determines which object is active at the moment...
int carrs[30]; //state of cars
int n;//score...
int t;//value of delay for controlling the speed of the game...
int tt;//to show the elapsed time of the game...
int level;//there are total 5 levels of the game...determined by the speed...
int change;//for the purpose of changing the states in state definer....
int dec;//its the value used to controll spacing between cars by increasing
		//or decreasing the no.of round for new object be to set active..

class cars
{
	int x, y;//position of the car...
	int l;//lane the car is driving//0:-left lane, 1:-middle lane, 2:-right lane;
	int score;
	public:

	cars(){	y=20; l=3; score=0;}//constructor initializing y value...

	void lane(int x){ l=x;carlane();}  //argument x determines the lane the car is on
	void carlane(){  //giving x value to the object as per the corresponding
	// l (lane) value from carrs[];
		if(l==0)x=getmaxx()/2-45;
		if(l==1) x=getmaxx()/2+45;
		if(l==2) x=getmaxx()/2+135;
	}

//this is not necessary in car5 i.e. two lane game..
//in 3 lane game, two cars can together in different lane...
//this sets which object actually increases the score
//i.e.increase the score of player only if it encounters the car object with score=1;
	void scorer(){ score=1; }

	int returnscorer(){ return score;}

	void pos() //same as cars() for player...
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

   //gives motion to other cars...
	void move()
	{
		setviewport(x-20,y-20,x+20,y+20,0);
		clearviewport();
		incrementer();
		pos();
	}

   //checks if the player crashed with other cars or not...
	int crashcheck()
	{
		char msg[26];
		if(xa==x&&abs(ya-y)<39) return 1;
		else return 0;
	}

   //player can't  move left or right in a lane but can switch lane...
   //so this function checks whether the direction player is suggesting is where the player is already at..
	int check(){ //end of road check;;
		if(y>getmaxy()-30) return 1;
		else return 0;
	}

   //simply reinitializes y-val
	void reloader()//if a car reaches end of road,,its y value is reloaded...
	{
		setviewport(x-20,y-20,x+20,y+20,0);
		clearviewport();
		y=20;
		score=0;
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
	outtextxy(15,getmaxy()/2,"YOU CRASHED.......");
	outtextxy(15,getmaxy()/2+20,"press q to terminate");
	delay(1200);
	bac:
	ch=getch();
	if(ch=='q')  spawnl(P_OVERLAY,"carindex.exe",NULL);
	else goto bac;
}

void objectdefiner(cars *obj[],int on[])  //all the good things happens here...
{
//this function does not delete the previously created object on time out and create new ones as the name might suggest
//but what this function does is, read from carrs[] and reinitializes objects which reached the end of road,i.e. bottom of the screen
//or, if the object is not at the bottom of screen, it increments y-val of object for the motion.
	static int m;
	int k,j=0;
	int stat=0;
	for(int i=0;i<12;i++) if(on[i]!=0){ stat=1; break;}
	if(stat==0) m=0; //should be true only in starting of game...
	i=m;//reloading m...description given 5 lines below;
	if(counter==0){
	    if(carrs[last]<=3){
		while(on[i]!=0){ i=(i+1)%12; }//searching for not active object to make it active..
		if(carrs[last]!=0){
			on[i]=1; m=(i+1)%12; //m=i+1 makes the searching of the not active
				//object from next position from last in next loop...
			obj[i]->lane(carrs[last]-1);
			obj[i]->scorer();
		//	if(carrs[(last-1)%30]==0) dec-=2;
		}
	    //	else dec+=2;//increasing spacing..
	//two statements above are commented because they didn't work as expected
	    }
	    else{
			while(on[i]!=0){ i=(i+1)%12;  } //searching for not active object to make it active..
			on[i]=1;
			j=i;
			obj[i]->scorer();//making object in ith pointer be counted for score
			while(on[i]!=0){ i=(i+1)%12;  } //searching for not active object to make it active..
			on[i]=1;
	    //the three if statements makes sure that atleast one side is open for player
			if(carrs[last]==4){ //left and middle lane
				obj[i]->lane(0);//this counts in score
				obj[j]->lane(1);//this doesn't count in score
			}

			if(carrs[last]==5){  //middle and right lane
				obj[i]->lane(2);
				obj[j]->lane(1);
			}

			if(carrs[last]==6){  //left and right lane...
				obj[i]->lane(0);
				obj[j]->lane(2);
			}
			m=i;
	    //this is commented because it didn't work as expected//
		   //	if(carrs[(last-1)%30]==0) dec-=2;

	    }
	    last=(last+1)%30;//counting through total states;
	    //next phase of states;
		if(last==0)statedefiner();
	}
//this for loops give each car object a motion( obj[i]->move() )
//and checks if player crashed with any of the car object( obj[i]->crashcheck() )
//if player crashed then ends and returns to carindex through end();
//otherwise checks if car object reaches end of screen, if it does increases the score of player
//and increases speed with increase of level as per score..
	for(i=0;i<12;i++){
		if(on[i]==1){
			obj[i]->move();
			j=obj[i]->crashcheck();
		}
		if(j==1) {end();}
		k=obj[i]->check();
		if(k==1){
			if(obj[i]->returnscorer()==1) n++;
			obj[i]->reloader();
			on[i]=0;
			if(n==20){t-=20;level=1;} //1st phase..
			if(n==50){t-=10;level=2;}//2nd phase..
			if(n==90){t-=5;level=3;}//3rd phase..
			if(n==120){t-=5;level=4;}//4th pahse...
			if(n==140){t-=5;level=5;;}//5th and final phase..
			score();
		}
		k=0;

	}
	counter=(counter+1)%dec;/* the delay in the main is caused by calling
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
	if(dir=='a'){
		if(xa==(getmaxx()/2-45)) return getmaxx()/2-45;
		if(xa==(getmaxx()/2+45)) return getmaxx()/2-45;
		if(xa==(getmaxx()/2+135)) return getmaxx()/2+45;
	}
	if(dir=='d'){
		if(xa==(getmaxx()/2-45)) return getmaxx()/2+45;
		if(xa==(getmaxx()/2+45)) return getmaxx()/2+135;
		if(xa==(getmaxx()/2+135)) return getmaxx()/2+135;
	}
	if(dir=='w') if(ya>15) return ya-5;
	if(dir=='s') if(ya<(getmaxy()-15)) return ya+5;
	return 0;
}

//this is here because defining carrs[100] caused problem
//also this is memory efficient
void statedefiner() //0;-left lane, 1;-right lane, 2:- no car
{
	if(change==2){
	carrs[0]=0;  carrs[1]=1;  carrs[2]=6;  carrs[3]=3;  carrs[4]=2;
	carrs[5]=0;  carrs[6]=6;  carrs[7]=0;  carrs[8]=5;  carrs[9]=0;
	carrs[10]=4; carrs[11]=0; carrs[12]=6; carrs[13]=1; carrs[14]=4;
	carrs[15]=0; carrs[16]=5; carrs[17]=0; carrs[18]=4; carrs[19]=0;
	carrs[20]=5; carrs[21]=3;
	carrs[22]=6; carrs[23]=0; carrs[24]=2; carrs[25]=2; carrs[26]=4;
	carrs[27]=0; carrs[28]=6; carrs[29]=1; change=0;}

	if(change==1){ change=2;
	carrs[0]=3; carrs[1]=5; carrs[2]=5; carrs[3]=3; carrs[4]=1; carrs[5]=1;
	carrs[6]=2; carrs[7]=0; carrs[8]=2; carrs[9]=1; carrs[10]=1; carrs[11]=4;
	carrs[12]=1; carrs[13]=3; carrs[14]=5; carrs[15]=0; carrs[16]=4;
	carrs[17]=0;  carrs[18]=2; carrs[19]=3;carrs[20]=6;carrs[20]=3; carrs[21]=2;
	carrs[22]=0; carrs[23]=4; carrs[24]=0; carrs[25]=6; carrs[26]=0;
	carrs[27]=5; carrs[28]=0; carrs[29]=4;}

	if(change==0){change=1;carrs[0]=0; carrs[1]=1;
	carrs[2]=4; carrs[3]=0; carrs[4]=5; carrs[5]=0; carrs[6]=6; carrs[7]=1;
	carrs[8]=4; carrs[9]=0; carrs[10]=6; carrs[11]=1; carrs[12]=2; carrs[13]=0;
	carrs[14]=6; carrs[15]=3; carrs[16]=1; carrs[17]=6; carrs[18]=3; carrs[19]=5;
	carrs[20]=2; carrs[21]=0; carrs[22]=6; carrs[23]=1; carrs[24]=0; carrs[25]=5;
	carrs[26]=0; carrs[27]=4; carrs[28]=2; carrs[29]=1;}

}

main()
{
	int gd=DETECT,gm;
	clock_t start, end;
//clock_t variables to measure elapsed time..
	start=clock();
//t=50 for delay(t);
	t=50;
	char c='a',mg[40];//initializing c with dummy value
	counter=0;//global variable, described at begining//used in objectdefiner() as others
	last=0;//bcoz (last+1)%100=0,,global variable described at begining..
	dec=12;//this variable is actually never changed because the statements that changed it are commented because it didn't work as expected
	n=level=change=0;//initially score and level is 0, so is change;global variable described at begining..
			//n and level used in score()...
	cars *obj[12];//defining total of 12 cars object pointer....
	for(int i=0;i<12;i++){
		on[i]=0; obj[i]=new cars;//on[] determines whether the car object are active or not...
		//so initially all car object are inactive and pointing each pointer to new car object...
	}
	statedefiner();//initiates the car out timings....
//initializing graphics..
	initgraph(&gd,&gm,"/tc/bgi ");

	setviewport(0,0,getmaxx(),getmaxy(),0);
	clearviewport();
//draw initial game scenario..
	draw();
	score();

	ya=getmaxy()-120;
	xa=getmaxx()/2-45;
	delay(1500);

	car();
	int sec,min=0,ss=0;
	delay(150);

   do{
	delay(t);//speed of the game...

	if(kbhit())	c=getch();
   //if player presses 'p', halt the game until 'r' is pressed
	if(c=='p'){
		setviewport(0,100,160,140,0);
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

   //this if checks if any key is pressed or not...i.e if pressed key is not any direction keys or q ignore the following if's
	if(c!='q'&&c!='a'&&c!='d'&&c!='w'&&c!='s') goto ahead;

  //if the pressed key is direction keys "w,s,a,d" make appropriate move

	if(c=='a'||c=='d'){ //switch lane
		setviewport(xa-20,ya-20,xa+20,ya+20,0);
		clearviewport();
		xa=value(c);
		car();
		c='r';
	}

	if(c=='w'||c=='s'){//move up and down
		setviewport(xa-20,ya-20,xa+20,ya+20,0);
		clearviewport();
		ya=value(c);
		car();
		c='r';
	}

	ahead:
	objectdefiner(obj,on);
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

   }while(c!='q'); //if player presses 'q' end do-while loop
   spawnl(P_OVERLAY,"carindex.exe",NULL); //return to carindex.exe
   return 0;
}