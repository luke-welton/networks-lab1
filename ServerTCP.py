import socket
import sys


def main(argv):
    try:
        server = argv[1]
        port = int(argv[2])

        socket = ServerSocket(server, port)
        socket.run()
    except exception:
        print("Improper arguments passed. Quitting.")
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

    @staticmethod
    def int_to_bytes(_int):
        return {
            tml: (_int & 0xFF00000000000000) >> (8 * 7),
            id: (_int & 0x00FF000000000000) >> (8 * 6),
            code: (_int & 0x0000FF0000000000) >> (8 * 5),
            num_ops: (_int & 0x000000FF00000000) >> (8 * 4),
            op1: (_int & 0x00000000FFFF0000) >> (8 * 2),
            op2: (_int & 0x000000000000FFFF)
        }

    def __init__(self, server, port):
        self.message = 0
        self.message_obj = {}
        self.answer = 0
        self.invalid_req = False

        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        try:
            self.socket.bind((server, port))
        except exception:
            print("Error occurred while connecting to the socket's server or port. Shutting Down.")
            exit()

    def run(self):
        self.socket.listen(1)
        client_connection = self.socket.accept()

        while 1:
            msg_size = int(client_connection.recv(1))
            if not msg_size:
                break
            msg_size = msg_size - 1

            self.message = (msg_size << (8 * msg_size)) + int(client_connection.recv(msg_size))

            self.interpret()
            self.send_response()

            self.clean()

        client_connection.close()

    def interpret(self):
        self.message_obj = int_to_bytes(self.message)
        try:
            self.answer = function_switch(message_obj.op1, message_obj.op2)
        except exception:
            self.answer = 0
            self.invalid_req = True

    def send_response(self):
        response_obj = {
            tml: 7,
            id: self.message_obj.id,
            error: 127 if self.invalid_req else 0,
            result: self.answer
        }

        response = response_obj.result
        response += (response_obj.error << (8 * 4))
        response += (response_obj.id << (8 * 5))
        response += (response_obj.tml << (8 * 6))

        self.socket.send(response)

    def clean(self):
        self.message = 0
        self.message_obj = {}
        self.answer = 0
        self.invalid_req = False


if __name__ == "__main__":
    main(sys.argv)
