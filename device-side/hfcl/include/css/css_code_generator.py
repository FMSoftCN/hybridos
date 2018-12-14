#!/usr/bin/python3
"""
Make CSS property value macros:
    1. Read 'propertylist.txt' file.
    2. Generate CSS property identifier list(enum).
    3. Generate CSS property value keyword list(enum).
    4. Generate CSS property value macros.
    5. Write code to 'csspropertyvalue.h' file.
"""

import os, sys
import time
import re

TOOL_NAME="css_code_generator.py"
SRC_FILE="propertylist.txt"
PROPERTYVALUE_FILE="propertyvalue.h"
STYLESHEETDECLARED_FILE="stylesheetdeclared.h"

RE_START_WITH_SPACE = re.compile(r"^\s")
def start_with_space(value):
    if RE_START_WITH_SPACE.match(value) == None:
        return False
    return True

def scan_src_file(fsrc):
    property_info = {}

    property_token = ""
    value_line = ""
    line_no = 1
    org_line = fsrc.readline()
    while org_line:
        stripped_line = org_line.strip()
        if stripped_line == "" or stripped_line[0] == '#':
            line_no = line_no + 1
            org_line = fsrc.readline()
            continue

        if property_token == "":
            if start_with_space (org_line):
                print("scan_src_file (Line %d): property name expected without leading space (%s)" % (line_no, stripped_line, ))
                return None
            else:
                property_token = stripped_line
        else:
            if start_with_space (org_line):
                value_line = stripped_line
            else:
                print("scan_src_file (Line %d): value list expected with leading space for property %s (%s)" % (line_no, property_token, stripped_line, ))
                return None

        if property_token != "" and value_line != "":
            if property_token in property_info:
                print("scan_src_file (Line %d): duplicated property name (%s)" % (line_no, stripped_line, ))
                return None

            value_list = value_line.split()
            property_info[property_token] = value_list

            print("property name (%s)" % (property_token, ))
            for value in value_list:
                print("    value: %s" % (value, ))

            property_token = ""
            value_line = ""

        org_line = fsrc.readline()

    return property_info

RE_NOT_KEYWORD = re.compile(r"^<\S+>$")
def is_keyword(value):
    if RE_NOT_KEYWORD.match(value) is None:
        return True
    return False

def generate_type_and_keyword_lists(property_info):
    type_dict = {}
    keyword_dict = {}

    for key in property_info:
        value_list = property_info[key]

        for j in range(0, len(value_list)):
            if is_keyword(value_list[j]):
                if value_list[j] not in keyword_dict:
                    keyword_dict[value_list[j]] = "";
            else:
                if value_list[j] not in type_dict:
                    type_dict[value_list[j]] = "";

    return list(type_dict.keys()), list(keyword_dict.keys())

def make_property_id(property_token):
    property_id = property_token.upper()
    property_id = property_id.replace('-', '_')

    return "PID_" + property_id;

def make_value_type_id(type_token):
    type_id = type_token.strip('<>')
    type_id = type_id.replace('-', '_')
    type_id = type_id.upper()
    return "PVT_" + type_id;

def make_value_keyword_id(keyword_token):
    keyword_id = keyword_token.upper()
    keyword_id = keyword_id.replace('-', '_')
    return "PVK_" + keyword_id;

def make_value_id(property_token, value_token):
    property_id = property_token.upper()
    property_id = property_id.replace('-', '_')

    value_id = value_token.strip('<>')
    value_id = value_id.replace('-', '_')
    value_id = value_id.upper()

    return "PV_" + property_id + "_" + value_id;

def make_value_macro(property_token, value_token):
    property_id = property_token.upper()
    property_id = property_id.replace('-', '_')

    value_id = value_token.strip('<>')
    value_id = value_id.replace('-', '_')
    value_id = value_id.upper()

    if is_keyword(value_token):
        return "MAKE_PROPERTY_VALUE(PVT_KEYWORD, PVK_" + value_id + ")"
    else:
        return "MAKE_PROPERTY_VALUE(PVT_" + value_id + ", PVK_USE_DATA"

def make_operator_name(property_token):
    operator_name = ''.join(x for x in property_token.title() if x.isalnum())
    return "set" + operator_name[0] + operator_name[1:]

def write_header(fout, header_guard):
    fout.write("/*\n")
    fout.write("** HFCL - HybridOS Foundation Class Library\n")
    fout.write("**\n")
    fout.write("** Copyright (C) 2018 Beijing FMSoft Technologies Co., Ltd.\n")
    fout.write("**\n")
    fout.write("** This file is part of HFCL.\n")
    fout.write("**\n")
    fout.write("** This program is free software: you can redistribute it and/or modify\n")
    fout.write("** it under the terms of the GNU General Public License as published by\n")
    fout.write("** the Free Software Foundation, either version 3 of the License, or\n")
    fout.write("** (at your option) any later version.\n")
    fout.write("**\n")
    fout.write("** This program is distributed in the hope that it will be useful,\n")
    fout.write("** but WITHOUT ANY WARRANTY; without even the implied warranty of\n")
    fout.write("** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n")
    fout.write("** GNU General Public License for more details.\n")
    fout.write("**\n")
    fout.write("** You should have received a copy of the GNU General Public License\n")
    fout.write("** along with this program.  If not, see <https://www.gnu.org/licenses/>.\n")
    fout.write("*/\n")
    fout.write("\n")
    fout.write("// NOTE\n")
    fout.write("// This file is auto-generated by using '%s'.\n" % (TOOL_NAME, ))
    fout.write("// Please take care when you modify this file mannually.\n")
    fout.write("\n")
    fout.write("#ifndef %s\n" % (header_guard, ))
    fout.write("#define %s\n" % (header_guard, ))
    fout.write("\n")
    fout.write("#include \"../common/common.h\"\n")
    fout.write("#include \"../common/stlalternative.h\"\n")

    if header_guard != "HFCL_CSS_PROPERTYVALUE_H_":
        fout.write("#include \"propertyvalue.h\"\n")
    if header_guard == "HFCL_CSS_STYLESHEETDECLARED_H_":
        fout.write("#include \"stylesheet.h\"\n")

    fout.write("\n")
    fout.write("namespace hfcl {\n")
    fout.write("\n")

def write_values(fout, property_info, type_list, keyword_list):
    property_tokens = list(property_info.keys())
    property_tokens.sort()

    fout.write("// The property identifiers\n")
    fout.write("typedef enum _PropertyIds {\n")
    for i in range(0, len(property_tokens)):
        fout.write("    // %s\n" %(property_tokens[i], ))
        fout.write("    %s,\n" %(make_property_id(property_tokens[i]), ))
    fout.write("    MAX_PID,\n")
    fout.write("} PropertyIds;\n")
    fout.write("\n")

    fout.write("// The property value types\n")
    fout.write("typedef enum _PropertyValueTypes {\n")
    fout.write("    PVT_KEYWORD = 0,\n")
    for i in range(0, len(type_list)):
        fout.write("    %s,\n" %(make_value_type_id(type_list[i]), ))
    fout.write("    MAX_PVT,\n")
    fout.write("} PropertyValueTypes;\n")
    fout.write("\n")

    fout.write("// The property value keywords\n")
    fout.write("typedef enum _PropertyValueKeywords {\n")
    for i in range(0, len(keyword_list)):
        fout.write("    %s,\n" %(make_value_keyword_id(keyword_list[i]), ))
    fout.write("    PVK_USER_DATA,\n")
    fout.write("    MAX_PVK,\n")
    fout.write("} PropertyValueKeywords;\n")
    fout.write("\n")

    fout.write("// The property values\n")
    fout.write("\n")
    fout.write("#define MAKE_PROPERTY_VALUE(type, keyword)      \\\n")
    fout.write("    ((DWORD32)(                                 \\\n")
    fout.write("        ((DWORD32)((keyword) & 0xFFFF)) |       \\\n")
    fout.write("        ((DWORD32)((type) & 0xFFFF) << 16)      \\\n")
    fout.write("    ))\n")
    fout.write("\n")
    fout.write("#define PROPERTY_VALUE_TYPE(value)      (((value) & 0xFFFF0000) >> 16)\n")
    fout.write("#define PROPERTY_VALUE_KEYWORD(value)   ((value) & 0x0000FFFF)\n")
    fout.write("\n")

    for i in range(0, len(property_tokens)):

        value_list = property_info[property_tokens[i]]
        value_list.sort()

        fout.write("// Values for for property %s\n" % (property_tokens[i], ))
        for value in value_list:
            fout.write("#define %s \\\n" % (make_value_id (property_tokens[i], value), ))
            fout.write("    %s\n" % (make_value_macro (property_tokens[i], value), ))
        fout.write("\n")

    fout.write("\n")

    return property_tokens

def write_class_propertyvalue(fout, property_tokens, property_info):
    fout.write("class PropertyValue {\n")
    fout.write("public:\n")
    fout.write("    PropertyValue();\n")
    fout.write("    PropertyValue(DWORD32 value, HTData data = 0);\n")
    fout.write("    ~PropertyValue() {};\n")
    fout.write("\n")
    fout.write("    DWORD32 getValue() const { return m_value; }\n")
    fout.write("    int getType() const { return PROPERTY_VALUE_TYPE(m_value); }\n")
    fout.write("    int getKeyword() const { return PROPERTY_VALUE_KEYWORD(m_value); }\n")
    fout.write("    HTData getData() const { return m_data; }\n")
    fout.write("    HTReal getDataAsNumber() const { return (HTReal)m_data; }\n")
    fout.write("    int getDataAsInteger() const { return (int)m_data; }\n")
    fout.write("    const char* getDataAsCString() const { return (const char*)m_data; }\n")
    fout.write("\n")
    fout.write("private:\n")
    fout.write("    DWORD32 m_value;\n")
    fout.write("    HTData m_data;\n")
    fout.write("};\n")

def write_class_stylesheetdeclared(fout, property_tokens):
    fout.write("class StyleSheetDeclared : public StyleSheet {\n")
    fout.write("public:\n")
    fout.write("    StyleSheetDeclared () : {}\n")
    fout.write("    StyleSheetDeclared (int id) : StyleSheet (id) {}\n")
    fout.write("    ~StyleSheetDeclared () {}\n")
    fout.write("\n")

    for i in range(0, len(property_tokens)):

        value_list = property_info[property_tokens[i]]
        value_list.sort()

        fout.write("    // Operator for property %s\n" % (property_tokens[i], ))
        fout.write("    bool %s (DWORD32 value, HTData addData = 0) {\n" % (make_operator_name (property_tokens[i]), ))
        fout.write("        return setProperty (%s, value, addData);\n" % (make_property_id (property_tokens[i]), ))
        fout.write("    }\n")
        fout.write("\n")

    fout.write("protected:\n")
    fout.write("    virtual bool setProperty (PropertyIds id, DWORD32 value, HTData addData = 0);\n")
    fout.write("\n")
    fout.write("private:\n")
    fout.write("    MAPCLASSKEY(PropertyIds, PropertyValue, PropertyValueMap);\n")
    fout.write("    PAIR(PropertyIds, PropertyValue, PropertyValuePair);\n")
    fout.write("\n")
    fout.write("    PropertyValueMap m_map;\n")
    fout.write("};\n")

def write_footer(fout, header_guard):
    fout.write("\n")
    fout.write("} // namespace hfcl\n")
    fout.write("\n")
    fout.write("#endif /* %s */\n" % (header_guard, ))

if __name__ == "__main__":
    try:
        fsrc = open(SRC_FILE, "r")
    except:
        print("%s: failed to open input file %s" % (TOOL_NAME, SRC_FILE, ))
        sys.exit(1)

    try:
        fdst = open(PROPERTYVALUE_FILE, "w")
    except:
        print("%s: failed to open output file %s" % (TOOL_NAME, PROPERTYVALUE_FILE, ))
        sys.exit(2)

    print("Scanning input file %s..." % SRC_FILE)
    property_info = scan_src_file(fsrc)
    if property_info is None:
        print("FAILED")
        sys.exit(3)
    fsrc.close()
    print("DONE")

    print("Generating type and keyword list")
    type_list, keyword_list = generate_type_and_keyword_lists(property_info)
    type_list.sort()
    keyword_list.sort()

    print("types:")
    for type_token in type_list:
        print("    %s" % (type_token, ))

    print("keywords:")
    for keyword_token in keyword_list:
        print("    %s" % (keyword_token, ))

    print("DONE")

    print("Writting header to dst file %s..." % PROPERTYVALUE_FILE)
    try:
        write_header(fdst, "HFCL_CSS_PROPERTYVALUE_H_")
    except:
        print("FAILED")
        sys.exit(4)
    print("DONE")

    print("Writting values to dst file %s..." % PROPERTYVALUE_FILE)
    try:
        property_tokens = write_values(fdst, property_info, type_list, keyword_list)
    except:
        print("FAILED")
        sys.exit(5)
    print("DONE > number of properties: %d; number of types: %d, number of keywords: %d" % (len(property_tokens), len(type_list), len(keyword_list), ))

    print("Generating classes to dst file %s..." % PROPERTYVALUE_FILE)
    try:
        write_class_propertyvalue(fdst, property_tokens, property_info)
    except:
        print("FAILED")
        sys.exit(6)
    print("DONE")

    print("Writting footer to dst file %s..." % PROPERTYVALUE_FILE)
    try:
        write_footer(fdst, "HFCL_CSS_PROPERTYVALUE_H_")
    except:
        print("FAILED")
        sys.exit(7)
    print("DONE")

    fdst.close()

    try:
        fdst = open(STYLESHEETDECLARED_FILE, "w")
    except:
        print("%s: failed to open output file %s" % (TOOL_NAME, STYLESHEETDECLARED_FILE, ))
        sys.exit(8)

    print("Writting header to dst file %s..." % STYLESHEETDECLARED_FILE)
    try:
        write_header(fdst, "HFCL_CSS_STYLESHEETDECLARED_H_")
    except:
        print("FAILED")
        sys.exit(9)
    print("DONE")

    print("Writting class StyleSheetDeclared to dst file %s..." % STYLESHEETDECLARED_FILE)
    try:
        write_class_stylesheetdeclared(fdst, property_tokens)
    except:
        print("FAILED")
        sys.exit(10)
    print("DONE")

    print("Writting footer to dst file %s..." % STYLESHEETDECLARED_FILE)
    try:
        write_footer(fdst, "HFCL_CSS_STYLESHEETDECLARED_H_")
    except:
        print("FAILED")
        sys.exit(11)
    print("DONE")

    sys.exit(0)

