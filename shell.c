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
	    /*if (!(strcmp(arr[i],"\n")))
	    {
	    	printf("Oh no\n");
	    }*/
	    i++;
	  }
	}
	//Free the copied string.
	free(yeahcpy);
	return arr;

}

int cshell()
{
	//char* string = '\n';
	//printf("%d\n", string);
	char* rawcmd;
	rawcmd = malloc(256);
	fgets(rawcmd,256,stdin);
	int len;
	len = strlen(rawcmd);
	if (len > 0 && rawcmd[len-1] == '\n') {
	    rawcmd[len-1] = '\0';
	}
	char** cmd;
	cmd = malloc(256);
	cmd = parse_args(rawcmd);
	execvp(cmd[0],cmd);
	//printf("%s\n", cmd[0]);
	//printf("%s\n", cmd);
	printf(">");
	return 0;

}

int main(int argc, char **argv)
{
	  // Load config files, if any.

	  // Run command loop.

	cshell();
		


	// Perform any shutdown/cleanup.

	return 0;
}
