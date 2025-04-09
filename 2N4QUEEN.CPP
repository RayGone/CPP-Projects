/*for this program to work for any value of n, change the value to b[n][0] in solutioner(); and
	adjust the dimension in show() and a[n][n] of chess-board..
	adjust the initialize function as per the no.of queens....
	I think that's pretty much it for this program to solve n-queen problem for any value of n..
	!!!!^^^^THANK YOU^^^^!!!!*/

#include<iostream>
#include<conio.h>

using namespace std;

void clrscr(void){
	try{
		system("cls");
	}catch(exception){
		system("clear");
	}
}

class board
{
	private:
		int a[20][20];//array to imitate upto 10*10 chess board
		int n;
		int queens;

	public:
		board(){ queens=0;initialize();}

		void initialize()
		{
			for(int i=0;i<20;i++)
				for(int j=0; j<20; j++)
					a[i][j]=0;

			n=10;
			show();
		}
		void nqueensolver()
		{
			again:
			clrscr();
			cout<<"This program can solve from n=4 upto n=10 queen"<<endl<<endl;
			cout<<"Input number of queen to solve problem for: ";
			cin>>n;
			cout<<endl<<endl;
			if(n<4||n>20){
				cout<<"please choose any number between n=4 upto 20"<<endl;
				getch();
				goto again;
			}
			solutioner();

		}

		void solutioner()
		{
			int b[10][1];//to keep track of the positions where queen has already been placed before
			a[0][1]=1;//starting by placing the first queen on 2nd box of first row
			queens=1;//no. of queens placed on the board is 1, initially.
			int stat=0;//flag for checking if a queen can be placed in current box
			int i=1;
			int j=0;
			do{
				nextcolumn:

				if(j==n)
				{
					i--;//bactracking
					j=b[i][0]+1;//going ahead from the position where queen is placed
					a[i][j-1]=0;//removing the queen.
					queens--;//subtracting no.of queens by 1 since backtracking
					cout<<"backtracking-no queens can be placed in the current row"<<endl;
					cout<<"stepping back up a row and removing queen and continuing from next column";
					if(j==n)
					{
						i--;j=b[i][0]+1;
						a[i][j-1]=0;
						queens--;
						cout<<endl;
						cout<<"!!!:D!!!!:D!!!!Since the queen is in the last column, we bactrack again";

					}
				}
				nextline:
				clrscr();
				cout<<"\tThis program is currently working to solve n-queen problem for "<<n<<"-queen"<<endl;
				cout<<endl<<endl;
				show();

				cout<<endl<<endl;
				cout<<"row number: "<<i+1<<endl<<endl;
				cout<<"column number: "<<j+1<<endl;

				stat=ym_check(i,j);//checking up
				cout<<"vertical up check "<<"flag= "<<stat<<endl;


				if(stat==0)
				{
					j++;
					cout<<"!!!!not good to go"<<endl;

					goto nextcolumn;
				}

				stat=xm_ym_check(i,j);//checking up left
				cout<<"diagonal up-left check "<<"flag= "<<stat<<endl;


				if(stat==0)
				{
					j++;
					cout<<"!!!!not good to go"<<endl;

					goto nextcolumn;
				}

				stat=xp_ym_check(i,j);//checking up right
				cout<<"diagonal up-right check flag= "<<stat<<endl;


				if(stat==0)
				{
					j++;
					cout<<"!!!!not good to go"<<endl;

					goto nextcolumn;
				}

				cout<<"\t****good to go****"<<endl;;


				a[i][j]=1;//placing the queen in ith row and jth column
				b[i][0]=j;//caching the position where queen is placed
				queens++;//updating no.of queens placed on the board
				i++;
				j=0;
				if(i!=n-1&&queens!=n)goto nextline;
			}while(queens!=n);
			clrscr();
			show();
			cout<<"%%%%THIS IS THE SOLUTION OF "<<n<<"-queen problem%%%%";
		}




		int check(int i, int j)
		{       //there is no horizontal line checks coz i don't think they are necessary
			int stat;
				stat=ym_check(i,j); //check vertical up;
				if(stat==0)return 0;

				stat=xm_ym_check(i,j); //diagonal left up
				if(stat==0)return 0;

				stat=xp_ym_check(i,j);//diagonal right up
				if(stat==0)return 0;

			return 1;
		}




		int ym_check(int i,int j)
		{
			int c;
			for(c=i;c>=0;c--)
				if(a[c][j]==1)return 0;
			return 1;
		}



		int xp_ym_check(int i,int j)
		{
			int c=i;
			int d=j;
			doit2:
				if(a[c][d]==1)return 0;
			if(c!=0&&d!=n-1){c--;d++; goto doit2;}
			return 1;
		}



		int xm_ym_check(int i,int j)
		{
			int c=i;
			int d=j;
			doit3:
				if(a[c][d]==1)return 0;
			if(c!=0&&d!=0){c--;d--; goto doit3;}
			return 1;
		}



		void show()
		{
			for(int i=0; i<n;i++)
			{
				for(int j=0; j<n; j++)
				{
					cout<<a[i][j]<<"   ";
				}
				cout<<endl<<endl;
			}
		}

};


main()
{
	clrscr();
	board b1;
	getch();
	b1.nqueensolver();
	cout<<endl<<endl<<endl<<endl;
	cout<<"\t\t!!!!^^^^THANK YOU^^^^!!!!";
	getch();
	return 0;

}