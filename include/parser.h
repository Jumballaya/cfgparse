/**
 * Config Parser
 */
#ifndef CFG_PARSER_H
#define CFG_PARSER_H

typedef struct {
  char** keys;
  char** values;
  int len;
  int size;
} Config;

Config* new_config();
char* config_get(Config* cfg, char* key);
void config_put(Config* cfg, char* k, char* v);
Config* cfg_parse(char* filename);

char** str_split(char* s, char* delim);

#endif

