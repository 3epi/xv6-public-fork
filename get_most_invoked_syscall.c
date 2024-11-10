#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf(2, "Wrong Argument Management for get_most_invoked_syscall \n");
        exit();
    }
    int pid = atoi(argv[1]);
    if (get_most_invoked_syscall(pid) < 0) {
        printf(2, "get_most_invoked_syscall: failed\n");
    }
    exit();
}
