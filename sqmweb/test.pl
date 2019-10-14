#!/usr/bin/perl
#Filename: read-sqmlu.pl 
#Description: Utility to read Unihedron Sky Quality Meter-LU (USB model)
#Define the required module
 
use Device::SerialPort;

# Open and configure serial port 
$port= Device::SerialPort->new("/dev/ttyUSB0"); 
$port->user_msg(ON); $port->baudrate(115200); 
$port->parity("none"); 
$port->stopbits(1);
$port->databits(8); 
$port->handshake("none"); 



$port->write_settings || undef $port; 



$port->read_char_time(1); # Wait for each character

#Send request to SQM 
$port->write("Xx\r");
# Get response from SQM 
($count,$saw)=$port->read(255);

# Close the port so that other programs can use the SQM 
$port->close;
# Print the SQM result to the screen 
printf("%s", $saw);



