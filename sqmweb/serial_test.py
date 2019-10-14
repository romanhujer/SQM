 


import serial

with serial.serial_for_url('spy:///dev/ttyUSB0?file=test.txt', timeout=1) as s:
    s.dtr = False
    s.write('rx')
    s.read(20)
    s.dtr = True
    s.write(serial.to_bytes(range(256)))
    s.read(400)
    s.send_break()

with open('test.txt') as f:
    print(f.read())

