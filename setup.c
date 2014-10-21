// Darin Doria and Jorge Berroa
// COP 3402 - Fall 2014
// HW3 - PL/0 Parser

// setup
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

typedef struct
{
	int 	kind; 		// const = 1, var = 2, proc = 3
	char 	name[10];	// name up to 11 chars
	int 	val;		// number (ASCII val)
	int 	level;		// L level
	int 	addr;		// M address
} symbol;

// Struct to represent instructions in code
typedef struct
{
	int OP;
	int L;
	int M;
} instruction;

int factor(FILE * input);