#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

#define LEXEME_LENGTH 1024

typedef enum {
  LPAREN,
  RPAREN,
  PLUS,
  MINUS,
  MULT,
  DIV,
  MOD,
  INT,
  ERROR,
  DONE,
} TokenType;

typedef enum {
  INIT,
  SAWMINUS,
  INNUMBER,
  INCOMMENT,
} State;

struct Token {
  TokenType tt;
  char lexeme[LEXEME_LENGTH];
  int lnumber;
};

Token __get_token(FILE* fp, int* lnumber) {
  Token token = {0};
  State state = INIT;
  char lexeme[LEXEME_LENGTH];
  int lexeme_length = 0;
  int c;
  while ((c = fgetc(fp)) != EOF) {;
    if (c == '\n') (*lnumber)++;
    switch (state) {
    case INIT:
      if (isspace(c)) continue;
      lexeme[lexeme_length++] = c;
      if (isdigit(c)) {
        state = INNUMBER;
      } else if (c == '-') {
        state = SAWMINUS;
      } else if (c == ';') {
        state = INCOMMENT;
      } else {
        token.tt = ERROR;
        sprintf(token.lexeme, "ERROR: unknown token `%c` on line %d\n", c, *lnumber);
        switch(c) {
        case '(':
          token.tt = LPAREN;
          lexeme[lexeme_length] = '\0';
          strcpy(token.lexeme, lexeme);
          break;
        case ')':
          token.tt = RPAREN;
          lexeme[lexeme_length] = '\0';
          strcpy(token.lexeme, lexeme);
          break;
        case '+':
          token.tt = PLUS;
          lexeme[lexeme_length] = '\0';
          strcpy(token.lexeme, lexeme);
          break;
        case '*':
          token.tt = MULT;
          lexeme[lexeme_length] = '\0';
          strcpy(token.lexeme, lexeme);
          break;
        case '/':
          token.tt = DIV;
          lexeme[lexeme_length] = '\0';
          strcpy(token.lexeme, lexeme);
          break;
        case '%':
          token.tt = MOD;
          lexeme[lexeme_length] = '\0';
          strcpy(token.lexeme, lexeme);
          break;
        }
        token.lnumber = *lnumber;
        return token;
      }
      break;
    case INCOMMENT:
      if (c == '\n') {
        state = INIT;
      }
      break;
    case SAWMINUS:
      if (isdigit(c)) {
        lexeme[lexeme_length++] = c;
        state = INNUMBER;
      } else if (c == ' ') {
        token.tt = MINUS;
        strcpy(token.lexeme, lexeme);
        token.lnumber = *lnumber;
        return token;
      }
      break;
    case INNUMBER:
      if (isdigit(c) || c == '.') {
        lexeme[lexeme_length++] = c;
      } else if (c == ' ' || c == ')') {
        ungetc(c, fp);
        token.tt = INT;
        lexeme[lexeme_length] = '\0';
        strcpy(token.lexeme, lexeme);
        token.lnumber = *lnumber;
        return token;
      } else {
        token.tt = ERROR;
        sprintf(token.lexeme, "ERROR: found `%c` while tokenizing `int` on line %d\n", c, *lnumber);
        token.lnumber = *lnumber;
        return token;
      }
      break;
    }
  }
  if (feof(fp)) {
    token.tt = DONE;
    strcpy(token.lexeme, "DONE");
    token.lnumber = *lnumber;
    return token;
  }
  token.tt = ERROR;
  strcpy(token.lexeme, "ERROR: something went wrong while performing I/O");
  token.lnumber = *lnumber;
  return token;
}

Token get_token(FILE* fp) {
  int lnumber = 0;
  return __get_token(fp, &lnumber);
}

void dump_tokens(FILE* fp) {
  Token token;
  do {
    token = get_token(fp);
    switch(token.tt) {
    case RPAREN:
      printf("RPAREN(%s)\n", token.lexeme);
      break;
    case LPAREN:
      printf("LPAREN(%s)\n", token.lexeme);
      break;
    case PLUS:
      printf("PLUS(%s)\n", token.lexeme);
      break;
    case MINUS:
      printf("MINUS(%s)\n", token.lexeme);
      break;
    case MULT:
      printf("MULT(%s)\n", token.lexeme);
      break;
    case DIV:
      printf("DIV(%s)\n", token.lexeme);
      break;
    case MOD:
      printf("MOD(%s)\n", token.lexeme);
      break;
    case INT:
      printf("NUMBER(%s)\n", token.lexeme);
      break;
    case DONE:
      printf("DONE(%s)\n", token.lexeme);
      break;
    default:
      printf("ERROR(%s)\n", token.lexeme);
    }
  } while (token.tt != ERROR && token.tt != DONE);
}
