#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"
#include <stddef.h>
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("error input!\n");
        exit(-1);
    }
    sleep(10);
    const int buf_size = 512;
    char buf[buf_size];
    char *xargv[MAXARG];
    int xargc = 0;
    for (int i = 1; i < argc; i++)
        xargv[xargc++] = argv[i];
    memset(buf, '\0', sizeof buf);
    read(0, buf, sizeof(buf));
    int out_count = 0;
    char out[100];
    for(int i = 0; i < 100; i++) out[i] = '\0';
    for (int i = 0; i < strlen(buf); i++)
    {
        if (buf[i] == '\n')
        {
            out[out_count] = '\0';
            xargv[xargc++] = out;
            int rc = fork();
            if (rc == 0)
            {
                exec(xargv[0], xargv);
            }
            else
            {
                wait(0);
                xargc--;
                memset(out, '\0', sizeof out);
                out_count = 0;
            }
        }
        else 
        {
            out[out_count++] = buf[i];
        }
    }

    exit(0);
}