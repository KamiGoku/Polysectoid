void readARX(Stream &serialOne, Stream /*AltSoftSerial*/ &serialTwo, int &read_flag, RingBuf *buf);

void readTRX(Stream &serialOne, AltSoftSerial &serialTwo, int &read_flag, RingBuf *buf);

void sendData(int which_serial, Stream &serialOne, AltSoftSerial &serialTwo, RingBuf *buf);
