import serial

COM_PORT = "COM19"
BAUD_RATE = 115200

def crc8(buffer: list[int]) -> int:
    crc = 0
    for byte in buffer:
        crc ^= byte
        for _ in range(8):
            if crc & 0x80:
                crc = ((crc << 1) ^ 0x07) & 0xFF
            else:
                crc = (crc << 1) & 0xFF
    return [crc]


# Acknowledgement Packet
ACK_PACKET = [0x01, 0x19] + [0xFF] * 15
ACK_PACKET = bytes(ACK_PACKET + crc8(ACK_PACKET))

# Request Retransmit Packet
REQ_RETX_PACKET = [0x01, 0x19] + [0xFF] * 15
REQ_RETX_PACKET = bytes(REQ_RETX_PACKET + crc8(REQ_RETX_PACKET))


