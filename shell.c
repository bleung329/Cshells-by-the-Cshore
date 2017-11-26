#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
char** parse_args(char* yeah)
{
	//Init copy of argstring and array to store it.
	char * yeahcpy = (char *)malloc(sizeof(yeah));
	strcpy(yeahcpy,yeah);
	char **arr = (char**) malloc(sizeof *arr * 8); 

	if (arr)
	{
	  //The i is just to help me keep track of the strings.
	  int i = 0;
	  while(yeahcpy!=NULL)
	  {
	  	//Copy the strsepped string into the array.
	    arr[i] = malloc(sizeof *arr[i] * 8);
	    //printf("yeahcpy is %s\n",yeahcpy);
	    strcpy(arr[i], strsep(&yeahcpy," ")); 
	    //printf("arr[%d] is %s\n",i,arr[i]);
	    i++;
	  }
	}
	//Free the copied string.
	free(yeahcpy);
	return arr;

}

int cshell()
{
	printf(">");
	char* rawcmd = malloc(256);
	fgets(rawcmd,256,stdin);
	int len = strlen(rawcmd);
	//This gets rid of that pesky newline
	if (len > 0 && rawcmd[len-1] == '\n') {
	    rawcmd[len-1] = '\0';
	}
	char** cmd = parse_args(rawcmd);
	if (fork() == 0)
	{
		execvp(cmd[0],cmd);
	}
	return 0;

}

int main(int argc, char **argv)
{
	  // Load config files, if any.

	  // Run command loop.
	while (true)
	{
		cshell();

	}
		


	// Perform any shutdown/cleanup.

	return 0;
}
