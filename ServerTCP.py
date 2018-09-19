import socket
import sys
import math
import struct


def main(argv):
    port = 0

    try:
        port = int(argv[1])
    except IndexError:
        print("Improper arguments passed. Shutting Down.")
        exit()

    socket = ServerSocket(port)
    socket.run()


class ServerSocket:
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

    @staticmethod
    def byte_to_int(byte):
        integer = 0
        for b in byte:
            integer += ord(b)
        return int(integer)

    def __init__(self, port):
        self.message = []
        self.message_obj = {}
        self.answer = 0
        self.invalid_req = False

        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        try:
            self.socket.bind((socket.gethostbyname(socket.gethostname()), port))
        except OSError:
            print("Error occurred while connecting to the socket's server. Shutting Down.")
            exit()
        except OverflowError:
            print("The port number must be between 0 and 65535. Shutting Down.")
            exit()

    def run(self):
        self.socket.listen(1)
        client_connection, address = self.socket.accept()

        while 1:
            msg_size = self.byte_to_int(client_connection.recv(1))
            if not msg_size:
                break
            self.message.append(msg_size)

            for _ in range(msg_size - 1):
                self.message.append(self.byte_to_int(client_connection.recv(1)))

            self.create_object()
            self.interpret()

            client_connection.send(self.form_response())

            self.clean()

        client_connection.close()

    def create_object(self):
        self.message_obj["tml"] = self.message[0]
        self.message_obj["id"] = self.message[1]
        self.message_obj["code"] = self.message[2]
        self.message_obj["num_ops"] = self.message[3]

        self.message_obj["op1"] = (self.message[4] << 4) + self.message[5]
        try:
            self.message_obj["op2"] = (self.message[6] << 4) + self.message[7]
        except KeyError:
            self.message_obj["op2"] = 0

    def interpret(self):
        try:
            self.answer = self.function_switch[self.message_obj["code"]](self.message_obj["op1"], self.message_obj["op2"])
        except KeyError:
            self.answer = 0
            self.invalid_req = True

    def form_response(self):
        response_obj = {
            "tml": 7,
            "id": self.message_obj["id"],
            "error": 127 if self.invalid_req else 0,
            "result": self.answer
        }

        to_send = struct.pack("B", response_obj["tml"])
        to_send += struct.pack("B", response_obj["id"])
        to_send += struct.pack("B", response_obj["error"])
        to_send += struct.pack("!i", response_obj["result"])

        return to_send

    def clean(self):
        self.message = []
        self.message_obj = {}
        self.answer = 0
        self.invalid_req = False


ServerSocket.function_switch = {
    0: ServerSocket.addition,
    1: ServerSocket.subtraction,
    2: ServerSocket.bitwise_or,
    3: ServerSocket.bitwise_and,
    4: ServerSocket.shift_right,
    5: ServerSocket.shift_left,
    6: ServerSocket.not_function
}

if __name__ == "__main__":
    main(sys.argv)
