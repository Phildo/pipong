#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef char byte;

#define N_LEDS 24
#define BUFF_N N_LEDS*3+1
byte buff[BUFF_N];

char filename[256];

int main(int argc, char **argv)
{
  FILE *fp = 0;

  while(!fp)
  {
    printf("file:"); fflush(stdout);

    if(fgets(filename, sizeof(filename), stdin) != filename) exit(1);
    filename[strlen(filename)-1] = '\0';
    fp = fopen(filename,"a");
  }

  buff[BUFF_N-1] = '\0';
  while(1)
  {
    for(int i = 0; i < N_LEDS; i++)
    {
      buff[i*3+0] = rand()%255;
      buff[i*3+1] = rand()%255;
      buff[i*3+2] = rand()%255;
    }
    if(fwrite(buff,sizeof(byte),BUFF_N,fp) != BUFF_N) break;
    fflush(fp);
    sleep(10);
  }

  fclose(fp);
}

