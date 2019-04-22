#define PACKET_SIZE 14

void sendData(AltSoftSerial &serialTwo, char packet[PACKET_SIZE+1]);

void readData(Stream &serialTwo, int &read_flag);
