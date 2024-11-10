// mv.c

#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf(1,"Usage: mv source_file destination_directory\n");
    exit();
  }

  if(move_file(argv[1], argv[2]) < 0){
    printf(2,"mv: failed to move %s to %s\n", argv[1], argv[2]);
    exit();
  }

  exit();
}
