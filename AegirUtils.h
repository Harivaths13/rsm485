#pragma once

#ifdef BTDEBUG
#ifndef LOCATION
#error "LOCATION must be defined. This is a friendly name for the Gateway."
#endif
#endif

#if defined(DEBUG) || defined(BTDEBUG)
#ifndef SerialDebug
#error "SerialDebug must be defined to enable DEBUG or BTDEBUG."
#endif
#endif

#ifdef BTDEBUG
  #define D(x)      SerialDebug.print(x)
  #define DF(x)     SerialDebug.print(x,6)
  #define DL(x)     SerialDebug.println(x)
  #define BD(x)     SerialDebug.print(x);ESP_BT.print(x)
  #define BDL(x)    SerialDebug.println(x);ESP_BT.println(x)
#elif DEBUG
  #define D(x)      SerialDebug.print(x)
  #define DF(x)     SerialDebug.print(x,6)
  #define DL(x)     SerialDebug.println(x)
  #define BD(x)     D(x)
  #define BDL(x)    DL(x)
#else
  #define D(x)
  #define DF(x)
  #define DL(x)
  #define BD(x)
  #define BDL(x)
#endif

#define TURNHI(x)   digitalWrite(x,HIGH);
#define TURNLO(x)   digitalWrite(x,LOW);
