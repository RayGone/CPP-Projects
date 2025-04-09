#include <iostream>
#include <conio.h>
#include <ctype.h>
#include <string.h>

using namespace std;

class translator
{
	private:
		char *msg;
		int length;
		int index;

	public:
		void prompt()
		{
			cout<<"Input the digits to translate: ";
			cin>>msg;
			length=strlen(msg);
		}

		char next()
		{
			if(index<length) ++index;

			return msg[index];
		}

		char prev()
		{
			if(index!=0)	--index;

			return msg[index];
		}

		char current()
		{
			return msg[index];
		}

		void translation()
		{
			prompt();
			length=strlen(msg);
			index=0;
			if(inputcheck()) engine();
			else cout<<"SORRY";
			cout<<"."<<endl;
		}

		int inputcheck()
		{
			if((index+1)<length){
				if(isdigit(current())){
					next();
					return inputcheck();
				}
				else return 0;
			}
			else{
				index=0;
				return 1;
			}
		}



		void engine()
		{
			switch(length-index){
				case 1: ones(); break;
				case 2: tens(); break;
				case 3: hundreds(); break;
				case 4: thousands(); break;
				case 5: tenthou(); break;
				case 6: hunthou(); break;
				case 7: millions(); break;
				case 8: tenmill(); break;
				case 9: hunmill(); break;
				case 10: billions(); break;
				case 11: tenbill(); break;
				case 12: hunbill(); break;
				case 13: trillions(); break;
				case 14: tentrill(); break;
				case 15: huntrill(); break;
				default: cout<<"SORRY"; goto end;
			}

			if((index+1)<length){
				 next();
				 engine();
			}
			end:
				1;
		}


		void ones()
		{
			switch(current()){
				case '0': cout<<"_ZERO"; break;
				case '1': cout<<"_ONE";  break;
				case '2': cout<<"_TWO";  break;
				case '3': cout<<"_THREE";break;
				case '4': cout<<"_FOUR"; break;
				case '5': cout<<"_FIVE"; break;
				case '6': cout<<"_SIX";  break;
				case '7': cout<<"_SEVEN";break;
				case '8': cout<<"_EIGHT";break;
				case '9': cout<<"_NINE"; break;
				default: break;
			}
		}

		void teens()
		{
			switch(current()){
				case '1': cout<<"_ELEVEN";    break;
				case '2': cout<<"_TWELVE";    break;
				case '3': cout<<"_THIRTEEN";  break;
				case '4': cout<<"_FOURTEEN";  break;
				case '5': cout<<"_FIFTEEN";   break;
				case '6': cout<<"_SIXTEEN";   break;
				case '7': cout<<"_SEVENTEEN"; break;
				case '8': cout<<"_EIGHTEEN";  break;
				case '9': cout<<"_NINETEEN";  break;
				default: break;
			}
		}

		void tens()
		{
			switch(current()){
				case '1': if(next()!='0') teens(); else cout<<"_TEN"; break;
				case '2': cout<<"_TWENTY"; next(); ones();  break;
				case '3': cout<<"_THIRTY"; next(); ones(); break;
				case '4': cout<<"_FOURTY"; next(); ones(); break;
				case '5': cout<<"_FIFTY";  next(); ones(); break;
				case '6': cout<<"_SIXTY";  next(); ones(); break;
				case '7': cout<<"_SEVENTY";next(); ones(); break;
				case '8': cout<<"_EIGHTY"; next(); ones(); break;
				case '9': cout<<"_NINETY"; next(); ones(); break;
				default: break;
			}
		}

		void hundreds()
		{
			switch(current()){
				case '0': break;
				default: ones(); cout<<"_HUNDRED";
			}
		}

		void thousands()
		{
			switch(current()){
				case '0': break;
				default: ones(); cout<<"_THOUSAND";
			}
		}

		void tenthou()
		{
			switch(current()){
				case '0': break;
				default: tens(); cout<<"_THOUSAND";
			}
		}

		void hunthou()
		{
			switch(current()){
				case '0': break;
				default: hundreds(); next(); tenthou();
			}
		}

		void millions()
		{
			switch(current()){
				case '0': break;
				default: ones(); cout<<"_MILLION";
			}
		}

		void tenmill()
		{
			switch(current()){
				case '0': break;
				default: tens(); cout<<"_MILLION";
			}
		}

		void hunmill()
		{
			switch(current()){
				case '0': break;
				default: hundreds(); next(); tenmill();
			}
		}

		void billions()
		{
			switch(current()){
				case '0': break;
				default: ones(); cout<<"_BILLION";
			}
		}

		void tenbill()
		{
			switch(current()){
				case '0': break;
				default: tens(); cout<<"_BILLION";
			}
		}

		void hunbill()
		{
			switch(current()){
				case '0': break;
				default: hundreds(); next(); tenbill();
			}
		}

		void trillions()
		{
			switch(current()){
				case '0': break;
				default: ones(); cout<<"_TRILLION";
			}
		}

		void tentrill()
		{
			switch(current()){
				case '0': break;
				default: tens(); cout<<"_TRILLION";
			}
		}

		void huntrill()
		{
			switch(current()){
				case '0': break;
				default: hundreds(); next(); tentrill();
			}
		}

};



int main()
{
	translator tt;
	tt.translation();
	getch();
	return 1;
}