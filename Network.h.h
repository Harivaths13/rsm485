#define TINY_GSM_MODEM_SIM7600
#if !defined(TINY_GSM_RX_BUFFER)
#define TINY_GSM_RX_BUFFER 1024
#endif
#define SIMRESET    3
#define TXAT        6
#define RXAT        7

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

// The sketch normally defines SerialAT (Serial1); this is only a fallback
#ifndef SerialAT
#define SerialAT Serial2
#endif

const char APN[] = "airteliot.com";
const char SERVER[] = "any.bolt.ac";
static uint8_t simFail;
static String imei = "869469088710352";   // Device ID sent in the x-i47-id header; used if the modem returns nothing
TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
HttpClient http(client, SERVER, 80);    

void RadioOff() { btStop(); }

void NetworkSetup(bool restart) {
  RadioOff();
  if(restart) {simFail++;D("************* Restarting SIM  **************  #");DL(simFail);}
  if(simFail==1) {digitalWrite(SIMRESET, HIGH);delay(1000);digitalWrite(SIMRESET, LOW);delay(1000);DL("****** BRDRESET Reset SIM800 ******");}
  modem.init();

  // Read the IMEI right after init (AT only, works even if the network step fails below).
  // Only overwrite the stored ID when the modem actually returns something.
  String modemImei = modem.getIMEI();
  modemImei.trim();
  if(modemImei.length() > 0) {
    imei = modemImei;
  } else {
    DL("IMEI read failed, using stored ID");
  }
  D("IMEI: ");
  DL(imei);

  D("Modem Info: ");
  DL(modem.getModemInfo());
  delay(3000);
  D("Waiting for network: ");
  if(!modem.waitForNetwork()) { DL("Fail"); delay(10000); return; }
  DL("Success");
  D("Waiting for GPRS: ");
  if(!modem.gprsConnect(APN)) { DL("Fail"); delay(10000); return; }
  DL("Success");
  delay(3000);
  DL(restart ? "Network Restart Complete" : "Initial Network Setup Complete");
}

bool HttpGet(String uri){
  DL(uri);
  http.beginRequest();
  http.get(uri);
  http.sendHeader("x-i47-id", imei);
  http.sendHeader("x-i47-ver", "502");
  http.sendHeader("x-i47-type", "WATERMTR");
  http.endRequest();
  int status = http.responseStatusCode();
  DL("HTTP Status: " + String(status));
  DL("HTTP Response: " + String(http.responseBody()));
  bool isOk = (status == 200 || status == 204);
  if(!isOk) NetworkSetup(true);
  else simFail = 0;
  return isOk;
}