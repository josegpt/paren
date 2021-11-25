#ifndef __LEXER_H_
#define __LEXER_H_

typedef struct Token Token;

Token get_token(FILE* fp);
void dump_tokens(FILE* fp);

#endif // __LEXER_H_
