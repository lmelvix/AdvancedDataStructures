/* UpTreeNode.h
 * Node structure definition for Union-Find Up Tree.
 */

#ifndef UPTREENODE_H
#define UPTREENODE_H

using namespace std;

struct UpTreeNode {
    public:
        UpTreeNode(string name) : name(name), parent(0), size(0) {}

        string name;
        UpTreeNode* parent;
        int size;
};

#endif // UPTREENODE_H
