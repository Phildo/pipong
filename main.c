#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef char byte;

#define FORCE_SERIAL

#ifdef FORCE_SERIAL
#include "wiringserial.h"
#endif

//#define RING
#define STRIP
//#define BIGSTRIP

#define LUT
#define COMPRESS

#define FLUSH_TRIGGER "FLUSH"
#ifdef RING
  #define STRIP_NUM_LEDS 24
  #define STRIP_LED_TYPE WS2812
#elif defined STRIP
  #define STRIP_NUM_LEDS 100
  #define STRIP_LED_TYPE WS2811
#elif defined BIGSTRIP
  #define STRIP_NUM_LEDS 300
  #define STRIP_LED_TYPE WS2812
#endif
#define BAUD_RATE 115200
//#define BAUD_RATE 230400
//#define BAUD_RATE 460800
//#define BAUD_RATE 500000
//#define BAUD_RATE 576000
//#define BAUD_RATE 921600
//#define BAUD_RATE 1000000
//#define BAUD_RATE 1152000
//#define BAUD_RATE 1500000
//#define BAUD_RATE 2000000
//#define BAUD_RATE 2500000
//#define BAUD_RATE 3000000
//#define BAUD_RATE 3500000
//#define BAUD_RATE 4000000

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

  #ifdef LUT
    #ifdef COMPRESS
  buff_n = STRIP_NUM_LEDS/2+strlen(FLUSH_TRIGGER);
    #else
  buff_n = STRIP_NUM_LEDS+strlen(FLUSH_TRIGGER);
    #endif
  #else
  buff_n = STRIP_NUM_LEDS*3+strlen(FLUSH_TRIGGER);
  #endif
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
    #ifdef LUT
      #ifdef COMPRESS
    if(step%2 == 0)
    {
      int i = step/2;
      buff[i] = 0x10;
      step++;
      if(step == STRIP_NUM_LEDS) step = 0;
    }
    else
    {
      int i = step/2;
      buff[i] &= 0x0F;
      step++;
      if(step == STRIP_NUM_LEDS) { step = 0; i = -1; }
      buff[i+1] &= 0xF0;
      buff[i+1] |= 0x01;
    }
      #else
    buff[step] = 0;
    step++;
    if(step == STRIP_NUM_LEDS) step = 0;
    buff[step] = 1;
      #endif
    #else
    buff[step*3+0] = 0;
    buff[step*3+1] = 0;
    buff[step*3+2] = 0;
    step++;
    if(step == STRIP_NUM_LEDS) step = 0;
    buff[step*3+0] = 0;
    buff[step*3+1] = 20;
    buff[step*3+2] = 0;
    #endif

    #ifdef FORCE_SERIAL
    serialPutns(fp,buff,buff_n);
    serialFlush(fp);
    #else
    if(fwrite(buff,sizeof(byte),buff_n,fp) != buff_n) break;
    fflush(fp);
    #endif

    for(int i = 0; i < 4000000; i++) ;
    //sleep(1);
  }

  #ifdef FORCE_SERIAL
  serialClose(fp);
  #else
  fclose(fp);
  #endif
}

