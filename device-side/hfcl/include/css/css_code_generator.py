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
PROPERTYVALUE_FILE="csspropertyvalue.h"
STYLESHEETDECLARED_FILE="cssdeclared.h"
STYLESHEETINITIAL_FILE="../../src/css/cssinitial.cc"

RE_START_WITH_VALUES = re.compile(r"^\s+values:")
def start_with_values(line):
    if RE_START_WITH_VALUES.match(line) == None:
        return False
    return True

RE_START_WITH_INITIAL = re.compile(r"^\s+initial:")
def start_with_initial(line):
    if RE_START_WITH_INITIAL.match(line) == None:
        return False
    return True

RE_START_WITH_INHERITED = re.compile(r"^\s+inherited:")
def start_with_inherited(line):
    if RE_START_WITH_INHERITED.match(line) == None:
        return False
    return True

RE_START_WITH_APPLIESTO = re.compile(r"^\s+appliesto:")
def start_with_appliesto(line):
    if RE_START_WITH_APPLIESTO.match(line) == None:
        return False
    return True

RE_START_WITH_SPACE = re.compile(r"^\s")
def start_with_space(line):
    if RE_START_WITH_SPACE.match(line) == None:
        return False
    return True

RE_NOT_KEYWORD = re.compile(r"^<\S+>$")
def is_keyword(value):
    if RE_NOT_KEYWORD.match(value) is None:
        return True
    return False

RE_IS_ARRAY = re.compile(r"^<array-\S+>")
RE_IS_ARRAY_STRING = re.compile(r"^<array-string-\S+>")
RE_IS_ARRAY_INTEGER = re.compile(r"^<array-integer-\S+>")
def get_array_type(value):
    if RE_IS_ARRAY.match(value):
        if RE_IS_ARRAY_STRING.match(value):
            return "const char*";
        elif RE_IS_ARRAY_INTEGER.match(value):
            return "int";
        else:
            # for other types (array-number, array-length, and array-percentage), returns HTReal
            return "HTReal";
    return None

def do_subexpand(def_info, definition):
    values = definition.split()
    if len(values) > 0:
        for value in values:
            if value in def_info:
                definition = definition.replace(value, def_info[value])
                return do_subexpand(def_info, definition)

    return definition

def expand_definition(def_info, token, definition):
    token = token.strip()
    word = token.strip("<>")
    definition = definition.replace('%', word)
    def_info[token] = do_subexpand(def_info, definition)

def get_value_list(line):
    fragments = line.split(":")

    if len(fragments) == 2:
        values = fragments[1].strip()
        value_list = values.split()
        if value_list is None or len(value_list) == 0:
            return None
        return value_list

    return None

def get_value(line):
    fragments = line.split(":")

    if len(fragments) == 2:
        value = fragments[1].strip()
        if len(value) == 0:
            return None
        return value

    return None

def set_value_list(property_info, property_token, def_info, values):
    value_list = values.split()
    if len(value_list) > 0:
        for value in value_list:
            if value in def_info:
                values = values.replace(value, def_info[value])

    value_list = values.split()
    property_info[property_token]['values'] = value_list

#    print("values for property %s" % (property_token, ))
#    for value in value_list:
#        print("%s" % (value, ))

def scan_src_file(fsrc):
    def_info = {}
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

        tokens = stripped_line.split(":")
        if len (tokens) > 0 and not is_keyword(tokens[0]):
            expand_definition (def_info, tokens[0], tokens[1])
        else:
            if start_with_values (org_line):
                values = get_value(stripped_line)
                if values is None:
                    print("scan_src_file (Line %d): value list expected (%s)" % (line_no, stripped_line, ))
                    return None
                else:
                    set_value_list(property_info, property_token, def_info, values)

            elif start_with_initial (org_line):
                initial_value = get_value(stripped_line)
                if initial_value is None:
                    print("scan_src_file (Line %d): initial value expected (%s)" % (line_no, stripped_line, ))
                    return None
                else:
                    property_info[property_token]['initial'] = initial_value

            elif start_with_inherited (org_line):
                inherited_value = get_value(stripped_line)
                if inherited_value is None or (inherited_value != "no" and inherited_value != "yes"):
                    print("scan_src_file (Line %d): inherited value (yes/no) expected (%s)" % (line_no, stripped_line, ))
                    return None
                else:
                    property_info[property_token]['inherited'] = inherited_value

            elif start_with_appliesto (org_line):
                appliesto_value = get_value(stripped_line)
                if appliesto_value is None:
                    print("scan_src_file (Line %d): appliesto value expected (%s)" % (line_no, stripped_line, ))
                    return None
                else:
                    property_info[property_token]['appliesto'] = appliesto_value

            elif not start_with_space (org_line):
                property_token = stripped_line
                if property_token in property_info:
                    print("scan_src_file (Line %d): duplicated property name (%s)" % (line_no, stripped_line, ))
                    return None
                property_info[property_token] = {}
            else:
                print("scan_src_file (Line %d): syntax error %s (%s)" % (line_no, property_token, stripped_line, ))
                return None

        line_no = line_no + 1
        org_line = fsrc.readline()

    return property_info

def generate_type_and_keyword_lists(property_info):
    type_dict = {}
    keyword_dict = {}

    for key in property_info:
        value_list = property_info[key]['values']

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

def make_value_macro(property_token, value_token, inherited):
    value_id = value_token.strip('<>')
    value_id = value_id.replace('-', '_')
    value_id = value_id.upper()

    if inherited == "yes":
        inherited = "CSS_PPT_FLAG_INHERITED, "
    else:
        inherited = "CSS_PPT_FLAG_NOT_INHERITED, "

    if is_keyword(value_token):
        return "MAKE_CSS_PPT_VALUE(" + inherited + "PVT_KEYWORD, PVK_" + value_id + ")"
    else:
        return "MAKE_CSS_PPT_VALUE(" + inherited + "PVT_" + value_id + ", PVK_USER_DATA)"

def make_initial_value(property_token, value_token, inherited):
    user_data = None
    values = value_token.split("=")
    if len(values) > 1:
        value_token = values[0]
        user_data = values[1]

    value_id = value_token.strip('<>')
    value_id = value_id.replace('-', '_')
    value_id = value_id.upper()

    if inherited == "yes":
        inherited = "CSS_PPT_FLAG_INHERITED, "
    else:
        inherited = "CSS_PPT_FLAG_NOT_INHERITED, "

    if is_keyword(value_token):
        user_data = None
        return "MAKE_CSS_PPT_VALUE(" + inherited + "PVT_KEYWORD, PVK_" + value_id + ")", user_data
    else:
        return "MAKE_CSS_PPT_VALUE(" + inherited + "PVT_" + value_id + ", PVK_USER_DATA)", user_data

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

    if header_guard != "HFCL_CSS_CSSPROPERTYVALUE_H_":
        fout.write("#include \"../css/csspropertyvalue.h\"\n")
    if header_guard == "HFCL_CSS_CSSDECLARED_H_":
        fout.write("#include \"../css/css.h\"\n")

    fout.write("\n")
    fout.write("namespace hfcl {\n")
    fout.write("\n")

def write_values(fout, property_info, type_list, keyword_list):
    property_tokens = list(property_info.keys())
    property_tokens.sort()

    fout.write("// The CSS property identifiers\n")
    fout.write("typedef enum _CssPropertyIds {\n")
    for i in range(0, len(property_tokens)):
        fout.write("    // %s\n" %(property_tokens[i], ))
        fout.write("    %s,\n" %(make_property_id(property_tokens[i]), ))
    fout.write("    MAX_CSS_PID,\n")
    fout.write("} CssPropertyIds;\n")
    fout.write("\n")

    fout.write("// The CSS property value types\n")
    fout.write("typedef enum _CssPropertyValueTypes {\n")
    fout.write("    PVT_NONE = 0,\n")
    fout.write("    PVT_KEYWORD,\n")
    for i in range(0, len(type_list)):
        fout.write("    %s,\n" %(make_value_type_id(type_list[i]), ))
    fout.write("    MAX_CSS_PVT,\n")
    fout.write("} CssPropertyValueTypes;\n")
    fout.write("\n")

    fout.write("// The CSS property value keywords\n")
    fout.write("typedef enum _CssPropertyValueKeywords {\n")
    fout.write("    PVK_UNDEFINED = 0,\n")
    for i in range(0, len(keyword_list)):
        fout.write("    %s,\n" %(make_value_keyword_id(keyword_list[i]), ))
    fout.write("    PVK_USER_DATA,\n")
    fout.write("    MAX_CSS_PVK,\n")
    fout.write("} CssPropertyValueKeywords;\n")
    fout.write("\n")

    fout.write("// The CSS property values\n")
    fout.write("\n")

    fout.write("#define CSS_PPT_FLAG_INHERITED      0x80000000\n")
    fout.write("#define CSS_PPT_FLAG_NOT_INHERITED  0x00000000\n")
    fout.write("#define CSS_PPT_INHERITED(value)    ((value) & CSS_PPT_FLAG_INHERITED)\n")
    fout.write("\n")
    fout.write("#define MAKE_CSS_PPT_VALUE(inherited, type, keyword)    \\\n")
    fout.write("    ((DWORD32)(                                         \\\n")
    fout.write("        ((DWORD32)((keyword) & 0xFFFF)) |               \\\n")
    fout.write("        ((DWORD32)((type) & 0x0FFF) << 16) |            \\\n")
    fout.write("        ((DWORD32)(inherited))                          \\\n")
    fout.write("    ))\n")
    fout.write("\n")
    fout.write("#define CSS_PPT_VALUE_TYPE(value)       (((value) & 0x0FFF0000) >> 16)\n")
    fout.write("#define CSS_PPT_VALUE_KEYWORD(value)    ((value) & 0x0000FFFF)\n")
    fout.write("\n")
    fout.write("#define CSS_PPT_VALUE_FLAG_INHERITED    0x40000000\n")
    fout.write("#define CSS_PPT_VALUE_INHERITED(value) \\\n")
    fout.write("    ((value) & CSS_PPT_VALUE_FLAG_INHERITED)\n")
    fout.write("#define MARK_CSS_PPT_VALUE_INHERITED(value) \\\n")
    fout.write("    ((value) |= CSS_PPT_VALUE_FLAG_INHERITED)\n")
    fout.write("\n")
    fout.write("#define CSS_PPT_VALUE_FLAG_COMPUTED     0x20000000\n")
    fout.write("#define CSS_PPT_VALUE_COMPUTED(value) \\\n")
    fout.write("    ((value) & CSS_PPT_VALUE_FLAG_COMPUTED)\n")
    fout.write("#define MARK_CSS_PPT_VALUE_COMPUTED(value) \\\n")
    fout.write("    ((value) |= CSS_PPT_VALUE_FLAG_COMPUTED)\n")
    fout.write("\n")
    for i in range(0, len(property_tokens)):

        value_list = property_info[property_tokens[i]]['values']
        value_list.sort()
        inherited = property_info[property_tokens[i]]['inherited']

        fout.write("// Values for property %s\n" % (property_tokens[i], ))
        for value in value_list:
            fout.write("#define %s \\\n" % (make_value_id (property_tokens[i], value), ))
            fout.write("    %s\n" % (make_value_macro (property_tokens[i], value, inherited), ))
        fout.write("\n")

    fout.write("\n")

    return property_tokens

def write_class_propertyvalue(fout, property_tokens, property_info):
    fout.write("class CssPropertyValue {\n")
    fout.write("public:\n")
    fout.write("    CssPropertyValue();\n")
    fout.write("    CssPropertyValue(DWORD32 value, HTData data = 0) {\n")
    fout.write("        m_value = value; m_data = data;\n")
    fout.write("    }\n")
    fout.write("    ~CssPropertyValue() {};\n")
    fout.write("\n")
    fout.write("    bool isInheritable() const { return (bool)CSS_PPT_INHERITED(m_value); }\n")
    fout.write("    bool isInherited() const { return (bool)CSS_PPT_VALUE_INHERITED(m_value); }\n")
    fout.write("    bool isComputed() const { return (bool)CSS_PPT_VALUE_COMPUTED(m_value); }\n")
    fout.write("    DWORD32 getValue() const { return m_value; }\n")
    fout.write("    int getType() const { return CSS_PPT_VALUE_TYPE(m_value); }\n")
    fout.write("    int getKeyword() const { return CSS_PPT_VALUE_KEYWORD(m_value); }\n")
    fout.write("    HTData getData() const { return m_data; }\n")
    fout.write("    void setValue(DWORD32 value, HTData data = 0) {\n")
    fout.write("        m_value = value; m_data = data;\n")
    fout.write("    }\n")
    fout.write("\n")
    fout.write("private:\n")
    fout.write("    DWORD32 m_value;\n")
    fout.write("    HTData m_data;\n")
    fout.write("};\n")

def write_class_cssdeclared(fout, property_tokens):
    fout.write("class CssDeclared : public Css {\n")
    fout.write("public:\n")
    fout.write("    CssDeclared (const char* selector) : m_selector (selector) {}\n")
    fout.write("    ~CssDeclared ();\n")
    fout.write("\n")

    for i in range(0, len(property_tokens)):

        value_list = property_info[property_tokens[i]]['values']
        value_list.sort()

        fout.write("    // Operator for property %s\n" % (property_tokens[i], ))
        fout.write("    bool %s (DWORD32 value, HTData addData = 0) {\n" % (make_operator_name (property_tokens[i]), ))
        fout.write("        return setProperty (%s, value, addData);\n" % (make_property_id (property_tokens[i]), ))
        fout.write("    }\n")
        fout.write("\n")

    fout.write("    // The helper functions for setting margin-* properties in batches.\n")
    fout.write("    bool setMargin (DWORD32 value, HTData addData = 0);\n")
    fout.write("\n")
    fout.write("    // The helper functions for setting padding-* properties in batches.\n")
    fout.write("    bool setPadding (DWORD32 value, HTData addData = 0);\n")
    fout.write("\n")
    fout.write("    virtual bool getProperty (CssPropertyIds pid, DWORD32 *value,\n")
    fout.write("            HTData *data = NULL);\n")
    fout.write("    virtual bool setProperty (CssPropertyIds pid, DWORD32 value,\n")
    fout.write("            HTData data = 0);\n")
    fout.write("\n")
    fout.write("protected:\n")
    fout.write("    const char* m_selector;\n")
    fout.write("\n")
    fout.write("private:\n")
    fout.write("    /* VincentWei:\n")
    fout.write("     * for map, the size of key type must be aligned at intptr_t.\n")
    fout.write("     * Or it->second cannot work correctly, you will get SEGV.\n")
    fout.write("     */\n")
    fout.write("    MAP(intptr_t, CssPropertyValue*, CssPropertyValueMap);\n")
    fout.write("\n")
    fout.write("    CssPropertyValueMap m_map;\n")
    fout.write("};\n")

def write_footer(fout, header_guard):
    fout.write("\n")
    fout.write("} // namespace hfcl\n")
    fout.write("\n")
    fout.write("#endif /* %s */\n" % (header_guard, ))

def write_class_cssinitial(fout, property_info):
    property_tokens = list(property_info.keys())
    property_tokens.sort()

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
    fout.write("#include \"css/cssinitial.h\"\n")
    fout.write("\n")
    fout.write("namespace hfcl {\n")
    fout.write("\n")
    fout.write("CssInitial* CssInitial::s_singleton = NULL;\n")
    fout.write("\n")
    fout.write("CssInitial::CssInitial ()\n")
    fout.write("{\n")

    fout.write("\n")
    fout.write("    memset(&m_data, 0, sizeof(m_data));\n")
    fout.write("\n")

    fout.write("    // Initial values\n")
    for property_token in property_tokens:
        pid = make_property_id(property_token);
        value_token = property_info[property_token]['initial']
        inherited = property_info[property_token]['inherited']
        initial_value, user_data = make_initial_value (property_token, value_token, inherited)
        fout.write("    m_values[%s] = %s;\n" % (pid, initial_value, ))
        if user_data is not None:
            array_type = get_array_type(value_token)
            if array_type is None:
                fout.write("    m_data[%s] = (HTData)%s;\n" % (pid, user_data, ))
            else:
                fout.write("    {\n")
                fout.write("        static %s _user_data[] = %s;\n" % (array_type, user_data, ))
                fout.write("        m_data[PID_MARGIN] = (HTData)&_user_data;\n")
                fout.write("    }\n")
        fout.write("\n")

    fout.write("}\n")
    fout.write("\n")
    fout.write("} // namespace hfcl\n")

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

    print("Generating type and keyword list...")
    type_list, keyword_list = generate_type_and_keyword_lists(property_info)
    type_list.sort()
    keyword_list.sort()

#    print("types:")
#    for type_token in type_list:
#        print("    %s" % (type_token, ))
#
#    print("keywords:")
#    for keyword_token in keyword_list:
#        print("    %s" % (keyword_token, ))

    print("DONE")

    print("Writting header to dst file %s..." % PROPERTYVALUE_FILE)
    try:
        write_header(fdst, "HFCL_CSS_CSSPROPERTYVALUE_H_")
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
        write_footer(fdst, "HFCL_CSS_CSSPROPERTYVALUE_H_")
    except:
        print("FAILED")
        sys.exit(7)
    fdst.close()
    print("DONE")

    try:
        fdst = open(STYLESHEETDECLARED_FILE, "w")
    except:
        print("%s: failed to open output file %s" % (TOOL_NAME, STYLESHEETDECLARED_FILE, ))
        sys.exit(8)

    print("Writting header to dst file %s..." % STYLESHEETDECLARED_FILE)
    try:
        write_header(fdst, "HFCL_CSS_CSSDECLARED_H_")
    except:
        print("FAILED")
        sys.exit(9)
    print("DONE")

    print("Writting class CssDeclared to dst file %s..." % STYLESHEETDECLARED_FILE)
    try:
        write_class_cssdeclared(fdst, property_tokens)
    except:
        print("FAILED")
        sys.exit(10)
    print("DONE")

    print("Writting footer to dst file %s..." % STYLESHEETDECLARED_FILE)
    try:
        write_footer(fdst, "HFCL_CSS_CSSDECLARED_H_")
    except:
        print("FAILED")
        sys.exit(11)
    fdst.close()
    print("DONE")

    try:
        fdst = open(STYLESHEETINITIAL_FILE, "w")
    except:
        print("%s: failed to open output file %s" % (TOOL_NAME, STYLESHEETINITIAL_FILE, ))
        sys.exit(12)

    print("Writting class CssInitial to dst file %s..." % STYLESHEETINITIAL_FILE)
    try:
        write_class_cssinitial(fdst, property_info)
    except:
        print("FAILED")
        sys.exit(13)
    fdst.close()
    print("DONE")

    sys.exit(0)

