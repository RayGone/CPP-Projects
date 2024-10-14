#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<dos.h>


class TowerOfHanoi
{
	private:
		int numOfDisks;//num of disks in tower of hanoi,,also corresponds to level of game
/* 	three pegs in tower of hanoi...
	each pointer will be dynamically allocated to be array
	of size of numOfDisks. Initially,secondTower and thirdTower will be 0's
	and first tower be serially numbered in reverse
	the number will be used to determine the radius of disk.. 	*/
		int *firstTower;
		int *secondTower;
		int *thirdTower;
/*	Since tower hanoi works as stack, these three variables keeps track
	of top of each tower or poles		*/
		int firstTop;
		int secondTop;
		int thirdTop;

//variable to temporarily store the disk when lifted from source
		int temp;

	public:
		TowerOfHanoi()
		{
			setNumOfDisks(3);
			setTower();
		}
	//to initialize the tower, after each success of player, for next level.
		void initializer(int num)
		{
		  //	delete firstTower;
		  //	delete secondTower;
		  //	delete thirdTower;
			setNumOfDisks(num);
			setTower();
		}

		void setNumOfDisks(int n)
		{
			numOfDisks=n;
		}


		void setTower()
		{
			firstTower=new int[numOfDisks];
			secondTower=new int[numOfDisks];
			thirdTower=new int[numOfDisks];

			firstTop=numOfDisks;
			secondTop=0;
			thirdTop=0;
			for(int i=0;i<numOfDisks;i++)
			{
				firstTower[i]=numOfDisks-i;
				secondTower[i]=0;
				thirdTower[i]=0;
			}
		}

		int getNumOfDisks()
		{
			return numOfDisks;
		}

		int getFirstTower(int index)
		{
			return firstTower[index];
		}

		int getSecondTower(int index)
		{
			return secondTower[index];
		}

		int getThirdTower(int index)
		{
			return thirdTower[index];
		}

		int getTowerTop(int tower)
		{
			if(tower==1) return firstTower[firstTop-1];
			else if(tower==2) return secondTower[secondTop-1];
			else thirdTower[thirdTop-1];
		}

		int getTemp()
		{
			return temp;
		}

		int remDisk(int sourceTower)
		{
		//making changes to source
			if(sourceTower==1){
				if(firstTop!=0){
					temp=firstTower[firstTop-1];
					firstTower[firstTop-1]=0;
					firstTop--;
					return 1;
				}
			}
			else if(sourceTower==2){
				if(secondTop!=0){
					temp=secondTower[secondTop-1];
					secondTower[secondTop-1]=0;
					secondTop--;
					return 1;
				}
			}
			else{
				if(thirdTop!=0){
					temp=thirdTower[thirdTop-1];
					thirdTower[thirdTop-1]=0;
					thirdTop--;
					return 1;
				}
			}
			return 0;//if no disks in the peg
		}

		int putDisk( int destinationTower)
		{
		//making changes to destination
			if(destinationTower==1){
				if(temp<firstTower[firstTop-1]){
					firstTower[firstTop]=temp;
					firstTop++;
					return 1;
				}
			}
			else if(destinationTower==2){
				if(temp<secondTower[secondTop-1]){
					secondTower[secondTop]=temp;
					secondTop++;
					return 1;
				}
			}
			else{
				if(temp<thirdTower[thirdTop-1]){
					thirdTower[thirdTop]=temp;
					thirdTop++;
					return 1;
				}
			}
			return 0;
		}
};


class graphicsForTOH: TowerOfHanoi
{
	private:
	//x and y co-ordinate of center of screen
		int midx;
		int midy;

	//x co-ordinate of pegs
		int peg1;
		int peg2;
		int peg3;

	//level of the game
		int level;

	//num of moves the player have made
		int move;

	public:
		graphicsForTOH()
		{
			level=1;
			move=0;
			midx=getmaxx()/2;
			midy=getmaxy()/2;

			peg1=midx-150;
			peg2=midx;
			peg3=midx+150;

			drawBase();
			drawPegs();
			drawDisks();
			pointer(1);

		}

		void drawBase()
		{
			setcolor(BLUE);

			for(int i=0;i<8;i++)
			{
				line(0,midy+50+i,midx*2,midy+50+i);
			}
		}

		void drawPegs()
		{
			setcolor(getmaxcolor());

			for(int i=0;i<5;i++)
			{
				line(peg1-2+i,midy+50,peg1-2+i,midy-50);
				line(peg2-2+i,midy+50,peg2-2+i,midy-50);
				line(peg3-2+i,midy+50,peg3-2+i,midy-50);
			}
		}


		void drawDisks()
		{
			setfillstyle(SOLID_FILL,GREEN);
			for(int i=0;i<getNumOfDisks();i++)
			{
				if(getFirstTower(i)!=0){
					fillellipse(peg1,midy+48-(5*i),getFirstTower(i)*6,4);
				}
				if(getSecondTower(i)!=0){
					fillellipse(peg2,midy+48-(5*i),getSecondTower(i)*6,4);
				}
				if(getThirdTower(i)!=0){
					fillellipse(peg3,midy+48-(5*i),getThirdTower(i)*6,4);
				}
			}
		}

		void pointer(int pos)
		{
			setfillstyle(SOLID_FILL, LIGHTGRAY);
			if(pos==1){
				fillellipse(peg1,midy+100,20,5);
			}
			else if(pos==2){
				fillellipse(peg2,midy+100,20,5);
			}
			else{
				fillellipse(peg3,midy+100,20,5);
			}
		}


		void rempointer(int pos)
		{
			setcolor(getbkcolor());
			setfillstyle(SOLID_FILL, getbkcolor());
			if(pos==1){
				fillellipse(peg1,midy+100,20,5);
			}
			else if(pos==2){
				fillellipse(peg2,midy+100,20,5);
			}
			else{
				fillellipse(peg3,midy+100,20,5);
			}
			setcolor(getmaxcolor());
		}

		void updateMoves()
		{
			char *c;
			//erasing previous value
			setcolor(getbkcolor());
			sprintf(c,"moves: %d",move-1);
			outtextxy(midx+100,midy-120,c);
			//updating
			setcolor(getmaxcolor());
			sprintf(c,"moves: %d",move);
			outtextxy(midx+100,midy-120,c);
		}

		void updateLevel()
		{
			char *c;
			//erasing previous value
			setcolor(getbkcolor());
			sprintf(c,"Level: %d",level-1);
			outtextxy(midx+100,midy-130,c);
			//updating
			setcolor(getmaxcolor());
			sprintf(c,"Level: %d",level);
			outtextxy(midx+100,midy-130,c);
		}


		char gamePlay()
		{
			updateLevel();
			updateMoves();
			char c='w';
			int pos=1;
			while(c!='q')
			{
				c=getch();
				if(c=='a'){
					rempointer(pos);
					if(pos!=1) pos--;
				}

				if(c=='s'){
					rempointer(pos);
					if(pos!=3) pos++;
				}

				if(c=='g'){
					move++;
					updateMoves();
					rempointer(pos);
					liftDisk(&pos);
				}

				pointer(pos);

				if(getThirdTower(getNumOfDisks()-1)!=0){
					rempointer(pos);
					pos=1;
					if(level<10)	levelup();
					else{
						c='a';
						return c;
					}
				}
			}
			return c;
		}

		void levelup()
		{
		//increment and update level
			level++;
			updateLevel();
			initializer(level+2);//function of TowerOfHanoi that reinitializes variables
		//erase the num of moves in previous level
			char *c;
			setcolor(getbkcolor());
			sprintf(c,"moves: %d",move);
			outtextxy(midx+100,midy-120,c);
			setcolor(getmaxcolor());
		//update move=0 for new level
			move=0;
			sprintf(c,"moves: %d",move);
			outtextxy(midx+100,midy-120,c);

			setviewport(0,midy-50,midx*2,midy+50,1);
			clearviewport();
			setcolor(getmaxcolor());
			outtextxy(midx,50,"LEVEL UP");
			delay(1500);
			clearviewport();
		//draw TOH again
			setviewport(0,0,midx*2,midy*2,1);
			drawBase();
			drawPegs();
			drawDisks();
			pointer(1);
		}

		void liftDisk(int *peg)
		{
			int ok;
			char c;
			int pos=*peg;
			ok=remDisk(pos);
			if(!ok)return;
			else{
				setviewport(0,midy-50,midx*2,midy+50,1);
				clearviewport();
				setviewport(0,0,midx*2,midy*2,1);
				drawBase();
				drawPegs();
				drawDisks();
				liftedDisk(pos);
				while(1){
					c=getch();
					if(c=='a'){
						erase(pos);
						if(pos!=1) pos--;
					}
					if(c=='s'){
						erase(pos);
						if(pos!=3) pos++;
					}
					if(c=='g'){
						ok=putDisk(pos);
						if(ok){
							erase(pos);
							setviewport(0,midy-50,midx*2,midy+50,1);
							clearviewport();
							setviewport(0,0,midx*2,midy*2,1);
							drawBase();
							drawPegs();
							drawDisks();
							*peg=pos;
							return;
						}
					}
					liftedDisk(pos);
				}
			}
		}

		void liftedDisk(int pos)
		{
			setfillstyle(SOLID_FILL,GREEN);
			if(pos==1){
				fillellipse(peg1,midy-100,getTemp()*6,4);
			}
			else if(pos==2){
				fillellipse(peg2,midy-100,getTemp()*6,4);
			}
			else{
				fillellipse(peg3,midy-100,getTemp()*6,4);
			}
		}

		void erase(int pos)
		{
			setcolor(getbkcolor());
			setfillstyle(SOLID_FILL,getbkcolor());
			if(pos==1){
				fillellipse(peg1,midy-100,getTemp()*6,4);
			}
			else if(pos==2){
				fillellipse(peg2,midy-100,getTemp()*6,4);
			}
			else{
				fillellipse(peg3,midy-100,getTemp()*6,4);
			}
			setcolor(getmaxcolor());

		}


};



void main()
{
	int gd=DETECT,gm;
	initgraph(&gd,&gm,"/tc/bgi");
	outtextxy(0,10,"press a to move left");
	outtextxy(0,20,"press s to move right");
	outtextxy(0,30,"press g to lift/put a disk from/to peg");
	outtextxy(0,40,"press q to quit");
	graphicsForTOH t;
	char c=t.gamePlay();
	if(c!='q'){
		outtextxy(getmaxx()/2-100,getmaxy()/2-100,"You are the master Of Tower Of Hanoi");
		outtextxy(getmaxx()/2-100,getmaxy()/2-80,"Press 'q' to terminate");
		while(c!='q')c=getch();
	}
}