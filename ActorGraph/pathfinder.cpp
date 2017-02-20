/* pathfinder.cpp
 * Program to find weighted/unweighted shortest path between actors.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "ActorGraph.h"

using namespace std;

int main(int argc, char** argv) {
    if(!(argv[1] && argv[2] && argv[3] && argv[4])) {
        cout << "Invalid arguments. Please try again." << endl;
        return -1;
    }

    char* movie_cast = argv[1];
    string edge_option = argv[2];
    char* test_pairs = argv[3];
    char* out_paths = argv[4];

    bool use_weighted_path;

    if(edge_option == "u")
        use_weighted_path = false; // Unweighted pathfinding
    else if(edge_option == "w")
        use_weighted_path = true;  // Weighted pathfinding
    else {
        cout << "Invalid weight option (u or w only). Please try again." << endl;
        return -1;
    }

    ActorGraph g;
    g.loadFromFile(movie_cast, use_weighted_path);

    // Read pair from test_pairs
    vector<string> src;
    vector<string> dst;

    ifstream infile(test_pairs);

    bool have_header = false;

    // Keep reading lines until the end of file is reached
    while(infile) {
        string s;
        if(!getline(infile, s)) 
            break;
        
        if(!have_header) {
            have_header = true;
            continue;
        }
        
        istringstream ss( s );
        vector <string> record;
        while(ss) {
            string next;
            if(!getline(ss, next, '\t')) 
                break;
            record.push_back(next);
        }

        if(record.size() != 2)
            continue;

        src.push_back(record[0]);
        dst.push_back(record[1]);
    }

    if (!infile.eof())
        cerr << "Failed to read " << test_pairs << "!\n";
    infile.close();

    ofstream outfile(out_paths);
    outfile << "(actor)--[movie#@year]-->(actor)--..." << '\n';

    // Run pathfinder algorithm for each pair and write output to outfile
    for(int i = 0; i < src.size(); i++)
        outfile << g.actorPath(src[i], dst[i], use_weighted_path) << '\n';

    outfile.close();
    return 0;
}
