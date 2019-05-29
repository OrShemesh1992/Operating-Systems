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
int _index;
int arr[5];

void sigCather(int sig_num)
{
        signal(SIGTERM,sigCather);
        printf("PID %d is caught one\n",getpid());
        if(_index >-1) {
                kill(arr[_index],SIGTERM);
        }
}

int main()
{
        int pid;
        int zombi;
        int state;

        signal(SIGTERM, sigCather);

        for (int i = 0; i < 5; i++)
        {
                pid = fork();
                if(pid == 0) {
                        //chlid Process
                        printf("PID %d is ready \n", getpid());
                        _index = i-1;
                        pause(); // wait for singal
                        exit(0); // become a zombi
                }
                // Parent Process
                arr[i] = pid;
        }
        sleep(1);
        kill(arr[4],SIGTERM);
        sleep(1);

        for (int i = 0; i < 5; i++)
        {
                zombi = wait(&state);
                printf("Process %d is dead\n",zombi);
                kill(zombi,SIGKILL);
        }


        return 0;
}
