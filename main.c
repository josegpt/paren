#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

typedef enum {
  NONE,
  TOKENIZE,
} Flag;

void usage(const char* program)
{
  printf("usage: %s -[FLAG]... filename\n", program);
  printf("\n");
  printf("flags:\n");
  printf("    -t          If you would like to print all tokens of the lexer\n");
  printf("    -h          print this info\n");
}

int main(int argc, char** argv)
{
  const char* program = *argv++;
  if (*argv == NULL) {
    usage(program);
    fprintf(stderr, "ERROR: filename was not provided.\n");
    exit(1);
  }
  const char* subcommand = *argv++;
  if (strcmp(subcommand, "-h") == 0) {
    usage(program);
  } else {
    if (*argv == NULL) {
      usage(program);
      fprintf(stderr, "ERROR: filename was not provided.\n");
      exit(1);
    }
    const char* filename = *argv++;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
      usage(program);
      fprintf(stderr, "ERROR: could not open %s", filename);
      exit(1);
    }
    if (strcmp(subcommand, "-t") == 0) {
      dump_tokens(fp);
    }
    fclose(fp);
  }
  return 0;
}
