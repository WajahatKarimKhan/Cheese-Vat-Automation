#include "vfd.h"
#include <ModbusMaster.h>


extern ModbusMaster stirrer;

float actualFreq = 0.0;
bool stirrerRunning = false;
bool stirrerDirection = false; // false = Forward, true = Reverse
int maxRPM = 1800; //configured max RPM of motor


void VFD_init(ModbusMaster& node) {
    pinMode(MAX485_RE_DE_PIN, OUTPUT);
    postTransmission();

    Serial1.begin(VFD_BAUD_RATE);
    //Serial1.setTimeout(200);      //  Timeout after 200 ms if no reply
    node.begin(VFD_SLAVE_ID, Serial1);
    
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);
}

void VFD_setFrequency(ModbusMaster& node, float hz) {
    uint16_t freqVal = static_cast<uint16_t>(hz * 100); // Convert Hz to 0.01 Hz units
    node.writeSingleRegister(REG_FREQUENCY_SET, freqVal);
}

void VFD_start(ModbusMaster& node, bool reverse) {
    uint16_t cmd = reverse ? CMD_RUN_REVERSE : CMD_RUN_FORWARD;
    node.writeSingleRegister(REG_RUN_COMMAND, cmd);
    stirrerRunning = true;
    stirrerDirection = reverse;
}

void VFD_stop(ModbusMaster& node) { 
    node.writeSingleRegister(REG_RUN_COMMAND, CMD_STOP_VFD);
    stirrerRunning = false; 
}

float VFD_readFrequency(ModbusMaster& node) {
    uint8_t result = node.readHoldingRegisters(REG_OUTPUT_FREQUENCY, 1);
    if (result == node.ku8MBSuccess) { return node.getResponseBuffer(0) / 100.0; } // 0.01 Hz units  
    return -1;
}

void VFD_setRPM(ModbusMaster& node, int rpm, int maxRPM) {
  float freq = (rpm / (float)maxRPM) * 50.0; // Scale to Hz for 50 Hz AC supply
  VFD_setFrequency(node, freq);
}

float VFD_readCurrent(ModbusMaster& node) {
    uint8_t result = node.readHoldingRegisters(REG_OUTPUT_CURRENT, 1);
    if (result == node.ku8MBSuccess) { return node.getResponseBuffer(0) / 10.0; } // 0.1 A units
    return -1;
}
void preTransmission() {
  // receive pin high , drive pin high = transmission enabled
  digitalWrite(MAX485_RE_DE_PIN, HIGH);
  
}

void postTransmission() {
  // receive pin low , drive pin low = receiving enabled
  digitalWrite(MAX485_RE_DE_PIN, LOW);
}

int readLiveRPM(ModbusMaster& node) {
    float freq = VFD_readFrequency(node); // in Hz
    if (freq < 0) return -1; // read failed

    actualFreq = freq; // optional: store for debugging
    return (int)((freq / 50.0) * maxRPM);
}

/*
    float freq = VFD_readFrequency(stirrer);
    float curr = VFD_readCurrent(stirrer);

    Serial.print("Freq: ");
    Serial.print(freq);
    Serial.print(" Hz | Current: ");
    Serial.print(curr);
    Serial.println(" A");
*/
