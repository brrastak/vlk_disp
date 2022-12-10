
// user
#include "display.h"

// std
#include <cmath>
#include <cstdlib>
#include <ctime>


#pragma once


namespace mock
{

void SetDisplay(test_setup::Display*);

int rand_value(int max);

class Wave
{
public:
    Wave(float amp, float freq, float phase)
        : m_amp{amp}, m_freq{freq}, m_phase{phase}
    {}

    int32_t getValue(uint32_t time)
    {
        float ampl = m_amp + rand_value(m_amp / 4);
        float phase = m_freq * float(time % 25000) * 3.14 / 2500.0 + m_phase;

        return static_cast<int32_t>(ampl * std::sin(phase));
    }

private:
    float m_amp;
    float m_freq;
    float m_phase;
};

class BitStream
{
public:
    BitStream()
        : m_currentValue{0}, m_bitIndex{numberOfBits - 1}
    {}

    // MSB first
    bool getBit(int32_t newValue)
    {
        bool res = ((m_currentValue & (1 << m_bitIndex)) != 0);

        if (m_bitIndex == 0)
        {
            m_bitIndex = numberOfBits;
            m_currentValue = newValue;
        }

        m_bitIndex--;
        return res;
    }

private:
    static constexpr int numberOfBits = 24;

    int32_t m_currentValue;
    int m_bitIndex;
};

}
