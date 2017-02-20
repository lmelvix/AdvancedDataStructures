/* ActorNode.h
 * Node structure definition for Actor Graph.
 */

#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <vector>
#include <limits>

using namespace std;

struct ActorEdge;
struct ActorNode {
    public:
        ActorNode(string name) :
            name(name), distance(numeric_limits<int>::max()), 
            prevNode(0), prevMovie(""), prevYear(-1), done(false) {}

        string name;
        vector<ActorEdge> adjList;

        int distance;
        
        ActorNode* prevNode;
        string prevMovie;
        int prevYear;

        bool done; // For Dijkstra's algorithm
};

#endif // ACTORNODE_H
