#!/usr/bin/env python3
import sys
import os
if len(sys.argv) != 4:
  print("usage: " + sys.argv[0] + " (csv file) (c output) (h output)")
  exit(1)
count = 0
head, tail = os.path.split(sys.argv[1])
variableName = tail.removesuffix(".csv") + "Data"

data = ""
with open(sys.argv[1], 'r') as fd:
  lines = fd.readlines()
  for line in lines:
    count += line.count(",") + 1
    data += "  " + line.rstrip("\n") + ",\n"

h_output = "#ifndef " + variableName + "_h\n" + \
  "  #define " + variableName + "_h\n" + \
  "  extern const short unsigned int " + variableName + "[" + str(count) + "];\n" + \
  "  #define " + variableName + "Length " + str(count * 2) + "\n" \
  "#endif"
output = "const short unsigned int " + variableName + "[" + str(count) + "] = {\n"
output += data
output = output.rstrip(",\n") + "\n};\n"

text_file = open(sys.argv[2], "w")
text_file.write(output)
text_file.close()

text_file = open(sys.argv[3], "w")
text_file.write(h_output)
text_file.close()