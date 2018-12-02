/* 
** HFCL - HybridOS Foundation Class Library
** 
** Copyright (C) 2018 Beijing FMSoft Technologies Co., Ltd.
**
** This file is part of HFCL.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
 /*
  ** $Id: ContextStream.cpp 2365 2012-01-17 09:30:08Z wangxin $
  **
  ** ContextStream.cpp:
  **
  ** Copyright (C) 2003 ~ 2011 Beijing FMSoft Technologies Co., Ltd.
  **
  ** All rights reserved by FMSoft.
  **
  ** Current maintainer: shidazhi
  **
  ** Create date: 2011-06-22
  */

#include "nguxcommon.h"
#include "log.h"
#include <string.h>
#include "contextstream.h"

NAMESPACE_BEGIN

#define STREAM_BUF_LEN         256
#define STRING_LENGTH_LEN      2

ContextStream::ContextStream()
{
    m_buf = NGUX_NEW_ARR(char, (STREAM_BUF_LEN * sizeof(char)));
    NGUX_ASSERT(m_buf != NULL);
    m_total = STREAM_BUF_LEN;
    m_inPos  = 0;
    m_outPos = 0;
}

ContextStream::~ContextStream()
{
    if (NULL != m_buf) {
        NGUX_DELETE_ARR(m_buf);
        m_buf = NULL;
    }
}

void ContextStream::reset()
{
    memset(m_buf, 0, m_total);
    m_inPos  = 0;
    m_outPos = 0;
}

ContextStream& ContextStream::operator<<(const int data)
{
    if (m_total < m_inPos + sizeof(data)) {
        extendBuffer();
    }
    memcpy(m_buf + m_inPos, &data, sizeof(data));
    m_inPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator>>(int& data)
{
    memcpy(&data, m_buf + m_outPos, sizeof(data));
    m_outPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator<<(const long long data)
{
    if (m_total < m_inPos + sizeof(data)) {
        extendBuffer();
    }
    memcpy(m_buf + m_inPos, &data, sizeof(data));
    m_inPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator>>(long long& data)
{
    memcpy(&data, m_buf + m_outPos, sizeof(data));
    m_outPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator<<(const double data)
{
    if (m_total < m_inPos + sizeof(data)) {
        extendBuffer();
    }
    memcpy(m_buf + m_inPos, &data, sizeof(data));
    m_inPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator>>(double& data)
{
    memcpy(&data, m_buf + m_outPos, sizeof(data));
    m_outPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator<<(const short data)
{
    if (m_total < m_inPos + sizeof(data)) {
        extendBuffer();
    }
    memcpy(m_buf + m_inPos, &data, sizeof(data));
    m_inPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator>>(short& data)
{
    memcpy(&data, m_buf + m_outPos, sizeof(data));
    m_outPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator<<(const char data)
{
    if (m_total < m_inPos + sizeof(data)) {
        extendBuffer();
    }
    memcpy(m_buf + m_inPos, &data, sizeof(data));
    m_inPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator>>(char& data)
{
    memcpy(&data, m_buf + m_outPos, sizeof(data));
    m_outPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator<<(const bool data)
{
    if (m_total < m_inPos + sizeof(data)) {
        extendBuffer();
    }
    memcpy(m_buf + m_inPos, &data, sizeof(data));
    m_inPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator>>(bool& data)
{
    memcpy(&data, m_buf + m_outPos, sizeof(data));
    m_outPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator<<(const float data)
{
    if (m_total < m_inPos + sizeof(data)) {
        extendBuffer();
    }
    memcpy(m_buf + m_inPos, &data, sizeof(data));
    m_inPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator>>(float& data)
{
    memcpy(&data, m_buf + m_outPos, sizeof(data));
    m_outPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator<<(const char* data)
{
//    LOGDEBUG("this string is:  %s\n", data);
    if (m_total < m_inPos + sizeof(data)) {
        extendBuffer();
    }
    memcpy(m_buf + m_inPos, &data, sizeof(data));
    m_inPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator>>(char*& data)
{
    memcpy(&data, m_buf + m_outPos, sizeof(data));
    m_outPos += sizeof(data);
//    LOGDEBUG("this string2 is:  %s\n", data);
    return *this;
}

ContextStream& ContextStream::operator<<(const void* data)
{
    if (m_total < m_inPos + sizeof(data)) {
        extendBuffer();
    }
    memcpy(m_buf + m_inPos, &data, sizeof(data));
    m_inPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::operator>>(void*& data)
{
    memcpy(&data, m_buf + m_outPos, sizeof(data));
    m_outPos += sizeof(data);
    return *this;
}

ContextStream& ContextStream::input(const char* data, const unsigned short len)
{
    while (m_total < m_inPos + len + STRING_LENGTH_LEN) {
        extendBuffer();
    }

    memcpy(m_buf + m_inPos, &len, STRING_LENGTH_LEN);
    m_inPos += STRING_LENGTH_LEN;
    if (len > 0) {
        memcpy(m_buf + m_inPos, data, len);
        m_inPos += len;
    }
    return *this;
}

ContextStream& ContextStream::output(char* data, unsigned short& len)
{
    memcpy(&len, m_buf + m_outPos, STRING_LENGTH_LEN);
    m_outPos += STRING_LENGTH_LEN;
    memcpy(data, m_buf + m_outPos, len);
    m_outPos += len;
    return *this;
}

inline void ContextStream::extendBuffer()
{
    int size = (m_total + STREAM_BUF_LEN) * sizeof(char);
    char* tmp_buf = NGUX_NEW_ARR(char, (size));
    NGUX_ASSERT(tmp_buf != NULL);
    memset(tmp_buf, 0x00, size);
    memcpy(tmp_buf, m_buf, m_total * sizeof(char));
    m_total += STREAM_BUF_LEN;
    if (NULL != m_buf)
        NGUX_DELETE_ARR(m_buf);
    m_buf = tmp_buf;
}

NAMESPACE_END

