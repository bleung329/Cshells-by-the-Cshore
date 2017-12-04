#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define READ 0
#define WRITE 1

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
void execfriend(char** cmd_line)
{
	char** cmd;
	char** cmdrest;
	int totalcmdlength, cmdrestlength;
	int i;
	for (totalcmdlength = 0; cmd_line[totalcmdlength]; totalcmdlength+=1){}
	for (i = 0; cmd_line[i]; i++)
	{
		if (strcmp(cmd_line[i],";")==0)
		{
			cmdrestlength = totalcmdlength-i-1;
			cmd = malloc(i*8);
			cmdrest = malloc(cmdrestlength*8);
			memcpy(cmd, cmd_line, i*8);
			memcpy(cmdrest, cmd_line+i+1, cmdrestlength*8);
		  	if (fork() == 0)
		  	{
		  		execfriend(cmd);
		  		exit(0);
		  	}
		  	else
		  	{
		  		wait(NULL);
		  		execfriend(cmdrest);
		  	}
			free(cmd);
			free(cmdrest);
		  	return;
		}
	}

	for (i = 0; cmd_line[i]; i++)
	{
		if (strcmp(cmd_line[i],"|")==0)
		{
			int fds[2];
			pipe(fds);
			cmdrestlength = totalcmdlength-i-1;
			cmd = malloc(i*8);
			cmdrest = malloc(cmdrestlength*8);
			memcpy(cmd, cmd_line, i*8);
			memcpy(cmdrest, cmd_line+i+1, cmdrestlength*8);
		  	if (fork() == 0)
		  	{
				close(fds[READ]);
				int stdout = dup(1);
				dup2(fds[WRITE], 1);
		  		execfriend(cmd);
		  		free(cmd);
				dup2(stdout, 1);
				close(fds[WRITE]);
		  		exit(0);
		  	}
		  	else
		  	{
		  		wait(NULL);
                close(fds[WRITE]);
				int stdin = dup(0);
				dup2(fds[READ], 0);
		  		execfriend(cmdrest);
				free(cmdrest);
				dup2(stdin, 0);
				close(fds[READ]);
		  	}
		  	return;
		}
	}

	if (totalcmdlength > 1 && strcmp(cmd_line[totalcmdlength-2],">")==0)
	{
		int stdout = dup(1);
		int fdout = open(cmd_line[totalcmdlength-1], O_WRONLY | O_CREAT, 0644);
		if(fdout == -1){
			printf("%s\n", strerror(errno));
			return;
		}
		dup2(fdout, 1);

		cmdrestlength = totalcmdlength-2;
		char** cmd = malloc(cmdrestlength*8);
		memcpy(cmd, cmd_line, cmdrestlength*8);
		// for(int i = 0; i < cmdrestlength; i++){
		// 	printf("cmd[%d]: %s\n", i, cmd[i]);
		// }
		if (fork() == 0)
		{
			execfriend(cmd);
			free(cmd);
			//printf("done with child\n");
			exit(0);
		}
		else
		{
			wait(NULL);
		}

		dup2(stdout, 1);
		close(fdout);
		return;
	}

	if (totalcmdlength > 1 && strcmp(cmd_line[totalcmdlength-2],"<")==0)
	{
		int stdin = dup(0);
		int fdin = open(cmd_line[totalcmdlength-1], O_RDONLY);
		if(fdin == -1){
			printf("%s\n", strerror(errno));
			return;
		}
		dup2(fdin, 0);

		cmdrestlength = totalcmdlength-2;
		char** cmd = malloc(cmdrestlength*8);
		memcpy(cmd, cmd_line, cmdrestlength*8);
		// for(int i = 0; i < cmdrestlength; i++){
		// 	printf("cmd[%d]: %s\n", i, cmd[i]);
		// }
		if (fork() == 0)
		{
			execfriend(cmd);
			free(cmd);
			//printf("done with child\n");
			exit(0);
		}
		else
		{
			wait(NULL);
		}

		dup2(stdin, 0);
		close(fdin);
		return;
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
  	return;
}

/*
Arguments:
    (none)
Return Value:
	1 if exited, 0 otherwise
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

	//For exit
	if (strcmp(cmd[0],"exit")==0)
	{
		return 1;
	}

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
Return Value:
	0
What it Does:
    It's the main.
*/
int main()
{

	// Run command loop.
	while (1)
	{
		printf(">");
		if(cshell()){
			break;
		}

	}


	return 0;
}
