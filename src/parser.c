/**
 * Config Parser
 */
#include "parser.h"

#define TOK_COMMENT '#'
#define TOK_ASSIGN  '='
#define TOK_COMMA  ','
#define TOK_DOLLAR  '$'
#define TOK_LPAREN  '('
#define TOK_RPAREN  ')'
#define TOK_EOF     '\0'
#define TOK_NEWLINE '\n'

/***
 * Helpers
 */

// array append
char** array_append(char** a, char* s, int len) {
  char **p = malloc(sizeof(char*) * (len));
  memmove(p, a, sizeof(char*) * len);
  p[len] = s;
  return p;
}

// string split
char** str_split(char* s, char* delim) {
  // convert char* to char[]
  char str[strlen(s)];
  strcpy(str, s);

  int len = 0;
  char** ret = malloc(sizeof(char*) * strlen(s));
  char *token;

  // Go through the tokens and append to an array
  token = strtok(str, delim);
  while(token != NULL) {
    char* ns = malloc(sizeof(char*));
    strcpy(ns, token);
    ret = array_append(ret, ns, len);
    len += 1;
    token = strtok(NULL, delim);
  }

  return ret;
}

// string append
char* append(char* s, char c) {
  int len = strlen(s);

  char* p = malloc(sizeof(char) * (len + 2));
  strcpy(p, s);
  p[len] = c;
  p[len+1] = '\0';

  return p;
}

// word append
char* append_word(char* s, char* c) {
  int lens = strlen(s);
  int lenc = strlen(c);
  char* p = malloc(sizeof(char) * (lens + lenc + 1));
  strcpy(p, s);
  strcat(p, c);
  return p;
}

// string trim whitespace
void trim(char* s) {
  char *p = s;
  int len = strlen(p);

  while(isspace(p[len - 1])) {
    p[--len] = 0;
  }

  while(*p && isspace(*p)) {
    ++p;
    --len;
  }

  memmove(s, p, len + 1);
}

/***********
 *
 *   File
 *
 * *********/

File* open_file(char* filename) {
  File* f = malloc(sizeof(File));

  f->fp = fopen(filename, "rb");
  if (!f->fp) {
    fprintf(stderr, "error: unable to open file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  fseek(f->fp, 0L, SEEK_END);
  f->size = ftell(f->fp);
  rewind(f->fp);

  // Allocation memory for the contents
  f->contents = calloc(1, f->size + 1);
  if (!f->contents) {
    fclose(f->fp);
    fprintf(stderr, "error: memory allocation failed reading file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  // Copy the file into the contents
  if (fread(f->contents, f->size, 1, f->fp) != 1) {
    fclose(f->fp);
    free(f->contents);
    fprintf(stderr, "error: failed reading file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  return f;
}

void close_file(File* f) {
  fclose(f->fp);
  free(f->contents);
  free(f);
}

/***********
 *
 *   Config
 *
 * *********/
Config* new_config() {
  Config* cfg = malloc(sizeof(Config));
  cfg->len = 0;
  cfg->size = 20;
  cfg->keys = malloc(sizeof(char*) * 20);
  cfg->values = malloc(sizeof(char*) * 20);

  return cfg;
}

char* cfg_get(Config* cfg, char* key) {
  for (int i = 0; i < cfg->len; i++) {
    if (strcmp(key, cfg->keys[i]) == 0) {
      return cfg->values[i];
    }
  }

  // If nothing is found return "\0"
  return "\0";
}

void cfg_put(Config* cfg, char* k, char* v) {
  // If the output of cfg_get with the new k is "\0", then it is not a duplicate
  if (strcmp(cfg_get(cfg, k), "\0") == 0) {
    if (cfg->len == cfg->size) {
      cfg->size += 20;
      cfg->keys = (char**)realloc(cfg->keys, sizeof(char*) * cfg->size);
      cfg->values = (char**)realloc(cfg->values, sizeof(char*) * cfg->size);
    }
    cfg->len += 1;
    cfg->keys[cfg->len - 1] = k;
    cfg->values[cfg->len - 1] = v;
  } else {
    fprintf(stderr, "error: duplicate config entry %s\n", k);
    exit(EXIT_FAILURE);
  }
}

/***********
 *
 *   Parser
 *
 * *********/
void parser_read_char(Parser* p) {
  if (p->readPos >= (int)strlen(p->input)) {
    p->cur = '\0';
  } else {
    p->cur = p->input[p->readPos];
  }
  p->pos = p->readPos;
  p->readPos += 1;
}

void parser_skip_newline(Parser* p) {
  if (p->cur == '\n') {
    parser_read_char(p);
  }
}

void parser_parse_comment(Parser* p) {
  while (p->cur != '\n' && p->cur != '\0') {
    parser_read_char(p);
  }
}

char* parser_read_var_key(Parser* p) {
  char* key = malloc(sizeof(char));
  while (p->cur != TOK_RPAREN) {
    key = append(key, p->cur);
    parser_read_char(p);
  }
  return key;
}

void parser_parse_assignment(Parser* p, Config* cfg) {
  char* key = "";
  char* val = "";

  // Get key
  while(p->cur != TOK_ASSIGN) {
    key = append(key, p->cur);
    parser_read_char(p);
  }
  trim(key);

  // Skip past '=' sign
  parser_read_char(p);

  // Get value
  while(p->cur != TOK_NEWLINE && p->cur != TOK_EOF && p->cur != TOK_COMMENT) {
    if (p->cur == TOK_DOLLAR && p->input[p->readPos] == TOK_LPAREN) {
      parser_read_char(p);
      parser_read_char(p);
      char* var_key = parser_read_var_key(p);
      char* var_val = cfg_get(cfg, var_key);
      if (var_val[0] == '\0') {
        // Variable not found, exit
        printf("Error: key \"%s\" not found\n", var_key);
        exit(EXIT_FAILURE);
      }
      val = append_word(val, var_val);
    } else {
      val = append(val, p->cur);
    }
    parser_read_char(p);
  }
  trim(val);

  cfg_put(cfg, key, val);
}

Parser* new_parser(char* input) {
  Parser* p = malloc(sizeof(Parser));
  p->input = input;
  p->pos = 0;
  p->readPos = 0;
  p->cur = *input;
  parser_read_char(p);

  return p;
}

// Parse the config file and return the result
Config* cfg_parse(char* filename) {
  File* f = open_file(filename);
  Parser* p = new_parser(f->contents);
  Config* cfg = new_config();

  while(p->cur != TOK_EOF) {
    switch (p->cur) {
      case (TOK_COMMENT):
        parser_parse_comment(p);
        break;
      case (TOK_NEWLINE):
        parser_skip_newline(p);
        break;
      default:
        parser_parse_assignment(p, cfg);
    }
  }

  free(p);
  close_file(f);
  return cfg;
}

// Cleanup Config memory
void cfg_cleanup(Config* cfg) {
  free(cfg->keys);
  free(cfg->values);
  free(cfg);
}
