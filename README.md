# nanners-protocol

nanners-protocol is a lightweight communication layer for UART, implemented in C with C++ compatibility.
It defines a simple and deterministic frame structure containing a frame identifier, payload, and CRC 
for data integrity verification.

The protocol provides a consistent interface for transmitting and receiving structured messages across devices, 
abstracting the underlying UART communication layer.

## Frame Layout ##
All multi-byte integers in the Nanners protocol are encoded in big-endian (network byte order).

Byte order: All multi-byte fields are big-endian (network byte order).

Field	Size (bytes)	Description
SOF	1	Start-of-frame marker (NANNERS_START_OF_FRAME)
ID	2	Frame identifier (uint16_t, big-endian)
LEN	1	Payload length (fixed 16)
PAYLOAD	16	Application data (raw bytes)
CRC	2	CRC-16 over ID ∥ LEN ∥ PAYLOAD (big-endian)
EOF	1	End-of-frame marker (NANNERS_END_OF_FRAME)

**Notes**

LEN is always 0x10 (16) in this version; reserved for future variable-length use.
CRC-16 polynomial/initial value should be specified in the spec (e.g., CRC-16/CCITT-FALSE). Both CRC bytes are transmitted high-byte first.

### frame building  ###
The receiver operates as a deterministic state machine, processing one byte at a time and reconstructing frames in order.
[rx.mmd](docs/rx.mmd)

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