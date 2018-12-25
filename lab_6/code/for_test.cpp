#include <iostream>


int main(int argc, char *argv[]) {
  

  FILE* fd = fopen(argv[1], "w");
  fprintf(fd, "%s\n", argv[2]);
  for(int i=0; i<100000; ++i) {

  	fprintf(fd, "%d\n%d\n", 1 , 2);
  	fprintf(fd, "%d\n%d\n", 2 , 1);
  	
  }  

  return 0;
}
