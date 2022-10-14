#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUFF 500

int main()
{
	char* input;
	char *split=" \n";
	char *d;
		
	input = readline ("myshell:$ ");
	while(1)
	{	
		int bgflag=0;
		if(!strcmp(input,"exit"))
		{
			break;
		}
		
		if(strlen(input)!=0)
		{
			add_history(input);
		}
		
		if(strchr(input,'&'))
		{	bgflag=1;
			int index=0;
			for(int i=0;i<strlen(input);i++)
			{
				if(input[i]=='&')
				{
					index=i;
				}
			}
			char *newinput;
			strncpy(newinput,input,index-1);
			//printf("%s\n",newinput);
		}

		d= strtok(input,split);
		if(!strcmp(d,"export"))
		{
			split = "PATH=$PATH";
			d = strtok(NULL,split);
			//printf("%s\n", d);
			setenv("PATH",d,1);

			printf("PATH: %s\n",getenv("PATH"));
		}
		else if(!strcmp(d,"echo"))
		{
			split = " \n";
			d = strtok(NULL,split);
			if(!strcmp(d,"$PATH"))
			{
				char* env;
				env = getenv("PATH");
				printf("PATH: %s\n",env);
			}
			else
			{
				printf("%s\n",d);
			}
		}
		else if(!strcmp(d,"pwd"))
		{
			char buf[100];
			printf("%s\n",getcwd(buf,sizeof(buf)));
		}
		else if(!strcmp(d,"cd"))
		{
			split = "\n";
			d = strtok(NULL,split);
			chdir(d);
		}
		else 
		{
			pid_t pid;
			pid = fork();
			if(pid<0)
			{
				printf("fork error\n");
			}
			else if(pid ==0)
			{
				
				char *cmd = d;
				char *argv[3];
				argv[0]=d;
				
				d = strtok(NULL,split);
				argv[1]=d;
				argv[2]=NULL;
				//printf("-%s-%s-\n",cmd,argv[1]);
				execvp(cmd,argv);
				
			}
			else
			{
				if(bgflag==0)
				{
					wait(NULL);
				}
			}	
		}
		input="\0";
		d="\0";
		input = readline ("myshell:$ ");
	}
}
