#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum
{
	ID,
	VAR,
	FUNCTION,
	IF,
	ELSE,
	WHILE,
	END,
	RETURN,
	TYPE_INT,
	TYPE_REAL,
	TYPE_STR,
	INT,
	REAL,
	STR,
	COMMA,
	COLON,
	SEMICOLON,
	LPAR,
	RPAR,
	FINISH,
	ADD,
	SUB,
	MUL,
	DIV,
	LESS,
	AND,
	OR,
	NOT,
	ASSIGN,
	EQUAL,
	NOTEQ
}; // toate codurile atomilor din Quick, fara SPACE si COMMENT

char tokenNames[][10] = {
	"ID",
	"VAR",
	"FUNCTION",
	"IF",
	"ELSE",
	"WHILE",
	"END",
	"RETURN",
	"TYPE_INT",
	"TYPE_REAL",
	"TYPE_STR",
	"INT",
	"REAL",
	"STR",
	"COMMA",
	"COLON",
	"SEMICOLON",
	"LPAR",
	"RPAR",
	"FINISH",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"LESS",
	"AND",
	"OR",
	"NOT",
	"ASSIGN",
	"EQUAL",
	"NOTEQ"};

typedef struct
{
	int cod, linie; // codul atomului ( ID, INT, ...)
	union
	{
		char s[100]; // pentru ID-uri si stringuri
		double r;	 // pentru real
		int i;		 // pentru int
	};
} Atom;

Atom atomi[10000];	  // vectorul cu atomii extrasi din fisierul de intrare
int nAtomi, line = 1; // numarul de atomi din vectorul atomi
char bufin[30001], *pch;

int addAtom(int Atom)
{
	atomi[nAtomi].linie = line;
	atomi[nAtomi++].cod = Atom;
	return Atom;
}

// la fiecare apel returneaza codul unui atom
int getNextTk()
{
	int state = 0, n = 0, i; // state - current state; n - nr caractere din buf
	char buf[100];
	for (;;)
	{
		char ch = *pch; // caracterul curent
		switch (state)
		{
		case 0:
			pch++;
			if (isalpha(ch) || ch == '_')
			{
				state = 1;
				buf[n++] = ch;
			}
			else if (isdigit(ch))
			{
				state = 3;
				buf[n++] = ch;
			}
			else if (isspace(ch))
			{
				if (ch == '\n')
					return line++;
			}
			else if (ch == ',')
				return addAtom(COMMA);
			else if (ch == ':')
				return addAtom(COLON);
			else if (ch == ';')
				return addAtom(SEMICOLON);
			else if (ch == '(')
				return addAtom(LPAR);
			else if (ch == ')')
				return addAtom(RPAR);
			else if (ch == '\0')
				return addAtom(FINISH);
			else if (ch == '+')
				return addAtom(ADD);
			else if (ch == '-')
				return addAtom(SUB);
			else if (ch == '*')
				return addAtom(MUL);
			else if (ch == '/')
				return addAtom(DIV);
			else if (ch == '<')
				return addAtom(LESS);
			else if (ch == '#')
				state = 5;
			else if (ch == '&')
				state = 6;
			else if (ch == '|')
				state = 7;
			else if (ch == '!')
				state = 8;
			else if (ch == '=')
				state = 9;

			else if (ch == '"')
				state = 10;
			break;
		case 1:
			if (isalnum(ch) || ch == '_')
			{
				pch++;
				buf[n++] = ch;
			}
			else
				state = 2;
			break;
		case 2:
			buf[n] = '\0'; // end the string
			char keywords[][10] = {"var", "function", "if", "else", "while",
								   "end", "return", "int", "real", "str"};
			for (i = 0; i < 10; i++)
				if (!strcmp(buf, keywords[i]))
					return addAtom(i + VAR);

			strcpy(atomi[nAtomi].s, buf); // copy string
			return addAtom(ID);

		case 3: // integer being read. if '.' is read go to real state
			if (ch == '.')
			{
				state = 4;
			}
			else if (!isdigit(ch))
			{
				buf[n] = '\0';
				atomi[nAtomi].i = atoi(buf);
				return addAtom(INT);
			}
			buf[n++] = ch;
			pch++;
			break;
		case 4: // real number being read
			if (isdigit(ch))
			{
				buf[n++] = ch;
				pch++;
				continue;
			}
			buf[n] = '\0';
			atomi[nAtomi].r = atof(buf);
			return addAtom(REAL);
		case 5: // comment being read
			pch++;
			if (ch == '\n')
			{
				line++;
				return 0;
			}
			break;
		case 6: // && (AND)
			if (ch == '&')
			{
				pch++;
				return addAtom(AND);
			}
		case 7: // || (OR)
			if (ch == '|')
			{
				pch++;
				return addAtom(OR);
			}
		case 8: // ! or !=
			if (ch == '=')
			{
				pch++;
				return addAtom(NOTEQ);
			}
			return addAtom(NOT);
		case 9: // = or ==
			if (ch == '=')
			{
				pch++;
				return addAtom(EQUAL);
			}
			return addAtom(ASSIGN);
		case 10: // string is being read
			pch++;
			if (ch == '"') // if end of string is found copy its value and return it
			{
				buf[n] = '\0';
				strcpy(atomi[nAtomi].s, buf);
				return addAtom(STR);
			}
			buf[n++] = ch;
			break;
		}
	}
}

void printTokens()
{
	for (int i = 0; i < nAtomi; i++)
	{
		if (i == 0 || atomi[i].linie > atomi[i - 1 > 0 ? i - 1 : 0].linie)
			printf("\n%2d:", atomi[i].linie);
		printf(" %s", tokenNames[atomi[i].cod]);
		if (atomi[i].cod == INT)
			printf(":%d", atomi[i].i);
		if (atomi[i].cod == REAL)
			printf(":%f", atomi[i].r);
		if (atomi[i].cod == STR || atomi[i].cod == ID)
			printf(":%s", atomi[i].s);
	}
}

int main()
{
	FILE *fis;
	fis = fopen("1.q", "rb");
	int n = fread(bufin, 1, 30000, fis); // returneaza nr de elemente citite integral
	bufin[n] = '\0';
	fclose(fis);
	pch = bufin; // initializare pch pe prima pozitie din bufin
	while (getNextTk() != FINISH)
		;
	printTokens();
}