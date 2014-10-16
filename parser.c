#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAXSTRINGS 1000
char * nulsym 		= 	"1"; 
char *	identsym 	= 	"2"; 
char *	numbersym 	= 	"3"; 
char *	plussym 	=	"4";
char *	minussym 	=	"5"; 
char *	multsym 	=	"6";  
char *	slashsym 	=	"7"; 
char *	oddsym 		=	"8";  
char *	eqlsym 		=	"9"; 
char *	neqsym 		= 	"10";
char *	lessym 		= 	"11";
char *	leqsym 		= 	"12"; 
char *	gtrsym 		= 	"13";
char *	geqsym 		= 	"14";
char *	lparentsym 	= 	"15";
char *	rparentsym 	= 	"16";
char *	commasym 	= 	"17";
char *	semicolomsym= 	"18"; 
char *	periodsym 	= 	"19";
char *	becomessym 	= 	"20";
char *	beginsym 	= 	"21";
char *	endsym 		= 	"22";
char *	ifsym 		= 	"23";
char *	thensym 	= 	"24";
char *	whilesym 	= 	"25"; 
char *	dosym 		= 	"26"; 
char *	callsym 	= 	"27"; 
char *	constsym 	= 	"28";
char *	varsym 		= 	"29";
char *	procsym 	= 	"30";
char *	writesym 	= 	"31"; 
char *	readsym 	= 	"32"; 
char *	elsesym 	= 	"33";

int	factor(char a[MAXSTRINGS][12], int counter);
int program(char a[MAXSTRINGS][12], int counter);
int tokens_to_array();
int	block(char a[MAXSTRINGS][12], int counter);
int	statement(char a[MAXSTRINGS][12], int counter);
int	condition(char a[MAXSTRINGS][12], int counter);
int	expression(char a[MAXSTRINGS][12], int counter);
int	term(char a[MAXSTRINGS][12], int counter);
int	factor(char a[MAXSTRINGS][12], int counter);
int number(char a[MAXSTRINGS][12], int cnt);

int number(char a[MAXSTRINGS][12], int cnt){


	if(strcmp(a[cnt],"0")==0)
		return 1;
	if (atoi(a[cnt])!=0)
		 return 1;
	return 0;

}


int 	tokens_to_array(){
	char a[MAXSTRINGS][12];
	int i=0;


	FILE * list = fopen("lexemelist.txt","r");

	while(!feof(list)){

		char input[12];
		if(fscanf(list, "%s", input)==1){

			printf("%s\n",input );
			strcpy(a[i],input);
			i++;
	 }
	 	}
printf("done\n\n\n\n");
int k =i;

for (int i = 0; i < k; ++i)
	{
		printf("%s \n", a[i] );
	}

	return program(a,0);
	 

}

int 	program(char a[MAXSTRINGS][12], int counter){

		counter = block(a,counter);

		if(counter == -1)
			return-1;

		if (!strcmp(a[counter],periodsym)==0){
			printf("missing period for end of program\n");
			return -1;
		}
		else 
			printf(" No errors, program is syntactically correct \n");
		return 0;
	
}



int	block(char a[MAXSTRINGS][12], int counter){
	int cnt = counter;

		if(strcmp(a[cnt],constsym)==0){

			do{

				if (strcmp(a[++cnt],identsym)!=0)
				{	
					printf("error identsym \n");
					return-1;

				}
				if (strcmp(a[++cnt],eqlsym)!=0)
				{
					printf("error eqsym\n");
					return-1;
				}
				if (number(a,++cnt)){
					printf("error number\n");
					return-1;
				
				}	
			}
			while(strcmp(a[++cnt],commasym)==0);

			if (strcmp(a[++cnt],semicolomsym)!=0)
			{
					printf("error semicolom\n");
					return-1;
			}
		}

		if (strcmp(a[++cnt], varsym)==0)
		{
			do
			{
				if (strcmp(a[++cnt],identsym)!=0)
				{
					printf("error identsym \n");
					return-1;
				}
			} 
			while (strcmp(a[++cnt],commasym)==0);

			if (strcmp(a[++cnt],semicolomsym)!=0)
			{
				printf("error semicolom \n");
				return-1;

			}
		}
		while (strcmp(a[++cnt],procsym)==0){

			if (strcmp(a[++cnt],identsym)!=0)
				{
					printf("error identsym \n");
					return-1;
				}

			if (strcmp(a[++cnt],semicolomsym)!=0)
			{
					printf("error semicolom\n");
					return-1;
			}


			cnt = block(a,++cnt);

			if (strcmp(a[++cnt],semicolomsym)!=0)
			{
					printf("error semicolom\n");
					return-1;
			}

		}

		if(statement(a,cnt+1) == -1)
			return-1;
		return 0;


		

	
}

int	statement(char a[MAXSTRINGS][12], int counter){

	int cnt = counter;

	if (strcmp(a[cnt],identsym)==0)
	{
		if (strcmp(a[++cnt],becomessym)!=0){
			
			printf("error becomessym\n");
			return-1;
		}
		cnt = expression(a,cnt+1);
		if(cnt == -1 )
			return cnt;
	}
	else if (strcmp(a[cnt],callsym)==0)
	{
		if (strcmp(a[++cnt],identsym)!=0)
		{
			printf("error identsym \n");
			return-1;
		}
	}
	else if (strcmp(a[++cnt],beginsym)==0)
	{
		cnt = statement(a,cnt +1);
		if (cnt == -1 )
		{
			return -1;
		}

		while(strcmp(a[cnt],semicolomsym)==0)
		{
			cnt =statement(a,cnt + 1);
			if(cnt == -1)
				return -1; 
		}
		if (strcmp(a[cnt],endsym)!=0)
		{
			printf("error endsym\n");
		}
	}
	else if (strcmp(a[++cnt],ifsym)==0)
	{
		cnt = condition(a,cnt +1);

		if (cnt == -1)
			return -1;

		if (strcmp(a[cnt],thensym)!=0)
		{
			cnt = statement(a,cnt +1);
		
			if (cnt == -1)
				return -1;
		}
	}
	else if (strcmp(a[cnt],whilesym)==0)
	{
		cnt = condition(a,cnt +1);
		
		if (cnt == -1)
			return -1;	

		if (strcmp(a[cnt],dosym)!=0)
		{
			printf("error dosym \n");
			return-1;
		}
		cnt = statement(a,cnt + 1);
		
			if (cnt == -1)
				return -1;
	}

	return	++cnt;

}

int	condition(char a[MAXSTRINGS][12], int counter){
	int cnt = counter;

	if (strcmp(a[cnt],oddsym)==0)
	{
		cnt = expression(a, cnt + 1);

			if (cnt == -1)
				return -1;

	}
	else
	{
		cnt = expression(a, cnt + 1);

		if (cnt == -1)
			return -1;

		if(strcmp(a[cnt],relation)!=0)
		{
			printf("error relation \n");
			return -1;
		}

		cnt = expression(a, cnt + 1);

		if (cnt == -1)
			return -1;

	}

	return	++cnt;
}

int	expression(char a[MAXSTRINGS][12], int counter){

	int cnt = counter;

	if (strcmp(a[cnt],plussym)==0 ||strcmp(a[cnt],minussym)==0)
	{
		++cnt;
	}
	cnt = term(a,cnt);

		if (cnt == -1)
			return -1;



	while(strcmp(a[cnt],plussym)==0 ||strcmp(a[cnt],minussym)==0){

		cnt = term(a,cnt + 1);

		if (cnt == -1)
			return -1;
	}

	return	++cnt;
}

int	term(char a[MAXSTRINGS][12], int counter){

	int cnt = counter;

	cnt = factor(a,cnt);

	if(cnt == 1 )
		return -1;


	while(strcmp(a[cnt],multsym)==0 ||strcmp(a[cnt],slashsym)==0)
	{
		cnt = factor(a,cnt +1);

		if(cnt == -1)
			return-1;

	}



	return	cnt; // no need for ++cnt because functions that are being called are returning ++cnt 
}

int	factor(char a[MAXSTRINGS][12], int counter){

	int cnt = counter -1;
	char * op = "(";  // op for open parenthesis
	char * cp = ")";  // cp for closed parenthesis 

	if (strcmp(a[cnt],identsym)==0)
	{
		++cnt;
	}
	else if (number(a,cnt)) // number might be a function dont know yet.
	{
		++cnt;
	}
	else if (strcmp(a[cnt],op)==0)
	{
		cnt = expression(a,cnt +1);

		if(cnt == -1)
			return -1; 
		if(strcmp(a[cnt],cp)!=0)
		{
			printf("error missing "")"" \n");  // missigf closing parenthesis 
			return -1;
		}


	}
	else
	{
		printf("error missing "")"" \n");   /// arithmetic syntax is incorrect.
			return -1;
	}
		

	return	++cnt;
}


int main(int argc, char const *argv[])
{
	if(tokens_to_array() == 0)

		return 0;

	else

		return-1; 
}


