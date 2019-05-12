/*
** HFCL - HybridOS Foundation Class Library
**
** Copyright (C) 2019 Beijing FMSoft Technologies Co., Ltd.
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

#ifndef HFCL_HVML_DOM_H_
#define HFCL_HVML_DOM_H_

#include <iostream>
#include <exception>
#include <string>

// _HFCL_INCLUDE_HISTORICAL_DOM_INTERFACES

namespace hfcl {
namespace dom {

typedef std::string DOMString;
typedef unsigned long long DOMTimeStamp;
typedef double DOMHighResTimeStamp;

class EvalError : public std::exception {
public:
    const char* what() const throw() {
        return "EvalError";
    }
};

class RangeError : public std::exception {
public:
    const char* what() const throw() {
        return "RangeError";
    }
};

class ReferenceError : public std::exception {
public:
    const char* what() const throw() {
        return "ReferenceError";
    }
};

class TypeError : public std::exception {
public:
    const char* what() const throw() {
        return "TypeError";
    }
};

class URIError : public std::exception {
public:
    const char* what() const throw() {
        return "URIError";
    }
};

/**
 * https://heycam.github.io/webidl/#idl-DOMException
 */
class DOMException : public std::exception {
public:
    DOMException(const DOMString& message = "" , const DOMString& name = "Error") {
        m_message = message;
        m_name = name;
        m_code = NO_ERR;
    }

    /* readonly attribute DOMString name; */
    const DOMString& getName() const {
        return m_name;
    }

    /* readonly attribute DOMString message; */
    const DOMString& getMessage() const {
        return m_message;
    }

    /* readonly attribute unsigned short code; */
    unsigned short getCode() const {
        return m_code;
    }

    const unsigned short NO_ERR = 0;
    const unsigned short INDEX_SIZE_ERR = 1;
    const unsigned short DOMSTRING_SIZE_ERR = 2;
    const unsigned short HIERARCHY_REQUEST_ERR = 3;
    const unsigned short WRONG_DOCUMENT_ERR = 4;
    const unsigned short INVALID_CHARACTER_ERR = 5;
    const unsigned short NO_DATA_ALLOWED_ERR = 6;
    const unsigned short NO_MODIFICATION_ALLOWED_ERR = 7;
    const unsigned short NOT_FOUND_ERR = 8;
    const unsigned short NOT_SUPPORTED_ERR = 9;
    const unsigned short INUSE_ATTRIBUTE_ERR = 10;
    const unsigned short INVALID_STATE_ERR = 11;
    const unsigned short SYNTAX_ERR = 12;
    const unsigned short INVALID_MODIFICATION_ERR = 13;
    const unsigned short NAMESPACE_ERR = 14;
    const unsigned short INVALID_ACCESS_ERR = 15;
    const unsigned short VALIDATION_ERR = 16;
    const unsigned short TYPE_MISMATCH_ERR = 17;
    const unsigned short SECURITY_ERR = 18;
    const unsigned short NETWORK_ERR = 19;
    const unsigned short ABORT_ERR = 20;
    const unsigned short URL_MISMATCH_ERR = 21;
    const unsigned short QUOTA_EXCEEDED_ERR = 22;
    const unsigned short TIMEOUT_ERR = 23;
    const unsigned short INVALID_NODE_TYPE_ERR = 24;
    const unsigned short DATA_CLONE_ERR = 25;

    const char* what() const throw();

private:
    DOMString m_name;
    DOMString m_message;
    unsigned short m_code;
};

} // namespace dom
} // namespace hfcl

#endif /* HFCL_HVML_DOM_H_ */

