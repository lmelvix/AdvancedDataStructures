#include <iostream>
#include "HCNode.h"

typedef unsigned char byte;

bool HCNode::operator<(const HCNode& other) {
    if(count != other.count)
        return count > other.count; // Node with less count has higher priority
    return symbol > other.symbol;   // Node with "smaller" symbol has higher priority
}

