#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(1, "please Pass in two parameters!\n");
        exit(1);
    }
    int times = atoi(argv[1]);
    if(sleep(times) < 0)
    {
        fprintf(1, "sleep fail!\n");
        exit(-1);
    }
    // sleep(times);
    exit(0);
}
