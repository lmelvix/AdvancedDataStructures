#include <queue>
#include <vector>
#include <fstream>
#include <queue>
#include <stack>
#include "HCNode.h"
#include "HCTree.h"

typedef unsigned char byte;

/** Check if root of Huffman tree exists.
 *  Used in case of empty file.
 */
bool HCTree::hasRoot() {
    return root;
}

/** Use the Huffman algorithm to build a Huffman coding trie.
 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
 *  the frequency of occurrence of byte i in the message.
 *  POSTCONDITION:  root points to the root of the trie,
 *  and leaves[i] points to the leaf node containing byte i.
 */
void HCTree::build(const vector<int>& freqs) {
    std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp> pq;
    // Create nodes for each slot of freqs
    for(unsigned int i = 0; i < freqs.size(); i++) {
        if(freqs[i] != 0) {
            leaves[i] = new HCNode(freqs[i], (byte)i, 0, 0, 0); 
            pq.push(leaves[i]);
        }
    }

    while(!pq.empty()) {
        auto n1 = pq.top();
        pq.pop();

        if(pq.empty()) {
            root = n1;
            return;
        }

        auto n2 = pq.top();
        pq.pop();

        int new_count = n1->count + n2->count;

        // Symbol will be of the one with higher priority (n1)
        // n1 will always be left child
        HCNode* new_node = new HCNode(new_count, n1->symbol, n1, n2, 0);
        n1->p = new_node;
        n2->p = new_node;
        pq.push(new_node); 
    }
}

/** Write to the given BitOutputStream
 *  the sequence of bits coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    HCNode* curr = leaves[symbol];

    // If curr is the root, don't write anything.
    // It means that there is only one character in the original file.
    // We can use just the frequency info. No need to write bits.
    if(!curr->p)
        return;

    stack<int> stack;
    while(curr->p) {
        if(curr == curr->p->c0)
            stack.push(0);
        else if(curr == curr->p->c1)
            stack.push(1);
        curr = curr->p;
    }

    while(!stack.empty()) {
        out.writeBit(stack.top());
        stack.pop();
    }
}

/** Write to the given ofstream
 *  the sequence of bits (as ASCII) coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT 
 *  BE USED IN THE FINAL SUBMISSION.
 */
void HCTree::encode(byte symbol, ofstream& out) const {
    HCNode* curr = leaves[symbol];

    // If curr is the root, don't write anything.
    // It means that there is only one character in the original file.
    // We can use just the frequency info. No need to write bits.
    if(!curr->p)
        return;

    stack<int> stack;
    while(curr->p) {
        if(curr == curr->p->c0)
            stack.push(0);
        else if(curr == curr->p->c1)
            stack.push(1);
        curr = curr->p;
    }

    while(!stack.empty()) {
        out << stack.top();
        stack.pop();
    }
}

/** Return symbol coded in the next sequence of bits from the stream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
int HCTree::decode(BitInputStream& in) const {
    unsigned int nextBit;
    auto curr = root;

    // If root has no children, do nothing.

    while(curr->c0 && curr->c1) {
        nextBit = in.readBit();
        if(nextBit == 0)
            curr = curr->c0;
        else
            curr = curr->c1;
    }

    // At this point, curr is a leaf node
    return curr->symbol;
}

/** Return the symbol coded in the next sequence of bits (represented as 
 *  ASCII text) from the ifstream.
 *  PRECONDITION: build() has been called, to create the coding tree, and 
 *  initialize root pointer and leaves vector. Root of tree must exist.
 *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT BE USED
 *  IN THE FINAL SUBMISSION.
 */
int HCTree::decode(ifstream& in) const {
    unsigned char nextChar;
    auto curr = root;

    // If root has no children, do nothing.

    while(curr->c0 && curr->c1) {
        nextChar = in.get();
        if(nextChar == '0')
            curr = curr->c0;
        else
            curr = curr->c1;
    }

    // At this point, curr is a leaf node
    return curr->symbol;
}

/* Destructor */
HCTree::~HCTree() {
    deleteAll(root);
}

void HCTree::deleteAll(HCNode* n) {
    if (!n)
        return;

    deleteAll(n->c0);
    deleteAll(n->c1);
    delete n;
}
