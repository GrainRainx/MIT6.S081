#include "kernel/types.h"
#include "user/user.h"
#include <stddef.h>
#define PIPE_READ 0
#define PIPE_WRITE 1
int main(int argc, char *argv[])
{
    int pipe_father_to_child[2], pipe_child_to_father[2];
    char buffer[256];
    if (pipe(pipe_child_to_father) == -1 || pipe(pipe_father_to_child) == -1)
    {
        printf("pipe create fail!\n");
        exit(-1);
    }
    int rc = fork();
    if (rc < 0)
    {
        printf("create fail!\n");
        exit(-1);
    }
    else if (rc == 0)
    {
        close(pipe_father_to_child[PIPE_WRITE]);
        read(pipe_father_to_child[PIPE_READ], buffer, sizeof(buffer));
        printf("%d: received %s\n", getpid(), buffer);
        close(pipe_father_to_child[PIPE_READ]);

        close(pipe_child_to_father[PIPE_READ]);
        const char *to = "pong";
        write(pipe_child_to_father[PIPE_WRITE], to, strlen(to) + 1);
        close(pipe_child_to_father[PIPE_WRITE]);
    }
    else
    {
        close(pipe_father_to_child[PIPE_READ]);
        const char *to = "ping";
        write(pipe_father_to_child[PIPE_WRITE], to, strlen(to) + 1);
        close(pipe_father_to_child[PIPE_WRITE]);

        wait(0);
        close(pipe_child_to_father[PIPE_WRITE]);
        read(pipe_child_to_father[PIPE_READ], buffer, sizeof(buffer));

        printf("%d: received %s\n", getpid(), buffer);
    }
    exit(0);
}
