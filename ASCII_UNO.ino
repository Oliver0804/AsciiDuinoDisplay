#include <Wire.h>
int count = 0;

const int startPin = 2; // Start pin number for digital pins
const int endPin = 13;  // End pin number for digital pins
char pinStatuses[endPin - startPin + 1]; // Array to store the statuses of pins (H/L)
float voltageReadings[4]; // Array to store voltage readings for analog pins

const int maxDevices = 3; // Maximum number of I2C devices
byte foundDevices[maxDevices]; // Array to store addresses of found I2C devices
int deviceCount = 0; // Number of found I2C devices

// Reads and saves the status of each digital pin
void readAndSavePinStatuses() {
  for (int i = startPin; i <= endPin; i++) {
    pinStatuses[i - startPin] = digitalRead(i) == HIGH ? 'H' : 'L';
  }
}

// Reads the voltages from the analog pins and saves them
void readAnalogVoltages() {
  for (int pin = A0; pin <= A3; pin++) {
    int sensorValue = analogRead(pin);
    voltageReadings[pin - A0] = sensorValue * 5.0 / 1023.0;
  }
}

// Scans and identifies I2C devices on the bus
void scanI2CDevices() {
  byte error, address;
  deviceCount = 0; // Reset the device count

  for (address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0 && deviceCount < maxDevices) {
      foundDevices[deviceCount++] = address;
    }
  }
}
// Prints an ASCII loading animation based on the frame count
void printLoadingAnimation(int frame) {
  switch (frame % 4) { // Loop through 4 different frames
    case 0:
      Serial.print(" - ");
      break;
    case 1:
      Serial.print(" \\ ");
      break;
    case 2:
      Serial.print(" I ");
      break;
    case 3:
      Serial.print(" / ");
      break;
  }
}

// Function to print a voltage value
void printVoltage(float voltage) {
  Serial.print(voltage);
  Serial.print(" V <-\t");
}

// Function to print a pin status
void printPinStatus(char status) {
  if (status == 'H') {
    Serial.print("\033[31mH\033[0m");
  } else {
    Serial.print("\033[32mL\033[0m");
  }
}

void setup() {
  Serial.begin(115200);
  // Set all digital pins to INPUT_PULLUP
  for (int i = startPin; i <= endPin; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  Wire.begin(); // Initialize I2C
  Serial.print("\033[2J");
}

void loop() {
  // Main loop of the program
  readAndSavePinStatuses(); // Read and save digital pin statuses
  readAnalogVoltages(); // Read analog voltages
  scanI2CDevices(); // Scan for I2C devices

  // Clear the screen and move the cursor to the top-left corner
  //Serial.print("\033[2J");
  Serial.print("\033[H");

  Serial.println(" \t");
  Serial.println(" \t");
  Serial.println(" \t\t                             +-----+");
  Serial.println(" \t\t+----[PWR]-------------------| USB |--+");
  Serial.println(" \t\t|                            +-----+  |");
  Serial.println(" \t\t|         GND/RST2  [G][E]            |");
  Serial.print(" \t\t|       MOSI2/SCK2  [");
  printPinStatus(pinStatuses[11 - startPin]);
  Serial.print("][");
  printPinStatus(pinStatuses[13 - startPin]);
  Serial.println("]  A5/SCL[\033[33mC\033[0m] |   C5 ");
  Serial.print(" \t\t|          5V/MISO2 [5][");
  printPinStatus(pinStatuses[12 - startPin]);
  Serial.println("]  A4/SDA[\033[33mC\033[0m] |   C4 ");
  Serial.println(" \t\t|                             AREF[A] |");
  Serial.println(" \t\t|                              GND[G] |");
  Serial.print(" \t\t| [ ]N/C                    SCK/13[");
  printPinStatus(pinStatuses[13 - startPin]);
  Serial.println("] |   B5");
  Serial.print(" \t\t| [ ]IOREF                 MISO/12[");
  printPinStatus(pinStatuses[12 - startPin]);
  Serial.println("] |   .");
  Serial.print(" \t\t| [R]RST                   MOSI/11[");
  printPinStatus(pinStatuses[11 - startPin]);
  Serial.println("]~|   .");
  Serial.print(" \t\t| [3]3V3    +---+               10[");
  printPinStatus(pinStatuses[10 - startPin]);
  Serial.println("]~|   .");
  Serial.print(" \t\t| [5]5v    -| A |-               9[");
  printPinStatus(pinStatuses[9 - startPin]);
  Serial.println("]~|   .");
  Serial.print(" \t\t| [G]GND   -| R |-               8[");
  printPinStatus(pinStatuses[8 - startPin]);
  Serial.println("] |   B0");
  Serial.println(" \t\t| [G]GND   -| D |-                    |");
  Serial.print(" \t\t| [I]Vin   -| U |-               7[");
  printPinStatus(pinStatuses[7 - startPin]);
  Serial.println("] |   D7");
  Serial.print(" \t\t|          -|");
  printLoadingAnimation(count++);
  Serial.print("|-               6[");
  printPinStatus(pinStatuses[6 - startPin]);
  Serial.println("]~|   .");
  Serial.print("     ");
  printVoltage(voltageReadings[0]);
  Serial.print("| [\033[91mA\033[0m]A0    -| N |-               5[");
  printPinStatus(pinStatuses[5 - startPin]);
  Serial.println("]~|   .");
  Serial.print("     ");
  printVoltage(voltageReadings[1]);
  Serial.print("| [\033[91mA\033[0m]A1    -| O |-               4[");
  printPinStatus(pinStatuses[4 - startPin]);
  Serial.println("] |   .");
  Serial.print("     ");
  printVoltage(voltageReadings[2]);
  Serial.print("| [\033[91mA\033[0m]A2     +---+           INT1/3[");
  printPinStatus(pinStatuses[3 - startPin]);
  Serial.println("]~|   .");
  Serial.print("     ");
  printVoltage(voltageReadings[3]);
  Serial.print("| [\033[91mA\033[0m]A3                     INT0/2[");
  printPinStatus(pinStatuses[2 - startPin]);
  Serial.println("] |   .");
  Serial.println(" \t\t| [\033[33mC\033[0m]A4/SDA  RST SCK MISO     TX>1[T] |   .");
  Serial.print(" \t\t| [\033[33mC\033[0m]A5/SCL  [R] [");
  printPinStatus(pinStatuses[13 - startPin]);
  Serial.print("] [");
  printPinStatus(pinStatuses[12 - startPin]);
  Serial.println("]      RX<0[R] |   D0");
  Serial.print(" \t\t|            [G] [");
  printPinStatus(pinStatuses[11 - startPin]);
  Serial.println("] [5]              |");
  Serial.println(" \t\t|  UNO_R3    GND MOSI 5V  ____________/");
  Serial.println(" \t\t \\_______________________/");


  // Print I2C device addresses
  if (deviceCount > 0) {
    Serial.println("\t\tFound I2C device(s) at address(es):");
    for (int i = 0; i < deviceCount; i++) {
      Serial.print("\t\t0x");
      if (foundDevices[i] < 16) Serial.print("0");
      Serial.print(foundDevices[i], HEX);
    }
  } else {
    Serial.println("\t\tNo I2C devices found.");
  }

  //delay(100);
}
