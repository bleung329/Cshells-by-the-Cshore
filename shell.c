#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
/*
Arguments: 
    char* argstring
What it Does: 
    It will return a char**. All the elements in this char** are just the elements of argstring, split by a space.
*/
char** parse_args(char* argstring)
{
	//Init copy of argstring and array to store it.
	char * argstringcpy = (char *)malloc(sizeof(argstring));
	strcpy(argstringcpy,argstring);
	char **arr = (char**) malloc(sizeof *arr * 8); 

	if (arr)
	{
	  //The i is just to help me keep track of the strings.
	  int i = 0;
	  while(argstringcpy!=NULL)
	  {
	  	//Copy the strsepped string into the array.
	    arr[i] = malloc(sizeof *arr[i] * 8);
	    //printf("argstringcpy is %s\n",argstringcpy);
	    strcpy(arr[i], strsep(&argstringcpy," ")); 
	    //printf("arr[%d] is %s\n",i,arr[i]);
	    i++;
	  }
	}
	//Free the copied string.
	free(argstringcpy);
	return arr;

}

/*
Arguments: 
    char** cmd_line
What it Does: 
    It helps out with executing commands that are split by semicolons.
    It makes 2 arrays out of an arbitrary list of commands split by ;.
    1 of those arrays is the 1st command and the 2nd is an array of 
    the rest of the commands, which may or may not have semicolons. 
    It executes whatever is in the first array and calls itself 
    again on the 2nd.
Note:
    oooo recursion
*/
int execfriend(char** cmd_line)
{
	char** cmd;
	char** cmdrest;
	int totalcmdlength, cmdrestlength;
    //Is there a better way to get length of an array of strings?
	for (totalcmdlength = 0; cmd_line[totalcmdlength]; totalcmdlength+=1){}
	printf("total length = %d\n", totalcmdlength);
	//onedirection(cmd_line);
	int fd = open("r.txt", O_WRONLY);
	printf("fd = %d\n",fd);
	int newfd = dup(fd);
	printf("dupfd = %d\n",newfd);

	for (int i = 0; cmd_line[i]; i++)
	{
		if (strcmp(cmd_line[i],";")==0)
		{
			cmdrestlength = totalcmdlength-i-1;
			cmd = malloc(i*8);
			cmdrest = malloc(cmdrestlength*8);
			memcpy(cmd, cmd_line, i*8);
			memcpy(cmdrest, cmd_line+i+1, cmdrestlength*8);
			int j = 0;
		  	if (fork() == 0)
		  	{
		  		execvp(cmd[0],cmd);
		  		exit(0);
		  	}
		  	else
		  	{
		  		wait(NULL);
                printf("\n");
		  		free(cmd);
		  		execfriend(cmdrest);
		  	}
		  	return 0;
		}
	}
    //Base case
	if (fork() == 0)
  	{
  		execvp(cmd_line[0],cmd_line);
  		exit(0);
  	}
  	else
  	{
  		wait(NULL);
  	}
  	return 0;
}

int onedirection(char** cmd_line,int totalcmdlength)
{
	if (strcmp(cmd_line[totalcmdlength-2],">")==0)
	{
		int fd = open(cmd_line[totalcmdlength-1],)
	}
}

/*
Arguments:
    (none)
What it Does:
    It's essentially the loop of the shell. It gets the input from 
    the user and processes it. It's also where the cd lives.
*/
int cshell()
{
	pid_t childpid;
	char* rawcmd = malloc(256);
	fgets(rawcmd,256,stdin);
	int len = strlen(rawcmd);
	//This gets rid of that pesky newline
	if (len > 0 && rawcmd[len-1] == '\n') 
	{
	    rawcmd[len-1] = '\0';
	}
	char** cmd = parse_args(rawcmd);
	
	//For cd
	if (strcmp(cmd[0],"cd")==0)
	{
		chdir(cmd[1]);
	}
		//childpid = getpid();
		//printf("%d\n",childpid);
	execfriend(cmd);

	return 0;

}


/*
Arguments:
    (none)
What it Does:
    It's the main.
*/
int main()
{

	// Run command loop.
	while (1)
	{
		printf(">");
		cshell();

	}


	return 0;
}
