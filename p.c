// Dylan Davidson and Ivan Lugo
// COP 3402 - Spring 2014
// HW3 - PL/0 Parser

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser_functions.h"

#define MAX_SYMBOL_TABLE_SIZE 10000
#define MAX_CODE_LENGTH 500

typedef enum
{
    nulsym = 1, identsym, numbersym, plussym, minussym,
    multsym,  slashsym, oddsym, eqsym, neqsym,
    lessym, leqsym, gtrsym, geqsym, lparentsym,
    rparentsym, commasym, semicolonsym, periodsym, becomessym,
    beginsym, endsym, ifsym, thensym, whilesym,
    dosym, callsym, constsym, intsym, procsym,
    writesym, readsym , elsesym
} token_type;

// nulsym 		= 	1;
// identsym 	= 	2;
// numbersym 	= 	3;
// plussym 	=	4;
// minussym 	=	5;
// multsym 	=	6; 
// slashsym 	=	7;
// oddsym 		=	8; 
// eqlsym 		=	9;
// neqsym 		= 	10;
// lessym 		= 	11;
// leqsym 		= 	12;
// gtrsym 		= 	13;
// geqsym 		= 	14;
// lparentsym 	= 	15;
// rparentsym 	= 	16;
// commasym 	= 	17;
// semicolomsym= 	18;
// periodsym 	= 	19;
// becomessym 	= 	20;
// beginsym 	= 	21;
// endsym 		= 	22;
// ifsym 		= 	23;
// thensym 	= 	24;
// whilesym 	= 	25;
// dosym 		= 	26;
// callsym 	= 	27;
// constsym 	= 	28;
// varsym 		= 	29;
// procsym 	= 	30;
// writesym 	= 	31;
// readsym 	= 	32;
// elsesym 	= 	33;

typedef struct
{
	int kind; 		// const = 1, var = 2, proc = 3
	char name[10];	// name up to 11 chars
	int val;		// number (ASCII val)
	int level;		// L level
	int addr;		// M address
}symbol;

// Struct to represent instructions in code
typedef struct
{
	int OP;
	int R;
	int L;
	int M;
}instruction;

// Structure to hold symbol table during parsing
symbol 		symbol_table[MAX_SYMBOL_TABLE_SIZE];
instruction code_table[MAX_CODE_LENGTH];

// Global Declarations
int next_open = 0, next_code = 0, token = 0, rp = 0;

// Error Handling Function, displays the error that occured during parsing.
void errorDisplay(int errorType) {
	switch(errorType){
		case(1):
			printf("\nERROR 1: Use = instead of :=\n");
			break;
		case(2):
			printf("\nERROR 2: = must be followed by a number.\n");
			break;
		case(3):
			printf("\nERROR 3: Constant Identifier must be followed by =\n");
			break;
		case(4):
			printf("\nERROR 4: const, int, procedure must be followed by identifier.\n");
			break;
		case(5):
			printf("\nERROR 5: Semicolon or comma missing.\n");
			break;
		case(6):
			printf("\nERROR 6: Incorrect symbol after procedure declaration.\n");
			break;
		case(7):
			printf("\nERROR 7: Statement expected.\n");
			break;
		case(8):
			printf("\nERROR 8: Incorrect symbol after statement part in block.\n");
			break;
		case(9):
			printf("\nERROR 9: Period expected.\n");
			break;	
		case(10):
			printf("\nERROR 10: Semicolon between statements missing.\n");
			break;
		case(11):
			printf("\nERROR 11: Undeclared Identifier\n");
			break;
		case(12):
			printf("\nERROR 12: Assignment to constant or procedure not allowed.\n");
			break;
		case(13):
			printf("\nERROR 13: Assignment operator expected.\n");
			break;
		case(14):
			printf("\nERROR 14: call must be followed by an identifier.\n");
			break;
		case(15):
			printf("\nERROR 15: Call of a constant or variable meaningless\n");
			break;
		case(16):
			printf("\nERROR 16: then expected.\n");
			break;
		case(17):
			printf("\nERROR 17: Semicolon or } expected.\n");
			break;
		case(18):
			printf("\nERROR 18: do expected.\n");
			break;
		case(19):
			printf("\nERROR 19: Incorrect symbol following statement.\n");
			break;
		case(20):
			printf("\nERROR 20: Relational operator expected.\n");
			break;
		case(21):
			printf("\nERROR 21: Expression must not contain a procedure identifier.\n");
			break;
		case(22):
			printf("\nERROR 22: Right parenthesis missing.\n");
			break;
		case(23):
			printf("\nERROR 23: The preceding factor cannot begin with this symbol.\n");
			break;
		case(24):
			printf("\nERROR 24: An expression cannot begin with this symbol.\n");
			break;
		case(25):
			printf("\nERROR 25: This number is too large.\n");
			break;
		case(26):
			printf("\nERROR 26: Identifier is too long.\n");
			break;
		case(27):
			printf("\nERROR 27: begin must be followed by end.\n");
			break;
		case(28):
			printf("\nERROR 28: read must be followed by valid identifier.\n");
			break;
		case(29):
			printf("\nERROR 29: write must be followed by valid identifier.\n");
			break;
		default:
			printf("\nERROR: Invalid Error\n");
			break;
	}
}

// Function to read lexemelist.txt and get tokens
void getToken(FILE * fin) {

	char hold[3], c;
	int temp;

	// Read in as a short string
	hold[0] = getc(fin);
	c 		= getc(fin);

	// If token was two chars, consume space char
	if(c != ' ')
	{
		hold[1] = c;
		c = getc(fin);
	} // Else token was one char, move token from hold[0] to hold[1] for easier conversion to int
	else{
		hold[1] = hold[0];
		hold[0] = '0';
	} // Null terminate token string so strtol works properly
	hold[2] = '\0';
	// Convert string to int
	token = strtol(hold, NULL, 10);
}

// Reads in identifier name from input
char * getIdentifier(FILE * fin)
{
	char * name = malloc(sizeof(char) * 11);
	char c;
	int i = 0;

	c = getc(fin);
	// Read in name until a space is reached
	while(c != ' ')
	{
		// if idefntifier is too long, exit
		if(i > 10)
		{
			free(name);
			errorDisplay(26);
			return NULL;
		}
		name[i++] = c;
		c = getc(fin);
	}
	// Null terminate string
	name[i] = '\0';

	return name;
}

// Read in number string from input and convert to int
int getNumber(FILE * fin)
{
	char * value = malloc(sizeof(char) * 7);
	char c;
	int i = 0, val = 0;

	c = getc(fin);
	// Read until next space
	while(c != ' ')
	{
		// If number is too many digits, exit
		if(i > 6)
		{
			free(value);
			errorDisplay(25);
			return -1;
		}
		value[i++] = c;
		c = getc(fin);
	}
	value[i] = '\0';
	// Convert str to int
	val = strtol(value, NULL, 10);

	return val;
}

// Adds symbols into symbol table according to inputs
void insertSymbolTable(int type, char * name, int value, int level, int addr)
{
	symbol_table[next_open].kind  = type;
	strcpy(symbol_table[next_open].name, name);
	symbol_table[next_open].val   = value;
	symbol_table[next_open].level = level;
	symbol_table[next_open].addr  = addr;
	next_open++;
}

// Get symbol index from symbol_table, returns -1 if not in it
int getSymbol(char * name)
{
	int i = 0;
	for(i = 0; i < next_open; i++)
	{
		if(strcmp(name, symbol_table[i].name) == 0)
			return i;
	}

	return -1;

}

// generate instruction based on inputs
void emit(int OP, int R, int L, int M)
{
	code_table[next_code].OP = OP;
	code_table[next_code].R  = R;
	code_table[next_code].L  = L;
	code_table[next_code].M  = M;
	next_code++;
}

// Determine if token is a relational operator, return op code for that op
int relation()
{
	switch(token)
	{
		case(eqsym):
			return 18;
			break;
		case(neqsym):
			return 19;
			break;
		case(lessym):
			return 20;
			break;
		case(leqsym):
			return 21;
			break;
		case(gtrsym):
			return 22;
			break;
		case(geqsym):
			return 23;
			break;
		default:
			return 0;
			break;
	}

	return 0;
}

int term(FILE * fin)
{
	int test = 0, mulop = 0;
	test = factor(fin);
	if(test == -1)
		return -1;

	while(token == multsym || token == slashsym)
	{
		mulop = token;
		getToken(fin);
		
		test = factor(fin);
		if(test == -1)
			return -1;

		if(mulop == multsym)
		{
			emit(14, 0, 0, 1); // MUL
			rp = 1;
		}
		else
		{
			emit(15, 0, 0, 1); // DIV
			rp = 1;
		}
	}
	return 0;
}

int expression(FILE * fin)
{
	int test = 0, addop = 0;
	if(token == plussym || token == minussym)
	{
		addop = token;
		getToken(fin);

		test = term(fin);
		if(test == -1)
			return -1;

		if(addop == minussym)
			emit(11, 0, 0,0);
	}
	else 
	{
		test = term(fin);
		if(test == -1)
			return -1;
	}
	while(token == plussym || token == minussym)
	{
		addop = token;
		getToken(fin);
		
		test = term(fin);
		if(test == -1)
			return -1;

		if(addop == plussym)
		{
			// ADD
			emit(12, 0, 0, 1);
			rp = 1;
		}
		else
		{
			// SUB
			emit(13, 0, 0, 1);
			rp = 1;
		}
	}
	return 0;
}


int factor(FILE * fin)
{
	int test = 0, temp = 0, index = 0;
	char * name;
	if(token == identsym)
	{
		name = getIdentifier(fin);
		index = getSymbol(name);
		if(index == -1)
		{
			errorDisplay(11);
			return -1;
		}
		if(symbol_table[index].kind == 2)
			emit(3, rp, symbol_table[index].level, symbol_table[index].addr);
		else
			emit(1, rp, 0, symbol_table[index].val);

		rp++;
		getToken(fin);
	}
	else if(token == numbersym)
	{
		temp = getNumber(fin);

		emit(1, rp, 0, temp);
		rp++;

		getToken(fin);
	}
	else if(token == lparentsym)
	{
		getToken(fin);
		
		test = expression(fin);
		if(test == -1)
			return -1;

		if(token != rparentsym)
		{
			errorDisplay(22);
			return -1;
		}
		getToken(fin);
	}
	else
	{
		errorDisplay(23);
		return -1;
	}

	return 0;
}


int condition(FILE * fin)
{
	int test = 0, relop;
	if(token == oddsym)
	{
		getToken(fin);
		
		test = expression(fin);
		if(test == -1)
			return -1;
		// ODD(R0)
		emit(16, 0, 0, 0);
	}
	else
	{
		test = expression(fin);
		if(test == -1)
			return -1;

		if(!(relop = relation()))
		{
			errorDisplay(20);
			return -1;
		}


		getToken(fin);
		
		test = expression(fin);
		if(test == -1)
			return -1;

		emit(relop, 0, 0, 1);
	}

	return 0;
}

int statement(FILE * fin)
{
	int test = 0, ctemp = 0;
	int cx1 = 0, cx2 = 0;
	int index = 0;
	char * name;
	if(token == identsym)
	{
		name = getIdentifier(fin);
		index = getSymbol(name);
		if(index == -1)
		{
			errorDisplay(11);
			return -1;
		}
		else if(symbol_table[index].kind == 1)
		{
			errorDisplay(12);
			return -1;
		}

		getToken(fin);

		if(token != becomessym)
		{
			errorDisplay(13);
			return -1;
		}

		getToken(fin);
		
		test = expression(fin);
		if(test == -1)
			return -1;
		rp = 0;
		emit(4, 0, symbol_table[index].level, symbol_table[index].addr);
	}
	else if(token == beginsym)
	{
		getToken(fin);
		
		test = statement(fin);
		if(test == -1)
			return -1;

		while(token == semicolonsym)
		{
			getToken(fin);
			test = statement(fin);
			
			if(test == -1)
				return -1;
		}

		if(token != endsym)
		{
			errorDisplay(27);
			return -1;
		}

		getToken(fin);
	}
	else if(token == ifsym)
	{
		getToken(fin);
		
		test = condition(fin);
		if(test == -1)
			return -1;

		if(token != thensym)
		{
			errorDisplay(16);
			return -1;
		}
		getToken(fin);
		
		ctemp = next_code;
		emit(8, 0, 0, 0);
		rp = 0;

		test = statement(fin);
		if(test == -1)
			return -1;
		code_table[ctemp].M = next_code;
	}
	else if(token == whilesym)
	{
		cx1 = next_code;
		getToken(fin);
		
		test = condition(fin);
		if(test == -1)
			return -1;

		cx2 = next_code;
		emit(8, 0, 0, 0);
		rp = 0;
		if(token != dosym)
		{
			errorDisplay(18);
			return -1;
		}

		getToken(fin);
		
		test = statement(fin);
		if(test == -1)
			return -1;

		emit(7, 0, 0, cx1);
		code_table[cx2].M = next_code;
	}
	else if(token == readsym)
	{
		getToken(fin);

		if(token != identsym)
		{
			errorDisplay(28);
			return -1;
		}

		name = getIdentifier(fin);
		index = getSymbol(name);

		if(index == -1)
		{
			errorDisplay(11);
			return -1;
		}


		// Read into R0
		emit(10, rp, 0, 2);
		// Store into variable in stack
		emit(4, rp, symbol_table[index].level, symbol_table[index].addr);
	
		getToken(fin);	
	}
	else if(token == writesym)
	{
		getToken(fin);

		if(token != identsym)
		{
			errorDisplay(29);
			return -1;
		}

		name = getIdentifier(fin);
		index = getSymbol(name);

		if(index == -1)
		{
			errorDisplay(11);
			return -1;
		}


		// Load variable user wants to write
		emit(3, rp, symbol_table[index].level, symbol_table[index].addr);
		// Write to screen
		emit(9, rp, 0, 1);

		getToken(fin);
	}

	return 0;
}

int block(FILE * fin, int level) 
{
	int test = 0, val = 0, addr = 4;
	char * name;
	if(token == constsym)
	{
		do
		{
			getToken(fin);
			if(token != identsym) 
			{
				errorDisplay(4);
				return -1;
			}

			name = getIdentifier(fin);

			getToken(fin);
			if(token != eqsym)
			{
				errorDisplay(3);
				return -1;
			}
			getToken(fin);
			if(token != numbersym)
			{
				errorDisplay(2);
				return -1;
			}

			val = getNumber(fin);

			insertSymbolTable(1, name, val, 0, 0);
			free(name);
			getToken(fin);
		}while(token == commasym);

		if(token != semicolonsym)
		{
			errorDisplay(5);
			return -1;
		}

		getToken(fin);
	}
	if(token == intsym)
	{
		do
		{
			getToken(fin);
			if(token != identsym)
			{
				errorDisplay(4);
				return -1;
			}
			
			name = getIdentifier(fin);
			insertSymbolTable(2, name, 0, level, addr);
			addr += 1;
			free(name);
			getToken(fin);
		
		}while(token == commasym);

		if(token != semicolonsym)
		{
			errorDisplay(5);
			return -1;
		}
		// Increment Stack Pointer by amount needed
		emit(6, 0, 0, addr);
		getToken(fin);

	}

	/*while(token == procsym)
	{
		getToken(fin);
		if(token != identsym)
		{
			errorDisplay(4);
			return -1;
		}

		name = getIdentifier(fin);
		insertSymbolTable(3, name, 0, level, 0);
		free(name);

		getToken(fin);
		if(token != semicolonsym)
		{
			errorDisplay(5);
			return -1;
		}
		getToken(fin);

		test = block(fin, level+1);

		if(test == -1)
			return -1;

		if(token != semicolonsym)
		{
			errorDisplay(5);
			return -1;
		}

		getToken(fin);
	}*/
	
	test = statement(fin);
	
	if(test == -1)
		return -1;

	return 0;
}

int program(FILE * fin) 
{
	int test;

	getToken(fin);
	
	test = block(fin, 0);
	if(test == -1)
		return -1;
	if(token != periodsym){
		errorDisplay(9);
		return -1;
	}

	emit(2, 0, 0, 0);

	printf("No errors, program is syntactically correct.\n\n");
	return 0;
}

//Prints instructions to a file
void printCodeTable()
{
	int i = 0;
	FILE * fout = fopen("assembly.txt", "w");
	for(i = 0; i < next_code; i++)
	{
		fprintf(fout, "%d ", code_table[i].OP);
		fprintf(fout, "%d ", code_table[i].R);
		fprintf(fout, "%d ", code_table[i].L);
		fprintf(fout, "%d\n", code_table[i].M);
	}
	fclose(fout);
}

// Prints symbol table to screen
void printSymbolTable()
{
	int i = 0;
	for(i = 0; i < next_open; i++)
	{
		printf("%d ", symbol_table[i].kind);
		printf("%s ", symbol_table[i].name);
		printf("%d ", symbol_table[i].val);
		printf("%d ", symbol_table[i].level);
		printf("%d\n", symbol_table[i].addr);
	}
	printf("\n");
}

// Main parsing function
int main(void) {
	// Open file to read tokens
	FILE * fin = fopen("lexemelist.txt", "r");
	if(fin == NULL){
		printf("ERROR: Failed to open Lexeme List file.\n");
		return -1;
	}

	int test = 0, i = 0;	

	test = program(fin);
	if(test == -1)
		return -1;

	//printSymbolTable();
	printCodeTable();
	fclose(fin);
	return 0;
}