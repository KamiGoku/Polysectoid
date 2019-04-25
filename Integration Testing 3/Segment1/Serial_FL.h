#define PACKET_SIZE 14

void sendData(Stream &serialTwo, char *packet, int packetsize);

void readData(Stream &serialTwo, int &read_flag);
