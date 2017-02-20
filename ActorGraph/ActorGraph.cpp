/* ActorGraph.cpp
 * Actor Graph class implementation.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <map>
#include "ActorGraph.h"
#include "ComparePathCost.h"

using namespace std;

/* Insert node to graph.
 * Return true if node was inserted successfully, and false otherwise.
 * Inserting duplicate actor is not allowed.
 */
bool ActorGraph::insertNode(string actorName) {
    try {
        nodes.at(actorName);
        return false;
    }
    catch (const std::out_of_range& oor) {
        ActorNode* newNode = new ActorNode(actorName);   
        nodes.insert({actorName, newNode});
        return true;
    }
}

/* Find the node with input actorName.
 * Return pointer to the node if found.
 * Return nullpointer if not found.
 */
ActorNode* ActorGraph::findNode(string actorName) {
    try {
        return nodes.at(actorName);
    } catch (const std::out_of_range& oor) {
        return nullptr;
    }
}

/* Insert an undirected edge to graph.
 * Return true if edge was inserted successfully, and false otherwise.
 */
bool ActorGraph::insertEdge(string src, string dst, string movie, int year, int weight) {
    if(!insertDirectedEdge(src, dst, movie, year, weight))
        return false;
    if(!insertDirectedEdge(dst, src, movie, year, weight))
        return false;
    return true;
}

/* Insert a directed edge to graph.
 * Return true if edge was inserted successfully, and false otherwise.
 * Self-loop is not allowed.
 */
bool ActorGraph::insertDirectedEdge(string src, string dst, string movie, int year, int weight) {
    if(src == dst)
        return false;

    auto srcNode = findNode(src);
    if(!srcNode)
        return false;

    auto dstNode = findNode(dst);
    if(!dstNode)
        return false;

    // Create edge
    ActorEdge newEdge(dstNode, movie, year, weight);

    // Insert edge
    srcNode->adjList.push_back(newEdge);    

    return true;
}

/* Load the graph from a tab-delimited file of actor->movie relationships.
 *
 * in_filename - input filename
 * use_weighted_edges - 
 *     if true, compute edge weights as 1 + (2015 - movie_year), 
 *     otherwise all edge weights will be 1
 *
 * Return true if file was loaded sucessfully, and false otherwise.
 */
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;

    // Create an ordered map to store movies and list of actors in that movie
    // Key = to_string(movie_year) + movie_title (so keys are ordered by year)
    map<string, MovieActorList*> movie_map;

    // Keep reading lines until the end of file is reached
    while(infile) {
        string s;

        // Get the next line
        if(!getline(infile, s)) break;

        if(!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss(s);
        vector<string> record;

        while(ss) {
            string next;

            // Get the next string before hitting a tab character and put it in 'next'
            if(!getline(ss, next, '\t')) 
                break;
            record.push_back(next);
        }

        if(record.size() != 3) {
            // We should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

        // Create actor nodes (Note: Method prevents duplicates)
        insertNode(actor_name);

        // Try finding the movie in map
        try {
            // Movie exists. 
            // Add actor name to movie's list.
            MovieActorList* malist = movie_map.at(to_string(movie_year) + movie_title);
            (*malist).actorList.push_back(actor_name);
        }
        catch (const std::out_of_range& oor) {
            // Movie doesn't exist. 
            // Create new MovieActorList object with actor name in list.
            MovieActorList* new_malist = new MovieActorList(movie_title, movie_year);
            new_malist->actorList.push_back(actor_name);
            movie_map.insert({to_string(movie_year) + movie_title, new_malist});
        }
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    
    infile.close();

    // Add edges to graph using data from movie_map
    for (auto pair : movie_map) {
        MovieActorList* item = pair.second;
        int num_actors = item->actorList.size();

        for (int i = 0; i < num_actors; i++) {
            for(int j = i; j < num_actors; j++) {
                int weight;

                if(use_weighted_edges)
                    weight = 1 + (2015 - item->year);
                else
                    weight = 1;

                insertEdge(item->actorList[i], item->actorList[j], item->movie, item->year, weight);
            }
        }

        // Delete MovieActorList object after finished adding edges for that movie
        delete item;
    }

    return true;
}

/* Run Breadth First Search on the graph, starting at src node.
 * Populate nodes with path data as it runs.
 * Return true if a path exists from src to dst, and false otherwise.
 */
bool ActorGraph::BFS(string src, string dst) {
    // Set distance of all nodes to INT_MAX and reset prev data
    for (auto item : nodes) {
        item.second->distance = numeric_limits<int>::max();
        item.second->prevNode = nullptr;
        item.second->prevMovie = "";
        item.second->prevYear = -1;
    }

    auto srcNode = findNode(src);
    if (!srcNode)
        return false;

    srcNode->distance = 0;

    auto dstNode = findNode(dst);
    if(!dstNode)
        return false;

    // BFS queue
    queue<ActorNode*> q;
    q.push(srcNode);

    while(!q.empty()) {
        auto curr = q.front();
        q.pop();

        // For each of curr's neighbour: edge.nextNode
        for(ActorEdge edge : curr->adjList) {
            // If neighbour hasn't been visited
            if(edge.nextNode->distance > curr->distance + 1) {
                // Neighbour distance = curr's distance + 1
                edge.nextNode->distance = curr->distance + 1;

                // Prev data = curr's data
                edge.nextNode->prevNode = curr;
                edge.nextNode->prevMovie = edge.movie;
                edge.nextNode->prevYear = edge.year;

                // If found dst node, terminate BFS
                if(edge.nextNode == dstNode)
                    return true;

                q.push(edge.nextNode);
            }
        }
    }

    return false;
}

/* Run Dijkstra's algorithm on the graph, starting at src node.
 * Populate nodes with path data as it runs.
 * Return false if src or dst node doesn't exist.
 */
bool ActorGraph::Dijkstras(string src, string dst) {
    // Set distance of all nodes to INT_MAX and reset prev data 
    for(auto item : nodes) {
        item.second->distance = numeric_limits<int>::max();
        item.second->prevNode = nullptr;
        item.second->prevMovie = "";
        item.second->prevYear = -1;
        item.second->done = false;
    }

    auto srcNode = findNode(src);
    if (!srcNode)
        return false;

    srcNode->distance = 0;

    auto dstNode = findNode(dst);
    if(!dstNode)
        return false;

    // Priority queue to store paths
    priority_queue<pair<int, ActorNode*>, vector<pair<int, ActorNode*>>, ComparePathCost> pq;
    pq.push(make_pair(srcNode->distance, srcNode));

    while(!pq.empty()) {
        auto curr = pq.top();
        pq.pop();

        if(!curr.second->done) {
            curr.second->done = true;
            for(ActorEdge edge : curr.second->adjList) {
                int c = curr.second->distance + edge.weight;

                // Update path details if this path thru curr is better
                if(c < edge.nextNode->distance) {
                    edge.nextNode->distance = c;
                    edge.nextNode->prevNode = curr.second;
                    edge.nextNode->prevMovie = edge.movie;
                    edge.nextNode->prevYear = edge.year;

                    pq.push(make_pair(c, edge.nextNode));
                }
            }
        }
    }

    // Always run on connected graph
    return true;
}

/* Run Dijkstras/BFS from src to dst and return the path string.
 * use_weighted_path = true -> Dijkstras
 * use_weighted_path = false -> BFS 
 */
string ActorGraph::actorPath(string src, string dst, bool use_weighted_path) {
    string output = "";
    bool succeed;

    // Run pathfinding algorithm on graph
    if(use_weighted_path)
        succeed = Dijkstras(src, dst);
    else
        succeed = BFS(src, dst);

    // If pathfinding succeeded, push path data into stack starting from dst node
    if(succeed) {
        auto curr = findNode(dst);
        stack<string> path;
        while(true) {
            path.push("(" + curr->name + ")");

            if(curr->prevMovie != "")
                path.push("--[" + curr->prevMovie + "#@" + to_string(curr->prevYear) + "]-->");

            if(curr->prevNode)
                curr = curr->prevNode;
            else
                break;
        }

        // Reverse stack content to get true path order
        while(!path.empty()) {
            output += path.top();
            path.pop();
        }
    } else {
        output = "Path from " + src + " to " + dst + " doesn't exist.";
    }

    return output;
}

/* Prepare the graph for actorconnections algorithm by
 * creating nodes with no edges for all actors and 
 * return pointer to a map containing movie->actors data.
 */
map<string, MovieActorList*>* ActorGraph::prepActorConnections(const char* in_filename) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
    
    // Create an ordered map to store movies and list of actors in that movie
    // Key = to_string(movie_year) + movie_title (so keys are ordered by year)
    map<string, MovieActorList*>* movie_map = new map<string, MovieActorList*>;
    
    // Keep reading lines until the end of file is reached
    while(infile) {
        string s;
        if (!getline(infile, s)) break;
        if (!have_header) {
            have_header = true;
            continue;
        }
        istringstream ss(s);
        vector<string> record;
        while(ss) {
            string next;
            if (!getline(ss, next, '\t'))
                break;
            record.push_back(next);
        }
        if(record.size() != 3) {
            continue;
        }
     
        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
     
        // Create actor nodes (Note: Method prevents duplicates)   
        insertNode(actor_name);
        
        // Try finding the movie in map
        try {
            // Movie exists. 
            // Add actor name to movie's list.
            MovieActorList* malist = (*movie_map).at(to_string(movie_year) + movie_title);
            (*malist).actorList.push_back(actor_name);
        } catch (const std::out_of_range& oor) {
            // Movie doesn't exist. 
            // Create new MovieActorList object with actor name in list.
            MovieActorList* new_malist = new MovieActorList(movie_title, movie_year);
            new_malist->actorList.push_back(actor_name);
            (*movie_map).insert({to_string(movie_year) + movie_title, new_malist});
        }
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return nullptr;
    }
    infile.close();

    return movie_map;
}

/* Run actorconnections algorithm on list of src and dst pairs.
 * Use movie data map returned from prepActorConnections.
 * Return vector of actorconnections data for each input pair.
 */
vector<string> ActorGraph::actorConnections(map<string, MovieActorList*>* movie_map, vector<string> src, vector<string> dst) {
    // Clear edges for all nodes
    for (auto item : nodes)
        item.second->adjList.clear();

    // Starting year of our movie data set
    int prev_y;
    if((*movie_map).begin() == (*movie_map).end())
        prev_y = 9999;
    else
        prev_y = (*movie_map).begin()->second->year;

    vector<string> output;  // Vector to store output of each pair
    vector<bool> done;      // Vector to store status of each pair
    int num_pairs = src.size();

    for(int i = 0; i < num_pairs; i++) {
        output.push_back(src[i] + '\t' + dst[i] + '\t');
        done.push_back(false);
    }

    // For each movie
    for(auto pair : *movie_map) {
        MovieActorList* item = pair.second;
        int num_actors = item->actorList.size();
        int y = item->year;

        // If this movie's year != prev movie's year, run BFS for each undone pair
        if(prev_y != y) {
            for(int i = 0; i < num_pairs; i++) {
                if(!done[i]) {
                    bool connected = BFS(src[i], dst[i]);
                    if(connected) {
                        output[i] += to_string(prev_y);
                        done[i] = true;
                    }
                }
            }

            prev_y = y;
        }

        // Add edges for this movie
        for(int i = 0; i < num_actors; i++) {
            for(int j = i; j < num_actors; j++) {
                insertEdge(item->actorList[i], item->actorList[j], item->movie, item->year, 1);
            }
        }
    }

    // Run BFS for all undone pairs again, in case all movies are from the same year
    for(int i = 0; i < num_pairs; i++) {
        if(!done[i]) {
            bool connected = BFS(src[i], dst[i]);
            if(connected)
                output[i] += to_string(prev_y);
            else
                output[i] += "9999";
        }
    }

    return output;
}

/* Destructor */
ActorGraph::~ActorGraph() {
    for(auto item : nodes)
        delete item.second;
}
