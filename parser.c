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
int next_symbol = 0, next_code = 0, token = 0, rp = 0, token_counter = 0;

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
	symbol_table[next_symbol].kind  = type;
	strcpy(symbol_table[next_symbol].name, name);
	symbol_table[next_symbol].val   = value;
	symbol_table[next_symbol].level = level;
	symbol_table[next_symbol].addr  = addr;
	next_symbol++;
}

// get symbol from table
int getSymbolFromTable(char * name)
{
	int i = 0;
	for(i = 0; i < next_symbol; i++)
	{
		if (strcmp(name, symbol_table[i].name) == 0) return i;
	}

	return -1;
}

void emit(int OP, int L, int M)
{
	mcode[next_code].OP = OP;
	mcode[next_code].L  = L;
	mcode[next_code].M  = M;
	next_code++;
}

int relation()
{
	switch(token)
	{
		case(eqsym):
			return 18;

		case(neqsym):
			return 19;

		case(lessym):
			return 20;

		case(leqsym):
			return 21;

		case(gtrsym):
			return 22;

		case(geqsym):
			return 23;

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
		if (index == -1)
		{
			throwError(11);
			return -1;
		}
		if (symbol_table[index].kind == 2)
			emit(3, symbol_table[index].level, symbol_table[index].addr);
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

		emit(relop, 0, 1);
	}

	return 0;
}

int statement(FILE * input)
{
	int temp = 0, addr = 0, temp2 = 0, temp3 = 0, index = 0;
	char * name;

	if (token == identsym)
	{
		name = getNextIdentifier(input);
		index = getSymbolFromTable(name);

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
		emit(4, symbol_table[index].level, symbol_table[index].addr);
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

		if (index == -1)
		{
			throwError(11);
			return -1;
		}


		// Read R0
		emit(10, 0, 2);

		// Store variable in stack
		emit(4, symbol_table[index].level, symbol_table[index].addr);
	
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

		if(index == -1)
		{
			throwError(11);
			return -1;
		}


		// Load variable user wants to write
		emit(3, symbol_table[index].level, symbol_table[index].addr);
		
		// Write to screen
		emit(9,0, 1);

		nextToken(input);
	}

	return 0;
}

int block(FILE * input, int level) 
{
	int val = 0, addr = 0;
	char * name;
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

			insertToSymbolTable(1, name, val, 0, 0);
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
	if (token == intsym)
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
			// printf("ADDR: %d\n", addr);
			insertToSymbolTable(2, name, 0, level, addr);
			addr += 1;
			free(name);
			nextToken(input);
		
		} while(token == commasym);

		if (token != semicolonsym)
		{
			throwError(5);
			return -1;
		}
		// Increment stack ptr
		emit(6, 0, addr);
		nextToken(input);

	}
	
	if (statement(input) == -1) return -1;

	return 0;
}

int program(FILE * input) 
{
	nextToken(input);
	
	if (block(input, 0) != 0) return -1;
	
	if (token != periodsym)
	{
		throwError(9);
		return -1;
	}

	emit(11, 0, 3);

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
		fprintf(output, "%d %d %d\n", mcode[i].OP, mcode[i].L, mcode[i].M);
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