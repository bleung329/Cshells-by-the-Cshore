#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

//All I can say: Its dirty and recurses

int execfriend(char** cmd_line)
{
	char** cmd;
	char** cmdrest;
	int cmdlength, cmdrestlength;
	for (cmdlength = 0; cmd_line[cmdlength]; cmdlength+=1){}
	//printf("cmdlength is %d\n", cmdlength );
	for (int i = 0; cmd_line[i]; i++)
	{
		if (strcmp(cmd_line[i],";")==0)
		{
			cmdrestlength = cmdlength-i-1;
			//printf("cmdrestlength is %d\n",cmdrestlength);
			//printf("i is %d\n",i );
			cmd = malloc(i*8);
			cmdrest = malloc(cmdrestlength*8);
			memcpy(cmd, cmd_line, i*8);
			memcpy(cmdrest, cmd_line+i+1, cmdrestlength*8);
			int j = 0;
		  	/*while(cmd[j])
		  	{
		  		printf("%s\n",cmd[j]);
		  		j++;
		  	}
		  	j = 0;
		  	while(cmdrest[j])
		  	{
		  		printf("%s\n",cmdrest[j]);
		  		j++;
		  	}*/
		  	if (fork() == 0)
		  	{
		  		execvp(cmd[0],cmd);
		  		exit(0);
		  	}
		  	else
		  	{
		  		wait(NULL);
		  		free(cmd);
		  		execfriend(cmdrest);
		  	}
		  	return 0;
		}
	}
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

int main(int argc, char **argv)
{

	// Run command loop.
	while (1)
	{
		printf(">");
		cshell();

	}


	// Perform any shutdown/cleanup.

	return 0;
}
