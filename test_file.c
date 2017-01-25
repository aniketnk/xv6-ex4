#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main()
{
  char buf[512];
  int fd, i, blocks;
  fd = open("test.file", O_CREATE | O_WRONLY);

  if(fd < 0){
    printf(2, "File Generation: Error while opening file\n");
    exit();
  }

  blocks = 0;
  while(1){
    *(int*)buf = blocks;
    int cc = write(fd, buf, sizeof(buf));
    // printf(2, "Blocks written: #%d \n", blocks);
    if(cc <= 0)
      break;
    blocks++;
  	if (blocks % 2 == 0) {
      int block_w = blocks/2;
      printf(2, "%dKB has been written to disk \n", block_w);
    }
    if (blocks == 2000)
      break;
  }

  printf(1, "\nwrote %d blocks\n", blocks);
  // If arrived here, 1MB has been just been written
  printf(1, "Finished writing 6KB (direct) \n");
  printf(1, "Finished writing 70KB (single direct) \n");
  printf(1, "Finished writing 1MB \n");

  close(fd);
  fd = open("test.file", O_RDONLY);
  if(fd < 0){
    printf(2, "test: cannot re-open test.file for reading\n");
    exit();
  }
  for(i = 0; i < blocks; i++){
    int cc = read(fd, buf, sizeof(buf));
    if(cc <= 0){
      printf(2, "test: read error at sector %d\n", i);
      exit();
    }
    if(*(int*)buf != i){
      printf(2, "test: read the wrong data (%d) for sector %d\n",
             *(int*)buf, i);
      exit();
    }
  }

  printf(1, "done; ok\n"); 

  exit();
}
