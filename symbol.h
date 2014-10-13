#define MAX_SYMBOL_TABLE_SIZE 100
#define MAX_CHAR_LENGTH 12

typedef struct symbol
{ 
	int		kind; 					// const = 1, var = 2, proc = 3
	char 	name[MAX_CHAR_LENGTH];	// name up to 11 chars
	int 	val; 					// number (ASCII value) 
	int 	level; 					// L level
	int 	addr; 					// M address
} symbol;