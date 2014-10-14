#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// global array of reserved words
const char *RESERVED[] = {"begin", "end", "if", "then", "while", "do", "call", "const", "var", "procedure", "write", "read", "else"};

int checkForReservedWord(char *word, FILE *lexemelist)
{
	if(word == NULL)
		return -2;

	// get the number of reserved words
	int length = sizeof(RESERVED) / sizeof(RESERVED[0]);
	int i;

	for(i = 0; i < length; i++)
	{
		if(strcmp(word, RESERVED[i]) == 0)
		{
			printf("%s\t%d\n", word, i + 21);
			fprintf(lexemelist, "%d ", i + 21);
			return i + 21;
		}
	}
	if (strcmp(word, "null") == 0)
	{
		printf("%s\t%d\n", word, 1);
		fprintf(lexemelist, "%d ", 1);
		return 1;
	}

	if (strcmp(word, "odd") == 0)
	{
		printf("%s\t%d\n", word, 8);	
		fprintf(lexemelist, "%d ", 8);
		return 8;
	}

	return -1;
}


int checkForSymbol(FILE *inputFile, char symbol, FILE *lexemelist)
{
	char tmp = ' ';
	switch(symbol)
		{
			case '<':
				if( (tmp = fgetc(inputFile)) == '>' )
				{
					printf("<>\t10\n");
					fprintf(lexemelist, "%d ", 10);
					return 10;
				}
				else if( tmp == '=')
				{
					printf("<=\t12\n");
					fprintf(lexemelist, "%d ", 12);
					return 12;
				}
				else
					ungetc(tmp, inputFile);
				fprintf(lexemelist, "%d ", 11);
				return 11;

			case '>':
				if( (tmp = fgetc(inputFile)) == '=' )
				{
					printf("<=\t12\n");
					fprintf(lexemelist, "%d ", 14);
					return 14;
				}
				else
					ungetc(tmp, inputFile);
				fprintf(lexemelist, "%d ", 13);
				return 13;

			case '+':
				printf("+\t4\n");
				fprintf(lexemelist, "%d ", 4);
				return 4;

			case '-':
				printf("-\t5\n");
				fprintf(lexemelist, "%d ", 5);
				return 5;

			case '*':
				printf("*\t6\n");
				fprintf(lexemelist, "%d ", 6);
				return 6;

			case '/':
				printf("/\t7\n");
				fprintf(lexemelist, "%d ", 7);
				return 7;

			case '=':
				printf("=\t9\n");
				fprintf(lexemelist, "%d ", 9);
				return 9;

			case '(':
				printf("(\t15\n");	
				fprintf(lexemelist, "%d ", 15);
				return 15;

			case ')':
				printf(")\t16\n");
				fprintf(lexemelist, "%d ", 16);
				return 16;

			case ',':
				printf(",\t17\n");
				fprintf(lexemelist, "%d ", 17);
				return 17;

			case ';':
				printf(";\t18\n");
				fprintf(lexemelist, "%d ", 18);
				return 18;

			case '.':
				printf(".\t19\n");
				fprintf(lexemelist, "%d ", 19);
				return 19;

			case ':':
				if ( (tmp = fgetc(inputFile)) == '=' )
				{
					printf(":=\t20\n");
					fprintf(lexemelist, "%d ", 20);
					return 20;
				}
				else
					ungetc(tmp, inputFile);
				return -1;

			default:
				return -1;

		}

	return -1;
}

void printError(char *error)
{
	printf("\nERROR: %s", error);
}

void skipComment(FILE *inputFile)
{
	char tmp = ' ';

	// start skipping comment
	while (!feof(inputFile))
	{
		tmp = fgetc(inputFile);
		if (tmp == '*')
		{
			printf("GOT STAR\n");
			tmp = fgetc(inputFile);
			if (tmp == '/')
			{
				// printf("FOUND LAST SLASH\n");
				// tmp = fgetc(inputFile);
				// printf("Last char: %c", tmp);
				return;
			}
			// else
			// {
			// 	printf("Ungetting: %c", tmp);
			// 	ungetc(tmp, inputFile);	
			// }
		}
	}

	
	// end of comment
}




