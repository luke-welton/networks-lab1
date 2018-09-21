Group 13: Lucas Welton, Jesse Roach, Bryant Kelley

# Introduction
This project implements TCP and UDP servers using C and Python.
Our TCP server is implemented using Python while the client is implemented in C.
The UDP server is implemented using C while the client is implemented in Python.

# Functionality
Known bugs:

The TCP Client and Server do not want to perform multiple requests. We currently do not know if this is an issue with the client, the server, or both. (I suspect the client is defaulting to quit rather than to send another and something is wrong on the input for continuing.)

# Compiling
The TCP client and UDP server are compiled using `gcc ClientTCP.c -o ClientTCP` and `gcc ServerUDP.c -o ServerUDP` and our TCP server and UDP client are in python, a scripting language, and are not compiled.

# Executing
To execute our files use 10023 for <port> and the address of where the server is running for <server_addr> in the following commands: 
```
python ServerTCP.py <port>
./ClientTCP <server_addr> <port>

./ServerUDP <port>
python ClientUDP.py <server_addr> <port>
```

# Results
In testing with a mock server, our UDP client appeared to be fully functional. The UDP server appears to be 

The TCP client and server appear to communication correctly for the first message but upon being prompted to continue both will close the connection.
