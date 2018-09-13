#include <Arduino.h>
#include <SoftwareSerial.h>

class SmartAudio
{
  void sendDataToHw(byte *data[], int8_t len);
  int dataPin;
  bool useHWSerial;

public:
  SmartAudio();
  SmartAudio(int softPin);
  void setFrequency(int freq);
  void setPower(int pwr);
  enum unifyPower
  {
    pwr_25mW = 0,
    pwr_200mW = 1,
    pwr_500mW = 2,
    pwr_800mW = 3,
  } pwr;
};