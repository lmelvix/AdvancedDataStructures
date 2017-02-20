#include "BitInputStream.h"

/** Initialize a BitInputStream that will use
 *  the given istream for input.
 */
BitInputStream::BitInputStream(std::istream& is) : buf(0), nbits(8), in(is) {}

/** Fill the buffer from the input stream.
*/
void BitInputStream::fill() {
    buf = in.get();
    nbits = 0;
}

/** Read and return the next bit from the bit buffer.
 *  Fill it first if necessary.
 */
unsigned int BitInputStream::readBit() {
    if(nbits == 8)
        fill();

    // Reading bits from buf starting from the leftmost bit
    unsigned int temp = buf & (0x80 >> nbits);
    unsigned int nextBit = temp >> (7-nbits);

    nbits++;

    return nextBit;
}

/** Read 3-byte ints from the input stream, 
 *  and return it as regular 4-byte int. 
 */
int BitInputStream::readThreeByteInt() {
    int nextInt = 0;
    unsigned char nextByte;
 
    nextByte = in.get();
    if(in.eof())
        return 0;
    nextInt += (nextByte << 16); 

    nextByte = in.get();
    if(in.eof())
        return 0;
    nextInt += (nextByte << 8);

    nextByte = in.get();
    if(in.eof())
        return 0;
    nextInt += nextByte;

    return nextInt;
}
