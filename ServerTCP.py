import socket
import sys
import math


def main(argv):
    try:
        port = int(argv[1])

        socket = ServerSocket(port)
        socket.run()
    except IndexError:
        print("Improper arguments passed. Shutting Down.")
        exit()


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

    function_switch = {
        0: addition,
        1: subtraction,
        2: bitwise_or,
        3: bitwise_and,
        4: shift_right,
        5: shift_left,
        6: not_function
    }

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
            msg_size = client_connection.recv(1)
            if not msg_size:
                break
            message = [msg_size]

            for _ in range(int(msg_size) - 1):
                message.append(client_connection.recv(1))

            self.message = bytes(message)

            self.create_object()
            self.interpret()
            self.send_response()

            self.clean()

        client_connection.close()

    def create_object(self):
        self.message_obj.tml = int(self.message[0])
        self.message_obj.id = int(self.message[1])
        self.message_obj.code = int(self.message[2])
        self.message_obj.num_ops = int(self.message[3])

        self.message_obj.op1 = int(self.message[4] << 4 + self.message[5])
        self.message_obj.op2 = int(self.message[6] << 4 + self.message[7])

    def interpret(self):
        self.message_obj = int_to_bytes(self.message)
        try:
            self.answer = function_switch(message_obj.op1, message_obj.op2)
        except KeyError:
            self.answer = 0
            self.invalid_req = True

    def send_response(self):
        response_obj = {
            tml: 7,
            id: self.message_obj.id,
            error: 127 if self.invalid_req else 0,
            result: self.answer
        }

        response = {bytes(response_obj.tml), bytes(response_obj.id), bytes(response_obj.error)}
        response.append(response_obj.result.to_bytes(4, "big"))

        self.socket.send(bytes(response))

    def clean(self):
        self.message = []
        self.message_obj = {}
        self.answer = 0
        self.invalid_req = False


if __name__ == "__main__":
    main(sys.argv)
