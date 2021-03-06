#include "datatypes.hh"
#include "stdlib.h"
#include <iostream>
#include <string>
#include <iomanip>
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

void DigitalPoint::print() const
{
    std::cout << "PointId=" << pointId << ", "
              << "Value=" << (int)value << ", "
              << "TimeTag=" << timestamp2str(timeTag) << ", "
              << "Quality=" << quality.str() << std::endl;
}

void AnalogPoint::print() const
{
    std::cout << "PointId=" << pointId << ", "
              << std::setprecision(1) << std::fixed << "Value=" << value << ", "
              << "TimeTag=" << timestamp2str(timeTag) << ", "
              << "Quality=" << quality.str() << std::endl;
}

void Frame::print() const
{
    if (header.frameType == FrameType::DigitalPoints)
    {
        for (const auto &elem : digital_points)
            elem.print();
    }
    if (header.frameType == FrameType::AnalogPoints)
    {
        for (const auto &elem : analog_points)
            elem.print();
    }
}