#include <stdio.h>
#include <stdlib.h>

#include "load_file.h"

char* load_file(const char* file_path) {
  FILE* fp;
  #ifdef WIN32
  fopen_s(&fp, file_path, "rb");
  #else
  fp = fopen(file_path, "rb");
  #endif

  if (fp == NULL) {
    fprintf(stderr, "Could not open file '%s'.\n", file_path);
    exit(EXIT_FAILURE);
  }

  fseek(fp, 0, SEEK_END);
  size_t length = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char* source = malloc((length + 1) * sizeof(char));

  if (fread(source, 1, length, fp) != length) {
    fprintf(stderr, "Could not read file '%s'.\n", file_path);
    fclose(fp);
    exit(EXIT_FAILURE);
  }

  fclose(fp);

  source[length] = '\0';

  return source;
}
