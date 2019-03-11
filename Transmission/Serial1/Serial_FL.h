#define PACKET_SIZE 14

void sendData(AltSoftSerial &serialTwo, char packet[PACKET_SIZE+1]);

void readData(AltSoftSerial &serialTwo, int &read_flag, RingBuf *buf);
