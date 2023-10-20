#include "kernel/types.h"
#include "user/user.h"
#include <stddef.h>
#define PIPE_READ 0
#define PIPE_WRITE 1
const int N = 35;

void dfs(int pipe_left [])
{
    int cur_primes;
    if(read(pipe_left[PIPE_READ], &cur_primes, sizeof (cur_primes)) == 0)
    {
        exit(0);
    }
    printf("prime %d\n", cur_primes);
    int pipe_right[2];
    if(pipe(pipe_right) == -1)
    {
        printf("fail open pipe!\n");
        exit(-1);
    }
    int rc = fork();
    if(rc == 0)
    {
        close(pipe_right[PIPE_WRITE]);
        dfs(pipe_right);
    }
    else 
    {
        close(pipe_right[PIPE_READ]);
        int i;
        while(read(pipe_left[PIPE_READ], &i, sizeof(i)) != 0)
        {
            if(i % cur_primes != 0)
            {
                write(pipe_right[PIPE_WRITE], &i, sizeof(i));
            }
        }
        close(pipe_right[PIPE_WRITE]);
        wait(0);
        exit(0);
    }


}

int main(int argc, char *argv[])
{
    int pipe_left[2];
    if (pipe(pipe_left) == -1)
    {
        printf("fail open pipe!\n");
        exit(-1);
    }
    int rc = fork();
    if (rc < 0)
    {
        printf("fail create process!\n");
        exit(-1);
    }
    else if (rc == 0)
    {
        close(pipe_left[PIPE_WRITE]);
        dfs(pipe_left);
        // wait(0);
    }
    else
    {
        close(pipe_left[PIPE_READ]);
        for(int i = 2; i <= N; i++)
            write(pipe_left[PIPE_WRITE], &i, sizeof (int));
        close(pipe_left[PIPE_WRITE]);
        wait(0);
    }
    exit(0);
}