#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf(2, "Wrong Argument Management for create_palindrome\n");
        exit();
    }
    int n = atoi(argv[1]);
    create_palindrome(n);
    if (create_palindrome(n) < 0) {
        printf(2, "create_palindrome: failed\n");
    }
    exit();
}
