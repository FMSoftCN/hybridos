#!/usr/bin/python3
"""
Make CSS property value macros:
    1. Read 'propertylist.txt' file.
    2. Generate CSS property identifier list(enum).
    3. Generate CSS property value keyword list(enum).
    4. Generate CSS property value macros.
    5. Write code to propertiesandvalues.inc file.
"""
import os, sys
import time
import re

SRC_FILE="propertylist.txt"
DST_FILE="propertiesandvalues.inc"

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
        return "MAKE_PROPERTY_VALUE(PID_" + property_id + ", PVT_KEYWORD, PVK_" + value_id + ")"
    else:
        return "MAKE_PROPERTY_VALUE(PID_" + property_id + ", PVT_" + value_id + ", PVK_USE_DATA"

def make_operator_name(property_token):
    operator_name = ''.join(x for x in property_token.title() if x.isalnum())
    return "set" + operator_name[0] + operator_name[1:]

def write_to_dst_file(fout, property_info, type_list, keyword_list):
    property_tokens = list(property_info.keys())
    property_tokens.sort()

    try:

        fout.write("// The property identifiers\n")
        fout.write("typedef enum _PropertyIds {\n")
        for i in range(0, len(property_tokens)):
            fout.write("    // %s\n" %(property_tokens[i], ))
            fout.write("    %s,\n" %(make_property_id(property_tokens[i]), ))
        fout.write("} PropertyIds;\n")
        fout.write("\n")

        fout.write("// The property value types\n")
        fout.write("typedef enum _PropertyValueTypes {\n")
        fout.write("    PVT_KEYWORD = 0,\n")
        for i in range(0, len(type_list)):
            fout.write("    %s,\n" %(make_value_type_id(type_list[i]), ))
        fout.write("} PropertyValueTypes;\n")
        fout.write("\n")

        fout.write("// The property value keywords\n")
        fout.write("typedef enum _PropertyValueKeywords {\n")
        for i in range(0, len(keyword_list)):
            fout.write("    %s,\n" %(make_value_keyword_id(keyword_list[i]), ))
        fout.write("    PVK_USE_DATA,\n")
        fout.write("} PropertyValueKeywords;\n")
        fout.write("\n")

        fout.write("// The property values\n")
        for i in range(0, len(property_tokens)):

            value_list = property_info[property_tokens[i]]
            value_list.sort()

            fout.write("// Values for for property %s\n" % (property_tokens[i], ))
            for value in value_list:
                fout.write("#define %s \\\n" % (make_value_id (property_tokens[i], value), ))
                fout.write("    %s\n" % (make_value_macro (property_tokens[i], value), ))
            fout.write("\n")

        fout.write("\n")

    except:
        trace_back()
        print("make_pv_macros.py: failed to write code to %s" % dst_file)
        return None

    return property_tokens;

if __name__ == "__main__":
    try:
        fsrc = open(SRC_FILE, "r")
    except:
        print("make_pv_macros.py: failed to open input file %s" % SRC_FILE)
        sys.exit(1)

    try:
        fdst = open(DST_FILE, "w")
    except:
        print("make_pv_macros.py: failed to open output file %s" % DST_FILE)
        sys.exit(2)

    print("Scanning input file %s..." % SRC_FILE)
    property_info = scan_src_file(fsrc)
    if property_info is None:
        print("FAILED")
        fsrc.close
        fdst.close
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

    print("Writting to dst file %s..." % DST_FILE)
    property_tokens = write_to_dst_file(fdst, property_info, type_list, keyword_list)
    if property_tokens is None:
        print("FAILED")
        fdst.close()
        sys.exit(4)
    print("DONE")
    fdst.close()

    print("StyleSheet::setProperty operators:")
    for i in range(0, len(property_tokens)):

        value_list = property_info[property_tokens[i]]
        value_list.sort()

        print("    // Operator for property %s" % (property_tokens[i], ))
        print("    bool %s (DWORD32 value, HTData addData = 0) {" % (make_operator_name (property_tokens[i]), ))
        print("        return setProperty (%s, value, addData);" % (make_property_id (property_tokens[i]), ))
        print("    }")
        print("")

    sys.exit(0)

