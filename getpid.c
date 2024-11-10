#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int pid = getpid();
  printf(1, "Current PID is %d\n", pid);
  exit();
}