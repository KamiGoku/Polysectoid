/*
 * This file defines functions that can be called by any Arduino that is in the middle of the data 
 * stream setup (i.e. not one of the two Arduinos at the ends) to carry out serial communication. These
 * Arduinos will be receiving and sending data from/to two other neighboring Arduinos on either side.
 */

#include <NeoSWSerial.h>
#include <AltSoftSerial.h>
#include <RingBuf.h>

#define PACKET_SIZE 14

/*
 * Maybe call this "processData()" later
 * This function basically pulls packets that were stored in the FIFO buffer previously and
 * then sends them to whichever neighboring Arduino is specified.
 * Might extend this function to also process whatever data is in the buffer before sending it,
 * e.g. pull a data value, see if it's meant for you, if so then do stuff, if not then send it away
 * (or potentially both, idk, we'll see).
 * Alternatively we might have two separate functions, one for just reading the buffer and sending the
 * data read, and the other for reading the buffer and processing the phase/bias/whatever appropriately.
 * The latter method might require a duplicate buffer so that both functions can easily read in the same
 * values.
 * TODO: try changing the func def to take two "Stream" parameters, then we can do away with the "which_serial"
 * crap and just swap the places of the streams as necessary. Not really a big deal though.
 */
void sendData(int which_serial, Stream &serialOne, Stream /*AltSoftSerial*/ &serialTwo, RingBuf *buf) {
  while(!buf->isEmpty(buf)){
      char packet[PACKET_SIZE+1];
      char *packet_ptr = packet+1;
      buf->pull(buf, packet_ptr);
      packet[0] = '\t';
      if (which_serial == 2){
        serialTwo.write(packet, PACKET_SIZE+1);
      }
      else if (which_serial == 1){
        serialOne.write(packet, PACKET_SIZE+1);
      }
    }
  return;
}

/*
 * Processes the byte that was just read from one of the serial ports. The bytes are stored
 * into an array that will be used to form a packet of data once every byte arrives. A newline
 * represents the end of the packet, so as long as no intermediate data was lost, the packet is
 * stored in the FIFO buffer after a newline is received.
 * Inputs: inByte is the byte that was read, which_buf specifies which buffer it'll be stored in 
 * (which then determines which Arduino the data will be sent to), read_flag is reset when a full
 * packet is read, buf is the buffer that the data is stored in.
 * Outputs: none
 */
void processIncomingByte(byte inByte, int which_buf, int &read_flag, RingBuf *buf){
  static char input[PACKET_SIZE];
  static char input2[PACKET_SIZE];
  static uint32_t input_idx = 0;
  static uint32_t input_idx2 = 0;

  switch(inByte) {
    case '\n':
      if (which_buf == 1) {
        if (input_idx != PACKET_SIZE-1) {
          input_idx = 0;
          read_flag = 0;
          break;
        }
        input[input_idx] = '\n';
        buf->add(buf, &input);
        input_idx = 0;
        read_flag = 0;
        //sendData(2);//just putting this here for now, maybe we'll call it somewhere else idk
        break;
      } else {
        if (input_idx2 != PACKET_SIZE-1) {
          input_idx2 = 0;
          read_flag = 0;
          break;
        }
        input2[input_idx2] = '\n';
        buf->add(buf, &input2);
        input_idx2 = 0;
        read_flag = 0;
        //sendData(1);//just putting this here for now, maybe we'll call it somewhere else idk
        break;
      }
      break;
      
    case '\r':
      break;

    case '\t':
      break;
      
    default:
    if (which_buf == 1) {
      if (input_idx < PACKET_SIZE-1) {
        input[input_idx++] = inByte;
        break;
      }
    } else {
      if (input_idx2 < PACKET_SIZE-1) {
        input2[input_idx2++] = inByte;
        break;
      }
    }
  }
}

/*
 * Reads from the AltSoftSerial port. Also takes the hardware Serial port as a function parameter
 * for now so that the sendData() function can be easily called from in here if need be.
 * TODO: try changing the function definition to take two "Stream" parameters, that way the caller
 * could just swap their places depending on which is the reader/writer. Not that important rn tho.
 */
void readARX(Stream &serialOne, Stream /*AltSoftSerial*/ &serialTwo, int &read_flag, RingBuf *buf){
  while(serialTwo.available() > 0){
    char c = serialTwo.read();
    if (read_flag == 1){
      processIncomingByte(c, 1, read_flag, buf);
      if (c == '\n'){
        sendData(2, serialOne, serialTwo, buf);
        break;
      }
    }
    if (c == '\t'){
      read_flag = 1;
    }
  }
}

/*
 * Same as readARX but reads from hardware Serial port.
 */
void readTRX(Stream &serialOne, AltSoftSerial &serialTwo, int &read_flag, RingBuf *buf){
  while(serialOne.available() > 0){
    char c = serialOne.read();
    if (read_flag == 1){
      processIncomingByte(c, 2, read_flag, buf);
      if (c == '\n'){
        sendData(1, serialOne, serialTwo, buf);
        break;
      }
    }
    if (c == '\t'){
      read_flag = 1;
    }
  }
}
