#ifndef BITINPUTSTREAM_H
#define BITINPUTSTREAM_H

#include <iostream>

class BitInputStream {
    private:
        unsigned char buf;  // bitwise buffer (one byte)
        int nbits;          // number of bits that have been read from buf
        std::istream& in;   // reference to the bytewise input stream

    public:
        /** Initialize a BitInputStream that will use
         *  the given istream for input.
         */
        BitInputStream(std::istream& is);

        /** Fill the buffer from the input stream.
         */
        void fill();

        /** Read and return the next bit from the bit buffer.
         *  Fill it first if necessary.
         */
        unsigned int readBit();

        /** Read 3-byte ints from the input stream, 
         *  and return it as regular 4-byte int. 
         */
        int readThreeByteInt();
};

#endif // BITINPUTSTREAM_H
