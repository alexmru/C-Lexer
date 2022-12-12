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
	AND,
	OR,
	NOT,
	ASSIGN,
	EQUAL,
	NOTEQ,
	LESS
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
	"AND",
	"OR",
	"NOT",
	"ASSIGN",
	"EQUAL",
	"NOTEQ",
	"LESS"};

typedef struct
{
	int cod;   // codul atomului ( ID, INT, ...)
	int linie; // linia atomului respectiv
	union
	{
		char s[100]; // pentru ID-uri si stringuri
		double r;	 // pentru real
		int i;		 // pentru int
	};
} Atom;

Atom atomi[10000]; // vectorul cu atomii extrasi din fisierul de intrare
int nAtomi;		   // numarul de atomi din vectorul atomi

char bufin[30001];
char *pch;	   // cursor la caracterul curent din bufin
int linie = 1; // linia curenta; adaugata automat la atom de addAtom

void addAtom(int Atom)
{
	atomi[nAtomi++].cod = Atom;
	printf("\nAdded %s\n", tokenNames[Atom]);
}

// la fiecare apel returneaza codul unui atom
int getNextTk() // get next token (atom lexical)
{

	int state = 0; // starea curenta
	char buf[100];
	int n = 0; // nr caractere din buf
	for (;;)
	{
		char ch = *pch;										 // caracterul curent
		printf("#%d %c\n", state, ch); // pt debugging
		// cate un case pentru fiecare stare din diagrama
		switch (state)
		{
		case 0:
			// testam toate tranzitiile posibile din acea stare
			if (isalpha(ch) || ch == '_')
			{
				state = 1;
				pch++;
				buf[n++] = ch;
				break;
			}
			else if (isdigit(ch))
			{
				state = 3;
				pch++;
				break;
			}
			else if (ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t')
			{
				pch++;
				if (ch == '\n')
					linie++;
				return 0;
			}
			else if (ch == '#')
			{
				pch++;
				state = 5;
				break;
			}
			else if (ch == ',')
			{
				addAtom(COMMA);
				pch++;
				return COMMA;
			}
			else if (ch == ':')
			{
				addAtom(COLON);
				pch++;
				return COMMA;
			}
			else if (ch == ';')
			{
				addAtom(SEMICOLON);
				pch++;
				return COMMA;
			}
			else if (ch == '(')
			{
				addAtom(LPAR);
				pch++;
				return LPAR;
				break;
			}
			else if (ch == ')')
			{
				addAtom(RPAR);
				pch++;
				return RPAR;
			}
			else if (ch == '\0')
			{
				addAtom(FINISH);
				return FINISH;
			}
			else if (ch == '+')
			{
				addAtom(ADD);
				pch++;
				return ADD;
			}
			else if (ch == '-')
			{
				addAtom(SUB);

				pch++;
				return SUB;
			}
			else if (ch == '*')
			{
				addAtom(MUL);
				pch++;
				return MUL;
			}
			else if (ch == '/')
			{
				addAtom(DIV);
				pch++;
				return DIV;
			}
			else if (ch == '&')
			{
				pch++;
				state = 6;
				break;
			}
			else if (ch == '|')
			{
				pch++;
				state = 7;
				break;
			}
			else if (ch == '!')
			{
				pch++;
				state = 8;
				break;
			}
			else if (ch == '=')
			{
				pch++;
				state = 9;
				break;
			}
			else if (ch=='<')
			{
				addAtom(LESS);
				pch++;
				return LESS;
			}
			else if (ch == '"')
			{
				pch++;
				state = 10;
				break;
			}
			break;
		case 1:
			if (isalnum(ch) || ch == '_')
			{
				pch++;
				buf[n++] = ch;
			}
			else
			{
				state = 2;
				
			}
			break;
		case 2:
			buf[n] = '\0'; // incheiere sir
			if (strcmp(buf, "var") == 0)
			{
				addAtom(VAR);
				return VAR;
			}
			else if (strcmp(buf, "function") == 0)
			{
				addAtom(FUNCTION);
				return FUNCTION;
			}
			else if (strcmp(buf, "if") == 0)
			{
				addAtom(IF);
				return IF;
			}
			else if (strcmp(buf, "else") == 0)
			{
				addAtom(ELSE);
				return ELSE;
			}
			else if (strcmp(buf, "while") == 0)
			{
				addAtom(WHILE);
				return WHILE;
			}
			else if (strcmp(buf, "end") == 0)
			{
				addAtom(END);
				return END;
			}
			else if (strcmp(buf, "return") == 0)
			{
				addAtom(RETURN);
				return RETURN;
			}
			else if (strcmp(buf, "int") == 0)
			{
				addAtom(TYPE_INT);
				return TYPE_INT;
			}
			else if (strcmp(buf, "real") == 0)
			{
				addAtom(TYPE_REAL);
				return TYPE_REAL;
			}
			else if (strcmp(buf, "str") == 0)
			{
				addAtom(TYPE_STR);
				return TYPE_STR;
			}
			else
			{
				addAtom(ID); // id simplu
				strcpy(atomi[nAtomi - 1].s, buf);
			}
			return ID;
		case 3:
			if (isdigit(ch))
			{
				state = 3;
				pch++;
			}
			else if (ch == '.')
			{
				state = 4;
				pch++;
			}
			else
			{
				addAtom(INT);
				pch++;
				return (INT);
			}
			break;
		case 4:
			if (isdigit(ch))
			{
				state = 4;
				pch++;
			}
			else
			{
				addAtom(REAL);
				pch++;
				return (REAL);
			}
		case 5:
			if (ch == '\n'){
				pch++;
				return 0;
			}
			else
				pch++;
			break;
		case 6:
			if (ch == '&')
			{
				pch++;
				addAtom(AND);
				return AND;
			}
		case 7:
			if (ch == '|')
			{
				pch++;
				addAtom(OR);
				return OR;
			}
		case 8:
			if (ch == '=')
			{
				pch++;
				addAtom(NOTEQ);
				return NOTEQ;
			}
			else
			{
				addAtom(NOT);
				return (NOT);
			}
		case 9:
			if (ch == '=')
			{
				pch++;
				addAtom(EQUAL);
				return EQUAL;
			}
			else
			{
				addAtom(ASSIGN);
				return (ASSIGN);
			}
		case 10:
			if (ch=='"')
			{
				pch++;
				addAtom(STR);
				return(STR);
			}
			else {
				pch++;
				n++;
			}
			break;

		default:
			printf("stare invalida %d\n", state);
		}
	}
}

void printTokens()
{
	for (int i = 0; i < nAtomi; i++)
	{
		printf("%s ", tokenNames[atomi[i].cod]);
	}
}

int main()
{
	FILE *fis;
	fis = fopen("1.q", "rb");
	if (fis == NULL)
	{
		printf("nu s-a putut deschide fisierul");
		return -1;
	}
	int n = fread(bufin, 1, 30000, fis); // returneaza nr de elemente citite integral
	bufin[n] = '\0';
	fclose(fis);
	pch = bufin; // initializare pch pe prima pozitie din bufin

	while (getNextTk() != FINISH)
		;
	// printf("%s",bufin);
	// extragere atomi

	// afisare atomi
	//...
}
