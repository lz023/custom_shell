#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<cstring>
#include<vector>
#include<fcntl.h>
using namespace std;
int main()
{
    string input;
    bool waitFg = 1;
    while (true)
    {
        cout << ">$ ";
        getline(cin, input);
        waitFg = 1;
        int lastIndex = input.size() - 1;
        if (input[lastIndex] == '&')
        {
            waitFg = 0;
            input.pop_back();
        }
        if (input != "exit")
        {
            int count = 0;
            int pos = 0;

            while ((pos = input.find_first_not_of(' ', pos)) != std::string::npos)
            {
                count++;
                pos = input.find(' ', pos);

            }



            char** command = new char* [count + 1];
            int index = 0;
            pos = 0;
            int wc = 0;

            while (wc < count)
            {

                pos = input.find_first_not_of(' ', pos);
                int next = input.find(' ', pos);
                if (wc == count - 1)
                {
                    next = input.length();
                }

                command[index] = new char[next - pos + 1];
                input.copy(command[index], next - pos, pos);
                command[index][next - pos] = '\0';
                index++;
                pos = next;
                wc++;

            }
            command[count] = nullptr;
            int i = 0, j = -1, k = -1;
            while (command[i])
            {

                if (strcmp(command[i], "<") == 0)
                {

                    j = i + 1;
                    command[i] = nullptr;
                }
                else if (strcmp(command[i], ">") == 0)
                {
                    k = i + 1;
                    command[i] = nullptr;
                }
                i++;

            }
            pid_t id = fork();
            if (id == 0)
            {

                if (j != -1)
                  {

                    int readfd = open(command[j], O_RDONLY);
                    if (readfd < 0) 
                    {

                        perror("Failed to open input file");
                        exit(1);
                    }

                    dup2(readfd, 0);
                    close(readfd);

                }

                if (k != -1)
                {

                    int writefd = open(command[k], O_WRONLY | O_CREAT | O_APPEND, 0666);
                    if (writefd < 0)
                    {

                        perror("Failed to open output file");
                        exit(1);
                    }

                    dup2(writefd, 1);
                    close(writefd);

                }
                execvp(command[0], command);
                perror("execvp failed");
                exit(1);

            }

            else if (id < 0)
            {
                perror("fork failed\n");
                exit(1);

            }

            else
            {
                if (waitFg)
                {
                    wait(NULL);

                }

            }

        }

        else
        {
            break;
        }

    }
    return 0;

}

