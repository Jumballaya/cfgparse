/**
 * Parser Entry
 */

#include <stdio.h>
#include <stdlib.h>
#include "include/parser.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: cfg <file>\n");
    return 1;
  }

  Config* cfg = cfg_parse(argv[1]);

  for (int i = 0; i < cfg->len; i++) {
    printf("%s = %s\n", cfg->keys[i], cfg->values[i]);
  }

  cfg_cleanup(cfg);

  return 0;
}
