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

#ifndef HFCL_COMMON_CONTEXTSTREAM_H_
#define HFCL_COMMON_CONTEXTSTREAM_H_

namespace hfcl {

class ContextStream
{
public:
    ContextStream();
    ~ContextStream();
    void reset();

    ContextStream& operator<<(const int data);
    ContextStream& operator>>(int& data);

    ContextStream& operator<<(const short data);
    ContextStream& operator>>(short& data);

    ContextStream& operator<<(const char data);
    ContextStream& operator>>(char& data);

    ContextStream& operator<<(const bool data);
    ContextStream& operator>>(bool& data);

    ContextStream& operator<<(const long long data);
    ContextStream& operator>>(long long& data);

    ContextStream& operator<<(const float data);
    ContextStream& operator>>(float& data);

    ContextStream& operator<<(const double data);
    ContextStream& operator>>(double& data);

    ContextStream& operator<<(const char* data);
    ContextStream& operator>>(char*& data);

    ContextStream& input(const char* data, const unsigned short len);
    ContextStream& output(char* data, unsigned short& len);

    ContextStream& operator<<(const void* data);
    ContextStream& operator>>(void*& data);

private:
    void extendBuffer();

    char* m_buf;
    unsigned short m_inPos;
    unsigned short m_outPos;
    unsigned short m_total;
};

} // namespace hfcl

#endif /* HFCL_COMMON_CONTEXTSTREAM_H_ */

