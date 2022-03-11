#ifndef DATATYPES_H

#include <stdint.h>
#include <string>

// enums

enum TransmissionType : uint8_t
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
#pragma pack(push, 1)

class Quality
{
private:
    uint8_t raw = 0;

public:
    Quality(){};
    Quality(uint8_t quality) : raw(quality){};

    inline bool isValid() const
    {
        return raw & (1);
    }

    inline bool isSubstituted() const
    {
        return raw & (1 << 2);
    }

    inline bool isOverflow() const
    {
        return raw & (1 << 1);
    }

    std::string str() const;
};

struct DigitalPoint
{
    uint32_t pointId = 0;
    uint8_t value = 0;
    int64_t timeTag = 0;
    Quality quality = 0;

    void print() const;
};

struct AnalogPoint
{
    uint32_t pointId = 0;
    float value = 0.0;
    int64_t timeTag = 0;
    Quality quality = 0;

    void print() const;
};

struct FrameHeader
{
    uint32_t length = 0;
    FrameType frameType;
};

struct PayloadHeader
{
    uint8_t causeOfTransmission = TransmissionType::Interrogation;
    uint16_t count = 0;
};

struct Frame
{
    FrameHeader header;
    PayloadHeader payload_header;
    DigitalPoint *digital_points = nullptr;
    AnalogPoint *analog_points = nullptr;
    ~Frame();
    void print() const;
};

#pragma pack(pop)
#define DATATYPES_H
#endif