//void readARX(Stream &serialOne, Stream /*AltSoftSerial*/ &serialTwo, int &read_flag);

//void readTRX(Stream &serialOne, AltSoftSerial &serialTwo, int &read_flag, RingBuf *buf);

//void sendData(int which_serial, Stream &serialOne, AltSoftSerial &serialTwo, RingBuf *buf);
void sendData(Stream &serialTwo, char *packet, int packetsize);

void readData(Stream &serialOne, int &read_flag, int which_buf);
