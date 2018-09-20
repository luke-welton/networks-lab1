Group 13: Lucas Welton, Jesse Roach, Bryant Kelley

# Introduction
This project implements UDP and TCP servers using C and Python.
Our TCP server is implemented using Python while the client is implemented in C.
The UDP server is implemented using C while the client is implemented in Python.

# Functionality


# Compiling
The TCP client and UDP server are compiled using ```gcc ClientTCP.c -o ClientTCP``` and ```gcc ServerUDP.c -o ServerUDP``` and our TCP server and UDP client are in python, a scripting language, and are not compiled.

# Executing
To execute our files use 10023 for <port> and the address of where the server is running for <server_addr> in the following commands: 
```
./ServerUDP <port>
python ClientUDP.py <server_addr> <port>

python ServerTCP.py <port>
./ClientTCP <server_addr> <port>
```

# Results

