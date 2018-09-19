#!/usr/bin/env python

import socket
import sys


def main(argv):
    try:
        client = argv[1]
        server_name = argv[2]
        port_number = argv[3]
    except:
        print("Invalid arguments.")
        sys.exit()

    print(client, server_name, port_number)

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect((server_name, port_number))

    continue = 'Y'
    request_id = 0
    while continue != 'n':
        op_code = input('Opcode: ')
        operand_one = input('Operand 1: ')
        operand_two = input('Operand 2: ')

        total_message_length = 8 // Default for everything but NOT op
        op_number = 2
        if op_code == 6:
            total_message_length = 6
            op_number = 1


        request = ''.join(chr(x) for x in [total_message_length, request_id, op_code, op_number, operand_one, operand_two])

        s.send(request)
        response = s.recv(4096)
        for i in range(0, 8):
            print(response[i].encode('hex'))

        request_id = request_id + 1
        continue = input('Continue (Y/n): ')

if __name__ == '__main__':
    main(sys.argv)
