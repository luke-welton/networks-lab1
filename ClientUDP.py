#!/usr/bin/env python

import socket
import sys
import time


def main(argv):
    try:
        server_name = argv[1]
        port_number = argv[2]
    except:
        print("Invalid arguments.")
        sys.exit()

    print(server_name, port_number)

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect((server_name, int(port_number)))

    keep_going = 'Y'
    request_id = 0
    while keep_going != 'n':
        try:
            op_code = input('Opcode: ')
        except:
            print("Opcode required!")
            break
        try:
            operand_one = input('Operand 1: ')
        except:
            print("Operand 1 required!")
            break
        if op_code != 6:
            try:
                operand_two = input('Operand 2: ')
            except:
                print("Operand 2 required!")
                break

        total_message_length = 8 # Default for everything but NOT op
        op_number = 2
        if op_code == 6:
            total_message_length = 6
            op_number = 1

        if op_code == 6 and int(operand_one) > 256:
            request = ''.join(chr(x) for x in [total_message_length, request_id, op_code, op_number, operand_one])
        elif op_code == 6:
            request = ''.join(chr(x) for x in [total_message_length, request_id, op_code, op_number, 0, operand_one])
        elif int(operand_one) > 256 and int(operand_two) > 256:
            request = ''.join(chr(x) for x in [total_message_length, request_id, op_code, op_number, operand_one, operand_two])
        elif int(operand_one) > 256:
            request = ''.join(chr(x) for x in [total_message_length, request_id, op_code, op_number, operand_one, 0, operand_two])
        elif int(operand_two) > 256:
            request = ''.join(chr(x) for x in [total_message_length, request_id, op_code, op_number, 0, operand_one, operand_two])
        else:
            request = ''.join(chr(x) for x in [total_message_length, request_id, op_code, op_number, 0, operand_one, 0, operand_two])

        start = time.clock()
        s.sendall(request)

        response = s.recv(4096)
        end = time.clock()

        for i in range(0, 7):
            print(response[i].encode('hex'))

        print('Response ID:', response[1])
        print('Response:', str(response[3]) + str(response[4]) + str(response[5]) + str(response[6]))
        print(end - start)

        request_id = request_id + 1
        try:
            keep_going = input('Continue (Y/n): ')
        except:
            keep_going = 'Y'

    s.close()

if __name__ == '__main__':
    main(sys.argv)
