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
  strcpy(buff+(buff_n-strlen(FLUSH_TRIGGER)),FLUSH_TRIGGER);

  while(!fp)
  {
    printf("file:"); fflush(stdout);

    if(fgets(filename, sizeof(filename), stdin) != filename) exit(1);
    filename[strlen(filename)-1] = '\0';
    fp = fopen(filename,"a");
  }

  while(1)
  {
    for(int i = 0; i < STRIP_NUM_LEDS; i++)
    {
      buff[i*3+0] = rand()%55;
      buff[i*3+1] = rand()%55;
      buff[i*3+2] = rand()%55;
    }
    if(fwrite(buff,sizeof(byte),buff_n,fp) != buff_n) break;
    fflush(fp);
    sleep(1);
  }

  fclose(fp);
}

