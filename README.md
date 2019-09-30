# SQM
Sky Qaulity Meter Implementations using TSL2591 with Ardruino NANO

Wiring giagram and PCB is on [EasyEDA](https://easyeda.com/hujer.roman/sqm-hr)     

The files for reading the TSL2591 and returning SQM values were downloaded from gshau's SQM_TSL2591 repository
https://github.com/gshau/SQM_TSL2591/tree/master/SQM_TSL2591.  If the .ino files are used as-is, SQM_TSL2591.cpp and SQM_TSL2591.h must be in the same directory/folder as the .ino file.

#The SQM LU protocol has been used but only responds to information requests (ix) and read requests (rx or RX).
#This has been tested using INDI.


