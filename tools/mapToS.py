#!/usr/bin/env python3
import sys
import os
import xml.etree.ElementTree as ET
if len(sys.argv) != 4:
  print("usage: " + sys.argv[0] + " (tmx file) (s output) (h output)")
  exit(1)
count = 0
head, tail = os.path.split(sys.argv[1])
variableName = tail.removesuffix(".tmx") + "Data"

data = []
smallestInt = 100000
xmlDat = ET.parse(sys.argv[1]).getroot().find("layer").find("data").text
lines = xmlDat.split("\n")
for line in lines:
  if(len(line)):
    nums = line.split(",")
    for num in nums:
      if len(num):
        count += 1
        newNum = int(num)
        data.append(newNum)
        if newNum < smallestInt:
          smallestInt = newNum


h_output = "#ifndef " + variableName + "_h\n" + \
  "  #define " + variableName + "_h\n" + \
  "  extern const short unsigned int " + variableName + "[" + str(count) + "];\n" + \
  "  #define " + variableName + "Length " + str(count * 2) + "\n" \
  "#endif"

output = "@ generated by mapToS.py\n" \
"@ data size = " + str(count * 2) + " bytes\n\n" \
".section .rodata\n" \
".align 2\n" \
".global " + variableName + "\n" \
".hidden " + variableName + "\n" + variableName + ":\n"

counter = 0
for num in data:
  if counter == 0:
    output += ".hword "
  output += str(num - smallestInt)
  counter += 1
  if counter == 16:
    output += "\n"
    counter = 0
  else:
    output += ","
output += "\n"

text_file = open(sys.argv[2], "w")
text_file.write(output)
text_file.close()

text_file = open(sys.argv[3], "w")
text_file.write(h_output)
text_file.close()