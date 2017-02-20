#include <iostream>
#include <fstream>
#include <cstdlib>
#include "HCTree.h"

using namespace std;

int main(int argc, char** argv) {
    if(!(argv[1] && argv[2])) {
        cout << "Not enough arguments." << endl;
        return -1;
    }

    string infile = argv[1];
    string outfile = argv[2];

    // Array to store the frequency of each byte pattern
    vector<int> freqs(256, 0);

    // Read file and load the array with frequencies
    ifstream in;
    in.open(infile, ios::binary);
    if(!in.is_open()) {
        cout << "Invalid infile path. Please try again." << endl;
        return -1;
    }
    unsigned char nextChar;
    while(true) {
        nextChar = in.get();
        if(in.eof())
            break;
        freqs[nextChar]++;
    }
    in.close();

    /*
    // Print frequencies out for debugging 
    for (unsigned int i = 0; i < freqs.size(); i++) {
    if(freqs[i] != 0)
    cout << i << '\t' << (unsigned char)i << '\t' << freqs[i] << endl;
    } 
    */

    HCTree hc;
    hc.build(freqs);

    // Write header to outfile 
    ofstream out;
    out.open(outfile);
    BitOutputStream bitout(out);
    for (int item : freqs)
        bitout.writeThreeByteInt(item);

    // Encode infile
    in.open(infile, ios::binary);
    while(true) {
        nextChar = in.get();
        if(in.eof())
            break;
        hc.encode(nextChar, bitout);       
    }

    // Must make sure that all bits have been written,
    // even if they are not multiple of 8 bits.
    // Do this by writing dummy 0s.
    bitout.fillByte();

    in.close();
    out.close();
    return 1;
}
