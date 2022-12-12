#include <stdlib.h>
#include <stdio.h>

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
    LESS
}; // toate codurile atomilor din Quick, fara SPACE si COMMENT

typedef struct{
	int cod;		// codul atomului ( ID, INT, ...)
	}Atom;
Atom atomi[10000];	// vectorul cu atomii extrasi din fisierul de intrare
int nAtomi;	// numarul de atomi din vectorul atomi

char bufin[30001];
char *pch;	// cursor la caracterul curent din bufin

// la fiecare apel returneaza codul unui atom
int getNextTk()			// get next token (atom lexical)
{
	int state=0;		// starea curenta
	for(;;){
		char ch=*pch;	// caracterul curent
		printf("#%d %c(%d)\n",state,ch,ch);	// pt debugging
		// cate un case pentru fiecare stare din diagrama
		switch(state){
			case 0:
				// testam toate tranzitiile posibile din acea stare
				if(isalpha(ch)||ch=='_'){state=1;pch++;}
				else if(isdigit(ch)){state=3;pch++;}
				else if(ch==' '||ch=='\r'||ch=='\n'||ch=='\t'){pch++;}
				break;
			case 1:
				if(isalnum(ch)||ch=='_'){pch++;}
				else{state=2;}
				break;
			case 2:
				addAtom(ID);	// adauga atomul gasit in lista de atomi
				return ID;
			//..........................
			default: printf("stare invalida %d\n",state);
			}
		}
	
}

int main()
{
	FILE *fis;
	fis=fopen("1.q","rb");
	if(fis==NULL){
		printf("nu s-a putut deschide fisierul");
		return -1;
		}
	int n=fread(bufin,1,30000,fis);	// returneaza nr de elemente citite integral
	bufin[n]='\0';
	fclose(fis);
	pch=bufin;	// initializare pch pe prima pozitie din bufin
	// extragere atomi
	while(getNextTk()!=FINISH){
		}
	// afisare atomi
	//...
}