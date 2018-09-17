#!/usr/bin/env python

import socket
import sys


def main(argv):
    # client = ''
    # server_name = ''
    # port_number = ''
    print("Oh no!")

    try:
        client = argv[1]
        server_name = argv[2]
        port_number = argv[3]
    except:
        print("Invalid arguments.")
        sys.exit()

    print(client, server_name, port_number)
    
    op_code = input('Opcode: ')
    operand_one = input('Operand 1: ')
    operand_two = input('Operand 2: ')

if __name__ == '__main__':
    main(sys.argv)
