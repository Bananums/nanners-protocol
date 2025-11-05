# nanners-protocol

The nanners-protocol is a lightweight communication layer for UART, written in C with C++ compatability.
It defines a simple frame structure that includes a frame identifier, payload, and CRC for data integrity verification. 
The protocol abstracts the underlying UART communication, providing a consistent interface for sending and receiving 
structured messages between devices.


## Build ##

Static library (default)
```shell
cmake -B build -S .
cmake --build build
```

Shared library
```shell
cmake -B build -S . -DBUILD_SHARED_LIBS=ON
cmake --build build
```