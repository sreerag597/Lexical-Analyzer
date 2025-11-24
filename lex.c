#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lex.h"

// ---------------------------------------------------------
// Shared global counters for error observations
// ---------------------------------------------------------
int paren = 0;        // Tracks ( and )
int brace = 0;        // Tracks { and }
int in_string = 0;    // Inside a string literal
int in_comment = 0;   // Inside /* ... */

// ---------------------------------------------------------
// Keyword table
// ---------------------------------------------------------
const char *keywords[] = {
    "int","float","char","double","long","short","void","if","else",
    "for","while","do","switch","case","break","continue","return",
    "static","struct","typedef"
};

// ---------------------------------------------------------
// Keyword check function
// ---------------------------------------------------------
int iskeyword(char *word)
{
    int n = sizeof(keywords) / sizeof(keywords[0]);
    for(int i = 0; i < n; i++)
    {
        if(strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

// ---------------------------------------------------------
// Preprocessor
// ---------------------------------------------------------
void preprocessor(FILE *fp)
{
    char buffer[MAX_BUFFER];
    int i = 0;

    buffer[i++] = '#';

    char ch;
    while((ch = fgetc(fp)) != '\n' && ch != EOF)
        buffer[i++] = ch;

    buffer[i] = '\0';

    printf("%-18s ->   %-15s\n", "PREPROCESSOR", buffer);
}

// ---------------------------------------------------------
// Identifier / Keyword
// ---------------------------------------------------------
void identifier(FILE *fp, char firstchar)
{
    char buffer[MAX_BUFFER];
    int i = 0;

    buffer[i++] = firstchar;
    char ch;

    while(isalnum((ch = fgetc(fp))) || ch == '_')
        buffer[i++] = ch;

    buffer[i] = '\0';
    fseek(fp, -1, SEEK_CUR);

    if(iskeyword(buffer))
        printf("%-18s ->   %-15s\n", "KEYWORD", buffer);
    else
        printf("%-18s ->   %-15s\n", "IDENTIFIER", buffer);
}

// ---------------------------------------------------------
// Number
// ---------------------------------------------------------
void number(FILE *fp, char firstchar)
{
    char buffer[MAX_BUFFER];
    int i = 0;

    buffer[i++] = firstchar;
    char ch;

    while(isdigit((ch = fgetc(fp))))
        buffer[i++] = ch;

    buffer[i] = '\0';
    fseek(fp, -1, SEEK_CUR);

    printf("%-18s ->   %-15s\n", "NUMBER", buffer);
}

// ---------------------------------------------------------
// Operator
// ---------------------------------------------------------
void Operator(FILE *fp, char firstchar)
{
    printf("%-18s ->   %-15c\n", "OPERATOR", firstchar);
}

// ---------------------------------------------------------
// Delimiter ( ) { } [ ] ; ,
// ---------------------------------------------------------
void delimiter(char ch)
{
    switch(ch)
    {
        case '(':
            paren++;
            printf("%-18s ->   %-15c\n", "OPEN PARENTHESIS", ch);
            break;

        case ')':
            paren--;
            printf("%-18s ->   %-15c\n", "CLOSED PARENTHESIS", ch);
            break;

        case '{':
            brace++;
            printf("%-18s ->   %-15c\n", "OPEN BRACE", ch);
            break;

        case '}':
            brace--;
            printf("%-18s ->   %-15c\n", "CLOSED BRACE", ch);
            break;

        case ';':
            printf("%-18s ->   %-15c\n", "END OF LINE", ch);
            break;

        case ',':
            printf("%-18s ->   %-15c\n", "DELIMITER", ch);
            break;

        case '[':
            printf("%-18s ->   %-15c\n", "OPEN BRACKET", ch);
            break;

        case ']':
            printf("%-18s ->   %-15c\n", "CLOSED BRACKET", ch);
            break;
    }
}

// ---------------------------------------------------------
// Comment handler ( // and /* */ )
// ---------------------------------------------------------
void comment(FILE *fp, char next)
{
    char ch;

    // Single-line comment
    if(next == '/')
    {
        while((ch = fgetc(fp)) != '\n' && ch != EOF);
    }
    // Multi-line comment
    else if(next == '*')
    {
        in_comment = 1;

        while(1)
        {
            ch = fgetc(fp);
            if(ch == EOF) break;

            if(ch == '*')
            {
                if((ch = fgetc(fp)) == '/')
                {
                    in_comment = 0;
                    break;
                }
            }
        }
    }
}

// ---------------------------------------------------------
// String Literal
// ---------------------------------------------------------
void stringliteral(FILE *fp)
{
    in_string = 1;

    char buffer[MAX_BUFFER];
    int i = 0;
    char ch;

    buffer[i++] = '"';

    while((ch = fgetc(fp)) != EOF && ch != '"')
    {
        buffer[i++] = ch;

        if(i >= MAX_BUFFER - 1)
            break;  
    }

    if(ch == '"')
    {
        buffer[i++] = '"';
        in_string = 0;
    }

    buffer[i] = '\0';

    printf("%-18s ->   %-15s\n", "STRING LITERAL", buffer);
}
