# nanners-protocol

nanners-protocol is a lightweight communication layer for UART, implemented in C with C++ compatibility.
It defines a simple and deterministic frame structure containing a frame identifier, payload, and CRC
for data integrity verification.

The protocol provides a consistent interface for transmitting and receiving structured messages across devices,
abstracting the underlying UART communication layer.

## Frame Layout ##

All multibyte values in the Nanners protocol are encoded in big-endian (network byte order).

| Field       | Size (bytes) | Description                                                   |
|-------------|--------------|---------------------------------------------------------------|
| **SOF**     | 1            | Start-of-frame marker (`NANNERS_START_OF_FRAME`)              |
| **ID**      | 2            | Frame identifier (`uint16_t`, big-endian)                     |
| **SEQ**     | 1            | Sequence counter (0...255, wraps)                             |
| **LEN**     | 1            | Number of valid bytes in `PAYLOAD` (0-8)                      |
| **PAYLOAD** | 8            | Application data (raw bytes)                                  |
| **CRC**     | 2            | CRC-16 over `ID ∥ LEN ∥ SEQ ∥ PAYLOAD[0..LEN-1]` (big-endian) |
| **EOF**     | 1            | End-of-frame marker (`NANNERS_END_OF_FRAME`)                  |
| **Total**   | 16           | Total frame size in bytes                                     |

### frame building  ###

The receiver operates as a deterministic state machine, processing one byte at a time and reconstructing frames in
order.
[rx.mmd](docs/rx.mmd)

## Build ##

Static library (default)

```shell
mkdir build
cd build
cmake ..
make
```

Shared library

```shell
mkdir build
cd build
cmake .. -DBUILD_SHARED_LIBS=ON
make
```

### Build options ###
This project uses CMake. The following options can be enabled at configure time:

| Option            | Default | Description                                        |
|-------------------|---------|----------------------------------------------------|
| `BUILD_TESTS`     | `OFF`   | Build unit tests                                   |
| `BUILD_EXAMPLES`  | `OFF`   | Build example applications                         |
| `NANNERS_DEBUG`   | `OFF`   | Enable debug logging in the protocol state machine |

## Install ##

```shell
sudo make install
```

## Linking with CMake ##

To link the installed library, simply find and link the target 

```shell
find_package(nanners REQUIRED)
add_executable(exampleapp main.cpp)
target_link_libraries(exampleapp PRIVATE BANANUMS::nanners)
```

## Inclusion ##

Include the library header
```c++
#include <nanners/nanners.h>
```

## TODOS ##

Look into `abi-compliance-checker`, once the library gets more complete.