#ifndef LEX_H
#define LEX_H

#include <stdio.h>
#include <string.h>

#define MAX_BUFFER 50  //max size of temp array

typedef enum {
    LEX_KEYWORD,    //keyword token
    LEX_IDENTIFIER, //identifier token
    LEX_NUMBER,     //number token
    LEX_OPERATOR,   //operator token
    LEX_DELIMITER,  //delimiter token
    LEX_PROCESSOR,  //preprocessor token
    LEX_UNKNOWN
} Lextype;

extern const char *keywords[];   //array to store keywords

   
 
int iskeyword(char *word);  //fn to check keyword

void preprocessor(FILE *fp);  //fn to check preprocessors
void identifier(FILE *fp, char firstchar);  //fn to check identifiers
void number(FILE *fp, char firstchar);   //fn to check numerical literals
void Operator(FILE *fp, char firstchar); //fn to check operators
void delimiter(char ch);                 //fn to check delimiters
void comment(FILE *fp, char next);      //fn to check comments
void stringliteral(FILE *fp);           //fn to check string literals

#endif
