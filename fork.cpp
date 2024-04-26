#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<cstring>

using namespace std;

int main(int counter, char* args[]) 
{

	int num = stoi(args[1]);
	if (num <= 0) 
	{
		cout << "Enter a valid  number.\n";
		return 0;
	}

	int fd[2];

	if (pipe(fd) < 0) 
	{
		perror("pipe failed");
		exit(1);
	}

	while (true) 
	{
		pid_t id = fork();
		if (id == 0) 
		{	//child process

			if (read(fd[0], &num, sizeof(int)) < 0) 
			{
				perror("Failed to read in child.\n");
				exit(1);
			}

			if (num % 2 == 0)
			{
				num = num / 2;
			}
			else 
			{
				num = (3 * num) + 1;
			}

			if (write(fd[1], &num, sizeof(int)) < 0) 
			{
				perror("Failed to write in child.\n");
				exit(1);
			}
			close(fd[1]);
			close(fd[0]);
			exit(0);
		}

		else if (id < 0) 
		{
			perror("Fork failed.\n");
			exit(1);
		}

		else
		{	//parent process

			if (write(fd[1], &num, sizeof(int)) < 0) 
			{
				perror("Write in parent failed.\n");
				exit(1);
			}

			pid_t child = wait(NULL);

			if (read(fd[0], &num, sizeof(int)) < 0) 
			{
				perror("Read in parent failed.\n");
				exit(1);
			}

			cout << "NUMBER: " << num << endl;
			if (num == 1) 
			{
				break;
			}
		}
	}

	close(fd[1]);
	close(fd[0]);
	return 0;

}
