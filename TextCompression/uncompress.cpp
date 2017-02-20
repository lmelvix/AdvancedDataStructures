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

    // Read the frequency from compressed file header and load to the array
    ifstream in;
    in.open(infile, ios::binary);
    if(!in.is_open()) {
        cout << "Invalid infile path. Please try again." << endl;
        return -1;
    }
    // Read 256 3-byte ints from the header
    BitInputStream bitin(in);
    for(int i = 0; i < 256; i++)
        freqs[i] = bitin.readThreeByteInt(); 

    // Sum the frequencies in array to get original file size in bytes
    int byteCount = 0;
    for(int item : freqs)
        byteCount += item;

    // Prite byte count out for debugging
    // cout << "Bytecount: " << byteCount << endl;

    /*
    // Print frequncies out for debugging 
    for (unsigned int i = 0; i < freqs.size(); i++) {
    if(freqs[i] != 0)
    cout << i << '\t' << (unsigned char)i << '\t' << freqs[i] << endl;
    }*/

    HCTree hc;
    hc.build(freqs);

    // Decode infile and write to outfile
    ofstream out;    
    out.open(outfile);
    int byteDecoded = 0;
    while(hc.hasRoot() && byteDecoded < byteCount) {
        unsigned char nextChar = hc.decode(bitin);       
        if(in.eof())
            break;
        out.put(nextChar);
        byteDecoded++;
    }

    in.close();
    out.close();
    return 1;
}
