#include "stdio.h"
#include "web.h"

int main(int argc, char **argv, char **env){

	web_print_header();

	while(*env){
		printf("%s<br>", *env);
		env += 1;
	}

	return 0;

}//main*/

