// Darin Doria and Jorge Berroa
// COP 3402 - Fall 2014
// HW3 - PL/0 Parser

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "setup.c"
#include "helper.c"

#define MAX_SYMBOL_TABLE_SIZE 10000
#define MAX_CODE_LENGTH 500


// Global tables to keep track of data
symbol 	symbol_table[MAX_SYMBOL_TABLE_SIZE];
instruction mcode[MAX_CODE_LENGTH];

// Global variables
int sx=0,next_symbol = 0, next_code = 0, token = 0, rp = 0, token_counter = 0, level= -1,m_index=0;

// get next token from input file and store it in the global variable
void nextToken(FILE * fin) {

	char s[3], c;
	int temp;

	s[0] = getc(fin);
	c = getc(fin);

	if (c != ' ')
	{
		s[1] = c;
		c = getc(fin);
	}
	else
	{
		s[1] = s[0];
		s[0] = '0';
	}
	
	s[2] = '\0';

	token = strtol(s, NULL, 10);
	token_counter++;
}

// insert to symbol table
void insertToSymbolTable(int type, char * name, int value, int level, int addr)
{
	//printf("level at symbol_table = %d\n",level );
	symbol_table[sx].kind  = type;
	strcpy(symbol_table[sx].name, name);
	symbol_table[sx].val   = value;
	symbol_table[sx].level = level;
	symbol_table[sx].addr  = addr;
	next_symbol++;
	sx++;
}

// get symbol from table
int getSymbolFromTable(char * name)
{ 
	int i = 0;
	for(i = sx-1; i >= 0; i--)
	{
		//printf("%s symbol at i = %d \n", symbol_table[i].name,i);
		if (strcmp(name, symbol_table[i].name) == 0) return i;
	}

	return -1;
}
void change_emit(int OP, int L, int M, int index){

	printf("changing %d %d %d to %d %d %d at index %d\n ",mcode[index].OP ,mcode[index].L , mcode[index].M , OP, L, M,index );

	mcode[index].OP = OP;
	mcode[index].L  = L;
	mcode[index].M  = M;

}

void emit(int OP, int L, int M)
{
	mcode[next_code].OP = OP;
	mcode[next_code].L  = L;
	mcode[next_code].M  = M;
	//printf("next code is %d emiting %d %d %d\n",next_code,OP,L,M );
	next_code++;
}

int relation()
{
	switch(token)
	{
		case(eqsym):
			return 8;

		case(neqsym):
			return 9;

		case(lessym):
			return 10;

		case(leqsym):
			return 11;

		case(gtrsym):
			return 12;

		case(geqsym):
			return 13;

		default:
			return 0;
	}

	return 0;
}

int term(FILE * input)
{
	int mulop = 0;

	if (factor(input) == -1) return -1;

	while (token == multsym || token == slashsym)
	{
		mulop = token;
		nextToken(input);
		
		if (factor(input) == -1) return -1;

		if (mulop == multsym)
		{
			// MUL
			emit(2, 0, 4); 
			rp = 1;
		}
		else
		{
			// DIV
			emit(2, 0, 5); 
			rp = 1;
		}
	}
	return 0;
}

int expression(FILE * input)
{
	int temp = 0;

	if (token == plussym || token == minussym)
	{
		temp = token;
		nextToken(input);

		if (term(input) == -1) return -1;

		if (temp == minussym) 
			// NEG
			emit(2, 0, 1);
	}
	else 
	{
		if (term(input) == -1) return -1;
	}
	while (token == plussym || token == minussym)
	{
		temp = token;
		nextToken(input);
		
		if (term(input) == -1) return -1;

		if (temp == plussym)
		{
			// ADD
			emit(2, 0, 2);
			rp = 1;
		}
		else
		{
			// SUB
			emit(2, 0, 3);
			rp = 1;
		}
	}
	return 0;
}


int factor(FILE * input)
{
	int temp = 0, index = 0, addr = 0;
	char * name;
	if (token == identsym)
	{
		name = getNextIdentifier(input);
		index = getSymbolFromTable(name);

		//printf("%s name were looking for is \n", name );

		if (index == -1)
		{
			throwError(11);
			return -1;
		}
		if (symbol_table[index].kind == 2){

			//printf("index for emit is %d\n",index);
			emit(3, level-symbol_table[index].level, symbol_table[index].addr);
		}
		else
			emit(1, 0, symbol_table[index].val);

		rp++;
		nextToken(input);
	}
	else if (token == numbersym)
	{
		temp = getNumber(input);

		emit(1, 0, temp);
		rp++;

		nextToken(input);
	}
	else if (token == lparentsym)
	{
		nextToken(input);
		
		if (expression(input) == -1) return -1;

		if (token != rparentsym)
		{
			throwError(22);
			return -1;
		}
		nextToken(input);
	}
	else
	{
		throwError(23);
		return -1;
	}

	return 0;
}

int condition(FILE * input)
{
	int relop;
	if (token == oddsym)
	{
		nextToken(input);
		
		if (expression(input) == -1) return -1;
		
		// ODD(R0)
		emit(2, 0, 6);
	}
	else
	{
		if (expression(input) == -1) return -1;

		if (!(relop = relation()))
		{
			throwError(20);
			return -1;
		}


		nextToken(input);
		
		if (expression(input) == -1) return -1;

		emit(2, 0, relop);
	}

	return 0;
}

int statement(FILE * input)
{
	int temp = 0, addr = 0, temp2 = 0, temp3 = 0, index = 0;
	char * name;
	// printf("Statement starting with token %d\n", token);
	if (token == identsym)
	{
		name = getNextIdentifier(input);
		index = getSymbolFromTable(name);

		//printf("%s name were looking for is \n", name );


		if (index == -1)
		{
			throwError(11);
			return -1;
		}
		else if (symbol_table[index].kind == 1)
		{
			throwError(12);
			return -1;
		}

		nextToken(input);

		if (token != becomessym)
		{
			throwError(13);
			return -1;
		}

		nextToken(input);

		if (expression(input) == -1) return -1;
		
		rp = 0;
		emit(4, level-symbol_table[index].level, symbol_table[index].addr);
	}
	else if (token == callsym)
	{
		nextToken(input);
		if (token != identsym)
			throwError(14);

		// get ident name
		name = getNextIdentifier(input);
		index = getSymbolFromTable(name);

		//printf("%s name were looking for is \n", name );

		if (index == -1)
		{
			throwError(11);
			return -1;
		}
	   if (symbol_table[index].kind == 3)
	   		emit(5, level- symbol_table[index].level, symbol_table[index].addr);
		else 
		{
			throwError(30);
			return -1;
		}


		// TODO: write the emit code for call

		nextToken(input);
	}
	else if (token == beginsym)
	{


				
		nextToken(input);

		if (statement(input) == -1) return -1;

		while (token == semicolonsym)
		{
			nextToken(input);

			if (statement(input) == -1) return -1;
		}

		if (token != endsym)
		{
			printf("Bad token is %d\n", token);
			printf("At token number %d\n", token_counter);
			throwError(27);
			return -1;
		}
		
		nextToken(input);
	}
	else if (token == ifsym)
	{
		nextToken(input);
		
		if (condition(input) == -1) return -1;

		if (token != thensym)
		{
			throwError(16);
			return -1;
		}

		nextToken(input);
		temp = next_code;
		emit(8, 0, 0);
		rp = 0;

		if (statement(input) == -1) return -1;

		mcode[temp].M = next_code;
	} 
	else if (token == whilesym)
	{
		temp2 = next_code;
		nextToken(input);
		
		if (condition(input) == -1) return -1;

		temp3 = next_code;
		emit(8, 0, 0);
		rp = 0;

		if(token != dosym)
		{
			throwError(18);
			return -1;
		}

		nextToken(input);
		
		if (statement(input) == -1) return -1;

		emit(7, 0, temp2);
		mcode[temp3].M = next_code;
	}
	else if (token == readsym)
	{
		nextToken(input);

		if(token != identsym)
		{
			throwError(28);
			return -1;
		}

		name = getNextIdentifier(input);
		index = getSymbolFromTable(name);

		//printf("%s name were looking for is \n", name );


		if (index == -1)
		{
			throwError(11);
			return -1;
		}


		// Read R0
		emit(10, 0, 2);

		// Store variable in stack
		emit(4, level-symbol_table[index].level, symbol_table[index].addr);
	
		nextToken(input);	
	}
	else if (token == writesym)
	{
		nextToken(input);

		if(token != identsym)
		{
			throwError(29);
			return -1;
		}

		name = getNextIdentifier(input);
		index = getSymbolFromTable(name);

		//printf("%s name were looking for is \n", name );

		if(index == -1)
		{
			throwError(11);
			return -1;
		}


		// Load variable user wants to write
		//printf("index for emit is %d\n",index);
		emit(3, level-symbol_table[index].level, symbol_table[index].addr);
		
		// Write to screen
		emit(9,0, 1);

		nextToken(input);
	}

	// printf("Statement ended at token %d\n", token);
	return 0;
}

int block(FILE * input, int l) 
{
	// ================ TODO ================
	// level ++ At beginning of block
	// level -- at the end of block
	// ======================================

	
	level++;
	int prev_sx=sx;
	//printf("level =%d\n",level );
	int val = 0, addr = 4,  jmp_address;
	
	char * name;
	jmp_address = next_code;
	emit(7,0,next_code+1);
	if (token == constsym)
	{
		do
		{
			nextToken(input);
			if (token != identsym) 
			{
				throwError(4);
				return -1;
			}

			name = getNextIdentifier(input);

			nextToken(input);
			if (token != eqsym)
			{
				throwError(3);
				return -1;
			}
			nextToken(input);
			if (token != numbersym)
			{
				throwError(2);
				return -1;
			}

			val = getNumber(input);

			insertToSymbolTable(1, name, val, level, 0);
			free(name);
			nextToken(input);
		} while(token == commasym);

		if (token != semicolonsym)
		{
			throwError(5);
			return -1;
		}

		nextToken(input);
	}
	if (token == varsym)
	{
		do
		{
			nextToken(input);
			if (token != identsym)
			{
				throwError(4);
				return -1;
			}
			
			name = getNextIdentifier(input);
			//printf("inserting %s\n",name );
			insertToSymbolTable(2, name, 0, level, addr);
			//printf("Storing var name %s\n", name);
			addr += 1;
			free(name);
			nextToken(input);
			//printf("NEXT input IS  %d\n", token);
		
		} while(token == commasym);

		if (token != semicolonsym)
		{
			throwError(5);
			return -1;
		}

				// Increment stack ptr
				nextToken(input);
	}
	// ================ TODO ================
	// Write the code to handle parsing a
	// procedure call
	// ======================================
	while (token == procsym)
	{
				// check for identsym

		int mcode_index = next_code;

		nextToken(input);
		if (token != identsym)
		{
			throwError(4);
			return -1;
		}

		// get procedure name and store it in
		// the symbol table
		name = getNextIdentifier(input);
		//printf("Storing addr name %d\n", addr);
		insertToSymbolTable(3, name, 0, level, next_code);
		free(name);

		// check next token for semicolon
		// this indicates end of procedure name
		nextToken(input);
		if (token != semicolonsym) 
			throwError(5);

		// get next token to start checking for block
		nextToken(input);
		if (block(input, level) != 0) return -1;

		// check next token to make sure block ends 
		// with a semicolon
		// nextToken(input);
		// printf("Next token is %d, comparing to %d\n", token, semicolonsym);
		if (token != semicolonsym)
		{
			throwError(5);
		}

		nextToken(input);
		m_index= mcode_index;


	}
	mcode[jmp_address].M=next_code;
	emit(6,0,addr);
	// printf("Going into statement with token %d\n", token);
	if (statement(input) == -1) return -1;
	emit(2,0,0);

	sx=prev_sx;
	level--;
	//printf("level =%d\n",level );
	return 0;
}

int program(FILE * input) 
{
	nextToken(input);
	
	if (block(input, level) != 0) return -1;
	
	if (token != periodsym)
	{
		throwError(9);
		return -1;
	}

	

	printf("No errors, program is syntactically correct.\n\n");
	return 0;
}

// print mcode to file
void printmCode()
{
	int i = 0;
	FILE * output = fopen("mcode.txt", "w");
	for (i = 0; i < next_code; i++)
	{
		fprintf(output, "%d %d %d\n\n", mcode[i].OP, mcode[i].L, mcode[i].M);
	}
	fclose(output);
}


int main() {

	FILE * input = fopen("lexemelist.txt", "r");
	
	if (!input) throwError(FILE_NOT_FOUND);
	
	if (program(input) == -1) return -1;

	printmCode();
	fclose(input);
	return 0;
}