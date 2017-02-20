#ifndef BITIOUTPUTSTREAM_H
#define BITOUTPUTSTREAM_H

#include <iostream>

class BitOutputStream {
    private:
        unsigned char buf;  // bitwise buffer (one byte)
        int nbits;          // how many bits have been written to buf
        std::ostream& out;  // reference to the bytewise output stream

    public:
        /** Initialize a BitOutputStream that will use
         *  the given ostream for output.
         */
        BitOutputStream(std::ostream& os);

        /** Send buffer content to the output stream and clear the buffer.
        */
        void flush();

        /** Write the least significant bit of the argument to
         *  the bit buffer, and increment the bit buffer index.
         *  After that, flush the buffer if it is full.
         */
        void writeBit(unsigned int bit);

        /** Write dummy bits to fill a byte,
         *  to be able to write all bits (not multiple of 8) to output stream.
         */
        void fillByte();

        /** Write 4-byte int as 3-byte (24 bits) data
         */
        void writeThreeByteInt(int intToWrite);
};

#endif // BITOUTPUTSTREAM_H
