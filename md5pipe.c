#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "md5.h"
using std::cout; using std::endl;
#define MSGSIZE 20
#define MSGSIZE32 10
int main()
{


        int fd1[2];
        int fd2[2];

        char buffer[MSGSIZE];
        char buffer32[MSGSIZE32];
        int pid;
        //check pipes 1
        if (pipe(fd1)==-1)
        {
                fprintf(stderr, "Pipe Failed" );
                return 1;
        }
        //check pipes 2
        if (pipe(fd2)==-1)
        {
                fprintf(stderr, "Pipe Failed" );
                return 1;
        }

        pid = fork();
        //check fork
        if (pid < 0)
        {
                fprintf(stderr, "fork Failed" );
                return 1;
        }

        // Parent process
        else if (pid > 0)
        {
                close(fd1[0]); // Close reading end of first pipe
                printf("plain text: ");
                scanf("%20s", buffer);
                // Write input string and close writing end of first
                // pipe.
                write(fd1[1], buffer, 20);
                close(fd1[1]);

                // Wait for child to send a string
                wait(NULL);

                close(fd2[1]); // Close writing end of second pipe

                // Read string from child, print it and close
                // reading end.

                read(fd2[0], buffer32, 32);

                close(fd2[0]);

                if(strlen(buffer32)==32) {
                  printf("encrypted by process %d : %s\n",getpid(),buffer32);
                        kill(pid, SIGKILL);
                }else{
                        exit(0);
                }
        }

        // child process
        else
        {
                close(fd1[1]); // Close writing end of first pipe

                // Read a string using first pipe
                read(fd1[0], buffer, 20);
                std::string str= md5(buffer);

                // Close both reading ends
                close(fd1[0]);
                close(fd2[0]);
                // Write concatenated string and close writing end
                write(fd2[1], str.c_str(), 32);
                close(fd2[1]);


                exit(0);
        }
        return 0;
}
