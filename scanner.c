#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_IDENT_LENGTH_PROT 9
#define MAX_NUM_LENGTH_PROT 5
#define MAX_IDENT_LENGTH 11
#define MAX_NUM_LENGTH 5

#include "helper.c"

int scanFile(FILE *inputFile, FILE *lexemelist)
{
	char current, comment;
	int lineLength = 0;

	char *buffer = malloc(sizeof(char) * (MAX_IDENT_LENGTH + MAX_IDENT_LENGTH_PROT));

	while ((current = fgetc(inputFile)) != EOF)
	{
		if (lineLength == MAX_IDENT_LENGTH + 1)
		{
			buffer[lineLength] = '\0';
			printError("Identifier length too line.\n");
			return -1;
		}

		if (isalnum(current) != 0)
		{
			buffer[lineLength++] = current;
			if(lineLength > 1)
			{
				if( isdigit(buffer[0]) != 0 && isalpha(buffer[1]) != 0 )
				{
					printError("Name mismatch\n");
					return -1;	
				}
				if( isdigit(buffer[0]) == 0 && isalpha(buffer[1]) == 0 )
				{
					printError("Variable does not start with letter\n");
					return -1;	
				}
				if(lineLength > MAX_NUM_LENGTH && isdigit(buffer[0]) != 0)
				{
					printError("Number too long\n");
				}
			}
			buffer[lineLength] = '\0';
		}

		else if (isspace(current) != 0)
		{
			if(lineLength >= 1 &&  checkForReservedWord(buffer, lexemelist) == -1)
			{
				printf("%s\t%d\n", buffer, (isdigit(buffer[0]) != 0) ? 3 : 2 );
				fprintf(lexemelist, "%d %s ", (isdigit(buffer[0]) != 0) ? 3 : 2, buffer );
			}

			buffer[lineLength] = '\0';
			free(buffer);
			buffer = malloc(sizeof(char) * (MAX_IDENT_LENGTH + MAX_IDENT_LENGTH_PROT));
			lineLength = 0;

			continue;
		}

		else if ( ispunct(current) != 0)
		{
			buffer[lineLength] = '\0';	

			if( lineLength >= 1 && checkForReservedWord(buffer, lexemelist) == -1)
			{
				printf("%s\t%d\n", buffer, (isdigit(buffer[0]) != 0) ? 3 : 2 );
				fprintf(lexemelist, "%d %s ", (isdigit(buffer[0]) != 0) ? 3 : 2, buffer );
			}

			free(buffer);
			buffer = malloc(sizeof(char) * (MAX_IDENT_LENGTH + MAX_IDENT_LENGTH_PROT));
			lineLength = 0;

			if (current == '/')
			{
				printf("Found s slash, checking for comment...\n");
				current = fgetc(inputFile);

				if (current == '*')
				{
					printf("Found comment, skipping comment\n");
					skipComment(inputFile);
					continue;
				}
				else
					ungetc(current, inputFile);
			}


			if (checkForSymbol(inputFile, current, lexemelist) == -1)
			{
				printError("Invalid symbol\n");
				return -1;
			}
		}
	}

	if (buffer != NULL)
		free(buffer);

	return -1;
}


int main(int argc, const char *argv[])
{
	// Init character and number arrays
	int *numBuffer, i;
	char *charBuffer;

	// create file vars
	FILE *inputFile, *lexemelist;

	// Open files
	inputFile = fopen("input.txt", "r");
	lexemelist = fopen("lexemelist.txt", "w");

	// return error if file is not found
	if(inputFile == NULL)
	{
		printf("ERROR: input.txt could not be opened\n");
		return -1;
	}

	// allocate space for buffers
	charBuffer = malloc(sizeof(char) * (MAX_IDENT_LENGTH + MAX_IDENT_LENGTH_PROT));
	numBuffer = malloc(sizeof(int) * (MAX_NUM_LENGTH + MAX_NUM_LENGTH_PROT));

	printf("lexeme\ttoken type\n");

	// start scanning file
	scanFile(inputFile, lexemelist);
	
	return 0;
}




