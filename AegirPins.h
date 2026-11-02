#pragma once

#ifdef ESPC3PCNT
  #define  SENSOR      5
  #define  LORAUX      4
  #define  LORAM0      0
  #define  LORAM1      1
  #define  LORARX      7
  #define  LORATX      6
  #define  LORASTRT    SerialLora.begin(9600,SERIAL_8N1,LORARX,LORATX);
  #define  HARDWARE_SERIAL_SELECTABLE_PIN
#endif

#ifdef ESPC3M
  #define CS      19
  #define INT     3
  #define RST     18
  #define MOSI    1
  #define MISO    0
  #define SCLK    5
  #define SPISTRT SPI.begin(SCLK, MISO, MOSI, CS);
#endif

#ifdef ESPC3L
  #define CS      19
  #define INT     8//3
  #define RST     18
  #define MOSI    1
  #define MISO    0
  #define SCLK    5
  #define LORAUX  4
  #define LORAM0  10//21
  #define LORAM1  3//22
  #define LORARX  7//6
  #define LORATX  6//7
  #define SPISTRT SPI.begin(SCLK, MISO, MOSI, CS);
  #define LORASTRT   SerialLora.begin(9600,SERIAL_8N1,LORARX,LORATX);
  //#define LORASTRT  SerialLora.begin(9600);
#endif

#ifdef ESPC3OLD
  #define CS      19
  #define INT     8
  #define RST     18
  #define MOSI    1
  #define MISO    0
  #define SCLK    5
  #define LORAUX  4
  #define LORAM0  21
  #define LORAM1  22
  #define LORASTRT   SerialLora.begin(9600,SERIAL_8N1,LORARX,LORATX);
  #define SPISTRT SPI.begin(SCLK, MISO, MOSI, CS);
#endif

#ifdef STML0
  #define CS      PB12
  #define INT     PB6
  #define RST     PB7
  #define MOSI    PB15
  #define MISO    PB14
  #define SCLK    PB13
  #define SPISTRT SPI.setMOSI(PB15);SPI.setMISO(PB14);SPI.setSCLK(PB13);SPI.begin()
  //#define LORASTRT   SerialLora.begin(9600,SERIAL_8N1,LORARX,LORATX);
#endif