#ifndef VFD_H
#define VFD_H

#include <ModbusMaster.h>

// Constants 
#define VFD_SLAVE_ID      1
#define VFD_BAUD_RATE     9600

//io map
#define MAX485_RE_DE_PIN     16
//#define

// Register addresses (Modbus)
#define REG_FREQUENCY_SET     0x0000  // Frequency command (0.01 Hz units)
#define REG_RUN_COMMAND       0x0001  // Run/Stop + direction
#define REG_OUTPUT_FREQUENCY  0x0100  // Actual output frequency (read)
#define REG_OUTPUT_CURRENT    0x0102  // Output current in 0.1 A (read)

// Run commands
#define CMD_STOP_VFD          0x0000
#define CMD_RUN_FORWARD       0x0001
#define CMD_RUN_REVERSE       0x0002

// Function prototypes
void VFD_init(ModbusMaster& node);
void VFD_setFrequency(ModbusMaster& node, float hz);
void VFD_start(ModbusMaster& node, bool reverse = false);
void VFD_stop(ModbusMaster& node);
float VFD_readFrequency(ModbusMaster& node);
float VFD_readCurrent(ModbusMaster& node);
void VFD_setRPM(ModbusMaster& node, int rpm, int maxRPM);

int readLiveRPM(ModbusMaster& node);
void preTransmission();
void postTransmission();



#endif