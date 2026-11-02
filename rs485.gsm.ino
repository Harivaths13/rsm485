
#define ESPC3PCNT (1)
#define SerialDebug     Serial
#define SerialAT        Serial1
#define RS485ID         1 //Fixed
#define REGISTRY        2     // DGWLR water level: 40003-40004 float (0-based address). Old values: 12292 = Emag, 1034 = Pulse mtr
//#define ISINT         (1)   // Define for Emag (32-bit integer). Leave commented for float meters like the DGWLR
#define RELAY           10
#define LOOPDELAY       3000
#define UPDATEIN        100000 //10000
#define DEBUG           (1)

static float litres = 0, lpm = 0;
static uint32_t pushed = 0;

#include "AegirUtils.h"
#include "modbusin.h"
#include "Network.h.h"   // Must be named exactly Network.h in the sketch folder
#include "AegirPins.h"

void setup()
{
  delay(30000);

  // Pins first, so SIMRESET is a defined output before NetworkSetup() can pulse it
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  pinMode(SIMRESET, OUTPUT);
  digitalWrite(SIMRESET, LOW);
  pinMode(19, OUTPUT); //**********LED************
  digitalWrite(19, LOW);

  // Serial (UART0) is shared by Modbus and debug output: 9600 8N1 on RX=4, TX=5
  Serial.begin(9600, SERIAL_8N1, 4, 5);

  // GSM modem on Serial1, using the pins defined in Network.h (RXAT=7, TXAT=6)
  SerialAT.begin(115200, SERIAL_8N1, RXAT, TXAT);

  NetworkSetup(false);
  delay(1000);

  ModbusSetup();

  // Make the first loop() push right away instead of waiting UPDATEIN after boot
  pushed = millis() - UPDATEIN - 1;
}

void loop()
{
  uint8_t result = node.readHoldingRegisters(REGISTRY,2);
  if (result == node.ku8MBSuccess)
  {
#ifdef ISINT
    litres = U16To32F(node.getResponseBuffer(0),node.getResponseBuffer(1));
#else
    litres = U16ToFloat(node.getResponseBuffer(0),node.getResponseBuffer(1));
    // If the value looks like garbage, the meter sends the low word first: swap the two arguments above
#endif
    DF(litres);DL(" (reading)");
  }
  else
  {
    // 0xE2 = timeout (check A/B wiring, baud, slave ID), 0x02 = illegal data address
    D("Modbus read failed, code 0x");
    DL(String(result, HEX));
  }
 
  if(millis()-pushed>UPDATEIN) 
  {
    Communicate();
    pushed = millis();
  }
  delay(LOOPDELAY);
}

void Communicate() {
  String uri = "/api/Ping?d=1," + String(litres) + ",3&code=W9QDDPBJsAWWSBUtaR2u7ewQoA7CEgP/oyKHa8cFRfdFGu55DsRnNg==";
  HttpGet(uri);
}

/*
void ShowAT() {
  while(SerialAT.available()!=0) Serial.write(SerialAT.read());
  delay(500);
}

void SendAT(const char* cmd, int wait=1000, bool show=false) {
  SerialAT.println(cmd);
  delay(wait);
  if(show) ShowAT();
}


void Communicate()
{
  ShowAT();
  SendAT("AT");
  SendAT("AT+CPIN?");
  SendAT("AT+CREG?");
  SendAT("AT+CGATT?");
  SendAT("AT+CIPSHUT");
  SendAT("AT+CIPSTATUS", 2000);
  SendAT("AT+CIPMUX=0", 2000);
  ShowSerialData();
  SendAT("AT+CSTT=\"airtelgprs.com\"", 1000, true); //start task and setting the APN
  SendAT("AT+CIICR", 3000, true); //bring up wireless connection
  SendAT("AT+CIFSR", 2000, true); //get local IP adress
  SendAT("AT+CIPSPRT=0", 3000, true);
  SendAT("AT+CIPSTART=\"TCP\",\"any.bolt.ac\",\"80\"", 6000, true);//start up the connection
  SendAT("AT+CIPSEND", 4000, true);//begin send data to remote server
  String uri="GET http://any.bolt.ac/api/Ping?d=1," + String(litres) + ",4&code=W9QDDPBJsAWWSBUtaR2u7ewQoA7CEgP/oyKHa8cFRfdFGu55DsRnNg==";
  SendAT(uri.c_str(), 4000, true);//begin send data to remote server
  SendAT("x-i47-id: AA-BB-CC-DD-EE-FF");
  SendAT("x-i47-ver: 100");
  SendAT("x-i47-type: WATERMTR");
  SendAT("\x1a\n", 5000, true);
  SendAT("AT+CIPSHUT", 100, true);//close the connection
  delay(500);

  //SendAT((char)26, 5000);//sending
  // digitalWrite(3, HIGH);
  // delay(1000);
  // digitalWrite(3, LOW);
  // delay(5000);
}


/*#define ESPC3PCNT (1)
#define SerialDebug     Serial
#define SerialAT        Serial1
#define RS485ID         1 //Fixed
#define REGISTRY        2     // DGWLR water level: 40003-40004 float (0-based address). Old values: 12292 = Emag, 1034 = Pulse mtr
//#define ISINT         (1)   // Define for Emag (32-bit integer). Leave commented for float meters like the DGWLR
#define RELAY           10
#define LOOPDELAY       3000
#define UPDATEIN        300000 //10000
#define DEBUG           (1)

static float litres = 0, lpm = 0;
static uint32_t pushed = 0;

#include "AegirUtils.h"
#include "modbusin.h"
#include "Network.h.h"   // Must be named exactly Network.h in the sketch folder
#include "AegirPins.h"

void setup()
{
  delay(30000);

  // Pins first, so SIMRESET is a defined output before NetworkSetup() can pulse it
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  pinMode(SIMRESET, OUTPUT);
  digitalWrite(SIMRESET, LOW);
  pinMode(19, OUTPUT); //**********LED************
  digitalWrite(19, LOW);

  // Serial (UART0) is shared by Modbus and debug output: 9600 8N1 on RX=4, TX=5
  Serial.begin(9600, SERIAL_8N1, 4, 5);

  // GSM modem on Serial1, using the pins defined in Network.h (RXAT=7, TXAT=6)
  SerialAT.begin(115200, SERIAL_8N1, RXAT, TXAT);

  NetworkSetup(false);
  delay(1000);

  ModbusSetup();

  // Make the first loop() push right away instead of waiting UPDATEIN after boot
  pushed = millis() - UPDATEIN - 1;
}

void loop()
{
  uint8_t result = node.readHoldingRegisters(REGISTRY,2);
  if (result == node.ku8MBSuccess)
  {
#ifdef ISINT
    litres = U16To32F(node.getResponseBuffer(0),node.getResponseBuffer(1));
#else
    litres = U16ToFloat(node.getResponseBuffer(0),node.getResponseBuffer(1));
    // If the value looks like garbage, the meter sends the low word first: swap the two arguments above
#endif
    DF(litres);DL(" (reading)");
  }
  else
  {
    // 0xE2 = timeout (check A/B wiring, baud, slave ID), 0x02 = illegal data address
    D("Modbus read failed, code 0x");
    DL(String(result, HEX));
  }
 
  if(millis()-pushed>UPDATEIN) 
  {
    Communicate();
    pushed = millis();
  }
  delay(LOOPDELAY);
}

void Communicate() {
  String uri = "/api/Ping?d=1," + String(litres) + ",3&code=W9QDDPBJsAWWSBUtaR2u7ewQoA7CEgP/oyKHa8cFRfdFGu55DsRnNg==";
  HttpGet(uri);
}

/*
void ShowAT() {
  while(SerialAT.available()!=0) Serial.write(SerialAT.read());
  delay(500);
}

void SendAT(const char* cmd, int wait=1000, bool show=false) {
  SerialAT.println(cmd);
  delay(wait);
  if(show) ShowAT();
}


void Communicate()
{
  ShowAT();
  SendAT("AT");
  SendAT("AT+CPIN?");
  SendAT("AT+CREG?");
  SendAT("AT+CGATT?");
  SendAT("AT+CIPSHUT");
  SendAT("AT+CIPSTATUS", 2000);
  SendAT("AT+CIPMUX=0", 2000);
  ShowSerialData();
  SendAT("AT+CSTT=\"airtelgprs.com\"", 1000, true); //start task and setting the APN
  SendAT("AT+CIICR", 3000, true); //bring up wireless connection
  SendAT("AT+CIFSR", 2000, true); //get local IP adress
  SendAT("AT+CIPSPRT=0", 3000, true);
  SendAT("AT+CIPSTART=\"TCP\",\"any.bolt.ac\",\"80\"", 6000, true);//start up the connection
  SendAT("AT+CIPSEND", 4000, true);//begin send data to remote server
  String uri="GET http://any.bolt.ac/api/Ping?d=1," + String(litres) + ",4&code=W9QDDPBJsAWWSBUtaR2u7ewQoA7CEgP/oyKHa8cFRfdFGu55DsRnNg==";
  SendAT(uri.c_str(), 4000, true);//begin send data to remote server
  SendAT("x-i47-id: AA-BB-CC-DD-EE-FF");
  SendAT("x-i47-ver: 100");
  SendAT("x-i47-type: WATERMTR");
  SendAT("\x1a\n", 5000, true);
  SendAT("AT+CIPSHUT", 100, true);//close the connection
  delay(500);

  //SendAT((char)26, 5000);//sending
  // digitalWrite(3, HIGH);
  // delay(1000);
  // digitalWrite(3, LOW);
  // delay(5000);
}
*/
