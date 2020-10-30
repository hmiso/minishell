#include "minishel.h"
#include <string.h>
#define PIPE_READ 0
#define PIPE_WRITE 1


int  main(int argc, char *argv[])
{
    int pipefd[2];
    pid_t cpid;
    char *ls[] ={ "ls", "-la", NULL};
    char *cat[] ={ "cat", "-e", NULL};
    pipe(pipefd); // [PIPE_in][PIPE_out]
    cpid = fork();
    if (cpid == 0)
    {
        dup2(pipefd[PIPE_READ], 0);
        close(pipefd[PIPE_WRITE]);          /* Close unused write end */
        execve("/bin/cat", cat ,NULL);
        close(pipefd[PIPE_READ]);
        exit(EXIT_SUCCESS);
    } 
    else
    {            /* Parent writes argv[1] to pipe */
        dup2(pipefd[PIPE_WRITE], 1);
        close(pipefd[PIPE_READ]);          /* Close unused read end */
        execve("/bin/ls", ls ,NULL);
        close(pipefd[PIPE_WRITE]);          /* Reader will see EOF */
        wait(NULL);                /* Wait for child */
        exit(EXIT_SUCCESS);
    }
}