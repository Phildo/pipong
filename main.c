#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef char byte;

#define FORCE_SERIAL

#ifdef FORCE_SERIAL
#include "wiringserial.h"
#endif

#define FLUSH_TRIGGER "FLUSH"
#define STRIP_NUM_LEDS 24
#define BAUD_RATE 115200

int main(int argc, char **argv)
{
  #ifdef FORCE_SERIAL
  int fp = 0;
  #else
  FILE *fp = 0;
  #endif
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
    #ifdef FORCE_SERIAL
    fp = serialOpen(filename, BAUD_RATE);
    #else
    fp = fopen(filename,"a");
    #endif
  }
  while(!fp)
  {
    printf("file:"); fflush(stdout);

    if(fgets(filename, sizeof(filename), stdin) != filename) exit(1);
    filename[strlen(filename)-1] = '\0';
    #ifdef FORCE_SERIAL
    fp = serialOpen(filename, BAUD_RATE);
    #else
    fp = fopen(filename,"a");
    #endif
  }

  int step = 0;
  while(1)
  {
    buff[step*3+0] = 0;
    buff[step*3+1] = 0;
    buff[step*3+2] = 0;
    step++;
    if(step == STRIP_NUM_LEDS) step = 0;
    buff[step*3+0] = 0;
    buff[step*3+1] = 20;
    buff[step*3+2] = 0;
    /*
    for(int i = 0; i < STRIP_NUM_LEDS; i++)
    {
      buff[i*3+0] = rand()%55;
      buff[i*3+1] = rand()%55;
      buff[i*3+2] = rand()%55;
    }
    */
    #ifdef FORCE_SERIAL
    serialPutns(fp,buff,buff_n);
    serialFlush(fp);
    #else
    if(fwrite(buff,sizeof(byte),buff_n,fp) != buff_n) break;
    fflush(fp);
    #endif
    for(int i = 0; i < 2000000; i++) ;
  }

  #ifdef FORCE_SERIAL
  serialClose(fp);
  #else
  fclose(fp);
  #endif
}

