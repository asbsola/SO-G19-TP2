// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <utils/utils.h>

static uint32_t m_z = 362436069;
static uint32_t m_w = 521288629;

uint32_t GetUint() {
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

uint32_t GetUniform(uint32_t max) {
    uint32_t u = GetUint();
    return (u + 1.0) * 2.328306435454494e-10 * max;
}