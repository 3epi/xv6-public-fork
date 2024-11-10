#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf(2, "Wrong Argument Management for sort_syscalls\n");
        exit();
    }
    int pid = atoi(argv[1]);
    if (sort_syscalls(pid) < 0) {
        printf(2, "sort_syscalls: failed\n");
    }
    exit();
}
