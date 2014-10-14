// Code generation

// program
// var decl
// statemnt
// ident
// if
// while
// read
// write
// e
// exp
// term 
// factor


void block()
{
	constDeclaration();
	varDeclaration();
	statement();
}

void varDeclaration( * )
{
	token = getNextToken(); // get next token in file

	int varCount = 0;
	if (token == varsym) // defined in file
	{
		token = getNextToken();

		if (token != identsym)
			throwError();


		char name[11];
		fscanf(fp, "%s", name);
		varCount++;
		insertToSymbolTable(name, varCount); //address starts at one andis incremented


		token = getNextToken();
		while (token == commasym)
		{
			token = getNextToken();

			if (token != identsym)
				throwError();

			fscanf(fp, "%s", name);
			varCount++;
			insertToSymbolTable(name, varCount); //address starts at one andis incremented

			token = getNextToken();
		}


		if (token != semicolonsym)
			throwError();

		emit(INC, 0, varCount);
	}
}

var statement() 
{
	case identsym:
		char name[11];
		fscanf(fp, "%s", name);

		token = getNextToken();

		if (token == becomesym)
			throwError();

		expression();

		lexeme l = lookUP(name); // some abstract data structure

		emit();

}















