/* UpTree.h
 * Union-Find Up Tree class definition.
 */

#ifndef UPTREE_H
#define UPTREE_H

#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include "UpTreeNode.h"
#include "MovieActorList.h"

using namespace std;

class UpTree {
    private:
        /* Hash map storing the nodes of the up tree. 
         * Key = Actor name.
         * Value = Pointer to that actor's up tree node.
         */
        unordered_map<string, UpTreeNode*> nodes;

    public:
        /* Constructor */
        UpTree() {}

        /* Insert node to graph.
         * Return true if node was inserted successfully, and false otherwise.
         * Inserting duplicate actor is not allowed.
         */
        bool insertNode(string actorName);

        /* Find the node with input actorName.
         * Return pointer to the node if found.
         * Return nullpointer if not found.
         */
        UpTreeNode* findNode(string actorName);

        /* Disjoint set find method.
         * Return the sentinel node of query actor.
         */
        UpTreeNode* findSet(string actorName);

        /* Disjoint set union method.
         * Union the sets that actorNameA and actorNameB belong.
         * Return true if union successfully.
         */
        bool unionSet(string actorNameA, string actorNameB);

        /* Prepare the disjoint set for actorconnections algorithm by
         * creating a set for each actors and 
         * return pointer to a map containing movie->actors data.
         */
        map<string, MovieActorList*>* prepActorConnections(const char* in_filename);

        /* Run actorconnections algorithm on list of src and dst pairs.
         * Use movie data map returned from prepActorConnections.
         * Return vector of actorconnections data for each input pair.
         */
        vector<string> actorConnections(map<string, MovieActorList*>* movie_map, vector<string> src, vector<string> dst);

        /* Destructor */
        ~UpTree();
};

#endif // UPTREE_H
