#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

#define BACKGROUND 1
#define INPUT 2
#define OUTPUT 3
#define READ_BUFSIZE 1024

int mode;

typedef void Sigfunc(int);

Sigfunc *install_signal_handler(int signo, Sigfunc *handler)
{
	struct sigaction act, oact;

	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	if( signo == SIGALRM ) {
#ifdef SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;  /* SunOS 4.x */
#endif
	} else {
#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART;  /* SVR4, 4.4BSD */
#endif
	}
	if( sigaction(signo, &act, &oact) < 0 )
		return (SIG_ERR);

	return(oact.sa_handler);
}


void handle_SIGINT()
{
  printf("got an interupt!");
}
void handle_SIGTSTP()
{	
	printf("\nFound your CTRL-Z \n");
  	signal(SIGINT, SIG_IGN);
	install_signal_handler(SIGTSTP, handle_SIGTSTP);
	install_signal_handler(SIGTSTP, handle_SIGTSTP);
	signal(SIGTSTP, handle_SIGTSTP);
	signal(SIGTSTP, SIG_DFL); 
	return;
}
  
void handle_SIGQUIT()
{
  printf("got an interrupt\n");
 	//kill;
}


char *read_line()
{
	int read_buffer = READ_BUFSIZE;
			int position = 0;
			char *line = malloc(sizeof(char)*read_buffer);
			int c;
			if(!line)
			{
				fprintf(stderr,"allocation error\n");
				exit(EXIT_FAILURE);
			}
			while(1)
			{
				c = getchar();
				if(c == EOF || c == '\n')
				{
					line[position] = '\0';
					return line;
				}
				else
					line[position] = c;
				position++;
			}
}

void split_line(char *r_char,char **args)
{
	while(*r_char != '\0')
	{
		while(*r_char == ' ' || *r_char == '\t' || *r_char == '\n'|| *r_char == '\r'|| *r_char == '\a')
		{
			*r_char++ = '\0';
		}
		if(*r_char == '&')
			mode = BACKGROUND;
		else if(*r_char == '<')
			mode = INPUT;
		else if(*r_char == '>')
			mode = OUTPUT;
		else{
			*args++ = r_char;
		}
		while (*r_char != '\0' && *r_char != ' ' && *r_char != '\t' && *r_char != '\n')
		{
			r_char++;
		}
	}
	*args = '\0';
}
void int_handler(int sig)
{
  printf("!!Got interrupt!!\n");
  exit(2);
}
			
int main(int argc,char*argv[])
{
	
	char *line;
	char *args[64];
	while(1)
	{
			printf("minish>");
			line = read_line();
			split_line(line,args);
			//printf("\n arg[0] %s", args[0]);
			if(strcmp(args[0],"cd")==0)
			{	
				if (args[1] == NULL) 
				{
					fprintf(stderr, "expected argument to \"cd\"\n");
				} 
				else 
				{
					if (chdir(args[1]) != 0) 
					{
					perror("");
					}
				
				}
			}
			else if(strcmp(args[0],"exit") == 0)
			{
				exit(0);
			}
			else
			{
				pid_t pid;
				int status;
				pid = fork();/*creating a child*/
				if(pid < 0)
				{
					printf("fork system call failure\n");
					exit(0);
				}
				else if(pid == 0)
				{
					if(mode == INPUT)
					{
						char line[1024];
						FILE *input = fopen(args[1], O_RDONLY);
						while (fgets(line, sizeof(line), input) != NULL)
							if(dup2((intptr_t)input,STDIN_FILENO)==-1)
							{
								perror(" ");
							}
                        line[strlen(line)] = '\0'; 
						fclose(input);
						split_line(line, args);
					}
					if(mode == OUTPUT)
					{
						int fd = open(args[1], O_CREAT | O_WRONLY, 0644);
						dup2(fd, 1);
						if(dup2(fd,STDIN_FILENO)== -1)
						{
							perror(" ");
						}
						*args[1] = '\0';
					}

					if(execvp(*args,args)<0)
					{
						printf("!!!!command not found!!!!\n");
						exit(1);
					}
				}
				else
				{
					int status;
					if(mode == BACKGROUND)
					{
                    printf("Process %d in background mode\n", pid);
                    waitpid(-1, &status, WNOHANG);
					install_signal_handler(SIGTSTP, handle_SIGTSTP);
					signal(SIGTSTP, handle_SIGTSTP);
					signal(SIGTSTP, SIG_DFL); 
					}
					else
					{	
                    while (wait(&status) != pid);
					install_signal_handler(SIGTSTP, handle_SIGTSTP);
					signal(SIGTSTP, handle_SIGTSTP);
					signal(SIGTSTP, SIG_DFL); 
					}
			    }
			}
	}
}
	
  