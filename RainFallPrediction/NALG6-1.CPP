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

//variables and methods for mouse operation


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

void welcomeimagecopier()
{
	int midx=getmaxx()/2;
	int midy=getmaxy()/2;
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

int numOfNode;


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
			else if(val<3.0)return 1;
			else if(val<6.0)return 2;
			else if(val<9.0)return 3;
			else if(val<12.0)return 4;
			else if(val<15.0)return 5;
			else if(val<18.0)return 6;
			else if(val<21.0)return 7;
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
			if(val<2)return 0;//no rain
			else return 1;//it rains

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

class id3
{
    protected:
	attr *samples;
	int totalSamples;
	float entropyS;//entropy of whole sample over precip
	float *entropyTmax;//entropy of max temperature
	float *entropyTmin;//entropy of min temperature
	float *entropyHmor;//entropy of humidity measured at morning
	float *entropyHeve;//entropy of humidity measured at evening
	float *entropyW;//entropy of windSpeed
	float gain[5];
    public:
	id3(){
		entropyTmax=new float[numOfAttrVal(0)];
		entropyTmin=new float[numOfAttrVal(1)];
		entropyHmor=new float[numOfAttrVal(2)];
		entropyHeve=new float[numOfAttrVal(3)];
		entropyW=new float[numOfAttrVal(4)];

		dataCopier();

		setEntropyS();
	//	show(samples,totalSamples);
	}

	void dataCopier()
	{
		DataReader r;
		int n=0;

		totalSamples=r.getTotalSamples()/3;
		samples=new attr[totalSamples];
		for(int i=0;i<r.getTotalSamples();i+=2){
			for(int j=0;j<6;j++){
				samples[n].param[j]=r.getSamples(i,j);
			}
			n++;
		}
	}


	float log2(float num)
	{
		return M_LOG2E*log(num);
	}

	float calcEntropy(int numOfSamples, int numOfattr, float *val)
	{
		float a=0.0;
		float temp;
		for(int i=0;i<numOfattr;i++)
		{
			temp=val[i];
			if((int)temp!=0){
				temp=temp/numOfSamples;
				a+= -( temp*log2(temp) );
			}
		}
		return a;
	}

	float getEntropyS()
	{
		return entropyS;
	}

	void setEntropyS()
	{
		int a=numOfAttrVal(5);
		float *b=new float[2];
		b=listOfAttrVal(5);
		entropyS=calcEntropy(totalSamples,a,b);
	     //	cout<<"no.of rain yes "<<b[1]<<" and no.of rain no "<<b[0];
	    //
	}

/*calculates gain for I (parameter entropyI belongs to)i.e. outlook,temp,
	etc.. in the base parameter determined by entropy
i.e. gain(rain,outlook) where entropy is of rain and entropyI is of outlook */
		float calcGain(float entropy, float *entropyI, float *val, int numOfsamples, int numOfAttr)
		{
			float ans;
			ans=entropy;
			if(numOfsamples!=0){
				for(int i=0;i<numOfAttr;i++)
				{
					ans+= -( val[i]/numOfsamples)* entropyI[i];
				}
			}
			return ans;
		}


	float *listOfAttrVal(int i)
	{
		int a=numOfAttrVal(i);
		float *b;
		b= new float[a];
		for(int j=0;j<a;j++) b[j]=0.0;

		for(j=0;j<totalSamples;j++)
		{
			if(samples[j].param[i]==0)b[0]+=1.0;
			else b[1]+=1.0;
		}

		return b;
	}


	int numOfAttrVal(int i)
	{
		switch(i){
			case 0: return 9;
			case 1:	return 9;
			case 2: return 8;
			case 3: return 8;
			case 4: return 7;
			case 5: return 2;
			default: cout<<"This code should never be reached";return 0;
		}
	}


	float *getEntropy(int num)
	{
		switch(num){
			case 0:	return entropyTmax;
			case 1:	return entropyTmin;
			case 2: return entropyHmor;
			case 3: return entropyHeve;
			case 4: return entropyW;
			default: return &entropyS;
		}
	}

	void show(attr *sample, int totalS)
	{
		char *c;
		cout<<endl<<endl<<"The set of samples S (total: "<<totalS<<")"<<endl<<endl;
		cout<<"Tmax\tTmin\tHmor\tHeve\twind\train"<<endl;
		cout<<"---------------------------------------------"<<endl;
		getch();
		for(int i=0;i<totalS;i++)
		{
			cout<<sample[i].param[0]<<"\t";
			cout<<sample[i].param[1]<<"\t";
			cout<<sample[i].param[2]<<"\t";
			cout<<sample[i].param[3]<<"\t";
			cout<<sample[i].param[4]<<"\t";
			if(sample[i].param[5]==0)cout<<"NO";
			else cout<<"YES";
			cout<<endl;
		     //	delay(50);
		}
	}
};


class decisionTree: id3
{
	protected:
		int a;
		tree node;

	public:
		decisionTree()
		{
	     //		show(samples,totalSamples);
	     //		cout<<"entropyS: "<<getEntropyS();
	       //		cout<<endl;
			setEntropyNGainI(samples,totalSamples,entropyS,entropyTmax,0,0);
			setEntropyNGainI(samples,totalSamples,entropyS,entropyTmin,1,0);
			setEntropyNGainI(samples,totalSamples,entropyS,entropyHmor,2,0);
			setEntropyNGainI(samples,totalSamples,entropyS,entropyHeve,3,0);
			setEntropyNGainI(samples,totalSamples,entropyS,entropyW,4,0);
		 //	shown();
		   //	getch();
		}


		int NOS(attr *sample, int numOfSamples, int baseAttr, int baseAttrVal)
		{
			int n=0;
		//calculating no.of samples for given baseAttr and baseAttrVal
			for(int i=0;i<numOfSamples;i++)
			{
				if(sample[i].param[baseAttr]==baseAttrVal)++n;

			}
			return n;
		}


		attr *sampleClassifier(attr *sample,int numOfSamples, int baseAttr, int baseAttrVal, attr *subSample)
		{
			int n=0;
			for(int i=0;i<numOfSamples;i++)
			{
				if(sample[i].param[baseAttr]==baseAttrVal){
					for(int j=0;j<6;j++){
						subSample[n].param[j]=sample[i].param[j];
					}
					n++;
				}
			}
			return subSample;
		}


		void getTree(tree *n)
		{
			n=&node;
		}
	//reinitializes the gian value to 0 as per the index provided by m
		void reinitgain(int *m)
		{
			for(int i=0;i<5;i++){
				if(m[i]==0)
					gain[i]=0;
			}
		}


		void starter(){

			int marker[5];
			for(int i=0;i<5;i++) marker[i]=0;
			float max=0;
			int temp;
		//selecting attribute with highest gain
			for(i=0;i<5;i++){
				float g=(gain[i]*1000);
				if(g>max){ max=g; temp=i; }
			}
			node.attrNo=temp;
			node.numOfChild=numOfAttrVal(temp);
			node.decision=3;
			node.child=new tree[node.numOfChild];
			attr *subSample;
			int numOfSubSample;
	//		call(temp);

			for(i=0;i<node.numOfChild;i++)
			{
       //	    			cout<<"i'm at starter "<<i<<endl;

				numOfNode++;
				marker[temp]=i+1;//temp=>attrNo of node
				numOfSubSample=NOS(samples,totalSamples,temp,i);
				if(numOfSubSample!=0){
					subSample=new attr[numOfSubSample];
					sampleClassifier(samples,totalSamples,temp,i,subSample);
				}
				else subSample=new attr;

      //	      			show(subSample,numOfSubSample);


				createTree(&node.child[i],marker,temp,i,subSample,numOfSubSample);

				delete subSample;
			}
      //			cout<<"root count: "<<i<<endl;

		}

	      //this function builds up the decision tree
		void createTree(tree *child, int *flag, int attrNum, int attrVal, attr *sample, int numOfSample)
		{
			float *entropySs=getEntropy(attrNum);

			int marker[5];
			int depth=0;
			for(int i=0;i<5;i++){
				marker[i]=flag[i];
				if(flag[i]!=0)depth++;
			}

       //	      		cout<<endl<<" depth: "<<depth<<endl;
			//if num Of sample is zero,
//i.e. there is no sample for current parameter(attribute value) sequence
			if(numOfSample==0){
      //				cout<<"num=0"<<endl;

				child->decision=0;
				child->numOfChild=0;
				child->attrNo=attrVal;
				child->child=NULL;
			}
		     //or if entropy is zero then create leaf node
//entropy can be zero if there is no sample for given sequence of attribute values
	//in such case, consider the leaf node and no decision..
			//child[].attrNo=10 flags this situation which is stated above
//entropy can also be zero if the sample belongs to only one class
//in such case leaf node gives result which is the value of attribute over which classification is done.

			else if(entropySs[attrVal]==0){
	//			cout<<"entropy=0"<<endl;

				child->attrNo=(attrVal);//to distinguish from attrNo and attrVal...this helps us know attrVal
				child->numOfChild=0;
				child->child=NULL;
				child->decision=sample[0].param[5];
			}
//if entropy is not zero||numOfSample is not zero
			else{
				for(i=0;i<5;i++){
					if(flag[i]==0){//if flag[i]!=0 then ith attribute(parameter) is parent(direct or indirect) of current node in tree hierarchy
						setEntropyNGainI(sample,numOfSample,entropySs[attrVal],getEntropy(i),i,depth);
					}
				}
				float max=0.0;
				int temp;
				//selecting attribute with highest gain
				for(i=0;i<5;i++){
					if(flag[i]==0){
						float g=(gain[i]*1000);
						if(g>=max){ max=g; temp=i; }
					}
				}
				if(depth<5){
	  //				cout<<"depth<5"<<endl;
					child->attrNo=temp;
	  //				call(temp);
					child->numOfChild= numOfAttrVal(temp);
					child->decision=3;
					child->child=new tree[child->numOfChild];
					reinitgain(marker);
					attr *subSample;
					int numOfSubSample;//contains num of samples in the subset of sample returned by sampleClassifier...

					for(i=0;i<child->numOfChild;i++)
					{
					//used as flag to know which attribute is selected for the node and which have already been selected in that hierarchy of tree...
						marker[temp]=i+1;
	    //					cout<<endl<<endl<<"I\'m at createTree"<<endl;
						numOfSubSample=NOS(sample,numOfSample,child->attrNo,i);
						if(numOfSubSample!=0){
							subSample=new attr[numOfSubSample];
							sampleClassifier(sample,numOfSample,temp,i,subSample);
						}
						else subSample=new attr;
	      //					show(subSample,numOfSubSample);

						numOfNode++;
					//if the samples are not yet classified and can be further classified
						if(numOfSubSample!=numOfSample){
							createTree(&child->child[i],marker,child->attrNo,i,subSample,numOfSubSample);
						}
					//if the samples are not yet classified but can't be further classifed
						else{
		//					cout<<"numOfSubSample=numOfsample"<<endl;

							child->attrNo=attrVal;
							child->numOfChild=0;
							delete child->child;//delete the child's made before entering the for loop
							child->child=NULL;//assign the child to NULL
							int i,j;
							for(int k=j=i=0;k<numOfSample;k++)
							{
								if(sample[k].param[5]==0)i++;//attribute value 5 i.e. param[5] is precipitation in sample
								else j++;
							}
							if(i==j)child->decision=0;//if the no.of yes and no are equal then choose no..
							else if(i>j)child->decision=0;
							else child->decision=1;

						}
						delete subSample;
					}
				}
      //if all the attributes have been considered then the next node must be a leaf node
      //this block reaches only if the numOfSample>0
				else{
	     //				cout<<"numOfsample>0"<<endl;

					child->attrNo=attrVal;
					child->numOfChild=0;
					child->child=NULL;
					int i,j;
//even when all the attributes are considered, the samples still may not be classified
	//in such case consider the value which occured the most...i.e. rain= yes|no
					for(int k=j=i=0;k<numOfSample;k++)
					{
						if(sample[k].param[5]==0)i++;//attribute value 5 i.e. param[5] is precipitation in sample
						else j++;
					}
					if(i==j)child->decision=1;//if the no.of yes and no are equal then choose no..
					else if(i>j)child->decision=0;//no
					else child->decision=1;//yes

				}
			}
		}




//calculates entropy for each attribute value of attribute pointed by paramNum
// on basis of attribute rain
//then calculates the gain for that attribute

		void setEntropyNGainI(attr *sample, int totalS, float entropySs, float *entropy, int paramNum, int depth)
		{
			float *true;
			float *false;
			if(depth==0){
				false=listOfAttrVal(sample,totalS,paramNum,0);
				true=listOfAttrVal(sample,totalS,paramNum,1);
			}
			else{
				false=listOfAttrVal(sample,totalS,paramNum,0);
				true=listOfAttrVal(sample,totalS,paramNum,1);
			}
			float *temp;
			temp=new float[2];
			int i=numOfAttrVal(paramNum);
			int *numOfSamples=new int[i];

			for(int j=0;j<i;j++){
				numOfSamples[j]= (int)(true[j]+false[j]);
				temp[0]=false[j];
				temp[1]=true[j];
      //				cout<<endl<<j<<"\t"<<numOfSamples[j]<<"\t"<<temp[0]<<"\t"<<temp[1]<<"\t";
				entropy[j]=calcEntropy(numOfSamples[j],2,temp);
			}
			temp=new float[i];
			//calculating *val (i.e. temp) to pass to calcGain();
			for(j=0;j<i;j++){
				temp[j]=true[j]+false[j];
			}
			gain[paramNum]=calcGain(entropySs,entropy,temp,totalS,i);
		}

//this is same as listOfAttrVal() in class id3 but it is more general while prior was specific with baseAttr=4,i.e. rain
		float *listOfAttrVal(attr *sample, int totalS, int attrNum, int rval)
		{
			int num=numOfAttrVal(attrNum);
			float *retVal;
			retVal=new float[num];
			for(int i=0;i<num;i++) retVal[i]=0.0;

			for(i=0;i<num;i++)
			{
				for(int j=0;j<totalS;j++)
				{
					if(sample[j].param[attrNum]==i){
						if(sample[j].param[5]==rval)
							retVal[i]+=1.0;

					}
				}
			}
			return retVal;
		}

		tree *throwNode()//returns the pointer to the root of decision tree build i.e. address of node
		{
			return &node;
		}

		void evaluate()
		{
			cout<<"\tThe decision tree created is as follows:\n\n";
			checkTree(&node,0);
		}

		void checkTree(tree *n, int depth)
		{
			if(n!=NULL){
				cout<<"This is node of depth: "<<depth<<endl;
				cout<<"Attribute No.: "<<n->attrNo;
				cout<<"\tNumOfChild: "<<n->numOfChild;
				cout<<"\tDecision: "<<n->decision;
				cout<<endl<<endl;
				for(int i=0;i<n->numOfChild;i++)
				{
					checkTree(&n->child[i],depth+1);
				}
			}
		}

		float calc(int m, int n, float *val)
		{
			return calcEntropy(m,n,val);
		}

		float calcG(float entropySs, float *entropy, float *temp, int numOfSamples, int attrNum)
		{
			return calcGain(entropySs,entropy,temp,numOfSamples,attrNum);
		}

		void shown()
		{
			cout<<endl<<"\t_____Entropy_____"<<endl;
			cout<<endl<<"Max Temp: "<<endl;
			for(int i=0;i<8;i++)
			{
				cout<<entropyTmax[i]<<"\t";
			}
			cout<<endl;
			cout<<endl<<"Min Temp: "<<endl;
			for(i=0;i<8;i++)
			{
				cout<<entropyTmin[i]<<"\t";
			}
			cout<<endl;
			cout<<endl<<"Humidity 8:45: "<<endl;
			for(i=0;i<7;i++)
			{
				cout<<entropyHmor[i]<<"\t";
			}
			cout<<endl;
			cout<<endl<<"Humidity 17:45: "<<endl;
			for(i=0;i<7;i++)
			{
				cout<<entropyHmor[i]<<"\t";
			}
			cout<<endl;
			cout<<endl<<"Wind: "<<endl;
			for(i=0;i<7;i++)
			{
				cout<<entropyW[i]<<"\t";
			}
			cout<<endl;
			cout<<endl<<"\t_____Gain_____"<<endl;
			for(i=0;i<5;i++)
			{
				cout<<"Gain of attribute number "<<i<<"\t"<<gain[i]<<endl;
			}
		}

		void call(int i)
		{
			if(i==0)cout<<"Tmax"<<endl;
			if(i==1)cout<<"Tmin"<<endl;
			if(i==2)cout<<"Hmor"<<endl;
			if(i==3)cout<<"Heve"<<endl;
			if(i==4)cout<<"Wind Speed"<<endl;
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
		MouseController mControl;
		int tabflag;
		int eventpart;

	public:
		GUI()
		{
			tabflag=0;
			eventpart=0;

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

			welcomeimagepaste();
		     //	initialDraw();
		     //	titles();
			mControl.showmouseptr();


		}

		void redraw()
		{
			Tmax=Tmin=H1=H2=WS=0.00;//dummy value
			for(int i=0;i<5;i++)
				stat[i]='n';
			mControl.hidemouseptr();

			welcomeimagepaste();
		    //	cleardevice();
		    //	initialDraw();
		    //	titles();
			mControl.showmouseptr();
		}

		void initialDraw()
		{
			setcolor(getmaxcolor());
			for(int i=0;i<5;i++) //box for main title
				rectangle(i,0,getmaxx()-i,29);

			setfillstyle(SOLID_FILL,DARKGRAY);
			floodfill(5,5,getmaxcolor());

			setcolor(1);
			for(i=0;i<5;i++)//box for rest of the window
				rectangle(i,30+i,getmaxx()-i,getmaxy()-i);

			drawResponseBox();
			drawInfoBox();
			drawInputBox();
			drawTextField();
			drawButton(150,50,20,50,6);//top-left co-ordinate for done button
			drawButton(240,200,20,50,2);//top-left co-ordinate for exit button

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
		   //	floodfill(midx-304,51,getmaxcolor());
		   //	floodfill(midx-76,51,getmaxcolor());
		    //	floodfill(midx+151,51,getmaxcolor());
		}

		void drawTextField()
		{
			//text field
			setcolor(getmaxcolor());
			setfillstyle(SOLID_FILL,getmaxcolor());
			rectangle(midx-250,90,midx-170,110); //Tmax
			floodfill(midx-249,91,getmaxcolor());
			rectangle(midx-250,120,midx-170,140);//Tmin
			floodfill(midx-249,121,getmaxcolor());
			rectangle(midx-20,90,midx+60,110);//H1
			floodfill(midx-19,91,getmaxcolor());
			rectangle(midx-20,120,midx+60,140);//H2
			floodfill(midx-19,121,getmaxcolor());
			rectangle(midx+210,90,midx+290,110);//WindSpeed
			floodfill(midx+212,91,getmaxcolor());
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
				line(midx+x-2+i,midy+y-2+i,midx+x-2+i,midy+y+h+i);
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

			setcolor(9);
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

			c="DONE";
			outtextxy(midx+175,midy+60,c);

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
			floodfill(x+10,y,LIGHTGREEN);
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
					setcolor(LIGHTGREEN);
					rectangle(xt-1,yt-11,xt+81,yt+11);
					mControl.showmouseptr();
					writemode(xt,yt,param);
				}

				mControl.showmouseptr();
				b=-1;
				while(b!=1){
					mControl.getmousepos(&b,&x,&y);
					if(kbhit())c=getch();
					if(c=='q') return 1;
				}
				if(abs(y-100)<=10){//this includes text fields for Tmax, H1 and WS
					if(abs(x-(midx-210))<=40){//Tmax
						setcolor(LIGHTGREEN);
						mControl.hidemouseptr();
						rectangle(midx-251,89,midx-169,111);
						mControl.showmouseptr();
						if(eventpart==0) writemode(midx-250,100,1);// middle value y, starting value x of the text field is passed
						mControl.showmouseptr();
						param=1;
					}
					else if(abs(x-(midx+20))<=40){//H1
						setcolor(LIGHTGREEN);
						mControl.hidemouseptr();
						rectangle(midx-21,89,midx+61,111);
						mControl.showmouseptr();
						if(eventpart==0) writemode(midx-20,100,3);
						mControl.showmouseptr();
						param=3;
					}
					else if(abs(x-(midx+250))<=40){//WS
						setcolor(LIGHTGREEN);
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
						setcolor(LIGHTGREEN);
						mControl.hidemouseptr();
						rectangle(midx-251,119,midx-169,141);
						mControl.showmouseptr();
						if(eventpart==0) writemode(midx-250,130,2);
						mControl.showmouseptr();
						param=2;
					}
					else if(abs(x-(midx+20))<=40){//H2
						setcolor(LIGHTGREEN);
						mControl.hidemouseptr();
						rectangle(midx-21,119,midx+61,141);
						mControl.showmouseptr();
						if(eventpart==0) writemode(midx-20,130,4);
						mControl.showmouseptr();
						param=4;
					}
					else continue;
				}

				else if(abs(x-(midx+175))<=25 && abs(y-(midy+60))<=10){//this includes done button

					mControl.hidemouseptr();
					drawButton(150,50,20,50,LIGHTGRAY);//erasing
					drawPressedButton(152,52,20,50,BROWN);

					setcolor(14);
					settextstyle(DEFAULT_FONT, HORIZ_DIR,1);
					settextjustify(CENTER_TEXT,CENTER_TEXT);
					if(eventpart==0) outtextxy(midx+177,midy+62,"Done");
					else outtextxy(midx+177,midy+62,"again");

					mControl.showmouseptr();
					while(b==1){
						mControl.getmousepos(&b,&x,&y);
					}
					mControl.hidemouseptr();

					drawPressedButton(152,52,20,50,	LIGHTGRAY);//erasing
					drawButton(150,50,20,50,6);

					if(eventpart==0) outtextxy(midx+175,midy+60,"Done");
					else outtextxy(midx+175,midy+60,"Done");
					int flag=check();
					mControl.showmouseptr();
					if(flag==0){
						setcolor(BROWN);
						outtextxy(midx,midy,"Please provide all the inputs");
					}
					else{
						if(abs(x-(midx+175))<=25 && abs(y-(midy+60))<=10){
							if(eventpart==0)eventpart=1;
							else eventpart=0;
							break;
						}
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

		void show(int response)
		{
			mControl.hidemouseptr();
			char *msg;
			setviewport(51,176,getmaxx()-51,269,1);
			clearviewport();
			setviewport(0,0,getmaxx(),getmaxy(),1);
			floodfill(51,176,BROWN);

			switch(response){
				case 1:
					setcolor(GREEN);
					sprintf(msg,"IT WILL RAIN");
					outtextxy(midx,midy-50,msg);
					break;

				case 2:
					setcolor(GREEN);
					sprintf(msg,"IT WILL NOT RAIN");
					outtextxy(midx,midy-50,msg);
					break;

				case 3:
					setcolor(GREEN);
					sprintf(msg,"IT CAN NOT BE SAID WHETHER IT RAINS OR NOT");
					outtextxy(midx,midy-50,msg);
					break;

				default: break;
			}
			settextjustify(LEFT_TEXT,RIGHT_TEXT);
			setcolor(GREEN);
			sprintf(msg,"Click on again to retry");
			outtextxy(midx-200,midy-30,msg);
			sprintf(msg,"Click on exit or press 'q' to terminate");
			outtextxy(midx-200,midy-20,msg);
			sprintf(msg,"press 'y' to view tree representation");
			outtextxy(midx-200,midy-10,msg);
			mControl.showmouseptr();
			settextjustify(CENTER_TEXT,CENTER_TEXT);
		}

};



int main()
{
   //	clrscr();
	numOfNode=0;

	tree *dtree;//pointer dtree will point to decisiontree build by object of decisionTree
	int stat;

	decisionTree *dt;
	dt=new decisionTree;
	dt->starter();
	dtree=dt->throwNode();
	delete dt;


	MouseController mControl;
	GUI gui;
	while(1){
		stat=gui.eventListener();//its work is to take input
		if(stat)break;//if user press 'q' or click on exit
		gui.done(dtree);
		mControl.hidemouseptr();
		gui.drawButton(150,50,20,50,LIGHTGRAY);//erasing
		gui.drawButton(150,50,20,50,6);
		outtextxy(getmaxx()/2+175,getmaxy()/2+60,"again");
		mControl.showmouseptr();
		stat=gui.eventListener();//its work is to see whether the user wants to try again or not
		if(stat)break;//if user press 'q' or click on exit
		gui.redraw();
	}
	closegraph();
	cout<<"It's a wrap............";
	delay(1000);
	return 1;
}