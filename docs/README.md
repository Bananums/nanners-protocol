The receiver operates as a deterministic state machine, processing one byte at a time and reconstructing frames in order.

State descriptions:

WAIT_FOR_SOF – Waits for the start-of-frame marker.

READ_FRAME_ID – Reads two bytes (big-endian) for the frame identifier.

READ_LENGTH – Reads the payload length byte (fixed 16).

READ_PAYLOAD – Accumulates 16 bytes of data.

READ_CRC – Reads two bytes for CRC-16.

VERIFY_EOF – Awaits the end-of-frame marker and validates CRC.

VALID_FRAME – Marks the frame as valid and notifies the application layer.