#!/usr/bin/python3
"""
Make CSS property value macros:
    1. Read 'property-list.txt' file.
    2. Generate CSS property identifier list(enum).
    3. Generate CSS property value keyword list(enum).
    4. Generate CSS property value macros.
    5. Write code to property-list.c file.
"""
import os, sys
import time
import re

SRC_FILE="property-list.txt"
DST_FILE="property-list.c"

def scan_src_file(fsrc):
    property_info = {}

    return property_info

data_pattern = re.compile(r"^\<\S\>$")
def is_keyword(value):
    m = data_pattern.match(value)
    if m == None:
        return True
    return False

def generate_type_and_keyword_lists(property_info):
    type_dict = {}
    keyword_dict = {}

    property_ids = list(property_info.keys())
    for key in property_info:
        value_list = property_info[key]

        for j in range(0, len(value_list)):
            if is_keyword(value_list[j]):
                if keyword_dict[value_list[j]] is None:
                    keyword_dict[value_list[j]] = "";
            else:
                if type_dict[value_list[j]] is None:
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

def write_to_dst_file(fout, property_info, type_list, keyword_list):
    property_ids = list(property_info.keys())
    property_ids.sort()

    try:

        fout.write("// The property identifiers\n")
        fout.write("typedef enum _PropertyIds {\n")
        for i in range(0, len(property_ids)):
            fout.write("    // %s\n" %(property_ids[i], ))
            fout.write("    %s,\n" %(make_property_id(property_ids[i]), ))
        fout.write("} PropertyIds; \n")
        fout.write("\n")

        fout.write("// The property value types\n")
        fout.write("typedef enum _PropertyValueTypes {\n")
        fout.write("    PVT_KEYWORD = 0,\n")
        for i in range(0, len(type_list)):
            fout.write("    %s,\n" %(make_value_type_id(type_list[i]), ))
        fout.write("} PropertyValueTypes; \n")
        fout.write("\n")

        fout.write("// The property value keywords\n")
        fout.write("typedef enum _PropertyValueKeywords {\n")
        for i in range(0, len(keyword_list)):
            fout.write("    %s,\n" %(make_value_keyword_id(keyword_list[i]), ))
        fout.write("    PVK_USE_DATA,\n")
        fout.write("} PropertyValueKeywords; \n")
        fout.write("\n")

        fout.write("// The property values\n")
        for i in range(0, len(property_ids)):

            my_values = property_info[i].sort()

            fout.write("// Values for for property %s\n", (property_ids[j], ))
            for j in range(0, len(my_values)):
                fout.write("#define %s \\\n", (make_value_id (property_ids[j], my_values[j]), ))
                fout.write("    %s\n", (make_value_macro (property_ids[j], my_values[j]), ))

        fout.write("\n")

    except:
        fout.close()
        trace_back()
        print("write_zipped_map: failed to write code to %s" % dst_file)
        return False

    fout.close()
    return True

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
    print("DONE")

    print("Writting to dst file %s..." % DST_FILE)
    if write_to_dst_file(fdst, property_info, type_list, keyword_list) == False:
        print("FAILED")
        fdst.close()
        sys.exit(4)
    print("DONE")

    fdst.close()
    sys.exit(0)

