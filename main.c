#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef char byte;

#define FLUSH_TRIGGER "FLUSH"
#define STRIP_NUM_LEDS 24

int main(int argc, char **argv)
{
  FILE *fp = 0;
  char filename[256];
  byte *buff;
  int buff_n;

  buff_n = STRIP_NUM_LEDS*3+strlen(FLUSH_TRIGGER);
  buff = (byte *)malloc(sizeof(byte)*buff_n+1);
  memset(buff,0,sizeof(byte)*buff_n+1);
  strcpy(buff+(buff_n-strlen(FLUSH_TRIGGER)),FLUSH_TRIGGER);

  if(argc == 2)
  {
    strcpy(filename,argv[1]);
    fp = fopen(filename,"a");
  }
  while(!fp)
  {
    printf("file:"); fflush(stdout);

    if(fgets(filename, sizeof(filename), stdin) != filename) exit(1);
    filename[strlen(filename)-1] = '\0';
    fp = fopen(filename,"a");
  }

  int step = 0;
  while(1)
  {
    buff[step*3+0] = 0;
    buff[step*3+1] = 0;
    buff[step*3+2] = 0;
    step++;
    if(step == STRIP_NUM_LEDS) step = 0;
    buff[step*3+0] = 20;
    buff[step*3+1] = 20;
    buff[step*3+2] = 20;
    /*
    for(int i = 0; i < STRIP_NUM_LEDS; i++)
    {
      buff[i*3+0] = rand()%55;
      buff[i*3+1] = rand()%55;
      buff[i*3+2] = rand()%55;
    }
    */
    if(fwrite(buff,sizeof(byte),buff_n,fp) != buff_n) break;
    fflush(fp);
    sleep(1);
  }

  fclose(fp);
}

