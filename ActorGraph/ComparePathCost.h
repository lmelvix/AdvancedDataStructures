/* ComparePathCost.h
 * Comparison class for priority queue used in Dijkstra's algorithm.
 */

#ifndef COMPAREPATHCOST_H
#define COMPAREPATHCOST_H

class ComparePathCost {
    public:
        bool operator()(pair<int, ActorNode*> n1, pair<int, ActorNode*> n2) {
            if(n1.first != n2.first)
                return n1.first > n2.first;
            else
                return n1.second->name > n2.second->name;
        }
};

#endif // COMPAREPATHCOST_H
