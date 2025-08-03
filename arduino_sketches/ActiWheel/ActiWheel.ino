#include <TimerOne.h>
#include <TimerThree.h>
#include "RTClib.h"
#include <Wire.h>
#include <SPI.h>
#include <SdFat.h>   //user SDA/SCL on Pins 20 and 21

// file system
#if USE_SDIO
// Traditional DMA version.
// SdFatSdio sd;
// Faster version.
SdFatSdioEX sd;
#else  // USE_SDIO
SdFat sd;
#endif  // USE_SDIO

//SdFat sd;

SdFile logfile;
SdFile logfile2;
RTC_DS1307 RTC;

//Pins declaration
const uint16_t potPin = A8;
const uint16_t ledPin = 42;

uint32_t timestamp_main;

char filename[31];

const uint16_t bufferSize = 1020;

//volatile variables handled in the ISR
volatile uint8_t bufferA[bufferSize];
volatile uint8_t bufferB[bufferSize];
volatile uint16_t potRead_int;
volatile boolean mainWriteFlag = false;
volatile boolean useBufferA = true;
volatile uint16_t blockIndex = 0;
volatile uint32_t oldTimestamp = 0;
volatile uint32_t timestamp;
volatile uint32_t microsTime;

const uint32_t samplingFreq = 100;
volatile uint16_t sampleCounter = 0;

uint8_t nowHour;
uint8_t nowMinute;
uint8_t nowSecond;
uint8_t newfileHour = 0;
uint8_t newfileMinute = 0;
boolean newfileFlag = true;

void createLogfile() {
  DateTime now = RTC.now();
  sprintf(filename, "Wheel1_%04d-%02d-%02d_%02d-%02d-%02d.dat", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  logfile.open(filename, O_WRITE | O_CREAT);
  Serial.println(filename);
}


void logData() {
  if (timestamp != oldTimestamp) {  //this is to avoid missed timestamp updates during occasional long latency writes
    oldTimestamp = timestamp;
    sampleCounter = 0;
  }
  sampleCounter++;
  if (sampleCounter > samplingFreq) { //it forces the timestamp to be incremented if 100 data points with that timestamp have already been acquired
    timestamp++;
    sampleCounter = 0;
  }
  potRead_int = analogRead(potPin);
  microsTime = micros();

  if (useBufferA) {                                             //The 2 buffers are used one at the time to avoid missing data.
    bufferA[blockIndex + 1] = uint8_t(potRead_int >> 8);        //As one gets full, it is written to the sd card during the loop()
    bufferA[blockIndex] = uint8_t(potRead_int);                 //so that the other buffer can be filled as the first one is written.
    bufferA[blockIndex + 5] = uint8_t(timestamp >> 24);
    bufferA[blockIndex + 4] = uint8_t(timestamp >> 16);
    bufferA[blockIndex + 3] = uint8_t(timestamp >> 8);
    bufferA[blockIndex + 2] = uint8_t(timestamp);
    bufferA[blockIndex + 9] = uint8_t(microsTime >> 24);
    bufferA[blockIndex + 8] = uint8_t(microsTime >> 16);
    bufferA[blockIndex + 7] = uint8_t(microsTime >> 8);
    bufferA[blockIndex + 6] = uint8_t(microsTime);

    blockIndex = blockIndex + 10;
  }
  else {
    bufferB[blockIndex + 1] = uint8_t(potRead_int >> 8);
    bufferB[blockIndex] = uint8_t(potRead_int);
    bufferB[blockIndex + 5] = uint8_t(timestamp >> 24);
    bufferB[blockIndex + 4] = uint8_t(timestamp >> 16);
    bufferB[blockIndex + 3] = uint8_t(timestamp >> 8);
    bufferB[blockIndex + 2] = uint8_t(timestamp);
    bufferB[blockIndex + 9] = uint8_t(microsTime >> 24);
    bufferB[blockIndex + 8] = uint8_t(microsTime >> 16);
    bufferB[blockIndex + 7] = uint8_t(microsTime >> 8);
    bufferB[blockIndex + 6] = uint8_t(microsTime);

    blockIndex = blockIndex + 10;
  }

  if (blockIndex >= bufferSize) {
    useBufferA = !useBufferA;
    blockIndex = 0;
    mainWriteFlag = true;
  }
}


void error() {
  while (true) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}


void setup() {
  Serial.begin(115200);
  Wire.begin();
  RTC.begin();
  SPI.begin();

  pinMode(potPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  delay(1000);

  if (!sd.begin(53)) {
    Serial.println("card not present");
    error();
  }
  else {
    Serial.println("card initialized");
    digitalWrite(ledPin, LOW);
  }

  createLogfile();

  if (sd.exists(filename)) {
    Serial.println("Logging...");
  }
  else {
    Serial.println("Could not create log file");
    error();
  }
  uint32_t samplingInterval = 1000000 / samplingFreq;
  
  Timer3.initialize();
  Timer3.setPeriod(samplingInterval);
  Timer3.attachInterrupt(logData);
}


void loop() {
  DateTime now = RTC.now();
  timestamp_main = now.unixtime();

  nowHour = now.hour();
  nowMinute = now.minute();
  nowSecond = now.second();

  noInterrupts();
  timestamp = timestamp_main;
  interrupts();

  if (mainWriteFlag) {
    if (useBufferA) {
      logfile.write((uint8_t *)&bufferB, sizeof(bufferB));
      logfile.sync();
      mainWriteFlag = false;
    }
    else {
      logfile.write((uint8_t *)&bufferA, sizeof(bufferA));
      logfile.sync();
      mainWriteFlag = false;
    }
  }
}
