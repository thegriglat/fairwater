#include "datatypes.hh"
#include "stdlib.h"
#include <iostream>
#include <string>
#include <chrono>

std::string Quality::str() const
{
    std::string s = "[";
    if (isValid())
        s += "Valid";
    else
        s += "Invalid";
    if (isSubstituted())
        s += ", Substituted";
    if (isOverflow())
        s += ", Overflow";
    s += "]";
    return s;
}

std::string timestamp2str(uint32_t timestamp)
{
    std::time_t temp = timestamp;
    char buf[50];
    strftime(buf, 50, "%d-%m-%Y %H:%M:%S", localtime(&temp));
    std::string s = buf;
    return s;
}

Frame::~Frame()
{
    /*
    if (digital_points)
    {
        delete[] digital_points;
    }
    if (analog_points)
    {
        delete[] analog_points;
    }
    */
}

void DigitalPoint::print() const
{
    std::cout << "PointId=" << pointId << ", "
              << "Value=" << (int)value << ", "
              << "TimeTag=" << timestamp2str(timeTag) << ", "
              << "Quality=" << quality.str() << std::endl;
}

void Frame::print() const
{
    if (header.frameType == FrameType::DigitalPoints)
    {
        for (int i = 0; i < payload_header.count; ++i)
        {
            digital_points[i].print();
        }
    }
}