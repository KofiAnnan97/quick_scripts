from serial import Serial
import serial.tools.list_ports as port_list

comm_port = "/dev/serial/by-id/usb-Prolific_Technology_Inc._USB-Serial_Controller_CODRe11BS13-if00-port0"#"/dev/serial/by-id/usb-Prolific_Technology_Inc._USB-Serial_Controller-if00-port0"
baud_rate = 115200

ser_port = Serial(port = comm_port,
                         baudrate = baud_rate,
                         bytesize = 8,
                         parity = 'N',
                         stopbits = 2,
                         timeout = 0,
                         xonxoff = 1, #Used to avoid receiving \x00 chars when using readline function
                         rtscts = 0)
if ser_port.isOpen():
    print('Port %s is open'%(comm_port))
    while True:
        line = ser_port.readline()
        print(line)
else:
    print('Error trying to open port %s'%(comm_port))