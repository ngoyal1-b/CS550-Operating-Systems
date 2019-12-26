#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
int h,c;//global variables
//function declarations
void child_process(char *argv[]);
void start_instructions(char *argv[]);

int main(int argc,char *argv[])
{
	pid_t process_id;
	int status;
	/*atoi is used to convert string number to integers*/
 	h = atoi(argv[1]);
 	c = atoi(argv[2]);
	start_instructions(argv);
	if(h > 1)
		{
			while(c != 0)
			{
				process_id = fork();
				/*fork system call will generate a value if that value is positive it's parent process, if negative fork call failed and if it's zero it's child process*/
				if(process_id < 0)
				{
					printf("fork failed\n");
					exit(0);
				}
				else if(process_id > 0)
				{
					process_id = wait(&status);
					printf("(%d) Terminating at height %d\n",process_id,h);//child processss
				}
				else if(process_id == 0)
				{
					child_process(argv);
				}

			c--;
			}
		}
	printf("(%d) : Terminating at height %d\n", getpid(),h);//parent process

}
void start_instructions(char *argv[])
{
	h = atoi(argv[1]);
	c = atoi(argv[2]);
	printf(" (%d) : Process Starting\n ",getpid());
	printf("(%d) : Parent's Id = (%d)\n ",getpid(),getppid());
	printf("(%d) : Height in the tree = %d\n ",getpid(),h);
	printf("(%d) : Creating %d children at height %d\n",getpid(),c,h-1);
}
void child_process(char *argv[])
{
	h = atoi(argv[1]);
	c = atoi(argv[2]);
	char child[3],height[3];
	/*sprintf is converting integer value of c and h into string value to be used in exec command*/
	sprintf(child,"%d",c);
	sprintf(height,"%d",h-1);
	int ret_value = execlp(argv[0],argv[0],height,child,(char*) NULL);
	if(ret_value == -1)
		{
			printf("exec failed\n");
		}
	exit(0);
}

		
