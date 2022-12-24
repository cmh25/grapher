//expression.h

//function definitions
int ParseExpression(char* expression,int* exp_index,int* exp_max);
double GetExpression(char* expression,int* exp_index,int* exp_max,double x);
double GetNum(char* expression,int* exp_index,int* exp_max,double x);
double GetTerm(char* expression,int* exp_index,int* exp_max,double x);
double GetTimes(char* expression,int* exp_index,int* exp_max,double x);
double GetFract(char* expression,int* exp_index,int* exp_max,double x);
double GetX(char* expression,int* exp_index,int* exp_max,double x);
double GetPower(char* expression,int* exp_index,int* exp_max,double x);
double GetLetterFunction(char* expression,int* exp_index,int* exp_max,double x);
char GetCurrent(char* expression,int* exp_index,int* exp_max);
