// Darin Doria and Jorge Berroa
// COP 3402 - Fall 2014
// HW3 - PL/0 Parser

#define FILE_NOT_FOUND 99
#define VM_ERROR 100
#define PARSER_ERROR 105
#define SCANNER_ERROR 101

void throwError(int code)
{
	printf("\nError: ");

	switch (code) 
	{
		case 1:
			printf("Use = instead of :=\n");
			break;

		case 2:
			printf("= must be followed by a number\n");
			break;

		case 3:
			printf("Identifier must be followed by =\n");
			break;

		case 4:
			printf("const, var, procedure must be followed by identifier\n");
			break;

		case 5:
			printf("Semicolon or comma missing\n");
			break;

		case 6:
			printf("Incorrect symbol after procedure declaration\n");
			break;

		case 7:
			printf("Statement expected\n");
			break;

		case 8:
			printf("Incorrect symbol after statement part in block\n");
			break;

		case 9:
			printf("Period expected\n");
			break;

		case 10:
			printf("Semicolon between statements missing\n");
			break;

		case 11:
			printf("Undeclared identifier\n");
			break;

		case 12:
			printf("Assignment to constant or procedure is not allowed\n");
			break;

		case 13:
			printf("Assignment operator expected\n");
			break;

		case 14:
			printf("call must be followed by an identifier\n");
			break;

		case 15:
			printf("Call of a constant or variable is meaningless\n");
			break;

		case 16:
			printf("then expected\n");
			break;

		case 17:
			printf("Semicolon or } expected\n");
			break;

		case 18:
			printf("do expected\n");
			break;

		case 19:
			printf("Incorrect symbol following statement\n");
			break;

		case 20:
			printf("Relational operator expected\n");
			break;

		case 21:
			printf("Expression must not contain a procedure identifier\n");
			break;

		case 22:
			printf("Right parenthesis missing\n");
			break;

		case 23:
			printf("The preceding factor cannot begin with this symbol\n");
			break;

		case 24:
			printf("An expression cannot begin with this symbol\n");
			break;

		case 25:
			printf("This number is too large\n");
			break;

		case 26:
			printf("Identifier is too long.\n");
			break;

		case 27:
			printf("Begin must be followed by end.\n");
			break;

		case 28:
			printf("Read must be followed by valid identifier.\n");
			break;

		case 29:
			printf("Write must be followed by valid identifier.\n");
			break;

		case 30:
		printf("call must be followed by a procedure identifier\n");
		break;

		case FILE_NOT_FOUND:
			printf("Input file could not be opened\n");
			break;

		case SCANNER_ERROR:
			printf("Scanner error\n");
			break;

		case PARSER_ERROR:
			printf("Virtual machine error\n");
			break;

		case VM_ERROR:
			printf("Virtual machine error\n");
			break;

		default:
			printf("Something went wrong\n");
	}

	exit(-1);
}