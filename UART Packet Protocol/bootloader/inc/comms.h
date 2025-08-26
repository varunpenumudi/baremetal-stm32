#ifndef INC_COMMS_H
#define INC_COMMS_H

#include "common-defines.h"

#define COMMS_PACKET_FULL_LEN    (18U)
#define COMMS_PACKET_DATALEN_LEN (1U)
#define COMMS_PACKET_PAYLOAD_LEN (16U)
#define COMMS_PACKET_CRC_LEN     (1U)

#define COMMS_RETX_PACKET_DATA0 (0x15U)
#define COMMS_ACK_PACKET_DATA0  (0x19U)


typedef struct {
    uint8_t length;
    uint8_t data[COMMS_PACKET_PAYLOAD_LEN];
    uint8_t crc;
} comms_packet_t;


void comms_setup(void);
void comms_update(void);

bool comms_packets_availabe(void);
void comms_write(comms_packet_t* packet);
void comms_read(comms_packet_t* packet);


#endif /* INC_COMMS_H */
