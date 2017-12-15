#include <stdio.h>
#include <ctype.h>



int main (int argc, char* argv[]){
	char c;
	while ((c = getchar()) != EOF) {
		putchar(toupper(c));
	}
	return 0;
}
