// expression.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "expression.h"

//evaluates the expression for the current x
//the expression must first be parsed and error checked by ParseExpression()
double GetExpression(char* expression,int* exp_index,int* exp_max,double x)
{
	double expval=0;
	
	for(;;)
	{
		if(*exp_index>=*exp_max)return expval;
		
		switch(GetCurrent(expression,exp_index,exp_max))
		{
		case ' ':
			*exp_index=*exp_index+1;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
			expval=GetNum(expression,exp_index,exp_max,x);
			break;
		case '(':
		case '[':
		case '{':
			*exp_index=*exp_index+1;
			expval=GetExpression(expression,exp_index,exp_max,x);
			break;
		case ')':
		case ']':
		case '}':
			*exp_index=*exp_index+1;
			if(GetCurrent(expression,exp_index,exp_max)=='\0')return expval;
			if(GetCurrent(expression,exp_index,exp_max)=='^')
			{
				*exp_index=*exp_index+1;
				expval=pow(expval,GetPower(expression,exp_index,exp_max,x));
			}
			return expval;
		case '+':
			*exp_index=*exp_index+1;
			expval+=GetTerm(expression,exp_index,exp_max,x);
			break;
		case '-':
			*exp_index=*exp_index+1;
			expval-=GetTerm(expression,exp_index,exp_max,x);
			break;
		case '*':
			*exp_index=*exp_index+1;
			expval*=GetTimes(expression,exp_index,exp_max,x);
			break;
		case '/':
			*exp_index=*exp_index+1;
			expval/=GetTimes(expression,exp_index,exp_max,x);
			break;
		case '^':
			*exp_index=*exp_index+1;
			expval=pow(expval,GetPower(expression,exp_index,exp_max,x));
			break;
		default:
			return 0;
		}
	}
}

double GetNum(char* expression,int* exp_index,int* exp_max,double x)
{
	double num=0;
	
	for(;;)
	{
		if(*exp_index>=*exp_max)return num;
		
		switch(GetCurrent(expression,exp_index,exp_max))
		{
		case ' ':
			*exp_index=*exp_index+1;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			num*=10;
			num+=(GetCurrent(expression,exp_index,exp_max)-48);//ascii adjust
			*exp_index=*exp_index+1;
			break;
		case '.':
			*exp_index=*exp_index+1;
			num+=GetFract(expression,exp_index,exp_max,x);
			break;
		case 'x':
		case 'X':
			*exp_index=*exp_index+1;
			num*=GetX(expression,exp_index,exp_max,x);
			return num;
		case 'e':
			*exp_index=*exp_index+1;
			num*=2.718281;
			return num;
		case 'P':
			*exp_index=*exp_index+1;
			num*=3.141592654;
			return num;
		case '(':
		case '[':
		case '{':
		case ')':
		case ']':
		case '}':
		case '+':
		case '-':
		case '*':
		case '/':
			return num;
		case '^':
			*exp_index=*exp_index+1;
			num=pow(num,GetPower(expression,exp_index,exp_max,x));
			return num;
			//sin cos tan csc sec cot abs log sqrt abs e pi
		case 'S':
		case 'C':
		case 'T':
		case 'c':
		case 's':
		case 'o':
		case 'l':
		case 'L':
		case 'q':
		case 'a':
			num*=GetLetterFunction(expression,exp_index,exp_max,x);
			return num;
		default:
			return 0;
		}
	}
}

double GetTerm(char* expression,int* exp_index,int* exp_max,double x)
{
	double term=0;
	
	for(;;)
	{
		if(*exp_index>=*exp_max)return term;
		
		switch(GetCurrent(expression,exp_index,exp_max))
		{
		case ' ':
			*exp_index=*exp_index+1;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
			term=GetNum(expression,exp_index,exp_max,x);
			break;
		case '(':
		case '[':
		case '{':
			term=GetExpression(expression,exp_index,exp_max,x);
			break;
		case ')':
		case ']':
		case '}':
		case '+':
		case '-':
			return term;
		case '*':
			*exp_index=*exp_index+1;
			term*=GetTimes(expression,exp_index,exp_max,x);
			break;
		case '/':
			*exp_index=*exp_index+1;
			term/=GetTimes(expression,exp_index,exp_max,x);
			break;
		case '^':
			*exp_index=*exp_index+1;
			term=pow(term,GetPower(expression,exp_index,exp_max,x));
			break;
		default:
			return 0;
		}
	}
}

double GetTimes(char* expression,int* exp_index,int* exp_max,double x)
{
	double times=0;
	
	for(;;)
	{
		if(*exp_index>=*exp_max)return times;
		
		switch(GetCurrent(expression,exp_index,exp_max))
		{
		case ' ':
			*exp_index=*exp_index+1;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
			times=GetNum(expression,exp_index,exp_max,x);
			return times;
		case '(':
		case '[':
		case '{':
			*exp_index=*exp_index+1;
			times=GetExpression(expression,exp_index,exp_max,x);
			return times;
		case ')':
		case ']':
		case '}':
		case '+':
		case '-':
		case '*':
		case '/':
			return times;
		case '^':
			times=pow(times,GetPower(expression,exp_index,exp_max,x));
		default:
			return 0;
		}
	}
}

double GetX(char* expression,int* exp_index,int* exp_max,double x)
{
	for(;;)
	{
		if(*exp_index>=*exp_max)return x;
		
		switch(GetCurrent(expression,exp_index,exp_max))
		{
		case ' ':
			*exp_index=*exp_index+1;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
		case '(':
		case '[':
		case '{':
		case ')':
		case ']':
		case '}':
		case '+':
		case '-':
		case '*':
		case '/':
			return x;
		case '^':
			*exp_index=*exp_index+1;
			x=pow(x,GetPower(expression,exp_index,exp_max,x));
			return x;
		default:
			return 0;
		}
	}
}

double GetPower(char* expression,int* exp_index,int* exp_max,double x)
{
	double power=0;
	
	for(;;)
	{
		if(*exp_index>=*exp_max)return power;
		
		switch(GetCurrent(expression,exp_index,exp_max))
		{
		case ' ':
			*exp_index=*exp_index+1;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
			power=GetNum(expression,exp_index,exp_max,x);
			return power;
		case '(':
		case '[':
		case '{':
			power=GetExpression(expression,exp_index,exp_max,x);
			return power;
		case ')':
		case ']':
		case '}':
		case '+':
		case '-':
		case '*':
		case '/':
		case '^':
			return power;
		default:
			return 0;
		}
	}
}

double GetFract(char* expression,int* exp_index,int* exp_max,double x)
{
	double fract=0;
	double mulval=1;
	
	for(;;)
	{
		mulval*=.1;
		if(*exp_index>=*exp_max)return fract;
		
		switch(GetCurrent(expression,exp_index,exp_max))
		{
		case ' ':
			*exp_index=*exp_index+1;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
			fract+=((GetCurrent(expression,exp_index,exp_max)-48)*mulval);
			*exp_index=*exp_index+1;
			break;
		case 'x':
		case 'X':
		case 'e':
		case 'P':
			return fract;
			//sin cos tan csc sec cot abs log sqrt abs
		case 'S':
		case 'C':
		case 'T':
		case 'c':
		case 's':
		case 'o':
		case 'l':
		case 'L':
		case 'q':
		case 'a':
			fract*=GetLetterFunction(expression,exp_index,exp_max,x);
			return fract;
		case '(':
		case '[':
		case '{':
		case ')':
		case ']':
		case '}':
		case '+':
		case '-':
		case '*':
		case '/':
			return fract;
		case '^':
			return fract;
		default:
			return 0;
		}
	}
}

double GetLetterFunction(char* expression,int* exp_index,int* exp_max,double x)
{
	double letterfunction=0;
	
	for(;;)
	{
		switch(GetCurrent(expression,exp_index,exp_max))
		{
		case 'S'://sin
			*exp_index=*exp_index+2;
			return sin(GetExpression(expression,exp_index,exp_max,x));
		case 'C'://cos
			*exp_index=*exp_index+2;
			return cos(GetExpression(expression,exp_index,exp_max,x));
		case 'T'://tan
			*exp_index=*exp_index+2;
			return tan(GetExpression(expression,exp_index,exp_max,x));
		case 'c'://csc
			*exp_index=*exp_index+2;
			return 1/sin(GetExpression(expression,exp_index,exp_max,x));
		case 's'://sec
			*exp_index=*exp_index+2;
			return 1/cos(GetExpression(expression,exp_index,exp_max,x));
		case 'o'://cot
			*exp_index=*exp_index+2;
			return 1/tan(GetExpression(expression,exp_index,exp_max,x));
		case 'l'://log
			*exp_index=*exp_index+2;
			return log10(GetExpression(expression,exp_index,exp_max,x));
		case 'L'://ln
			*exp_index=*exp_index+2;
			return log(GetExpression(expression,exp_index,exp_max,x));
		case 'q'://sqrt
			*exp_index=*exp_index+2;
			return sqrt(GetExpression(expression,exp_index,exp_max,x));
		case 'a'://abs
			*exp_index=*exp_index+2;
			letterfunction=GetExpression(expression,exp_index,exp_max,x);
			if(letterfunction<0)return -letterfunction;
			return letterfunction;
		default:
			return 0;//error
		}
	}
}

//parses and error checks the expression
//initializes exp_max when it finds '\n' or '\0'
//makes sure every 'x' has a coefficient; if there is not one, it puts a '1' 
//as the coefficient places tokens for the letter functions for faster 
//processing according to:
//	sin  :  S
//	cos  :  C
//	tan  :  T
//	csc  :  c
//	sec  :  s
//	cot  :  o
//	log  :  l
//	ln   :  L
//	sqrt :  q
//	abs  :  a
//  e    :  e
//  p    :  p
//
//return value:
//         0: OK
//         1: no expression 
//         2: letter function error
//         3: illegal character
//         4: parentheses uneven
int ParseExpression(char* expression,int* exp_index,int* exp_max)
{
	char temp_buff[100];
	int temp_index;
	int exp_ind;
	char current;
	char last;
	int open_parenth_count=0;
	int close_parenth_count=0;
	
	//see if an expression was entered
	exp_ind=0;
	if(*(expression+exp_ind)=='\n')return 1;
	if(*(expression+exp_ind)=='\0')return 1;
	
	//remove white space
	exp_ind=0;
	temp_index=0;
	while(*(expression+exp_ind)!='\n' && *(expression+exp_ind)!='\0')
	{
		if(*(expression+exp_ind)==' ')exp_ind++;
		else temp_buff[temp_index++]=*(expression+exp_ind++);
	}
	temp_buff[temp_index]='\0';
	
	//copy back to expression
	temp_index=0;
	exp_ind=0;
	*exp_max=0;
	while(temp_buff[temp_index]!='\0')
	{
		*(expression+exp_ind++)=temp_buff[temp_index++];
		*exp_max=*exp_max+1;
	}
	*(expression+exp_ind)='\0';
	
	//tokens, coefficients, and check for illegal characters
	//tokens:
	//	sin : S
	//	cos : C
	//	tan : T
	//	csc : c
	//	sec : s
	//	cot : o
	//	log : l
	//  ln  : L
	//	sqrt : q
	//	abs : a
	//  e   : e
	//  p   : p
	current='\0';
	temp_index=0;
	exp_ind=0;
	for(;temp_index<*exp_max;temp_index++)
	{
		last=current;
		current=temp_buff[temp_index];
		switch(current)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			*(expression+exp_ind++)=temp_buff[temp_index];
			break;
		case '(':
		case '{':
		case '[':
			open_parenth_count++;
			*(expression+exp_ind++)=temp_buff[temp_index];
			break;
		case ')':
		case '}':
		case ']':
			close_parenth_count++;
			*(expression+exp_ind++)=temp_buff[temp_index];
			break;
		case '+':
		case '-':
		case '*':
		case '/':
		case '^':
		case '.':
			*(expression+exp_ind++)=temp_buff[temp_index];
			break;
		case 'x':
		case 'X':
			switch(last)
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				*(expression+exp_ind++)=temp_buff[temp_index];
				break;
			default://previous was not a number so add coefficient of '1'
				*(expression+exp_ind++)='1';
				*(expression+exp_ind++)=temp_buff[temp_index];
				break;
			}
			break;
			case 's':
			case 'S'://sin sec sqrt
				if( ( temp_buff[temp_index+1]=='i' || temp_buff[temp_index+1]=='I') &&
					( temp_buff[temp_index+2]=='n' || temp_buff[temp_index+2]=='N') )
				{
					switch(last)
					{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						break;
					default://previous was not a number so add coefficient of '1'
						*(expression+exp_ind++)='1';
						break;
					}
					*(expression+exp_ind++)='S';
					temp_index+=2;//it's incremented again in the 'for' statement
					break;
				}
				if( ( temp_buff[temp_index+1]=='e' || temp_buff[temp_index+1]=='E') &&
					( temp_buff[temp_index+2]=='c' || temp_buff[temp_index+2]=='C') )
				{
					switch(last)
					{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						break;
					default://previous was not a number so add coefficient of '1'
						*(expression+exp_ind++)='1';
						break;
					}
					*(expression+exp_ind++)='s';
					temp_index+=2;//it's incremented again in the 'for' statement
					break;
				}
				if( ( temp_buff[temp_index+1]=='q' || temp_buff[temp_index+1]=='Q') &&
					( temp_buff[temp_index+2]=='r' || temp_buff[temp_index+2]=='R') &&
					( temp_buff[temp_index+3]=='t' || temp_buff[temp_index+3]=='T') )
				{
					switch(last)
					{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						break;
					default://previous was not a number so add coefficient of '1'
						*(expression+exp_ind++)='1';
						break;
					}
					*(expression+exp_ind++)='q';
					temp_index+=3;//it's incremented again in the 'for' statement
					break;
				}
				return 2;//error
			case 'c':
			case 'C'://cos csc cot
				if( ( temp_buff[temp_index+1]=='o' || temp_buff[temp_index+1]=='O') &&
					( temp_buff[temp_index+2]=='s' || temp_buff[temp_index+2]=='S') )
				{
					switch(last)
					{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						break;
					default://previous was not a number so add coefficient of '1'
						*(expression+exp_ind++)='1';
						break;
					}
					*(expression+exp_ind++)='C';
					temp_index+=2;//it's incremented again in the 'for' statement
					break;
				}
				if( ( temp_buff[temp_index+1]=='s' || temp_buff[temp_index+1]=='S') &&
					( temp_buff[temp_index+2]=='c' || temp_buff[temp_index+2]=='C') )
				{
					switch(last)
					{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						break;
					default://previous was not a number so add coefficient of '1'
						*(expression+exp_ind++)='1';
						break;
					}
					*(expression+exp_ind++)='c';
					temp_index+=2;//it's incremented again in the 'for' statement
					break;
				}
				if( ( temp_buff[temp_index+1]=='o' || temp_buff[temp_index+1]=='O') &&
					( temp_buff[temp_index+2]=='t' || temp_buff[temp_index+2]=='T') )
				{
					switch(last)
					{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						break;
					default://previous was not a number so add coefficient of '1'
						*(expression+exp_ind++)='1';
						break;
					}
					*(expression+exp_ind++)='o';
					temp_index+=2;//it's incremented again in the 'for' statement
					break;
				}
				return 2;//error
			case 't':
			case 'T'://tan
				if( ( temp_buff[temp_index+1]=='a' || temp_buff[temp_index+1]=='A') &&
					( temp_buff[temp_index+2]=='n' || temp_buff[temp_index+2]=='N') )
				{
					switch(last)
					{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						break;
					default://previous was not a number so add coefficient of '1'
						*(expression+exp_ind++)='1';
						break;
					}
					*(expression+exp_ind++)='T';
					temp_index+=2;//it's incremented again in the 'for' statement
					break;
				}
				return 2;//error
			case 'l':
			case 'L'://log ln
				if( ( temp_buff[temp_index+1]=='o' || temp_buff[temp_index+1]=='O') &&
					( temp_buff[temp_index+2]=='g' || temp_buff[temp_index+2]=='G') )
				{
					switch(last)
					{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						break;
					default://previous was not a number so add coefficient of '1'
						*(expression+exp_ind++)='1';
						break;
					}
					*(expression+exp_ind++)='l';
					temp_index+=2;//it's incremented again in the 'for' statement
					break;
				}
				if( temp_buff[temp_index+1]=='n' || temp_buff[temp_index+1]=='N' ) 
				{
					switch(last)
					{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						break;
					default://previous was not a number so add coefficient of '1'
						*(expression+exp_ind++)='1';
						break;
					}
					*(expression+exp_ind++)='L';
					temp_index+=1;//it's incremented again in the 'for' statement
					break;
				}
				return 2;//error
			case 'a':
			case 'A'://abs
				if( ( temp_buff[temp_index+1]=='b' || temp_buff[temp_index+1]=='B') &&
					( temp_buff[temp_index+2]=='s' || temp_buff[temp_index+2]=='S') )
				{
					switch(last)
					{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						break;
					default://previous was not a number so add coefficient of '1'
						*(expression+exp_ind++)='1';
						break;
					}
					*(expression+exp_ind++)='a';
					temp_index+=2;//it's incremented again in the 'for' statement
					break;
				}
				return 2;//error
			case 'e':
			case 'E'://e
				switch(last)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					break;
				default://previous was not a number so add coefficient of '1'
					*(expression+exp_ind++)='1';
					break;
				}
				*(expression+exp_ind++)='e';
				//this is a single letter and doesn't need to be incremented
				break;
			case 'P':
			case 'p'://pi
				if( temp_buff[temp_index+1]=='i' || temp_buff[temp_index+1]=='I' ) 
				{
					switch(last)
					{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						break;
					default://previous was not a number so add coefficient of '1'
						*(expression+exp_ind++)='1';
						break;
					}
					*(expression+exp_ind++)='P';
					temp_index+=1;//it's incremented again in the 'for' statement
					break;
				}
				return 2;//error
			default:
				return 3;//error
		}
	}
	*(expression+exp_ind)='\0';//add the null
	//get exp_max
	exp_ind=0;
	*exp_max=0;
	while(*(expression+exp_ind++)!='\0')*exp_max=*exp_max+1;
	
	if(open_parenth_count!=close_parenth_count)return 4;//error
	
	return 0;
}

char GetCurrent(char* expression,int* exp_index,int* exp_max)
{
	return expression[*exp_index];
}
