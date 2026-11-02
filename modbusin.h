#define MAX485_REDE_PIN 18
#include <ModbusMaster.h>
ModbusMaster node;

void preTransmission() { digitalWrite(MAX485_REDE_PIN, 1); }
void postTransmission() { digitalWrite(MAX485_REDE_PIN, 0); }

void ModbusSetup() {
  pinMode(MAX485_REDE_PIN, OUTPUT);
  digitalWrite(MAX485_REDE_PIN, 0);
  
  // Directly bind ModbusMaster to the Serial engine
  node.begin(RS485ID, Serial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

float U16ToFloat(uint16_t u16Part1, uint16_t u16Part2) {
    uint32_t combinedValue = ((uint32_t)u16Part1 << 16) | u16Part2;
    float reconstructedFloat;
    memcpy(&reconstructedFloat, &combinedValue, sizeof(float));
    return reconstructedFloat;
}

float U16To32F(uint16_t u16Part1, uint16_t u16Part2) {
    uint32_t combinedValue = ((uint32_t)u16Part1 << 16) | u16Part2;
    float temp = combinedValue;
    return temp;
}