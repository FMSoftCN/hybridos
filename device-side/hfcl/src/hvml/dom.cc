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

/*
** dom.cc: The implementation of basic DOM interfaces including:
**
**  - DOMException
**    https://heycam.github.io/webidl/#idl-DOMException
**
** Create by WEI Yongming at 2019/05/12
*/

#include "hvml/dom.h"

#include <minigui/common.h>

namespace hfcl {
namespace dom {

static const char* _error_names_table [] =
{
    "UnknownError",             // placeholder for error code 0.
    "IndexSizeError",           // Deprecated. Use RangeError instead. INDEX_SIZE_ERR (1)
    "DOMStringSizeError",       // Deprecated. Use RangeError instead. DOMSTRING_SIZE_ERR (2)
    "HierarchyRequestError",    // The operation would yield an incorrect node tree. [DOM] HIERARCHY_REQUEST_ERR (3)
    "WrongDocumentError",       // The object is in the wrong document. [DOM]  WRONG_DOCUMENT_ERR (4)
    "InvalidCharacterError",    // The string contains invalid characters. INVALID_CHARACTER_ERR (5)
    "NoDataAllowedError",       // Deprecated. NO_DATA_ALLOWED_ERR (6)
    "NoModificationAllowedError",   // The object can not be modified. NO_MODIFICATION_ALLOWED_ERR (7)
    "NotFoundError",            // The object can not be found here.   NOT_FOUND_ERR (8)
    "NotSupportedError",        // The operation is not supported. NOT_SUPPORTED_ERR (9)
    "InUseAttributeError",      // The attribute is in use.    INUSE_ATTRIBUTE_ERR (10)
    "InvalidStateError",        // The object is in an invalid state.  INVALID_STATE_ERR (11)
    "SyntaxError",              // The string did not match the expected pattern.  SYNTAX_ERR (12)
    "InvalidModificationError", // The object can not be modified in this way. INVALID_MODIFICATION_ERR (13)
    "NamespaceError",           // The operation is not allowed by Namespaces in XML.  [XML-NAMES] NAMESPACE_ERR (14)
    "InvalidAccessError",       // Deprecated. Use TypeError for invalid arguments, "NotSupportedError" DOMException for unsupported operations, and "NotAllowedError" DOMException for denied requests instead.   INVALID_ACCESS_ERR (15)
    "ValidationError",          // Deprecated. VALIDATION_ERR (16)
    "TypeMismatchError",        // Deprecated. Use TypeError instead.  TYPE_MISMATCH_ERR (17)
    "SecurityError",            // The operation is insecure.  SECURITY_ERR (18)
    "NetworkError",             // A network error occurred.   NETWORK_ERR (19)
    "AbortError",               // The operation was aborted.  ABORT_ERR (20)
    "URLMismatchError",         // The given URL does not match another URL.   URL_MISMATCH_ERR (21)
    "QuotaExceededError",       // The quota has been exceeded.    QUOTA_EXCEEDED_ERR (22)
    "TimeoutError",             // The operation timed out.    TIMEOUT_ERR (23)
    "InvalidNodeTypeError",     // The supplied node is incorrect or has an incorrect ancestor for this operation. INVALID_NODE_TYPE_ERR (24)
    "DataCloneError",           // The object can not be cloned.   DATA_CLONE_ERR (25)
    "EncodingError",            // The encoding operation (either encoded or decoding) failed. —
    "NotReadableError",         // The I/O read operation failed.  —
    "UnknownError",             // The operation failed for an unknown transient reason (e.g. out of memory).  —
    "ConstraintError",          // A mutation operation in a transaction failed because a constraint was not satisfied.    —
    "DataError",                // Provided data is inadequate.    —
    "TransactionInactiveError", // A request was placed against a transaction which is currently not active, or which is finished. —
    "ReadOnlyError",            // The mutating operation was attempted in a "readonly" transaction.   —
    "VersionError",             // An attempt was made to open a database using a lower version than the existing version. —
    "OperationError",           // The operation failed for an operation-specific reason.  —
    "NotAllowedError",          // The request is not allowed by the user agent or the platform in the current context, possibly because the user denied permission.   —
};

DOMException::DOMException(const char* message, const char* name)
{
    m_name = name;
    m_message = message;

    m_code = UNKNOWN_ERR;
    // TODO: optimize
    // set m_name from name by looking up the _error_names_table.

    for (size_t i = 0; i < TABLESIZE(_error_names_table); i++) {
        if (strcmp(name, _error_names_table[i]) == 0) {
            m_code = i;
            break;
        }
    }
}

const char* DOMException::what() const throw()
{
    return m_name.c_str();
}

} // namespace dom
} // namespace hfcl

