#include<math.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<string.h>
#include<dos.h>
#include<iostream.h>
#include<bios.h>
#include<ctype.h>
#include<time.h>
#include<process.h>

//variables and methods for mouse operation

int eventpart;
int numOfNode;
int diff;
int midx;
int midy;
float tmax;
float tmin;
float h1;
float h2;
float wind;
FILE *f;


//User Data Type to keep weather attribute values
typedef struct{
	int param[6];

}attr;


//User Data Type ot create Decision Tree
struct tree{
	int attrNo;//number mapping to param[] in attr i.e. param[attrNo]
	int numOfChild;//no of attribute values that decides no. of children the should have..
	int decision;//1=yes, 2=no, 3=no decision
	tree *child;
};

struct fraction
{
	int numerator;
	int denominator;
};


void welcomeimagepaste();
void anlayseimagepaste();
void builddecisiontree(tree *);

class MouseController{
	private:
		union REGS i,o;

	public:
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
};

class DataClass
{
	public:
		int tempMaxClass(float val)
		{
			//classifying temperature in discrete class..
			if(val<15.0)return 0;//all the temperature below 15
			else if(val<18.0)return 1;
			else if(val<21.0)return 2;
			else if(val<24.0)return 3;
			else if(val<27.0)return 4;
			else if(val<30.0)return 5;
			else if(val<32.0)return 6;
			else if(val<35.0)return 7;
			else return 8;//all the temperature above 35
		}

		int tempMinClass(float val)
		{
			//classifying temperature in discrete class..
			if(val<0.0)return 0;//all the temperature below 0.0
			else if(val<5.0)return 1;
			else if(val<10.0)return 2;
			else if(val<13.0)return 3;
			else if(val<16.0)return 4;
			else if(val<18.0)return 5;
			else if(val<20.0)return 6;
			else if(val<22.0)return 7;
			else return 8;//all the temperature above 21
		}

		int humidClass(float val)
		{
			//classifying humidity in discrete class..
			if(val>90.0)return 7;//all the humidity above 90
			else if(val>80.0)return 6;
			else if(val>70.0)return 5;
			else if(val>60.0)return 4;
			else if(val>50.0)return 3;
			else if(val>40.0)return 2;
			else if(val>30.0)return 1;
			else return 0;//all the humidity 20 and below
		}

		int wsClass(float val)
		{
			//classifying wind speed in discrete class..
			if(val>3.0)return 6;//all the speed above 3.0
			else if(val>2.5)return 5;
			else if(val>2.0)return 4;
			else if(val>1.5)return 3;
			else if(val>1.0)return 2;
			else if(val>0.5)return 1;
			else return 0;//all the humidity below 0.5
		}

		int precipClass(float val)
		{
			//classifying precipitation in yes or no class..
			if(val>0)return 1;//no rain
			else return 0;//it rains

		}
};


class DataReader:DataClass
{
	attr *samples;
	int totalSamples;

	public:
		DataReader(){
			totalSamples=calcTotalSample();
			samples=new attr[totalSamples];
			readTemp("tstData/04/temp04.txt");
			readHumidity("tstData/04/humid04.txt");
			readWindSpeed("tstData/04/wind04.txt");
			readPrecipitation("tstData/04/precip04.txt");
		      //	show();
		}

		void setTotalSamples(int num)
		{
			totalSamples=num;
			delete samples;
			samples=new attr[totalSamples];
		}


		int getTotalSamples()//to get a copy of private data member totalSamples
		{
			return totalSamples;
		}

		int getSamples(int sampleNo, int attrNo)//to get a pointer to private data member samples
		{
			return samples[sampleNo].param[attrNo];
		}

		int calcTotalSample()
		{
			int n=0;
			char c;
			FILE *fp;
			fp=fopen("tstData/04/temp04.txt","r");
			c=getc(fp);
			while(c!=EOF){
				if(c=='\n')n++;
				c=getc(fp);
			}
			fclose(fp);
			return n-2;
		}

		void empty(char *cp)
		{
			for(int i=0;i<8;i++)
			{
				cp[i]='0';
			}
		}

		void readTemp(char *path)
		{
			int i=0,j=0,k=0;
			char c;
			char *c1;
			c1=new char[8];
			FILE *fp=fopen(path,"r");
			c=getc(fp);
			while(i<2){
				if(c=='\n')i++;
				c=getc(fp);
			}
			c1="0.000000";

			for(i=0;i<totalSamples;i++)
			{
				if(c=='\n')c=getc(fp);
				while(c!='\t'){
					c1[j]=c;
					if(c=='.')k=1;
					c=getc(fp);
					j++;
				}
				if(k!=1)c1[j]='.';

				Classifier(0,i,c1);
				c=getc(fp);
				empty(c1);
				j=k=0;
				while(c!='\n'){
					c1[j]=c;
					if(c=='.')k=1;
					c=getc(fp);
					j++;
				}
				if(k!=1)c1[j]='.';

				Classifier(1,i,c1);
				empty(c1);
				j=k=0;
			}
		}


		void readHumidity(char *path)
		{
			int i=0,j=0,k=0;
			char c;
			char *c1;
			c1=new char[8];
			FILE *fp=fopen(path,"r");
			c=getc(fp);
			while(i<2){
				if(c=='\n')i++;
				c=getc(fp);
			}
			c1="0.000000";
			for(i=0;i<totalSamples;i++)
			{
				if(c=='\n')c=getc(fp);
				while(c!='\t'){
					c1[j]=c;
					if(c=='.')k=1;
					c=getc(fp);
					j++;
				}
				if(k!=1)c1[j]='.';

				Classifier(2,i,c1);
				c=getc(fp);
				empty(c1);
				j=k=0;
				while(c!='\n'){
					c1[j]=c;
					if(c=='.')k=1;
					c=getc(fp);
					j++;
				}
				if(k!=1)c1[j]='.';

				Classifier(3,i,c1);
				empty(c1);
				j=k=0;
			}
		}

		void readWindSpeed(char *path)
		{
			int i=0,j=0,k=0;
			char c;
			char *c1;
			c1=new char[8];
			FILE *fp=fopen(path,"r");
			c=getc(fp);
			while(i<2){
				if(c=='\n')i++;
				c=getc(fp);
			}
			c1="0.000000";
			for(i=0;i<totalSamples;i++)
			{
				if(c=='\n')c=getc(fp);

				while(c!='\n'){
					c1[j]=c;
					if(c=='.')k=1;
					c=getc(fp);
					j++;
				}
				if(k!=1)c1[j]='.';

				Classifier(4,i,c1);
				empty(c1);
				j=k=0;
			}
		}

		void readPrecipitation(char *path)
		{
			int i=0,j=0,k=0;
			char c;
			char *c1;
			c1=new char[8];
			FILE *fp=fopen(path,"r");
			c=getc(fp);
			while(i<2){
				if(c=='\n')i++;
				c=getc(fp);
			}
			c1="0.000000";
			for(i=0;i<totalSamples;i++)
			{
				if(c=='\n')c=getc(fp);

				while(c!='\n'){
					if(c=='T')c='0';
					c1[j]=c;
					if(c=='.')k=1;
					c=getc(fp);
					j++;
				}
				if(k!=1)c1[j]='.';

				Classifier(5,i,c1);
				empty(c1);
				j=k=0;
			}
		}

		void Classifier(int attrNo, int sampleNo,char *cp)
		{
			float val;
			val=atof(cp);
			switch(attrNo){
				case 0:	samples[sampleNo].param[attrNo]=tempMaxClass(val); break;
				case 1: samples[sampleNo].param[attrNo]=tempMinClass(val); break;
				case 2: samples[sampleNo].param[attrNo]=humidClass(val); break;
				case 3: samples[sampleNo].param[attrNo]=humidClass(val); break;
				case 4: samples[sampleNo].param[attrNo]=wsClass(val); break;
				case 5: samples[sampleNo].param[attrNo]=precipClass(val); break;
				default: cout<<"This line should never execute";
			}
	       //		cout<<cp<<" "<<val<<" "<<attrNo<<" "<<samples[sampleNo].param[attrNo]<<endl;
	       //		delay(100);

		}

		void show()
		{
			cout<<totalSamples<<endl;
			for(int i=0;i<totalSamples;i++)
			{
				cout<<i<<"\t"<<samples[i].param[0]<<" "<<samples[i].param[1]<<" "<<samples[i].param[2]<<" "<<samples[i].param[3]<<" "<<samples[i].param[4]<<" "<<samples[i].param[5]<<endl;
			    //	delay(100);
			}
		}


};

class UserInterface:DataClass
{
	private:
		attr input;
		tree *dtree;

	public:
		void setDTree(tree *t){
			dtree=t;
		}
		//takes input in DTAnalyser class
		void takeInput(int *temp)
		{
			for(int i=0;i<6;i++)
				input.param[i]=temp[i];
		}


		//takes input in GUI class
		void takeInput(double Tmax, double Tmin, double H1, double H2, double WS)
		{
			input.param[0]=tempMaxClass(Tmax);
			input.param[1]=tempMinClass(Tmin);
			input.param[2]=humidClass(H1);
			input.param[3]=humidClass(H2);
			input.param[4]=wsClass(WS);
		}

		int traverse()
		{
			tree *temp=dtree;

			while(temp->child!=NULL){
				temp=&temp->child[input.param[temp->attrNo]];
			}

			if(temp->decision==1){
			//	cout<<endl<<"\t\tAS PER THE INPUT GIVEN, IT WILL RAIN";
				return 1;
			}
			else if(temp->decision==0){
			//	cout<<endl<<"\n\t\tAS PER THE INPUT GIVEN, IT WILL NOT RAIN";
				return 2;
			}
			else{
			//	cout<<endl<<"\nAS PER THE INPUT GIVEN, IT  IS NOT POSSIBLE TO SAY WHETHER IT RAINS OR NOT";
				return 3;
			}
		}
};


class DTAnalyser:DataReader
{
	private:
		const char *parentdir;//contains string "tstData/"; directory where data is looked for
		const char *tp;//contains string "temp"; later the directory name given by user is concatenated to get file name; e.g. 95 then file is temp95.txt//
		const char *ws;//contins string "wind"; same as temp;
		const char *hm;//contins string "humid"; same as temp;
		const char *pr;//contins string "precip"; same as temp;
		char *dir;//pointer dir contains the name of folder inside folder tstData that contains temp,humidity,wind and precipitation data files
		char *filepath;//complete path to the file

		UserInterface ui;
		int *array;
		fraction precisionVal;
		fraction recallVal;
		int part;
	public:

		DTAnalyser(tree *node, char *path)
		{
			dir=path;
			automate(node);
		}


		void automate(tree *node)
		{
			parentdir="tstData/";//folder which contains test files for analysis
			tp="temp";
			ws="wind";
			hm="humid";
			pr="precip";

			ui.setDTree(node);
			dataReap();
			analyse();
			part=1;
			GUIshow();

			read();
			dataReap();
			analyse();
			part=2;
			GUIshow();
		}

		void dataReap()
		{
			int temp[6];
			array=new int[getTotalSamples()];
			for(int i=0;i<getTotalSamples();i++){
				for(int j=0;j<6;j++){
					temp[j]=getSamples(i,j);
				}
				ui.takeInput(temp);
				array[i]=ui.traverse();
				if(array[i]==2)array[i]=0;//value 2 is returned when prediction is no rain but in sample no rain is denoted by 0...
			}
		}

		void path(int param)
		{
			sprintf(filepath,"%s%s/",parentdir,dir);
			switch(param){
				case 1:	strcat(filepath,tp);  break;
				case 2: strcat(filepath,hm); break;
				case 3: strcat(filepath,ws); break;
				case 4: strcat(filepath,pr); break;
				default: break;
			}
			strcat(filepath,dir);
			strcat(filepath,".txt");
		}

		void read()
		{
			path(1);
			calcTotalSample(filepath);
	       //		cout<<filepath<<endl;
			readTemp(filepath);
			sprintf(filepath," ");

			path(2);
	      //		cout<<filepath<<endl;
			readHumidity(filepath);
			sprintf(filepath," ");

			path(3);
	      //		cout<<filepath<<endl;
			readWindSpeed(filepath);
			sprintf(filepath," ");

			path(4);
	      //		cout<<filepath;
			readPrecipitation(filepath);

			sprintf(filepath," ");
		}

		void calcTotalSample(char *path)
		{
			int n=0;
			char c;
			FILE *fp;
			fp=fopen(path,"r");
			c=getc(fp);
			while(c!=EOF){
				if(c=='\n')n++;
				c=getc(fp);
			}
			fclose(fp);
			setTotalSamples(n-2);
		}

		void analyse()
		{
			int val=0;//number of yes and no..
			for(int i=0;i<getTotalSamples();i++)
			{
				if(array[i]==1 || array[i]==0) val++;
			}
	     //		cout<<endl<<"number of yes or no values returned by dtree for the sample:"<<val<<endl;
			precision(val);
			recall(val);
		}

		void precision(int val)
		{
			int match=0;//number of matching value in array and samples[5] i.e. precipitaion
			for(int i=0;i<getTotalSamples();i++)
			{
				if(array[i]==getSamples(i,5))match++;
			}
	     //		cout<<"number of returned yes or no that matches with sample: "<<match<<endl;
			while(1){
				if(match==0 || val==0){
			//val=0 is the case of infinity coz' num/0 is inifinity
					precisionVal.numerator=0;
					precisionVal.denominator=1;
					break;
				}
				else if(val%2==0 && match%2==0){
					val=val/2;
					match=match/2;
				}
				else if(val%3==0 && match%3==0){
					val=val/3;
					match=match/3;
				}
				else if(val%5==0 && match%5==0){
					val=val/5;
					match=match/5;
				}
				else if(val%7==0 && match%7==0){
					val=val/7;
					match=match/7;
				}

				else{
					precisionVal.numerator=match;
					precisionVal.denominator=val;
					break;
				}
			}

		}

		void recall(int val)
		{
			int nos=getTotalSamples();
			while(1){
				if(nos==0 || val==0){
			//totalSamples=0 is the case of infinity coz' num/0 is inifinity
					recallVal.numerator=0;
					recallVal.denominator=1;
					break;
				}
				else if(val%2==0 && nos%2==0){
					val=val/2;
					nos=nos/2;
				}
				else if(val%3==0 && nos%3==0){
					val=val/3;
					nos=nos/3;
				}
				else if(val%5==0 && nos%5==0){
					val=val/5;
					nos=nos/5;
				}
				else if(val%7==0 && nos%7==0){
					val=val/7;
					nos=nos/7;
				}
				else{
					recallVal.numerator=val;
					recallVal.denominator=nos;
					break;
				}
			}
		}

		void show()
		{
			cout<<"precision of the program is: "<<precisionVal.numerator<<"/"<<precisionVal.denominator<<endl;
			cout<<"recall of the program is: "<<recallVal.numerator<<"/"<<recallVal.denominator<<endl;
		}

		void GUIshow()
		{
			setcolor(LIGHTBLUE);
			settextjustify(LEFT_TEXT,CENTER_TEXT);
			int midx=getmaxx()/2;
			int midy=getmaxy()/2;
			if(part==1){
				sprintf(filepath,"Sample: Training data");
				outtextxy(midx+20, midy-100,filepath);
				sprintf(filepath,"No.of samples: %d", getTotalSamples());
				outtextxy(midx+20,midy-90,filepath);
				sprintf(filepath,"Recall: %d/%d",recallVal.numerator,recallVal.denominator);
				outtextxy(midx+20,midy-80,filepath);
				sprintf(filepath,"precision: %d/%d",precisionVal.numerator,precisionVal.denominator);
				outtextxy(midx+20,midy-70,filepath);
			}
			if(part==2){
				sprintf(filepath,"Sample: %s",dir);
				outtextxy(midx+20, midy,filepath);
				sprintf(filepath,"No.of samples: %d", getTotalSamples());
				outtextxy(midx+20,midy+10,filepath);
				sprintf(filepath,"Recall: %d/%d",recallVal.numerator,recallVal.denominator);
				outtextxy(midx+20,midy+20,filepath);
				sprintf(filepath,"precision: %d/%d",precisionVal.numerator,precisionVal.denominator);
				outtextxy(midx+20,midy+30,filepath);
			}
			settextjustify(CENTER_TEXT,CENTER_TEXT);
		}
};

class GUI
{
	private:
		float Tmax;
		float Tmin;
		float H1;
		float H2;
		float WS;
		int midx;
		int midy;
		char stat[5];//'n'=no||'y'=yes;
		char *dir;
		MouseController mControl;
		int tabflag;

	public:
		GUI()
		{
			tabflag=0;

			Tmax=Tmin=H1=H2=WS=0.00;//dummy value
			for(int i=0;i<5;i++)
				stat[i]='n';

			int gdriver = DETECT, gmode, errorcode;

			/* initialize graphics mode */
			initgraph(&gdriver, &gmode, "/tc/bgi");

			/* read result of initialization */
			errorcode = graphresult();

			if (errorcode != grOk)  /* an error occurred */
			{
				printf("Graphics error: %s\n", grapherrormsg(errorcode));
				printf("Maybe the path is incorrect in initgraph()/n");
				printf("Press any key to halt:");
				getch();
				exit(1);             /* return with error code */
			}
			//initialize mouse device
			mControl.initmouse();

			midx=getmaxx()/2;
			midy=getmaxy()/2;

			initialDraw();
			titles();
			mControl.showmouseptr();


		}

		void redraw()
		{
			Tmax=Tmin=H1=H2=WS=0.00;//dummy value
			for(int i=0;i<5;i++)
				stat[i]='n';
			mControl.hidemouseptr();
			cleardevice();
			initialDraw();
			titles();
			mControl.showmouseptr();
		}

		void initialDraw()
		{
			welcomeimagepaste();
			settextjustify(CENTER_TEXT,CENTER_TEXT);
			settextstyle(1,HORIZ_DIR,2);
			setcolor(BLACK);
			outtextxy(getmaxx()/2,getmaxy()/2-50-80*2,"WELCOME TO RAINFALL PREDICTION SYSTEM");


			setcolor(getmaxcolor());
			for(int i=0;i<5;i++) //box for main title
				rectangle(i,0,getmaxx()-i,29);

	   //		setfillstyle(SOLID_FILL,DARKGRAY);
	  //		floodfill(5,5,getmaxcolor());

			setcolor(1);
			for(i=0;i<5;i++)//box for rest of the window
				rectangle(i,30+i,getmaxx()-i,getmaxy()-i);

	   //		setbkcolor(LIGHTGRAY);

			drawResponseBox();
			drawInfoBox();
			drawInputBox();
			drawTextField();
			drawButton(145,50,20,60,6);//top-left co-ordinate for done button
			drawButton(240,200,20,50,2);//top-left co-ordinate for exit button
			drawButton(-200,50,20,60,6);//top-left co-ordinate for analyse button
			//Underlining parameters
			setcolor(CYAN);
			line(midx-205,75,midx-295,75);
			line(midx-205,76,midx-295,76);
			line(midx-205,77,midx-295,77);
			line(midx-65,77,midx+5,77);
			line(midx-65,76,midx+5,76);
			line(midx-65,75,midx+5,75);
			line(midx+155,77,midx+245,77);
			line(midx+155,76,midx+245,76);
			line(midx+155,75,midx+245,75);
		}

		void drawResponseBox()
		{
			setcolor(BROWN);
			for(int i=0;i<8;i++){
				if(i%2==1)rectangle(50-i,175-i,getmaxx()-50+i,270+i);
			}
			setfillstyle(SOLID_FILL,WHITE);
			floodfill(51,176,BROWN);
		}


		void drawInfoBox()
		{
			//Info box
			setcolor(DARKGRAY);
			setfillstyle(SOLID_FILL,YELLOW);
			for(int i=0;i<5;i++)
				rectangle(midx-200-i,midy+100-i,midx+200+i,midy+200+i);
			floodfill(midx-199,midy+101,DARKGRAY);

			//box containing title "Info box"
			rectangle(midx-50,midy+83,midx+50,midy+95);
			setfillstyle(SOLID_FILL,DARKGRAY);
			floodfill(midx,midy+90,DARKGRAY);
		}

		void drawInputBox()
		{
			//input boxes
			setcolor(getmaxcolor());
			for(int i=0;i<3;i++){
				rectangle(midx-306-i,49-i,midx-149+i,161+i);
				rectangle(midx-78-i,49-i,midx+78+i,161+i);
				rectangle(midx+306+i,49-i,midx+149-i,161+i);
			}

			//filling input boxes with yellow color
			setfillstyle(SOLID_FILL,GREEN);
	  //		floodfill(midx-304,51,getmaxcolor());
	  //		floodfill(midx-76,51,getmaxcolor());
	   //		floodfill(midx+151,51,getmaxcolor());
		}

		void drawTextField()
		{
			//text field
			setcolor(BROWN);
			setfillstyle(SOLID_FILL,getmaxcolor());
			rectangle(midx-250,90,midx-170,110); //Tmax
			floodfill(midx-249,91,BROWN);
			rectangle(midx-250,120,midx-170,140);//Tmin
			floodfill(midx-249,121,BROWN);
			rectangle(midx-20,90,midx+60,110);//H1
			floodfill(midx-19,91,BROWN);
			rectangle(midx-20,120,midx+60,140);//H2
			floodfill(midx-19,121,BROWN);
			rectangle(midx+210,90,midx+290,110);//WindSpeed
			floodfill(midx+212,91,BROWN);
		}

		//x and y values are relative position value to the center of window
		void drawButton(int x, int y, int h, int w, int col)//(x,y) top-left co-ordinate for rectangle, h=height and w=width of rectangle
		{
			setcolor(col);
			rectangle(midx+x,midy+y,midx+x+w,midy+y+h);
			setfillstyle(SOLID_FILL,col);
			floodfill(midx+x+3,midy+y+3,col);
			setcolor(LIGHTGREEN);
			for(int i=0;i<2;i++){
				line(midx+x+1+i,midy+y+h+1+i,midx+x+w+1+i,midy+y+h+1+i);
				line(midx+x+w+1+i,midy+y+1+i,midx+x+w+1+i,midy+y+h+1+i);
			}
		}

		void drawPressedButton(int x,int y, int h, int w, int col)
		{
			setcolor(col);
			rectangle(midx+x,midy+y,midx+x+w,midy+y+h);
			setfillstyle(SOLID_FILL,col);
			floodfill(midx+x+3,midy+y+3,col);
			setcolor(GREEN);
			for(int i=0;i<2;i++){
				line(midx+x-2+i,midy+y-2+i,midx+x+w-2+i,midy+y-2+i);
				line(midx+x-2+i,midy+y-2+i,midx+x-2+i,midy+y+h+i-2);
			}
		}

		void titles()
		{
			char *c;

			setcolor(getmaxcolor());
			c="RAIN FALL PREDICTION SYSTEM";
			settextstyle(TRIPLEX_FONT, HORIZ_DIR,4);
			settextjustify(CENTER_TEXT, CENTER_TEXT);
			outtextxy(midx,10,c);

			setcolor(getmaxcolor());
			settextstyle(0, HORIZ_DIR,1);
			c="Temperature";
			outtextxy(midx-250,70,c);
			c="Tmax:";
			outtextxy(midx-280,100,c);
			c="Tmin:";
			outtextxy(midx-280,130,c);

			c="Humidity";
			outtextxy(midx-30,70,c);
			c="H1:";
			outtextxy(midx-50,100,c);
			c="H2:";
			outtextxy(midx-50,130,c);

			c="Wind Speed";
			outtextxy(midx+200,70,c);
			c="WS:";
			outtextxy(midx+180,100,c);

			c="Info Box";
			setcolor(14);
			settextstyle(DEFAULT_FONT, HORIZ_DIR,1);
			outtextxy(midx,midy+90,c);

			c="Predict";
			outtextxy(midx+175,midy+60,c);

			c="Analyse";
			outtextxy(midx-170,midy+60,c);

			setcolor(RED);
			c="exit";
			outtextxy(midx+265,midy+210,c);

			settextstyle(2, HORIZ_DIR,4);

		//bulleting in info box with arrows..
			c="->";
			for(int i=0;i<7;i++)
				outtextxy(midx-150,midy+110+(i*10),c);

			settextjustify(LEFT_TEXT,CENTER_TEXT);
			c="click on the text field and input the value.";
			outtextxy(midx-110,midy+110,c);
			c="press Enter to return from text field";
			outtextxy(midx-110,midy+120,c);
			c="Temperature should be in Degree Centigrade.";
			outtextxy(midx-110,midy+130,c);
			c="Wind speed should be in Km/h.";
			outtextxy(midx-110,midy+140,c);
			c="H1 is initial Relative humidity measured.";
			outtextxy(midx-110,midy+150,c);
			c="H2 is Humidity measured 12 hours after H1.";
			outtextxy(midx-110,midy+160,c);
			c="Click on the Done button after all the inputs";
			outtextxy(midx-110,midy+170,c);
			c="are inserted.";
			outtextxy(midx-110,midy+180,c);

		}

		//blinking effect in textfield during writemode
		void blinker(int col, int offsetlength, int x, int y)
		{
			int z=(offsetlength*6);
			setcolor(col);
			outtextxy(x+8+z,y+2,"_");
		}

//x is the starting x co-ordinate value of text field rectangle
//and y is the middle y co-ordinate value of text field rectangle
		void writemode(int x, int y, int param)
		{
			char input[10];
			char *msg;
			int pointer;
			double val;
			int dotflag;
			int blinkflag=0;
			//erasing previously written numbers
			mControl.hidemouseptr();
			setfillstyle(SOLID_FILL,WHITE);
			floodfill(x+10,y,BROWN);
			floodfill(51,176,BROWN);
			mControl.showmouseptr();

			clock_t start, end;
			start=clock();

			for(pointer=0;pointer<10;pointer++) input[pointer]='0';
			pointer=0;
			char key;

			settextjustify(LEFT_TEXT,CENTER_TEXT);
			while(1){
				mControl.hidemouseptr();
				settextstyle(1,HORIZ_DIR,1);
				if(param==1)
					sprintf(msg,"Input Temperature:Tmax");
				if(param==2)
					sprintf(msg,"Input Temperature:Tmin");
				if(param==3)
					sprintf(msg,"Input Humidity:H1");
				if(param==4)
					sprintf(msg,"Input Humidity:H2");
				if(param==5)
					sprintf(msg,"Input WindSpeed");
				setcolor(BLUE);
				outtextxy(55,180,msg);
				settextstyle(DEFAULT_FONT, HORIZ_DIR,1);
				mControl.showmouseptr();
				if(pointer>7){
				//Erasing
					setviewport(51,176,getmaxx()-51,269,1);
					clearviewport();
					setviewport(0,0,getmaxx(),getmaxy(),1);
					floodfill(51,176,BROWN);
				//Writing
					setcolor(RED);
					sprintf(msg,"!!!!Input Buffer OverFlow!!!");
					outtextxy(midx-100,200,msg);
					sprintf(msg,"**more than 7 digits are not allowed");
					outtextxy(midx-100,210,msg);
					key='\b';
				}
				else{
					while(!kbhit()){
						end=clock();
						if((end-start)>5){
							start=clock();
							if(blinkflag==0){
								blinker(BLUE,pointer,x,y);
								blinkflag=1;
							}
							else{
								blinker(WHITE,pointer,x,y);
								blinkflag=0;
							}
						}
					}
					mControl.hidemouseptr();
					key=getch();
				}

				if(isdigit(key)){//if the key is a digit
					input[pointer]=key;
					blinker(WHITE,pointer,x,y);//hide blinker
					blinkflag=0;
					pointer++;
					setcolor(RED);
					sprintf(msg,"%c",input[pointer-1]);
					outtextxy(x+2+(pointer*6),y,msg);
					stat[param-1]='y';
				}
				else{
					if(key=='.'){
						input[pointer]=key;
						blinker(WHITE,pointer,x,y);//hide blinker
						blinkflag=0;
						pointer++;
						setcolor(RED);
						sprintf(msg,"%c",input[pointer-1]);
						outtextxy(x+2+(pointer*6),y,msg);
					}
					else if(key=='\t'){
						setcolor(GREEN);
						rectangle(x-1,y-11,x+81,y+11);
						blinker(WHITE,pointer,x,y);//hide blinker
						tabflag=1;
						break;
					}
					else if(key=='\r'){
						blinker(WHITE,pointer,x,y);//hide blinker
						break;//if enter is pressed i.e. carriage return
					}
					else if(key=='\b' && pointer!=0){//if backspace is pressed
						setcolor(getmaxcolor());
						sprintf(msg,"%c",input[pointer-1]);
						outtextxy(x+2+(pointer*6),y,msg);
						input[pointer]='0';
						blinker(WHITE,pointer,x,y);//hide blinker
						blinkflag=0;
						pointer--;
					}
					else{ //alphabets are pressed
					//Erasing
						setviewport(51,176,getmaxx()-51,269,1);\
						clearviewport();
						setviewport(0,0,getmaxx(),getmaxy(),1);
						floodfill(51,176,BROWN);
					//Writing
						setcolor(RED);
						sprintf(msg,"!!!Illegal Input");
						outtextxy(midx-100,200,msg);
						sprintf(msg,"Please input [0-9] or '.'");
						outtextxy(midx-100,210,msg);
					}
				}
				mControl.showmouseptr();

			}
			if(pointer==0) stat[param-1]='n';

			dotflag=0;
			for(pointer=0;pointer<10;pointer++){
				if(input[pointer]=='.'){ dotflag=1; break; }
			}
//since the character array, input, contains 0 by default
//and the input is placed from index 0,i.e. input[0].
//then input of 1 will become 100000.
//so to avoid this the following line places '.' after 1 i.e. 1.00000
			if(dotflag==0){
				for(pointer=9;pointer>0;pointer--){
					if(input[pointer]!='0'){ input[pointer+1]='.'; break; }//if a digit except 0 is found then place '.' after it
					if(pointer==1 && input[pointer]==0)input[pointer]='.';//if pointer is indexed to 1 and input value at that index is 0 then place '.' at that index such that we have 0.00 even if no input by user
				}
			}

			mControl.hidemouseptr();
			setviewport(51,176,getmaxx()-51,269,1);
			clearviewport();
			setviewport(0,0,getmaxx(),getmaxy(),1);
			floodfill(51,176,BROWN);

			val=atof(input);
			updateValue(val,param);

			mControl.hidemouseptr();
			setcolor(BLACK);
			rectangle(x-1,y-11,x+81,y+11);
			mControl.showmouseptr();
			mControl.showmouseptr();
			mControl.showmouseptr();
		}

		void updateValue(int val, int param)
		{
			switch(param)
			{
				case 1: Tmax=val; break;
				case 2: Tmin=val; break;
				case 3: H1=val; break;
				case 4: H2=val; break;
				case 5: WS=val; break;
				default:break;
			}
		}

		int eventListener()
		{
			int param;
			int xt,yt;
			int b=-1,x,y;
			char c='a';
			do{
				while(tabflag){
					mControl.hidemouseptr();
					tabflag=0;
					param=(param%5+1);
					if(param%2==0) yt=130;
					else yt=100;
					if(param==1 || param==2) xt= (midx-250);
					else if(param==3|| param==4) xt=(midx-20);
					else xt=midx+210;
					setcolor(GREEN);
					rectangle(xt-1,yt-11,xt+81,yt+11);
					mControl.showmouseptr();
					writemode(xt,yt,param);
				}

				mControl.showmouseptr();
				b=-1;
				while(b!=1){
					mControl.getmousepos(&b,&x,&y);
					if(kbhit())c=getch();
					if(c=='q' || c=='Q') return 1;
					if(eventpart!=0) if(c=='y' || c== 'Y') return 2;

				}
				if(abs(y-100)<=10){//this includes text fields for Tmax, H1 and WS
					if(abs(x-(midx-210))<=40){//Tmax
						setcolor(GREEN);
						mControl.hidemouseptr();
						rectangle(midx-251,89,midx-169,111);
						mControl.showmouseptr();
						if(eventpart==0) writemode(midx-250,100,1);// middle value y, starting value x of the text field is passed
						mControl.showmouseptr();
						param=1;
					}
					else if(abs(x-(midx+20))<=40){//H1
						setcolor(GREEN);
						mControl.hidemouseptr();
						rectangle(midx-21,89,midx+61,111);
						mControl.showmouseptr();
						if(eventpart==0) writemode(midx-20,100,3);
						mControl.showmouseptr();
						param=3;
					}
					else if(abs(x-(midx+250))<=40){//WS
						setcolor(GREEN);
						mControl.hidemouseptr();
						rectangle(midx+209,89,midx+291,111);
						mControl.showmouseptr();
						if(eventpart==0) writemode(midx+210,100,5);
						mControl.showmouseptr();
						param=5;
					}
					else continue;
				}

				else if(abs(y-130)<=10){//this includes text fields for Tmin, H2
					if(abs(x-(midx-210))<=40){//Tmin
						setcolor(GREEN);
						mControl.hidemouseptr();
						rectangle(midx-251,119,midx-169,141);
						mControl.showmouseptr();
						if(eventpart==0) writemode(midx-250,130,2);
						mControl.showmouseptr();
						param=2;
					}
					else if(abs(x-(midx+20))<=40){//H2
						setcolor(GREEN);
						mControl.hidemouseptr();
						rectangle(midx-21,119,midx+61,141);
						mControl.showmouseptr();
						if(eventpart==0) writemode(midx-20,130,4);
						mControl.showmouseptr();
						param=4;
					}
					else continue;
				}

				else if(abs(x-(midx+175))<=30 && abs(y-(midy+60))<=10){//this includes done button

					mControl.hidemouseptr();
					drawButton(145,50,20,60,LIGHTGRAY);//erasing
					drawPressedButton(147,52,20,60,BROWN);

					setcolor(14);
					settextstyle(DEFAULT_FONT, HORIZ_DIR,1);
					settextjustify(CENTER_TEXT,CENTER_TEXT);
					if(eventpart==0) outtextxy(midx+177,midy+62,"Predict");
					else outtextxy(midx+177,midy+62,"again");
					mControl.showmouseptr();
					while(b==1){
						mControl.getmousepos(&b,&x,&y);
					}
					mControl.hidemouseptr();

					drawPressedButton(147,52,20,60,	LIGHTGRAY);//erasing
					drawButton(145,50,20,60,6);

					if(eventpart==0) outtextxy(midx+175,midy+60,"Predict");
					else 	outtextxy(midx+175,midy+60,"again");
					int flag=check();
					mControl.showmouseptr();
					if(flag==0){
						setcolor(BROWN);
						outtextxy(midx,midy,"Please provide all the inputs");
					}
					else
						if(abs(x-(midx+175))<=25 && abs(y-(midy+60))<=10) break;

				}

				else if(abs(x-(midx-178))<=30 && abs(y-(midy+60))<=10){//this includes done button
				    if(eventpart==0){
					mControl.hidemouseptr();
					drawButton(-200,50,20,60,LIGHTGRAY);//erasing
					drawPressedButton(-198,52,20,60,BROWN);

					setcolor(14);
					settextstyle(DEFAULT_FONT, HORIZ_DIR,1);
					settextjustify(CENTER_TEXT,CENTER_TEXT);
					outtextxy(midx-168,midy+62,"Analyse");

					mControl.showmouseptr();
					while(b==1){
						mControl.getmousepos(&b,&x,&y);
					}
					mControl.hidemouseptr();

					drawPressedButton(-198,52,20,60,LIGHTGRAY);//erasing
					drawButton(-200,50,20,60,6);

					outtextxy(midx-170,midy+60,"Analyse");
					if(abs(x-(midx-178))<=30 && abs(y-(midy+60))<=10) return 2;
				    }
				}

				else if(abs(x-(midx+265))<=25 && abs(y-(midy+210))<=10){ //this includes exit button
					mControl.hidemouseptr();
					drawButton(240,200,20,50,LIGHTGRAY);//erasing
					drawPressedButton(242,202,20,50,RED);

					setcolor(CYAN);
					settextstyle(DEFAULT_FONT, HORIZ_DIR,1);
					settextjustify(CENTER_TEXT,CENTER_TEXT);
					outtextxy(midx+266,midy+212,"exit");

					mControl.showmouseptr();
					while(b==1){
						mControl.getmousepos(&b,&x,&y);
					}
					if(abs(x-(midx+265))<=25 && abs(y-(midy+210))<=10)  return 1;

					drawPressedButton(242,202,20,50,LIGHTGRAY);
					drawButton(240,200,20,50,GREEN);//erasing
					setcolor(RED);
					outtextxy(midx+265,midy+210,"exit");
				}
				else ;
			}while(1);
			return 0;
		}

		int check()
		{
			for(int i=0;i<5;i++)
				if(stat[i]=='n') return 0;

			return 1;
		}

		void done(tree *dtree)
		{
			UserInterface ui;
			ui.setDTree(dtree);
			ui.takeInput(Tmax,Tmin,H1,H2,WS);
			int response=ui.traverse();
			show(response);
		}

		void analyse(tree *dtree)
		{

			analyseimagepaste();

			mControl.showmouseptr();

			int b,x,y;
			int selection_flag=0;
			int back=0;
			do{
				mControl.getmousepos(&b,&x,&y);
				if(b==1){
					if(abs(x-(midx-100))<=20 && abs(y-(midy+130))<=10){//this checks clicks on Ok button
						mControl.hidemouseptr();
						if(back==1){
							drawButton(-120,120,20,40,LIGHTGRAY);//erasing
							drawPressedButton(-118,122,20,40,BROWN);
							outtextxy(midx-98,midy+132,"exit");
							exit(0);
						}
						drawButton(-120,120,20,40,LIGHTGRAY);//erasing
						drawPressedButton(-118,122,20,40,BROWN);
						outtextxy(midx-98,midy+132,"OK");
						mControl.showmouseptr();
						while(b) mControl.getmousepos(&b,&x,&y);
						delay(100);
						mControl.hidemouseptr();
						drawPressedButton(-118,122,20,40,LIGHTGRAY);//erasing
						drawButton(-120,120,20,40,6);
						if(selection_flag){
							outtextxy(midx-100,midy+130,"exit");
							DTAnalyser dta(dtree,dir);
							mControl.showmouseptr();
							back=1;
						}
						else{
							mControl.hidemouseptr();
							outtextxy(midx-100,midy+130,"OK");
							settextstyle(0, HORIZ_DIR,1);
							setcolor(RED);
							outtextxy(midx-98,midy+108,"!First Make a Selection");
							mControl.showmouseptr();
							delay(1000);
							mControl.hidemouseptr();
							setcolor(LIGHTGRAY);//erasing
							outtextxy(midx-98,midy+108,"!First Make a Selection");
							settextstyle(0, HORIZ_DIR,1);
							mControl.showmouseptr();
						}
						mControl.showmouseptr();
					}
					else{
						if(selector(x,y)!=0){

							if(selection_flag==selector(x,y)){
								mControl.hidemouseptr();
								setfillstyle(SOLID_FILL,getmaxcolor());
								if(selection_flag!=0) floodfill(midx-145,midy-86+(selection_flag*10),MAGENTA);
								floodfill(midx-60,midy+90,GREEN);
								selection_flag=0;
								mControl.showmouseptr();
							}
							else{
								mControl.hidemouseptr();
								setfillstyle(SOLID_FILL,getmaxcolor());

								if(selection_flag) floodfill(midx-145,midy-86+(selection_flag*10),MAGENTA);

								floodfill(midx-60,midy+90,GREEN);
								selection_flag=selector(x,y);
								setfillstyle(SOLID_FILL,GREEN);
								floodfill(midx-145,midy-86+(selection_flag*10),MAGENTA);
								selected(selection_flag);
								mControl.showmouseptr();
							}
						}
					}
					b=-1;
				}
				if(kbhit())
					if(getch()=='q') break;


			}while(1);
		}

		int selector(int x, int y)
		{
			delay(100);
			if(abs(x-(midx-145))<=3){
				if(abs(y-(midy-76))<=3) return 1;
				if(abs(y-(midy-66))<=3) return 2;
				if(abs(y-(midy-56))<=3) return 3;
				if(abs(y-(midy-46))<=3) return 4;
				if(abs(y-(midy-36))<=3) return 5;
				if(abs(y-(midy-26))<=3) return 6;
				if(abs(y-(midy-16))<=3) return 7;
				if(abs(y-(midy-6))<=3) return 8;
				if(abs(y-(midy+4))<=3) return 9;
				if(abs(y-(midy+14))<=3) return 10;
			}
			else	return 0;
		}

		void selected(int select)
		{
			setcolor(BLUE);
			switch(select)
			{
				case 1: outtextxy(midx-100,midy+90,"1993"); dir="93"; break;
				case 2: outtextxy(midx-100,midy+90,"1994"); dir="94"; break;
				case 3: outtextxy(midx-100,midy+90,"1995"); dir="95"; break;
				case 4: outtextxy(midx-100,midy+90,"1999"); dir="99"; break;
				case 5: outtextxy(midx-100,midy+90,"2000"); dir="20"; break;
				case 6: outtextxy(midx-100,midy+90,"2004"); dir="04"; break;
				case 7: outtextxy(midx-100,midy+90,"2005"); dir="05"; break;
				case 8: outtextxy(midx-100,midy+90,"2006"); dir="06"; break;
				case 9: outtextxy(midx-100,midy+90,"2007"); dir="07"; break;
				case 10: outtextxy(midx-100,midy+90,"2008"); dir="08"; break;
				default: break;
			}
		}

		int showfiles()  //midx-190,midy-110,midx-10,midy+100
		{
			setcolor(MAGENTA);
			FILE *fp;
			fp=fopen("tstData/files.txt","r");
			char c;
			char buf[4];
			char *msg;
			int index=0;
			c=getc(fp);
			int i=0;
			mControl.hidemouseptr();
			outtextxy(midx-120,midy-100+(index*10),"Available Data");
			outtextxy(midx-100,midy-90+(index*10),"----------------------");
			while(c!='x'){
				buf[i]=c;
				i++;
				c=getc(fp);
				if(c=='\n'){
					sprintf(msg,"%c%c%c%c",buf[0],buf[1],buf[2],buf[3]);
					outtextxy(midx-170,midy-75+(index*10),msg);
					rectangle(midx-142,midy-79+(index*10),midx-148,midy-73+(index*10));
					index++;
					i=0;
					c=getc(fp);
				}
			}
			mControl.showmouseptr();
			return index;
		}

		void show(int response)
		{
			mControl.hidemouseptr();
			char *msg;
			setfillstyle(SOLID_FILL,getmaxcolor());
			settextjustify(LEFT_TEXT,CENTER_TEXT);
			setviewport(51,176,getmaxx()-51,269,1);
			clearviewport();
			setviewport(0,0,getmaxx(),getmaxy(),1);
			floodfill(51,176,BROWN);

			switch(response){
				case 1:
					setcolor(GREEN);
					sprintf(msg,"***\"IT WILL RAIN\"***");
					outtextxy(midx-100,midy-50,msg);
					break;

				case 2:
					setcolor(GREEN);
					sprintf(msg,"***\"IT WILL NOT RAIN\"***");
					outtextxy(midx-100,midy-50,msg);
					break;

				case 3:
					setcolor(GREEN);
					sprintf(msg,"***\"IT CAN NOT BE SAID WHETHER IT RAINS OR NOT\"***");
					outtextxy(midx-200,midy-50,msg);
					break;

				default: break;
			}
			setcolor(GREEN);
			sprintf(msg,"- Click on again to retry.");
			outtextxy(midx-250,midy-30,msg);
			sprintf(msg,"- Click on exit or press 'q' to terminate.");
			outtextxy(midx-250,midy-20,msg);
			outtextxy(midx-250,midy-10,"- Press 'y' to get visual tree representation.");
			mControl.showmouseptr();
			settextjustify(CENTER_TEXT,CENTER_TEXT);
		}

};



int main()
{
   //	clrscr();
	numOfNode=0;
	eventpart=0;

	tree *dtree=new tree;//pointer dtree will point to decisiontree build by object of decisionTree
	int stat;

	f=fopen("Decision","r");

	MouseController mControl;
	GUI gui;
	while(1){
		builddecisiontree(dtree);
		stat=gui.eventListener();//its work is to take input
		if(stat==1)break;//if user press 'q' or click on exit
		if(stat==2){
			gui.analyse(dtree);
		}
		else{
			eventpart=1;
			gui.done(dtree);
			mControl.hidemouseptr();
			gui.drawButton(145,50,20,60,LIGHTGRAY);//erasing
			gui.drawButton(145,50,20,60,6);
			outtextxy(getmaxx()/2+175,getmaxy()/2+60,"again");
			mControl.showmouseptr();
			stat=gui.eventListener();//its work is to see whether the user wants to try again or not
			if(stat==1)break;//if user press 'q' or click on exit
			if(stat==2){
				mControl.hidemouseptr();
				break;
			}
		}
		eventpart=0;
		gui.redraw();
	}
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	welcomeimagepaste();
	setcolor(getmaxcolor());
	settextstyle(1,DEFAULT_FONT, 4);
	outtextxy(getmaxx()/2,getmaxy()/2,"THANK YOU");
	delay(2000);
	closegraph();
	cout<<"It's a wrap............";
	delay(500);
	return 1;
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

void builddecisiontree(tree *dtree)
{
	int val;
	fread(&val, sizeof(val),1,f);
	dtree->attrNo=val;
	fread(&val, sizeof(val),1,f);
	dtree->numOfChild=val;
	fread(&val, sizeof(val),1,f);
	dtree->decision=val;


	if(dtree->numOfChild!=0)
	{
		dtree->child=new tree[dtree->numOfChild];
		for(int i=0;i<dtree->numOfChild;i++)
		{
			builddecisiontree(&dtree->child[i]);
		}
	}
	else{
		dtree->child=NULL;
	}


}

