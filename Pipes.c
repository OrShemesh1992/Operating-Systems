#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "md5.h"
using std::cout; using std::endl;
#define MSGSIZE 20


int main(int argc, char const *argv[]) {
        char buffer[MSGSIZE];
        int pid;
        int fd[2];
        pipe(fd);
        //בדיקה
        pid=fork();

        if(pid>0) {
                // parent
                close(fd[0]);
                printf("Give me String to send to Child!\n");
                scanf("%20s", buffer);
                write(fd[1], buffer, 20);
                close(fd[1]);
                sleep(2);

                  char buffer32[32];

                close(fd[1]);
                read(fd[0], buffer32, 32);
                 printf("Child(%d) received value: %s\n", getpid(), buffer32);
                close(fd[0]);
        }else{
                //child
                close(fd[1]);
                read(fd[0], buffer, 20);
                cout << "md5 of 'grape': " << buffer << endl;
                cout << "md5 of 'grape': " << md5(buffer) << endl;
                std::string str= md5(buffer);
                char answer[str.length() + 1];
                strcpy(answer, str.c_str());
                write(fd[1],answer,strlen(answer) + 1);

                //printf("Child(%d) received value: %s\n", getpid(), buffer);
                //printf("%d\n",pid );
                close(fd[0]);
        }
        return 0;
}





using std::cout; using std::endl;
//
// int main(int argc, char *argv[])
// {
//     cout << "md5 of 'grape': " << md5("grape") << endl;
//     return 0;
// }
