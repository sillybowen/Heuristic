#include <stdio.h>

#define MAXTMPARRLEN 1000

int main(int argc, char* argv[]) {
  if (argc < 2) {
    puts("Usage: ./a.out TEAM_NAME");
    return 1;
  }
  char tmpstr[MAXTMPARRLEN];
  int x, y;
  setvbuf(stdin, NULL, _IONBF, 0); 
  setvbuf(stdout, NULL, _IONBF, 0);

  printf("%s\n", argv[1]);

  while (1) {
    // sleep(5);

    if (fgets(tmpstr, MAXTMPARRLEN, stdin) != NULL) {
      sscanf(tmpstr, "%d,%d", &x, &y);
      printf("%d,%d \n", x + 10, y + 10);
    } else {
      break;
    }
  }
  return 0;
}
