/*

        Name: Nils Erik Persson
        Onyen: npersson
        PID: 720462120
        Honor Code: I certify that no unauthorized assistance has been received or given in the completion of this work

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 256

//function to parse the input line into an array of arrays
void parse(char *input, char **output){
	//loop until the null terminator is reached
	while(*input != '\0'){
		//loop while white space is encountered and replace it with null terminator
		while(*input == '\n' || *input == '\t' || *input == ' '){
			*input++ = '\0';	
		}
		//if the read char is not null terminator then put input line into array
		if(*input != '\0'){
			*output++ = input;
			//increment until the next whitespace or terminator is reached
			while(*input != '\0' && *input != '\n' && *input != '\t' && *input != ' '){
				input++;
			}	
		}
	}
	//terminate the array of arrays
	*output = NULL;
}

int main(){
        char line[BUFFER_SIZE];
	char *parsedLine[BUFFER_SIZE];
	pid_t pid;

	//print shell prompt for output
	printf("%% ");

	//start loop that takes in user input from stdin 
	//input stored in line with max size of BUFFER_SIZE
	//loop stops when null is reached (hopefully due to eof)
        while(fgets(line, BUFFER_SIZE, stdin) != NULL){

		//determine if fgets truncated user input before inserting into line
		size_t lenLine;
		lenLine = strlen(line);
		if(line[lenLine - 1] == '\n'){
			//parse the input line
			parse(line, parsedLine);
			
			//fork the process to make a child process (child process will have pid of 0)
			pid = fork();

			//if pid is less than 0 then an error has occured
			if(pid < 0){
				fprintf(stderr, "error occured while calling fork()\n");
				exit(1);
			}
			//if pid is equal to 0 then this code is executing in the child process
			else if(pid == 0){
				//execute the given program from the child process
				int execState = execvp(parsedLine[0], parsedLine);
				//if the returned int is negative an error occured
				if(execState < 0){
					fprintf(stderr, "error occured while calling execvp()\n");
					exit(1);
				}			
			}
			//otherwise the code is executing in the parent process
			else{
				wait(NULL);
			}
		}
		//if truncation occured, print message and skip rest of line
		else{
			char c;
			while((c = getchar()) != '\n' && c != EOF){};
			fprintf(stderr, "user input exceeded buffer size of %i (including \\n and \\0)\n", BUFFER_SIZE);
		}
	//print shell prompt for output
	printf("%% ");
        }

	printf("\n");

	//make sure that loop stopped due to eof reached
	if(feof(stdin)){
		//do nothing since we exited for the correct reasons (EOF)
	}
	else{
		//exiting badly
		fprintf(stderr, "exiting without reaching EOF\n");
	}

        return 0;
}

