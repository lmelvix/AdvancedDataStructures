#include "BitOutputStream.h"

/** Initialize a BitOutputStream that will use
 *  the given ostream for output.
 */
BitOutputStream::BitOutputStream(std::ostream& os) : buf(0), nbits(0), out(os){}

/** Send buffer content to the output stream and clear the buffer.
*/
void BitOutputStream::flush() {
    out.put(buf);   // write the bitwise buffer to the ostream
    buf = 0;        // clear the bitwise buffer
    nbits = 0;      // bitwise buffer is cleared, so there are 0 bits in it
}

/** Write the least significant bit of the argument to
 *  the bit buffer, and increment the bit buffer index.
 *  After that, flush the buffer if it is full.
 */
void BitOutputStream::writeBit(unsigned int bit) {
    // New bit becomes the rightmost bit of the buffer
    buf = buf << 1;
    buf += (bit & 0x01); 

    nbits++;

    if(nbits == 8)
        flush();
}

/** Write dummy bits to fill a byte,
 *  to be able to write all bits (not multiple of 8) to output stream.
 */
void BitOutputStream::fillByte() {
    while(true) {
        if(nbits == 0)
            return;
        writeBit(0);
    }
}

/** Write 4-byte int as 3-byte (24 bits) data
 */
void BitOutputStream::writeThreeByteInt(int intToWrite) {
    for(int n = 24; n > 0; n--)
        writeBit((intToWrite >> n-1) & 1);
}
