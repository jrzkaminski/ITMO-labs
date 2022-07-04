#include <ctype.h>
#include <omp.h>
#include <stdio.h>
#include <string.h>


// function to define if the character is a space
int is_space(char inp){ 
  if (inp == ' ' || inp == '\n' || inp == '\t')
  return 1;
  return 0;
}

// function to count words 
int main(int argc, char *argv[]) {
  int i = 0;
  int answer = 1;
  char cur;
//#pragma omp parallel for
	for (i = 0; i < strlen(argv[1]); i++) {
//#pragma omp atomic update
  	answer += (is_space(argv[1][i]) > 0);
	 }

  printf("Number of words in %s = %d\n", argv[1], answer);
}
