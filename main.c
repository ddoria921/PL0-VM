#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbol.h"
#include "parser_functions.h"
#include "error.h"

int main(int argc, char const *argv[])
{
	/* testing arguments */
	// printf("%d\n", argc);
	// printf("%s\n", argv[1]);

	// get input from file
	FILE *inputFile = fopen("input.txt", "r");

	if(!inputFile) 
		throwError(FILE_NOT_FOUND);

	printf("Found input file.\n");
	
	// pass input to scanner

	// get output from scanner and pass it to the parser

	// get ouput from the parser and pass it to the vm




	// flags to know what to print
	int shouldPrintLexemeTokens = 0;
	int shouldPrintAssemblyCode = 0;
	int shouldPrintVmOutput = 0;

	// -l --> print the list of lexemes/tokens (scanner output) to the screen
	// -a --> print the generated assembly code (parser/codegen output) to the screen
	// -v --> print virtual machine execution trace (virtual machine output) to the screen
	int i;
	for (i = 1; i < argc; i++) 
	{
		if (strcmp(argv[i], "-l"))
			shouldPrintLexemeTokens = 1;
		
		if (strcmp(argv[i], "-a"))
			shouldPrintAssemblyCode = 1;
		
		if (strcmp(argv[i], "-v"))
			shouldPrintVmOutput = 1;
	}


	/* UNCOMMENT TO PRINT OUTPUT */
	// if (shouldPrintLexemeTokens)
	// 	// print lexeme tokens
	// if (shouldPrintAssemblyCode)
	// 	// print assembly code
	// if (shouldPrintVmOutput)
		// print vm output


	return 0;
}
