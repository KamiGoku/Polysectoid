#define PACKET_SIZE 14

void sendData(AltSoftSerial &serialTwo, char *packet, int packetsize);

void readData(Stream &serialTwo, int &read_flag);
