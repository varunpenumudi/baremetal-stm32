import serial

class COMMS_Packet:
    def __init__(self, data: list[int] = [0xFF]):
        self.length = len(data)
        self.data = data + ([0xFF] * (16-len(data)))
        self.crc = self.crc8([self.length] + self.data)
    
    def __str__(self):
        return f"Packet(Length={bytes([self.length]).hex()}   Data={bytes(self.data).hex(' ')}   CRC={bytes([self.crc]).hex()})"
    
    def as_bytes(self):
        return bytes([self.length]) + bytes(self.data) + bytes([self.crc])
    
    def from_bytes(self, byts: bytes):
        self.length = (byts[0])
        self.data = [int(b) for b in byts[1:-1]]
        self.crc = int(byts[-1])
        return self

    def crc8(self, data: list[int]) -> int:
        crc = 0
        for byte in data:
            crc ^= byte
            for _ in range(8):
                if crc & 0x80:
                    crc = ((crc << 1) ^ 0x07) & 0xFF
                else:
                    crc = (crc << 1) & 0xFF
        return crc


# Acknowledgement Packet
ACK_PACKET = COMMS_Packet([0x19])
REQ_RETX_PACKET = COMMS_Packet([0x15])

if __name__ == "__main__":
    COM_PORT = "COM5"
    BAUD_RATE = 115200
    ser_port = serial.Serial(port=COM_PORT, baudrate=BAUD_RATE, timeout=4)

    # Write Port
    tx_packet = REQ_RETX_PACKET
    rx_packet = COMMS_Packet()

    ser_port.write(tx_packet.as_bytes())
    rx_bytes = ser_port.read(18)
    print("Sent: ", tx_packet)
    if rx_bytes:
        print("Recv: ",rx_packet.from_bytes(rx_bytes))
        if (rx_packet.as_bytes() == ACK_PACKET.as_bytes()): print("Acknowledgement Recieved")
    else:
        print("None")

    # Close Port
    ser_port.close()

