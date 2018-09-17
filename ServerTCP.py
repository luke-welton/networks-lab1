import socket
import sys

if __name__ == "__main__":
    print((0x0800000000000000 & 0xFF00000000000000) >> (8 * 7))


class ClientMessage:
    def __init__(self):
        return


class ServerSocket:
    def __init__(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        sock.connect(("localhost", 10023))

    def interpret(self, message):
        message_obj = int_to_bytes(message)

        return 0

    @staticmethod
    def int_to_bytes(_int):
        byte_obj = ClientMessage()
        byte_obj.tml = (_int & 0xFF00000000000000) >> (8 * 7)
        byte_obj.id = (_int & 0x00FF000000000000) >> (8 * 6)
        byte_obj.code = (_int & 0x0000FF0000000000) >> (8 * 5)
        byte_obj.num_ops = (_int & 0x000000FF00000000) >> (8 * 4)
        byte_obj.op1 = (_int & 0x00000000FFFF0000) >> (8 * 2)
        byte_obj.op2 = (_int & 0x000000000000FFFF)

    @staticmethod
    def addition(op1, op2):
        return op1 + op2

    @staticmethod
    def subtraction(op1, op2):
        return op1 - op2

    @staticmethod
    def bitwise_or(op1, op2):
        return op1 | op2

    @staticmethod
    def bitwise_and(op1, op2):
        return op1 & op2

    @staticmethod
    def shift_right(op1, op2):
        return op1 >> op2

    @staticmethod
    def shift_left(op1, op2):
        return op1 << op2

    @staticmethod
    def not_function(op):
        return ~op
