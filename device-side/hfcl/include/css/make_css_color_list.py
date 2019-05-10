#!/usr/bin/python3

#
# HFCL - HybridOS Foundation Class Library
#
# Copyright (C) 2018 Beijing FMSoft Technologies Co., Ltd.
#
# This file is part of HFCL.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

"""
Make CSS color RGBA values:
    1. Read 'colorlist.txt' file.
    5. Write code to 'pvcolors.inc' file.
"""

import os, sys
import time
import re

TOOL_NAME="make_css_color_list.py"
SRC_FILE="colorlist.txt"
PVCOLORS_FILE="../../src/css/pvcolors.inc"

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

RE_START_WITH_ARRAYSIZE = re.compile(r"^\s+arraysize:")
def start_with_arraysize(line):
    if RE_START_WITH_ARRAYSIZE.match(line) == None:
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

RE_IS_INTEGER = re.compile(r"^<integer")
RE_IS_SYSID = re.compile(r"^<sysid")

RE_IS_RGB = re.compile(r"^<rgb")
RE_IS_RESOLUTION = re.compile(r"^<resolution")
RE_IS_COUNTER = re.compile(r"^<counter")

RE_IS_NUMBER = re.compile(r"^<number")
RE_IS_LENGTH = re.compile(r"^<length")
RE_IS_PERCENTAGE = re.compile(r"^<percentage")
RE_IS_ALPHAVALUE = re.compile(r"^<alphavalue")
RE_IS_ANGLE = re.compile(r"^<angle")
RE_IS_TIME = re.compile(r"^<time")
RE_IS_FREQUENCY = re.compile(r"^<frequency")

RE_IS_STRING = re.compile(r"^<string")
RE_IS_URL = re.compile(r"^<url")
RE_IS_ARRAY = re.compile(r"^<array")
RE_IS_HSL = re.compile(r"^<hsl")
RE_IS_SHAPE = re.compile(r"^<sharp")
def get_value_type(value):
    if RE_IS_INTEGER.match(value):
        return "HTInt"
    if RE_IS_SYSID.match(value):
        return "HTInt"

    if RE_IS_RGB.match(value):
        return "HTUint"
    if RE_IS_RESOLUTION.match(value):
        return "HTUint"
    if RE_IS_COUNTER.match(value):
        return "HTUint"

    if RE_IS_NUMBER.match(value):
        return "HTReal"
    if RE_IS_LENGTH.match(value):
        return "HTReal"
    if RE_IS_PERCENTAGE.match(value):
        return "HTReal"
    if RE_IS_ALPHAVALUE.match(value):
        return "HTReal"
    if RE_IS_ANGLE.match(value):
        return "HTReal"
    if RE_IS_TIME.match(value):
        return "HTReal"
    if RE_IS_FREQUENCY.match(value):
        return "HTReal"

    if RE_IS_STRING.match(value):
        return "HTPointer"
    if RE_IS_URL.match(value):
        return "HTPointer"
    if RE_IS_ARRAY.match(value):
        return "HTPointer"
    if RE_IS_HSL.match(value):
        return "HTPointer"
    if RE_IS_SHAPE.match(value):
        return "HTPointer"

    return None

RE_END_WITH_INC = re.compile(r"\S*_INC_$")
def end_with_inc(value):
    if RE_END_WITH_INC.match(value):
        return True
    return False

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

def set_value_list(color_info, color_token, def_info, values):
    value_list = values.split()
    color_info[color_token]['org_values'] = value_list

    if len(value_list) > 0:
        for value in value_list:
            if value in def_info:
                values = values.replace(value, def_info[value])

    value_list = values.split()
    color_info[color_token]['values'] = value_list

#    print("values for property %s" % (color_token, ))
#    for value in value_list:
#        print("%s" % (value, ))

def scan_src_file(fsrc):
    color_info = {}

    color_token = ""
    line_no = 1
    org_line = fsrc.readline()
    while org_line:
        stripped_line = org_line.strip()
        if stripped_line == "" or stripped_line[0] == '#':
            line_no = line_no + 1
            org_line = fsrc.readline()
            continue

        tokens = stripped_line.split()
        if len (tokens) > 1:
            color_token = tokens[0]
            color_token = color_token.upper()
            rgb = tokens[1].split(",")
            if len(rgb) != 3:
                print("scan_src_file (Line %d): RGB color expected (%s)" % (line_no, tokens[1], ))
                return None
            else:
                color_info[color_token] = rgb
        else:
            print("scan_src_file (Line %d): color definition expected (%s)" % (line_no, stripped_line, ))
            return None

        line_no = line_no + 1
        org_line = fsrc.readline()

    return color_info

def make_color_keyword(color_token):
    color_name = color_token.upper()
    color_name = color_name.replace('-', '_')
    return "PVK_COLOR_" + color_name;

def write_pv_colors(fout, color_info):
    color_tokens = list(color_info.keys())
    color_tokens.sort()

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

    fout.write("static RGBCOLOR _css_color_keyword_to_rgba[] = {\n")
    for color_token in color_tokens:
        fout.write("    // %s\n" % (make_color_keyword(color_token), ))
        fout.write("    MakeRGBA(%s, %s, %s, 255),\n" % (color_info[color_token][0], color_info[color_token][1], color_info[color_token][2], ))
    fout.write("};\n")
    fout.write("\n")

if __name__ == "__main__":
    try:
        fsrc = open(SRC_FILE, "r")
    except:
        print("%s: failed to open input file %s" % (TOOL_NAME, SRC_FILE, ))
        sys.exit(1)

    try:
        fdst = open(PVCOLORS_FILE, "w")
    except:
        print("%s: failed to open output file %s" % (TOOL_NAME, PVCOLORS_FILE, ))
        sys.exit(2)

    print("Scanning input file %s..." % SRC_FILE)
    color_info = scan_src_file(fsrc)
    if color_info is None:
        print("FAILED")
        sys.exit(3)
    fsrc.close()
    print("DONE")

    print("Writting color values to dst file %s..." % PVCOLORS_FILE)
    try:
        write_pv_colors(fdst, color_info)
    except:
        print("FAILED")
        traceback.print_exc()
        sys.exit(13)
    fdst.close()
    print("DONE")

    sys.exit(0)

