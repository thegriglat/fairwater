#ifndef DATATYPES_H

#include <stdint.h>

// enums

enum TransmissionType
{
    Interrogation = 1,
    Spontaneous = 2
};

enum FrameType : uint8_t
{
    DigitalPoints = 1,
    AnalogPoints = 2,
    DigitalControl = 3,
    Start = 4,
    Stop = 5,
    GeneralInterrogation = 6,
    Ack = 7,
    Nack = 8
};

// data types

class Quality
{
private:
    uint8_t raw = 0;

public:
    Quality(){};
    Quality(uint8_t quality) : raw(quality){};

    bool isValid() const
    {
        return raw & 1; // bit 1
    }

    bool isSubstituted() const
    {
        return raw & 2; // bit 2
    }

    bool isOverflow() const
    {
        return raw & 4; // bit 3
    }

    uint8_t raw() const
    {
        return raw;
    }
};

class DataPoint
{
    int32_t timeTag;
    uint32_t pointId;
    Quality quality;
};

class DigitalPoint : public DataPoint
{
    uint8_t value;
};

class AnalogPoint : public DataPoint
{
    float value;
};

struct FrameHeader
{
    uint32_t length;
    FrameType frameType;
};

struct Frame
{
    FrameHeader header;
    void *data;
};

#define DATATYPES_H
#endif