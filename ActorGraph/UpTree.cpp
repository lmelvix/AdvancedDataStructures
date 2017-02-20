/* UpTree.cpp
 * Union-Find Up Tree class implementation.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "UpTree.h"
#include "MovieActorList.h"

using namespace std;

/* Insert node to graph.
 * Return true if node was inserted successfully, and false otherwise.
 * Inserting duplicate actor is not allowed.
 */
bool UpTree::insertNode(string actorName) {
    try {
        nodes.at(actorName);
        return false;
    }
    catch (const std::out_of_range& oor) {
        UpTreeNode* newNode = new UpTreeNode(actorName);   
        nodes.insert({actorName, newNode});
        return true;
    }
}

/* Find the node with input actorName.
 * Return pointer to the node if found.
 * Return nullpointer if not found.
 */
UpTreeNode* UpTree::findNode(string actorName) {
    try {
        return nodes.at(actorName);
    } catch (const std::out_of_range& oor) {
        return nullptr;
    }
}

/* Disjoint set find method.
 * Return the sentinel node of query actor.
 */
UpTreeNode* UpTree::findSet(string actorName) {
    auto curr = findNode(actorName);
    if(!curr)
        return nullptr;

    // Vector to store nodes for path compression
    vector<UpTreeNode*> to_update_parent;

    while(curr->parent) {
        to_update_parent.push_back(curr);
        curr = curr->parent;
    }

    // Path compression
    for(auto node : to_update_parent) 
        node->parent = curr;

    return curr;
}

/* Disjoint set union method.
 * Union the sets that actorNameA and actorNameB belong.
 * Return true if union successfully.
 */
bool UpTree::unionSet(string actorNameA, string actorNameB) {
    auto a = findSet(actorNameA);
    if(!a)
        return false;

    auto b = findSet(actorNameB);
    if(!b)
        return false;

    // Actor A and B are already in the same set
    if(a == b)
        return true;

    // Union-by-size
    // If set A is larger than set B, A becomes the parent set
    if(a->size > b->size) { 
        b->parent = a;
        b->size = 0;
        a->size++;
    } else {
        a->parent = b;
        a->size = 0;
        b->size++;
    }
    return true;
}

/* Prepare the disjoint set for actorconnections algorithm by
 * creating a set for each actors and 
 * return pointer to a map containing movie->actors data.
 */
map<string, MovieActorList*>* UpTree::prepActorConnections(const char* in_filename) {
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
        }
        catch (const std::out_of_range& oor) {
            // Movie doesn't exist. 
            // Create new MovieActorList object with actor name in list.
            MovieActorList* new_malist = new MovieActorList(movie_title, movie_year);
            new_malist->actorList.push_back(actor_name);
            (*movie_map).insert({to_string(movie_year) + movie_title, new_malist});
        }
    }

    if(!infile.eof()) {
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
vector<string> UpTree::actorConnections
(map<string, MovieActorList*>* movie_map, vector<string> src, vector<string> dst) {
    // Isolate each node into their own set
    for(auto item : nodes) {
        item.second->parent = nullptr;
        item.second->size = 0;   
    }

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

        // If this movie's year != prev movie's year, check connection for each undone pair
        if(prev_y != y) {
            for(int i = 0; i < num_pairs; i++) {
                if(!done[i]) {
                    bool connected = (findSet(src[i]) == findSet(dst[i]));

                    if(connected) {
                        output[i] += to_string(prev_y);
                        done[i] = true;
                    }
                }
            }

            prev_y = y;
        }

        // Union actors in this movie
        for(int i = 0; i < num_actors; i++) {
            for(int j = i; j < num_actors; j++) {
                unionSet(item->actorList[i], item->actorList[j]);
            }
        }
    }
    
    // Check connection for all undone pairs again, in case all movies are from the same year
    for(int i = 0; i < num_pairs; i++) {
        if(!done[i]) {
            bool connected = (findSet(src[i]) == findSet(dst[i]));

            if(connected)
                output[i] += to_string(prev_y);
            else
                output[i] += "9999";
        }
    }
    
    return output;
}

/* Destructor */
UpTree::~UpTree() {
    for(auto item : nodes)
        delete item.second;
}
