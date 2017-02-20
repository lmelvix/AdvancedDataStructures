/* ActorGraph.h
 * Actor Graph class definition.
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <map>
#include "ActorNode.h"
#include "ActorEdge.h"
#include "MovieActorList.h"

using namespace std;

class ActorGraph {
    private:
        /* Hash map storing the nodes of the graph. 
         * Key = Actor name.
         * Value = Pointer to that actor's node.
         */
        unordered_map<string, ActorNode*> nodes;

    public:
        /* Constructor */
        ActorGraph() {}

        /* Insert node to graph.
         * Return true if node was inserted successfully, and false otherwise.
         * Inserting duplicate actor is not allowed.
         */
        bool insertNode(string actorName);

        /* Find the node with input actorName.
         * Return pointer to the node if found.
         * Return nullpointer if not found.
         */
        ActorNode* findNode(string actorName);

        /* Insert an undirected edge to graph.
         * Return true if edge was inserted successfully, and false otherwise.
         */
        bool insertEdge(string src, string dst, string movie, int year, int weight);

        /* Insert a directed edge to graph.
         * Return true if edge was inserted successfully, and false otherwise.
         * Self-loop is not allowed.
         */
        bool insertDirectedEdge(string src, string dst, string movie, int year, int weight);

        /* Load the graph from a tab-delimited file of actor->movie relationships.
         *
         * in_filename - input filename
         * use_weighted_edges - 
         *     if true, compute edge weights as 1 + (2015 - movie_year), 
         *     otherwise all edge weights will be 1
         *
         * Return true if file was loaded sucessfully, and false otherwise.
         */
        bool loadFromFile(const char* in_filename, bool use_weighted_edges);

        /* Run Breadth First Search on the graph, starting at src node.
         * Populate nodes with path data as it runs.
         * Return true if a path exists from src to dst, and false otherwise.
         */
        bool BFS(string src, string dst);

        /* Run Dijkstra's algorithm on the graph, starting at src node.
         * Populate nodes with path data as it runs.
         * Return false if src or dst node doesn't exist.
         */
        bool Dijkstras(string src, string dst);

        /* Run Dijkstras/BFS from src to dst and return the path string.
         * use_weighted_path = true -> Dijkstras
         * use_weighted_path = false -> BFS 
         */
        string actorPath(string src, string dst, bool use_weighted_path);

        /* Prepare the graph for actorconnections algorithm by
         * creating nodes with no edges for all actors and 
         * return pointer to a map containing movie->actors data.
         */
        map<string, MovieActorList*>* prepActorConnections(const char* in_filename);

        /* Run actorconnections algorithm on list of src and dst pairs.
         * Use movie data map returned from prepActorConnections.
         * Return vector of actorconnections data for each input pair.
         */
        vector<string> actorConnections(map<string, MovieActorList*>* movie_map,
            vector<string> src, vector<string> dst);

        /* Destructor */
        ~ActorGraph();
};

#endif // ACTORGRAPH_H
