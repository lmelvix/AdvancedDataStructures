/* ActorEdge.h
 * Edge structure definition for Actor Graph.
 */

#ifndef ACTOREDGE_H
#define ACTOREDGE_H

#include "ActorNode.h"

using namespace std;

struct ActorEdge {
    public:
        ActorEdge(ActorNode* nextNode, string movie, int year, int weight) :
            nextNode(nextNode), movie(movie), year(year), weight(weight) {}

        ActorNode* nextNode;
        string movie;
        int year;
        int weight;
};

#endif // ACTOREDGE_H
