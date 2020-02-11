#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main ( int argc, const char* argv[]) {
	
	//char
	if ( argc > 2 ){
		printf("too many args \n");
	}	
	 
	do {

			printf("smash> ");
	}			while ( (!(strcmp(argv[0],  "exit"))) || !(strcmp(argv[1],  "exit")) );


	return 0;
}





