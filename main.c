#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lex.h"


extern int paren;  //to check parenthesis
extern int brace;  //to check braces
extern int in_string;  
extern int in_comment;

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Usage : %s <filename.c>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if(!fp)
    {
        printf("Cannot open file!\n");
        return 1;
    }

    printf("----------------------------------------\n");
    printf("TOKEN TYPE         ->    LEXEME\n");
    printf("----------------------------------------\n");

    char ch;
    while((ch = fgetc(fp)) != EOF)
    {
        if(isspace(ch))  //whitespace
            continue;

        
        if(ch == '#')
        {
            preprocessor(fp);  //preprocessor
            continue;
        }

        
        if(isalpha(ch) || ch == '_')
        {
            identifier(fp, ch); //identifier
            continue;
        }

        
        if(isdigit(ch))
        {
            number(fp, ch);   //number
            continue;
        }

        
        if(strchr(";,(){}[]", ch))
        {
            delimiter(ch);  //delimiter
            continue;
        }

        
        if(ch == '/')
        {
            char next = fgetc(fp);

            if(next == '/' || next == '*')
            {
                comment(fp, next);  //comment
                continue;
            }
            else
            {
                fseek(fp, -1, SEEK_CUR);
                Operator(fp, ch);  //operator
                continue;
            }
        }

        
        if(ch == '"')
        {
            stringliteral(fp);  //string literals
            continue;
        }

        
        Operator(fp, ch);  //operator
    }

    fclose(fp);

    
    printf("\n----------------------------------------\n");
    printf("OBSERVATIONS:\n");

    if(paren != 0) //not zero means error
        printf("POSSIBLE ERROR: Unmatched parentheses detected!\n");

    if(brace != 0) //not zero means error
        printf("POSSIBLE ERROR: Unmatched braces detected!\n");

    if(in_string)  //1 means error
        printf("POSSIBLE ERROR: Unterminated string literal!\n");

    if(in_comment) //1 means error
        printf("POSSIBLE ERROR: Unterminated /* comment */!\n");

    if(paren > 0)  //greater than zero means error
        printf("POSSIBLE ERROR: Missing '(' or ')' in expression!\n");

    if(brace > 0)  //greater than zero means error
        printf("POSSIBLE ERROR: Missing '{' or '}' in expression!\n");

    if(!paren && !brace && !in_string && !in_comment) //everything zero means correct
        printf("No lexical errors detected.\n");

    printf("----------------------------------------\n");

    return 0;
}
