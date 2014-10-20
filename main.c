#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbol.h"
#include "parser_functions.h"
#include "error.h"

#define SCANNER_OUTPUT 0
#define PARSER_OUTPUT 1
#define VM_OUTPUT 2
#define TRUE 1

static const char scanCommand[] = "./scanner > lexemetable.txt";
static const char parserCommand[] = "./parser";
static const char vmCommand[] = "./vm";

int runSanner()
{
	int status = -1;
	status = system(scanCommand);

	// if (status != 0)
	// 	throwError(SCANNER_ERROR);

	return status;
}

int runParser()
{
	// TODO : fill in code
	int status = -1;
	status = system(parserCommand);

	// if (status != 0)
	// 	throwError(SCANNER_ERROR);

	return status;
}

int runVirtualMachine()
{
	int status = -1;
	status = system(vmCommand);

	// if (status != 0)
	// 	throwError(VM_ERROR);

	return status;
}

void printOutputFromFile(char *filename)
{
	int c;
	FILE *file;
	file = fopen(filename, "r");
	if (file)
	{
	    while ((c = getc(file)) != EOF)
	        putchar(c);
	    
	    fclose(file);
	    putchar('\n');
	}
}

void printOutput(int *flags)
{
	if (flags[SCANNER_OUTPUT])
	{
		printf("\nScanner output\n================\n");
		printOutputFromFile("lexemelist.txt");
	}

	if (flags[PARSER_OUTPUT])
	{
		printf("\nParser output\n================\n");
		printOutputFromFile("mcode.txt");
	}
		
	if (flags[VM_OUTPUT])
	{
		printf("\nVirtual machine output\n=========================\n");
		printOutputFromFile("stacktrace.txt");
	}
}

int main(int argc, const char *argv[])
{
	// run scanner
	// input	=> input.txt
	// output 	=> lexemelist.txt
	if (runSanner() != 0)
	{
		printf("SCANNER ERROR\n");
		exit(-1);
	}
		

	// run parser using output from scanner
	// input 	=> lexemelist.txt
	// output 	=> mcode.txt
	if (runParser() != 0)
	{
		printf("PARSER ERROR\n");
		exit(-1);
	}

	// get ouput from the parser and pass it to the vm
	// input 	=> mcode.txt
	// ouput 	=> stacktrace.txt
	if (runVirtualMachine() != 0)
	{
		printf("VM ERROR\n");
		exit(-1);
	}

	// flags to know what to print
	int flags[3] = { 0, 0, 0 };

	// -l --> print the list of lexemes/tokens (scanner output) to the screen
	// -a --> print the generated assembly code (parser/codegen output) to the screen
	// -v --> print virtual machine execution trace (virtual machine output) to the screen
	int i;
	for (i = 1; i < argc; i++) 
	{
		if (strcmp(argv[i], "-l") == 0)
			flags[SCANNER_OUTPUT] = TRUE;
		
		if (strcmp(argv[i], "-a") == 0)
			flags[PARSER_OUTPUT] = TRUE;
		
		if (strcmp(argv[i], "-v") == 0)
			flags[VM_OUTPUT] = TRUE;
	}

	// printf("Contents of flags %d %d %d\n", flags[0], flags[1], flags[2]);
	printOutput(flags);

	return 0;
}
