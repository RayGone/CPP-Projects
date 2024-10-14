 #include<graphics.h>
 #include<stdio.h>
 #include<conio.h>
 #include<dos.h>
 #include<stdlib.h>
 #include<time.h>
 #include<bios.h>
 #include<iostream.h>

 int changeindir;//indicates change in direction..
 int xcd,ycd;//x and y co-ordinate value where the position changes...
 int score;//simply gives score..no.of food eaten, basically.(big/small)
 int gmovr;//counter to figure out self biting//to check after 3rd node...
 int number;//counts no.of food eaten(small only)..

 class snake
 {
      public:
		snake *parent;
		snake *next;//pointer to next circle//body of the snake..
		int direction;//direction of the snake, required for increment() to make snake move...

		int radius;//radius of circles
		int x,y;//x and y co-ordinate of center of circle


		snake(){ radius=10; direction=0x4d00; x=400; y=300; parent=NULL; next=NULL; }

		void draw()
		{
			setviewport(x-radius,y-radius,x+radius,y+radius,1);
			circle(radius,radius,radius);
			//this is only drawn on the head to differ it from rest of the body...
			if(parent==NULL){
				circle(radius,radius,radius/2);
				setfillstyle(SOLID_FILL,BLUE);
				floodfill(radius,radius,getmaxcolor());
				setfillstyle(SOLID_FILL,8);
				floodfill(radius/2+1,radius/2+1,getmaxcolor());
			}
		}

  //this function makes rest of the body of snake
	//to follow the path that head has taken...
		int comparer()
		{       //checking if the node is the head
		      if(parent!=NULL){
		      //if not then checking if the direction of the node
				//and its immediate parent is same or not...
				if(parent->direction!=direction)
				//if the direction is not same then checking
					//whether the node and its immediate parent align or not
						//either horizontally or vertically...
					if(parent->x==x||parent->y==y)
						return 1;
		      }
		      return 0;
		}

		void valueup()
		{  //this i intend to use to detect collision and feeding///...
			xcd=x;
			ycd=y;
		}

		void increment()
		{
			setviewport(x-radius,y-radius,x+radius,y+radius,2);
			clearviewport();
			//this is for rest of the body of snake to follow the head...
			if(comparer()){ direction=parent->direction; }
			//moving the snake as per the direction the head is headed
			if(direction==0x4800){ if(y<=radius)y=getmaxy()-radius; else y-=radius; }
			if(direction==0x5000){ if(y>=getmaxy()-radius)y=radius; else y+=radius; }
			if(direction==0x4b00){ if(x<=radius)x=getmaxx()-radius; else x-=radius; }
			if(direction==0x4d00){ if(x>=getmaxx()-radius)x=radius; else x+=radius; }
			if(next==NULL)changeindir=0;
			recheck();
			draw();

		}
		void recheck()
		{       //this is to avoid a circle left drawn when passing through borders
			if(y<radius)y=getmaxy()-radius;
			if(y>getmaxy()-radius)y=radius;
			if(x<radius)x=getmaxx()-radius;
			if(x>getmaxx()-radius)x=radius;
		}

     /*		void adjustheadposition()
		{
			int i,j;
			i=this->next->x;
			j=this->next->y;
			setviewport(i-radius,j-radius,i+radius,j+radius,1);
			clearviewport();
			setviewport(x-radius,y-radius,x+radius,y+radius,1);
			clearviewport();
			if(direction==0x4800){ if(this->next->y<=radius)this->next->y=getmaxy()-radius; else this->next->y-=radius; }
			if(direction==0x5000){ if(this->next->y>=getmaxy()-radius)this->next->y=radius; else this->next->y+=radius; }
			if(direction==0x4b00){ if(this->next->x<=radius)this->next->x=getmaxx()-radius; else this->next->x-=radius; }
			if(direction==0x4d00){ if(this->next->x>=getmaxx()-radius)this->next->x=radius; else this->next->x+=radius; }
			next->draw();
			if(direction==0x4800){ x=next->x; y=next->y-radius; }
			if(direction==0x5000){ x=next->x; y=next->y+radius; }
			if(direction==0x4b00){ x=next->x-radius; y=next->y; }
			if(direction==0x4d00){ x=next->x+radius; y=next->y; }
			this->draw();
		}
       */
		int changedir(int dir)
		{
			//avoiding the movement in opposite direction
			if(direction==0x4800&&dir==0x5000||direction==0x5000&&dir==0x4800) return 0;
			if(direction==0x4b00&&dir==0x4d00||direction==0x4d00&&dir==0x4b00) return 0;
			//if only one of four direction keys are pressed
			if(dir==0x4800||dir==0x5000||dir==0x4d00||dir==0x4b00)
			{	direction=dir;
				return 1;
			}
			return 0;
		}

		void newobject()
		{
		//defining new objects(nodes)
			next= new snake;
			next->parent=this;
			next->direction=direction;
			if(direction==0x4800){ next->x=x; next->y=y+radius;}
			if(direction==0x5000){ next->x=x; next->y=y-radius;}
			if(direction==0x4b00){ next->x=x+radius; next->y=y;}
			if(direction==0x4d00){ next->x=x-radius; next->y=y;}
			next->draw();
		}

		void addon()
		{  //adding nodes to the body of snake after eating food..
			newobject();
			next->newobject();
			score++;
			number++;
		}

		void gameover()
		{
			//checking whether the head and body collides
			if(abs(x-xcd)<(2*radius)&&abs(y-ycd)<(2*radius)){
				char msg[20];
				setviewport(0,0,getmaxx(),getmaxy(),1);
				outtextxy(getmaxx()/2,getmaxy()/2,"GAME OVER");
				sprintf(msg,"YOUR SCORE IS: %d", score);
				outtextxy(getmaxx()/2,getmaxy()/2+10,msg);
				outtextxy(getmaxx()/2,getmaxy()/2+20,"press any key to terminate");
				delay(1000);
				getch();
				//if they collide then exit...
				closegraph();
				exit(0);
			}
		}

	      //defining food the be friend 'coz we gonna need to use
	      //a function of snake from food..
	      friend class food;
 };

 class food
 {
	protected:
		int x,y;//the co-ordinate of the food
		int r;//radius of the food...
		int count;//helps to make blinking effect

	public:

		void newpos(snake *sn)
		{     //determining new position for food if snake eats the food.
			snake *s;
			s=sn;
			//re-initializing the count..it is used in draw to determine the speed of blinking
			count=0;
			//maxm value that random can return...
			int i=getmaxx()-5;
			int j=getmaxy()-5;
			randomize();
			doit:
			//using built in function to get random numbers..
			x=random(i);
			y=random(j);
			//if the numbers returned are less then five then give them value of 5
			if(x<5)x=5;;
			if(y<5)y=5;;
	//this while statement is to avoid drawing the next food within in the body area of snake..
			while(s!=NULL)
			{
		//if the food lies within the body area then get new random values...
				if(abs(s->x-x)<20&&abs(s->y-y)<20) goto doit;
				//if not then check with next node of body of snake...
				else s=s->next;
			}
		}

		void fdraw()
		{
			int i=r/4;
			if(count==0){//draws big //but still small than snake.
				setviewport(x-r,y-r,x+r,y+r,1);
				clearviewport();
				circle(r,r,r);
				setfillstyle(SOLID_FILL,RED);
				floodfill(r,r,getmaxcolor());
			}

			if(count==5){
				setviewport(x-r,y-r,x+r,y+r,1);
				clearviewport();
				circle(r,r,r-i);//draws small
				setfillstyle(SOLID_FILL,RED);
				floodfill(r,r,getmaxcolor());
			}

			count=(count+1)%10;

		}

		//checks if the snake head is in collision with food i.e. eating
		void eater(snake *sn)
		{
			snake *s;
			s=sn;
			//checking if head meets food or not
			if(abs(xcd-x)<=14&&abs(ycd-y)<=14){
			//if so clear the food
				setviewport(x-r,y-r,x+r,y+r,1);
				clearviewport();
			//then traverse to final node of snake
				while(s->next!=NULL) s=s->next;
			//add to the body of snake
				s->addon();
			//find new position for the food
				newpos(sn);
			//then draw the food in the new position
				fdraw();
			}
		}
 };

 class smallfd: public food
 {
	public:
		smallfd(){ r=6;count=0;x=getmaxx()/2;y=getmaxy()/2; }

		void eat(snake *ss)
		{
			eater(ss);
	  //		if(number!=0&&bdd.on==0)bdd.redraw(ss);//if 10 small food are eaten
			//then draw big food...
		}
 };

 class bigfd: public food
 {
	public:
		int round;//this works like flag to remove the big food if its not eaten in time...
		int on;//flag that determines whether a food is already drawn or not..

		bigfd(){ round=0;on=0;r=15;count=0;x=getmaxx()/2;y=getmaxy()/2; }
	       //overriding eater...
		void eater(snake *sn)
		{
			snake *s;
			s=sn;
			round=(round++)%100;
			//checking if head meets food or not
			if(abs(xcd-x)<=14&&abs(ycd-y)<=14){
			//if so clear the food
				setviewport(x-r,y-r,x+r,y+r,1);
				clearviewport();
			//increase score by 5 points..
				score=score+5;
				on=0;//big food is eaten..
			}

			else if(round==99){
				//if its not eaten even after 100 round, clear...
				setviewport(x-r,y-r,x+r,y+r,1);
				clearviewport();
				on=0;//there's no food anymore..
			}
		}

		void redraw(snake *sn)
		{
			if(number%10==0){
				on=1;
			//find new position for the food
				newpos(sn);
			//then draw the food in the new position
				fdraw();
			}
		}
		friend class smallfd;

 };

 void initialdraw(snake *sn)
 {
	sn->draw();
	sn->newobject();
	sn->next->newobject();
	sn->next->next->newobject();
	sn->next->next->next->newobject();
	sn->next->next->next->next->newobject();
 }

 main()
 {
	number=1;//global variable..
	int gd=DETECT, gm;
	snake sn;
	snake *ss;
	streambuf f;
	smallfd fd;
   //	bigfd bdd;
	clrscr();
	initgraph(&gd, &gm, "/tc/bgi");
	//get position (x,y) to draw the food
	fd.newpos(&sn);
	cleardevice();
	changeindir=score=0;
	xcd=ycd=0;
	//draws initial body of snake...
	initialdraw(&sn);

	ss=&sn;
	setbkcolor(GREEN);

	int sk;//remembers the last direction of snake
	int key=sk=0x4d00; //initializing key to right
	do{
		gmovr=0;
		delay(50);
		fd.fdraw(); //drawing food...
		if(kbhit()){key = bioskey(0); //if there is a keyboard hit fetch the key...
		   if(sk!=key){//if the key is same as previous avoid
				sn.changedir(key);//then change direction,,
		   }
		}
		onceagain:
		while(ss!=NULL){//moving through head to tail of snake..
			if(gmovr>=4){ //start checking for game over after 3rd node
				sn.valueup();
				ss->gameover();  //checking for gameover
			}
			gmovr++;
			ss->increment();  //moving the snake
			ss=ss->next; //next node
		}
		ss=&sn; //re-initializing pointer to head...
		if(sk!=key){ sk=key; f.overflow(); gmovr=0; goto onceagain;}//if the direction is changed then update sk and run the while loop once again...
		fd.eat(ss); //checking if the snake ate the food
      //		if(bdd.on==1)bdd.eater(ss); //if there is food then check if snake ate it or not...
		//also if times out clear the bigfood..
	}while(key!=0x11b);//if the key is esc. then end...
      ;	closegraph();
   }