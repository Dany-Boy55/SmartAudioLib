#include "FPVFrequencies.h"
#include "SmartAudio.h"
#include <EEPROM.h>
#include <Arduino.h>

#define preset1pwr 200
#define preset1frq FPVFrequencies::CH_R5
#define preset2pwr 800
#define preset2frq FPVFrequencies::CH_A1

SmartAudio *smadevice = new SmartAudio(2);

void setup()
{
    EEPROM.begin();
    smadevice->setFrequency(5800);
    smadevice->setPower(800);
    // put your setup code here, to run once:
}

void loop()
{
    Serial.begin(9600);
    if(Serial.available() > 1){
        smadevice->setPower(SmartAudio::unifyPower::pwr_200mW);
        smadevice->setFrequency(FPVFrequencies::CH_A5);
    }

    // put your main code here, to run repeatedly:
}