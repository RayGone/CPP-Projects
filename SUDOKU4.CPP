#include<graphics.h>
#include<stdio.h>
#include<iostream.h>
#include<conio.h>
#include<dos.h>
#include<time.h>
#include<ctype.h>
#include<math.h>

//math.h to use abs(), e.g. abs(-1) returns 1;
//ctype.h to use isdigit() function
//time.h to use clock_t
//dos.h to use delay()
//conio.h to use getch()
//stdio.h to use sprintf()
//graphics.h for graphics


union REGS p,o;
short int sudoku[9][9];
short int flag;//this flag when set to 1 indicates the process of taking input is complete
short int pos[2];//keeps position where the mouse was hovering the last time...
char msg[30];//used if any message is to be displayed

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

class inputSudoku
{
	private:
		char c;
		int xc,yc;//co-ordinate of center of screen
		int x,y;//mouse position
		int b;//mouse button clicked; left or right

	public:
		inputSudoku(){ xc=getmaxx()/2; yc=getmaxy()/2;
		//initializing matrix sudoku declared as global variable
			for(int i=0;i<9;i++)
				for(int j=0;j<9;j++)
					sudoku[i][j]=0;
		}

		void setMpos(int *button, int xp, int yp)
		{
			b=*button;
			x=xp;
			y=yp;
			*button=-1;
		}

		char takeInput()
		{
			int stat=0;//to check if the key is pressed or not
		 //declaring clock object;
			clock_t start, end;
			c='o';

			//if the left mouse button is clicked
			if(b==1){

				b=-1;
			 //if "DONE" button is pressed
				if(abs(xc+240-x)<40 && abs(yc-y)<15){
					if(Done()==1)
						return 'q';  //proceed with finding solution
				}

				//if the mouse position isn't inside the grid
				else if(x<(xc-180)) return c;
				else if(x>(xc+180)) return c;
				else if(y<(yc-170)) return c;
				else if(y>(yc+190)) return c;
				//if the mouse position is inside the grid
				else{
					start = clock();
					for(end=clock();((end-start)/CLK_TCK)<2;end=clock())\
					{
					//display message only once at the begining
						if(((end-start)/CLK_TCK)==0)
						{
							sprintf(msg,"Waiting for the input");
							outtextxy(0,getmaxy()-30,msg);
						}
					//if there is keyboard hit
						if(kbhit()){
						//fetch the key
							c=getch();
							//erase the waiting message if key is pressed...
							setviewport(0,getmaxy()-30,getmaxx(),getmaxy(),1);
							clearviewport();
							setviewport(0,0,getmaxx(),getmaxy(),1);
							stat=1;
							break;
						}
					}

					//if no key is pressed c==1 to avoid the execution of if statement below
					if(stat==0)c='1';

					//if the input is not a digit return
					if(!isdigit(c))
					{
						sprintf(msg,"Please input only digits [1-9]");
						outtextxy(0,getmaxy()-30,msg);
						delay(1000);
						//erasing the message written above
						setviewport(0,getmaxy()-30,getmaxx(),getmaxy(),1);
						clearviewport();
						setviewport(0,0,getmaxx(),getmaxy(),1);
						return c;
					}

					//erasing the message written, if no key are pressed in above for statement.
					setviewport(0,getmaxy()-30,getmaxx(),getmaxy(),1);
					clearviewport();
					setviewport(0,0,getmaxx(),getmaxy(),1);

					//if key is pressed,,and execution reaches to this line means a digit is pressed
					if(stat==1){
						write();
					}
					return c;

				}
			}
			 return c;
		}
		//end of takeInput();

	//this checks the input and determines if the no.of input is enough or not to carry on
//if not then promt the user to provide more input.....
		int Done()
		{
			int num=0;//records no.of inputs to sudoku
			int digit[9];
			int i,j;
			flag=0;
		      //initializing digit[] to 0
			for(i=0;i<9;i++)
				digit[i]=0;
		      //calculating no.of inputs and no.of each digits
			for(i=0;i<9;i++)
				for(j=0;j<9;j++)
				{
					if(sudoku[i][j]!=0){
						 ++num;
						 ++digit[sudoku[i][j]-1];
					}
				}
		      //calculating no.of different digits
			j=0;
			for(i=0;i<9;i++)
			{	if(digit[i]!=0) ++j;     }

			if(num>=17 || j>=8){
				if(check()==0){
					sprintf(msg,"!!!ILLEGAL INPUT!!! Duplicate values");
					outtextxy(0,getmaxy()-30,msg);
					delay(1500);
					setviewport(0,getmaxy()-30,getmaxx(),getmaxy(),1);
					clearviewport();
					setviewport(0,0,getmaxx(),getmaxy(),1);

					return 0;
				}
				flag=1;
				return 1;//alright to proceed
			}

			if(num<17 || j<8) {
				if(num<17){
					sprintf(msg,"Not enough Input, atleast 17 required");
					outtextxy(0,getmaxy()-30,msg);
					delay(1500);
				}
		//because only one of two condition should be met
				if(j<8){
						setviewport(0,getmaxy()-30,getmaxx(),getmaxy(),1);
						clearviewport();
						setviewport(0,0,getmaxx(),getmaxy(),1);
						sprintf(msg,"Provide atleast 8 different digit,");
						outtextxy(0,getmaxy()-30,msg);
						sprintf(msg," more than >1 solution possible");
						outtextxy(300,getmaxy()-30,msg);
						delay(1500);

				}
			      //erasing the message written above
				setviewport(0,getmaxy()-30,getmaxx(),getmaxy(),1);
				clearviewport();
				setviewport(0,0,getmaxx(),getmaxy(),1);

				flag=0;
				return 0;//not all right to proceed
			}

		}//end of Done();

		void nToP(int l,int *m, int *n)
		{
			int dum=0;
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					if(dum==l){
						*m=i;
						*n=j;
						return;
					}
				++dum;
				}
			}
		}//end of nToP();


		int check()
		{
			int i,j,l,m,n;
		//for all places in matrix
			for(i=0;i<9;i++){
				for(j=0;j<9;j++){
			//if there is input in current position
					if(sudoku[i][j]!=0){
						for(l=0;l<9;l++){
					     //check for the places except itself
							if(l!=j){
							//row checking
								//we provide (i,j) to fill in reverse order because while mapping (i,j) in matrix to (x,y) in co-ordinate, j maps to x and i maps to y...
								if(sudoku[i][j]==sudoku[i][l]){ fill(j+1,i+1,RED); fill(l+1,i+1,RED); return 0; }
							}
					     //check for the places except itself
							if(l!=i){
							//column checking
								//we provide (i,j) to fill in reverse order because while mapping (i,j) in matrix to (x,y) in co-ordinate, j maps to x and i maps to y...
								if(sudoku[i][j]==sudoku[l][j]){ fill(j+1,i+1,RED); fill(j+1,l+1,RED); return 0; }
							//return 0 if there is duplication
							}
							//box checking

//					same as numToPos() in class sudokuSolver
//						only difference is it returns 3*3 position
//							while the other returns 9*9
							nToP(l,&m,&n);
				//mapping m and n value in accordance to i and j
							if(i<3);
							else if(i<6) m+=3;
							else m+=6;

							if(j<3);
							else if(j<6) n+=3;
							else n+=6;

					     //check for the places except itself
							if(m!=i && n!=j){
								//we provide (i,j) to fill in reverse order because while mapping (i,j) in matrix to (x,y) in co-ordinate, j maps to x and i maps to y...
								if(sudoku[i][j]==sudoku[m][n]){ fill(j+1,i+1,RED); fill(n+1+1,m+1,RED); return 0; }
							}
						}
					}
				}
			}
			return 1;
		}//end of check

 //though the name is calcIpos, it calculates j of sudoku(i,j)
		int calcIpos()
		{
			int m=xc-180;

		    //for a m*n grid (or matrix)
//this for statement finds m position where mouse is clicked i.e. m position of cell
 //here i<=9 because even when the mouse hovered down or right of the grid(outside)
 //it still returned i=9, so i<=9 will cause it to return 10...
			for(int i=0;i<=9;i++){
				//(i*40) because all the cells are 40px rectangles
				if((x-m)<(i*40))break;//if m position is found exit loop
			}
			return i;
		}//end of calcIpos();

 //though the name is calcJpos, it calculates i of sudoku(i,j)
		int calcJpos()
		{
			int n=yc-170;
			 //for a m*n grid (or matrix)
// this for finds n position where mouse is clicked i.e. n position of cell
//here i<=9 because even when the mouse hovered down or right of the grid(outside)
 //it still returned i=9, so i<=9 will cause it to return 10...
			for(int j=0;j<=9;j++){
				if((y-n)<(j*40))break;//if n position is found exit loop
			}
			return j;
		} //end of calcJpos();

//writes on the grid, on the cell the mouse is clicked, the digit read from keyboard...
		void write()
		{
			int m,n;
			m=xc-180;
			n=yc-170;
//calculating sudoku[j][i] position w.r.t the mouse position on the screen
			int i=calcIpos();
			int j=calcJpos();
//updating sudoku[j][i] value
			setInputToMatrix(j-1,i-1);
//hidemouseptr() so that mouse doesn't intervene(shadow) message display
			hidemouseptr();

/*it took me 5:30 hrs to figure out what was exactly wrong( that output is not displayed where mouse was clicked)
	and how did the lines below pan out to work as i wanted;
it turns out that above for statements(changed to calcIpos() and calcJpos() ) do increment the value of i and j
at the point of executing break; so if i=0 when if was true and break executed
the value of i was incremented to 1 when control was transferred out of for...
this fact does help understand the calculation below */
			m-=20;
			n-=20;
//clearing the cell if some values were already placed there
			setviewport((m-10+(i*40)),(n-10+(j*40)),(m+10+(i*40)),(n+10+(j*40)),1);
			clearviewport();
			setfillstyle(SOLID_FILL,2);
			floodfill(1,1,getmaxcolor());
			setviewport(0,0,getmaxx(),getmaxy(),1);

//outtext the input if c!=0,otherwise just clear the cell
			if(c!='0'){
				setcolor(WHITE);
				sprintf(msg,"%c",c);
				outtextxy((m-4+(i*40)),(n-12+(j*40)),msg);
			}

			showmouseptr();
		}
		//end of write()

//simply updates the matrix sudoku with the input recieved
	//which will be manipulated to find solution
		void setInputToMatrix(int i,int j)
		{
			if(c=='0')
				sudoku[i][j]=0;
			if(c=='1')
				sudoku[i][j]=1;

			if(c=='2')
				sudoku[i][j]=2;
			if(c=='3')
				sudoku[i][j]=3;
			if(c=='4')
				sudoku[i][j]=4;
			if(c=='5')
				sudoku[i][j]=5;
			if(c=='6')
				sudoku[i][j]=6;
			if(c=='7')
				sudoku[i][j]=7;
			if(c=='8')
				sudoku[i][j]=8;
			if(c=='9')
				sudoku[i][j]=9;
		}
		//end of setInputToMatrix();

		void hoverEffect()
		{
			int i,j;

		     //if mouse is within the grid
			if((xc+180-x)<360 && (yc+190-y)<360){
				i=calcIpos();
				j=calcJpos();
		       //if mouse moves across cells fill the new cell and clear previous
				if(pos[0]!=i || pos[1]!=j){
					fill(i,j,10);
					unfill();
				}
		       //one step backtracking to unfill
				pos[0]=i;
				pos[1]=j;
			}
		       //if mouse goes outside the grid
			if((xc+180-x)>360 || (yc+190-y)>360){
				unfill();
				pos[0]=10;
				pos[1]=10;
			}

		}//end of hoverEffect

       //fills the hovered cell with color
		void fill(int i,int j,int k)
		{ // though the values passed are (i,j), the actual order is in reverse i.e (j,i)

			int m,n;
			m=xc-200;
			n=yc-190;
			setviewport(0,0,getmaxx(),getmaxy(),1);
		   //if the mouse doesnt hover outside the grid..
			if(i<10 && j<10){
			//setting the fill color
				setfillstyle(SOLID_FILL, k);

				hidemouseptr();//so that mouse doesn't intervene drawing

				floodfill((m-10+(i*40)),(n-10+(j*40)),getmaxcolor());
				floodfill((m-15+(i*40)),(n-15+(j*40)),getmaxcolor());

				showmouseptr();
			}
		}
		//end of fill()

	//unfills the previously hovered cell
		void unfill()
		{

			int m,n;
			m=xc-200;
			n=yc-190;
	  //i=j=9 when the mouse hovers outside the grid
			if(pos[0]<10&&pos[1]<10){
		//setting the fill color
				setfillstyle(SOLID_FILL, 2);

				hidemouseptr();//so that mouse doesn't intervene drawing
				setviewport((m-16+(pos[0]*40)),(n-16+(pos[1]*40)),(m+16+(pos[0]*40)),(n+16+(pos[1]*40)),1);
				floodfill(1,1,getmaxcolor());
				setviewport(0,0,getmaxx(),getmaxy(),1);
				floodfill((m-10+(pos[0]*40)),(n-5+(pos[1]*40)),getmaxcolor());
				showmouseptr();
			}
		}
		//end of fill()
};
//end of class inputSudoku

class sudokuSolver
{
	private:
//contains input initially and then manipulated to find solution
		short int matrix[9][9];
//keeps track of each unit a cell or (i,j)th position of-
//-matrix[9][9] belongs to, for the purpose of finding peers, to propagate
//the constraints
		short int peers[9][9][3];
//there are altogether 27 units in 9*9 sudoku each with 9 cell
//each cell have 2 points of co-ordinate i.e.(i,j)
		short int units[27][9][2];
//this will contain the possible values the (i,j)th position in matrix can take
//i.e constraint[i][j][k]//here k=9 too, because there are 9 digits, 9 possibilities
		short int cnstrnt[81][10];
/*	if value of constraint[][0] is '11', it means that's the input and shouldn't be messed with;
	if value of constraint[][i], at any point i<9 is '0', it means that we don't have to look after it...
	//10th position of cnstrnt i.e. cnstrnt[][10] contains no.of constraints...
*/              int tag[20];//keeps track of the positions whose constraints are affected by recent trial

	public:
	//initializing obect with its own copy of input through constructor
		sudokuSolver(){
			for(int i=0;i<9;i++)
				for(int j=0;j<9;j++)
					matrix[i][j]=sudoku[i][j];

			initialCalculation();
			constraintCalculation();

		}

		void initTag(){
			for(int i=0;i<20;i++)
				tag[i]=81;//the choice of number 81 is because 9*9 is 81, but in array index ranges from 0-80
		}

		void copyFromTag(int *mark)
		{
			for(int i=0;i<20;i++)
				mark[i]=tag[i];
		}

		void copyToTag(int *mark)
		{
			for(int i=0;i<20;i++)
				tag[i]=mark[i];
		}

/*
I worked so hard to make these two functions, specially posToNum(), but
if i declare cnstrnt, a 2-dimensional array [81][10], to be a 3-dimensional
array [9][9][10], these two functions can simply be avoided........
	////so that's what i'm doing now//////
also use of these functions requires a lot of calculation, so there is + point..///
///only downside is that 3-dimensional arrays are difficult to understand///
//it took me 17hrs to realize this///
//after 30min and not being able to make the code work with 3-dimensional array
i'm back to posToNum()////
*/

//maps the num in cnstrnt[num][], to its corresponding (i,j) position in matrix (or sudoku grid)
		void numToPos(int num, int *i,int *j)
		{
			int count=0;
			for(int k=0;k<9;k++){
				for(int l=0;l<9;l++){
					if(count==num){
						*i=k;
						*j=l;
						return;
					}
					++count;
				}
			}
		}

//maps (i,j) position of matrix[i][j] to num of cnstrnt[num][]...
		int posToNum(int i,int j)
		{
			int count=0;
			int k,l;
		//finally figured this function;;;;
	//wrong perception in use of && and || had been a problem so far
		//also in other places of this program
			for(k=0;k<9;k++){
				for(l=0;l<9;l++){
					if(k<=i || l<=j) ++count;
					if(k==i && l==j) break;
				}
				if(k==i && l==j) break;
			}
			return count-1;
		}


		void initialCalculation()
		{
			int i,j,k=0,l=0;
		//calculate position of each member of each units in matrix[9][9]
			for(i=0;i<9;i++){
				for(j=0;j<9;j++)
				{
				//row units
					units[i][j][0]=i;
					units[i][j][1]=j;
	//this shows that cell (i,j) is in ith row unit ||
		// or it has peers in ith row..
					peers[i][j][0]=i;

				//column units
					units[9+j][i][0]=i;
					units[9+j][i][1]=j;
					peers[i][j][1]=9+j;

				//box units
		//calculating kth position for units[k][][]
					if(j<=2) k=0;
					else{
						if(j<=5)k=1;
						else k=2;
					}
					if(i>2){
						if(i<=5) k=k+3;
						else k=k+6;
					}
		//calculating lth position for units[][l][]
					if(j%3==0){//while traversing through each cell in row, if box changes..do following..
						if(i%3==0) l=0;
						else if(i%3==1)l=3;
						else l=6;
					}
					else ++l;
					units[k+18][l][0]=i;
					units[k+18][l][1]=j;
					peers[i][j][2]=k+18;
				}
			}
		}

		void constraintCalculation()
		{
			int k,n=0;
//this for statement initializes the constraint....
	//if the cell is empty (i.e. no input) or there is a digit
			for(int i=0;i<9;i++){
				for(int j=0;j<9;j++)
				{
				//if there is digit
					if(matrix[i][j]){
						cnstrnt[n][0]=11;
						cnstrnt[n][9]=0;
						for(k=1;k<9;k++)
							cnstrnt[n][k]=0;
					}
				//if there is no digit then cnstrnt are digits [0-9]
					else{
						for(k=0;k<9;k++)
							cnstrnt[n][k]=k+1;
						cnstrnt[n][9]=9;
					}
					++n;
				}
			}
	//above we initialized the constraint which is not the real constraint
//now we propagate contraint across the peers of the cell where there is input...
			initialPropagation();
		}//end of constraintcalculation()


		void initialPropagation()
		{
//for each cell of the sudoku grid if there is input, propagate the value to its peers
			for(int i=0;i<9;i++)
				for(int j=0;j<9;j++)
					if(matrix[i][j]){
						initTag();
//1 passed to constraintPropagator() means that initialPropagation is forward propagation
						constraintPropagator(i,j);
					}
		} //end of initialPropagation()


//here num is the number that points to the peer unit
// and value is that which is to be removed from cnstrnt[][]...
//here stat tells whether to call remove() or write()//forward or backward propagation
		void Adjustment(int num, short int value)
		{
			int x,y,n;

			for(int i=0;i<9;i++){

				x=units[num][i][0];
				y=units[num][i][1];
//only remove or write the constraint for that position in matrix which is not filled
		//it also says if the position is where input is, ignore...
				if(matrix[x][y]==0){
					n=posToNum(x,y);
					remove(n,value);
				}
			}
		}//end of Adjustment()

		void unitElim(int n)
		{
			int cns[9];
			int val[9]={0,0,0,0,0,0,0,0,0};
			int max=0;
			for(int i=0;i<9;i++){
				cns[i]=posToNum(units[n][i][0],units[n][i][1]);
			}
			for(i=0;i<9;i++){
				if(max<cnstrnt[cns[i]][9]) max=cnstrnt[cns[i]][9];
				if(cnstrnt[cns[i]][0]==11)val[i]=matrix[units[n][i][0]][units[n][i][1]];
			}



		}
//here is num is same as above but pos is the position in cnstrnt[][pos] which is to be removed
//remove() works pretty much like pop operation in stack...
		void remove(int num, short int value)
		{
		//if flag=1, there is cnstrnt that matches value, if flag=0, there is no match
			short int flag=0;
			int n=0;

			for(int i=0;i<9;i++)
			{

				if(cnstrnt[num][i]==value){
					while(tag[n]!=81){ ++n; }
					tag[n]=num;
					flag=1;
				}
				if(flag==1){
					cnstrnt[num][i]=cnstrnt[num][i+1];
				}
			}
			if(flag==1){
				cnstrnt[num][8]=0;
		 //reducing no.of constraint//i.e. one less constraint(possiblity)....
				if(cnstrnt[num][9]!=0)
					--cnstrnt[num][9];
			}
		} //end of remove()

//   places the constraints back into the queue to be tried next time..a constraint Repropagator
		void write(int value)
		{
			int m=0,n=0,num;
			while(tag[n]!=81){
				num=tag[n];
				m=cnstrnt[num][9];
				++cnstrnt[num][9];
				cnstrnt[num][m]=value;
		 //increasing no.of constraint//i.e. one more constraint(possiblity)....
				++n;
			}
		}


//////////////start coding here////////////////////
//what follows from here are the codes that is added to sudokupr.cpp file

		void constraintPropagator(int i, int j)
		{
			int x,y,z;
			x=peers[i][j][0];
			y=peers[i][j][1];
			z=peers[i][j][2];

			Adjustment(x, matrix[i][j]);
			Adjustment(y, matrix[i][j]);
			Adjustment(z, matrix[i][j]);

		} //end of constraintPropagator()

//return the pos in cnstrnt[pos][] with minimum no.of constraint(possibilities left)
		int minimumConstraint()
		{
			int counter=0, pos, minimum;
		//initializing the minimum no.of constraint to be maximum
			minimum=9;

//searching for minimum constraint from the end.....
			for(int i=0;i<9;i++){
				for(int j=0;j<9;j++)
				{
			//if the cell is not already filled
					if(matrix[i][j]==0){
						if(minimum>cnstrnt[counter][9]){
							minimum=cnstrnt[counter][9];
							pos=counter;
						}
					}
					++counter;
				}
			}
			return pos;
		}//end of minimumConstraint()

		int check()
		{
	//see if any cell is still unfilled
			for(int i=0;i<9;i++)
				for(int j=0;j<9;j++)
					if(matrix[i][j]==0) return 0;
	  // if all the cells are filled
			return 1;
		}

      //this is the real deal...
		int solutionFinder(int depth=0)
		{
//pos contains position in cnstrnt with minimum constraint
		//and num; number of constraints in that position
//(i,j)th cell of matrix with minimum constraints
//value contains the constraint that is examined (tried)
			int i,j,pos,status,num,counter=0;
//finding the position in cnstrnt[pos][] with minimum no.of constraints remaining
	//i.e. it minimumConstraint() returns pos value...
			pos=minimumConstraint();
			numToPos(pos,&i,&j);

			num=cnstrnt[pos][9];
			//since tag is global for this class and the contents changes in every depth of recursion.
	//this array copies the contents in tag for purpose of respective recursion depth
			int mark[20];
			if(num<0)num=0;
//while(num) means try each and every constraint for current cell i.e. untill soln found
			while(num!=0){
	       //here value works as buffer element coz' constraint propagation erases the constraint.....
				initTag();
				matrix[i][j]=cnstrnt[pos][counter];
				--num;
				++counter;
				gShow();
	       //
				setviewport(0,0,100,100,1);
				clearviewport();
				sprintf(msg,"depth=%d", depth);
				outtextxy(0,20,msg);
				sprintf(msg,"(%d,%d)",i,j);
				outtextxy(0,40,msg);
				sprintf(msg,"value=%d",cnstrnt[pos][9]);
				outtextxy(0,60,msg);
				setviewport(0,0,getmaxx(),getmaxy(),1);
	       //
			 //	delay(500);
				constraintPropagator(i,j);
				copyFromTag(mark);
		//check if all the cells of sudoku grid || matrix is completely filled
			//if yes then the solution is found.....
				status=check();
				if(status==1) return status;

				status=solutionFinder(depth+1);
//only return if status=true from this point,,
//if status!=true function will return in next loop, if no constraints left,,
// see if statement 6 lines above
				 //
				setviewport(0,0,100,100,1);
				clearviewport();
				sprintf(msg,"depth=%d", depth);
				outtextxy(0,20,msg);
				sprintf(msg,"(%d,%d)",i,j);
				outtextxy(0,40,msg);
				setviewport(0,0,getmaxx(),getmaxy(),1);
				//
				if(status==1) return status;
				else gShow();
			//	delay(250);
//push back the current value tried in current cell to its corresponding cnstrnt[num][] position

				initTag();
				copyToTag(mark);
				write(matrix[i][j]);
				matrix[i][j]=0;

			}
			return 0;
		}

		void gShow()
		{
			int m,n,p;
			m=getmaxx()/2-200;
			n=getmaxy()/2-190;
			setfillstyle(SOLID_FILL,BLUE);

			for(int i=1;i<=9;i++){
				for(int j=1;j<=9;j++)
				{

					setviewport((m-10+(j*40)),(n-10+(i*40)),(m+10+(j*40)),(n+10+(i*40)),1);
					clearviewport();
					if(matrix[i-1][j-1]!=0){
						p=posToNum(i-1,j-1);
						if(cnstrnt[p][0]==11){
							floodfill(1,1,getmaxcolor());
						}
						setviewport(0,0,getmaxx(),getmaxy(),1);
						setcolor(WHITE);
						sprintf(msg,"%d",matrix[i-1][j-1]);
				// j pos maps to x co-ordinate and i pos maps to y co-ordinate
						outtextxy((m-4+(j*40)),(n-12+(i*40)),msg);
					}
				}
			}

			sprintf(msg,"Press \'q\' to terminate");
			outtextxy(50,getmaxy()-40,msg);
		}
/*
		void show()
		{
			int i,j;
			for(i=0;i<9;i++){
				for(j=0;j<9;j++)
					cout<<matrix[i][j]<<"\t";
				cout<<endl;
			}
			getch();
			clrscr();

			for(i=0;i<27;i++){
				for(j=0;j<9;j++)
					cout<<units[i][j][0]<<" "<<units[i][j][1]<<"   ";
				cout<<endl;
			}
			getch();
			clrscr();

			for(i=0;i<9;i++){
				for(j=0;j<9;j++)
					cout<<peers[i][j][0]<<" "<<peers[i][j][1]<<" "<<peers[i][j][2]<<"  ";
				cout<<endl;
			}
			getch();
			clrscr();

			for(j=0;j<81;j++){
				for(i=0;i<10;i++)
					cout<<cnstrnt[j][i]<<" ";
				cout<<endl;
			}
			getch();
			clrscr();

		} //end of show()
*/
};



void drawGrid()
{
//center co-ordinate of the screen
	int x=getmaxx()/2;
	int y=getmaxy()/2;
	int k;

	setbkcolor(0);
	//lines to separates the 3*3 boxes
	rectangle(x-178,y-168,x+178,y+188);
	line(x-62,y-178,x-62,y+198);
	line(x+62,y-175,x+62,y+198);
	line(x-188,y-52,x+188,y-52);
	line(x-188,y+72,x+188,y+72);

	//drawing grid, 9*9 rectangles...
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++){
			setfillstyle(SOLID_FILL,10);
			rectangle(x-180+(j*40),y-170+(i*40),x-140+(j*40),y-130+(i*40));
			floodfill(x-160+(j*40),y-150+(i*40),getmaxcolor());
			setfillstyle(SOLID_FILL,2);
			setviewport(x-176+(j*40),y-166+(i*40),x-144+(j*40),y-134+(i*40),1);
			floodfill(1,1,getmaxcolor());
			setviewport(0,0,getmaxx(),getmaxy(),1);
			rectangle(x-174+(j*40),y-164+(i*40),x-146+(j*40),y-136+(i*40));
		}


//drawing done button....
	settextstyle(0, HORIZ_DIR, 2);
	rectangle(x+200,y-5,x+280,y+20);
	setfillstyle(SOLID_FILL,9);
	floodfill(x+201,y-3,getmaxcolor());
	setcolor(BLACK);
	sprintf(msg,"DONE");
//this outtexts below causes shadowing..
	outtextxy(x+212,y+2,msg);
	outtextxy(x+211,y+1,msg);
//this is real text...
	setcolor(BLUE);
	outtextxy(x+210,y,msg);

//setting text style which is used to display "SUDOKU"
	settextstyle(1, HORIZ_DIR, 5);
	i=133;j=18;
	for(k=0;k<8;k++){//text shadowing...
		if(k<7)setcolor(3);
		else	setcolor(12);
		outtextxy(x-i,j,"S   D   K  ");
		//this increment and decrement gives the shadow effect
		++i;--j;
	}

//j=25 is the y-co-ordinate value to display "SUDOKU"
	j=25;

	setcolor(BLUE);
	//text decoration; line-through
		line(x-145,j+15,x+140,j+15);
		line(x-145,j+16,x+140,j+16);

	setcolor(WHITE);
	//values that determine the position where the message below will be displayed
	i=133;j=18;
	for(k=0;k<8;k++){//text shadowing...
		if(k<7)setcolor(2);
		else	setcolor(11);
		outtextxy(x-i,j,"  U   O   U");
		//this increment and decrement gives the shadow effect
		++i;--j;
	}

	j=41;

	setcolor(BLUE);
	//text decoration; Underline
		line(x-145,j+15,x+140,j+15);
		line(x-145,j+16,x+140,j+16);

	setcolor(WHITE);
	//changing text style to that which will be used for message displaying..
	settextstyle(1, HORIZ_DIR, 1);

	clock_t start, end;

	//setting viewport to display initial messages
	setviewport(0,getmaxy()-40,getmaxx(),getmaxy(),1);

//Initial message display follows below:
	delay(200);
	start = clock();
	for(end=clock();((end-start)/CLK_TCK)<13;end=clock())
	{
		if(kbhit())break;
   //display message only once at the begining
		if(((end-start)/CLK_TCK)<=3)
		{
			sprintf(msg,"Click Within grid");
			outtextxy(0,10,msg);
			sprintf(msg," and press q to terminate");
			outtextxy(150,10,msg);
		}

		if(((end-start)/CLK_TCK)>3 &&((end-start)/CLK_TCK)<4 ) clearviewport();

		if(((end-start)/CLK_TCK)>4&&((end-start)/CLK_TCK)<7){
			sprintf(msg,"Click on the cell");
			outtextxy(0,10,msg);
			sprintf(msg,"press [1-9] key to");
			outtextxy(160,10,msg);
			sprintf(msg," input SUDOKU");
			outtextxy(320,10,msg);
		}

		if(((end-start)/CLK_TCK)>7 &&((end-start)/CLK_TCK)<8 ) clearviewport();

		if(((end-start)/CLK_TCK)>8&&((end-start)/CLK_TCK)<11){
			sprintf(msg,"Click on the DONE");
			outtextxy(0,10,msg);
			sprintf(msg,"Button on the left side");
			outtextxy(180,10,msg);
			sprintf(msg," to find the solution");
			outtextxy(400,10,msg);
		}
		if(((end-start)/CLK_TCK)>11){
			clearviewport();
			sprintf(msg,"GO......");
			outtextxy(getmaxx()/2-20,10,msg);
		}

	}
	clearviewport();
	setviewport(0,0,getmaxx(),getmaxy(),1);

}
//end of drawGrid()


main()
{
	//variables required for initializing graphics..
	int gd=DETECT,gm;

	char c='o';//initialization of input buffer...
	pos[0]=pos[1]=9;//initialization of global variables

      //these variables are for mouse input...
      //b:button clicked, x: x position of mouse, y: y position of mouse...
	int b,x,y;

       //initializing graphics
	initgraph(&gd,&gm,"/tc/bgi");
      //drawing sudoku grid
	drawGrid();
      //initializing mouse device and showing mouse...
	initmouse();
	showmouseptr();

	inputSudoku *sudo;
	sudokuSolver *solver;

      //creating inputSudoku object...
	sudo=new inputSudoku;

	while(c!='q')
	{
		getmousepos(&b,&x,&y);
		sudo->setMpos(&b,x,y);
		sudo->hoverEffect();
		c=sudo->takeInput();
	//this avoids keys/input pressed when left mouse button is not clicked
		if(kbhit()){ c=getch(); }
	}

	if(flag==1){

//now that we've collected the input,, there is no use of sudo object
		delete sudo;
//if the user provided with adequate input and pressed done
		solver= new sudokuSolver;
		flag=solver->solutionFinder();
		solver->gShow();
		if(flag==0){
			sprintf(msg,"!!!!!Solution Does Not Exist!!!!");
			outtextxy(getmaxx()/2-30,getmaxy()/2,msg);
			flag=1;
		}
		while(getch()!='q');
	}
	if(flag==0){
    //displaying message that user has pressed 'q' and the program is terminating...
		sprintf(msg,"You Pressed \'q\' Terminating....");
		outtextxy(0,getmaxy()-30,msg);
		delay(1000);
	}

	closegraph();

	return 0;
}
