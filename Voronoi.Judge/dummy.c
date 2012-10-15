#include <stdio.h>

#define MAXTMPARRLEN 1000

int main() {
  char tmpstr[MAXTMPARRLEN];
  int x, y;
  setvbuf(stdin, NULL, _IONBF, 0); 
  setvbuf(stdout, NULL, _IONBF, 0);

  while (1) {
    if (fgets(tmpstr, MAXTMPARRLEN, stdin) != NULL) {
      sscanf(tmpstr, "%d,%d", &x, &y);
      printf("%d,%d \n", x + 10, y + 10);
    } else {
      break;
    }
  }
  return 0;
}
