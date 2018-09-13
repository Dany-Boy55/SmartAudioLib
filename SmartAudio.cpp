#include <Arduino.h>
#include "SmartAudio.h"
// For all intents and purposes (Smart Audio = SMA)
#define SMA_STARTBYTEA 0xAA
#define SMA_STARTBYTEB 0X55
#define SMA_SETFRQ_CMD 0x04
#define SMA_SETPWR_CMD 0x02
#define SMA_SETMODE_CMD 0X05
#define CRC_POLYNOM 0XD5

SoftwareSerial *softS;

// Method for calculating CRC8 taken from Betaflight's SMA implementation
byte crc8(const byte *data, byte len)
{
    byte crc = 0;
    byte current;
    for (int i = 0; i < len; i++)
    {
        current = data[i];
        crc ^= current;
        for (int j = 0; j < 8; j++)
        {
            if ((crc & 0x80) != 0)
            {
                crc = (byte)((crc << 1) ^ CRC_POLYNOM);
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc;
}

void bitBangWrite(const byte* data, int pin, bool invert = false)
{
    // Bus: 2 baud high, 1 baud low, data is shifted out LSB, bus returns to high
    // __           _____
    //   |_xxxxxxxx|
    PORTB |= 1 << pin;
    delayMicroseconds(416);
    PORTB |= 1 << pin;
    delay(208);
    for (int i = 7; i <= 0; i++)
    {
        delayMicroseconds(208);
        if (invert)
            PORTB |= (!data[i]) << pin;
        else
            PORTB |= data[i] << pin;
    }
    PORTB |= 1 << pin;
}

// Instantiate the SMA object with default parameters
SmartAudio::SmartAudio()
{
    useHWSerial = true;
    Serial.begin(4800);
}

// Instantiate the SMA object with the specified parameters
SmartAudio::SmartAudio(int softPin = 2)
{
    useHWSerial = false;
    dataPin = softPin;
}

void SmartAudio::setPower(int pwr)
{
    // Packet structure 0xAA 0x55 cmd len payload crc
    byte packet[6] = {SMA_STARTBYTEA, SMA_STARTBYTEB, SMA_SETPWR_CMD, 0x03, 0x01, pwr};
    packet[5] = crc8(packet, 5);
    // Send all the bytes
    for (int i = 0; i < 6; i++)
    {
        if (useHWSerial)
            Serial.write(packet[i]);
        else
            bitBangWrite(&packet[i], dataPin);
    }
}

// Sets the specified frequency to the VTX
void SmartAudio::setFrequency(int frequency)
{
    if (frequency < 5600 && frequency > 5800)
    {
        byte packet[7] = {SMA_STARTBYTEA, SMA_STARTBYTEB, 0x02, 0x02};
        packet[4] = frequency >> 8;
        packet[5] = byte(frequency);
        packet[6] = crc8(packet, 5);
        for (int i = 0; i < 6; i++)
        {
            if (useHWSerial)
                Serial.write(packet[i]);
            else
                bitBangWrite(&packet[i], dataPin);
        }
    }
    else
    {
    }
}
