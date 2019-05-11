/**
 * Config Parser
 */
#ifndef CFG_PARSER_H
#define CFG_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
  char** keys;
  char** values;
  int    len;
  int    size;
} Config;

Config* new_config();
char*   cfg_get(Config* cfg, char* key);
void    cfg_put(Config* cfg, char* k, char* v);
Config* cfg_parse(char* filename);
void    cfg_cleanup(Config* cfg);

typedef struct {
  FILE* fp;
  char* contents;
  long  size;
} File;

File* open_file(char* filename);
void  close_file(File* f);

typedef struct {
  char* input;
  int   pos;
  int   readPos;
  char  cur;
} Parser;

char** str_split(char* s, char* delim);

#endif

