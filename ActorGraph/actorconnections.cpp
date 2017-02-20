/* actorconnections.cpp
 * Program to find the year in which two actors become connected.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "ActorGraph.h"
#include "UpTree.h"
#include "util.h"

using namespace std;

int main(int argc, char** argv) {
    if(!(argv[1] && argv[2] && argv[3] && argv[4])) {
        cout << "Invalid arguments. Please try again." << endl;
        return -1;
    }

    char* movie_cast = argv[1];
    char* test_pairs = argv[2];
    char* out_connections = argv[3];
    string alg = argv[4];

    if(!(alg == "bfs" | alg == "ufind")) {
        cout << "Invalid algorithm option (bfs or ufind only). Please try again." << endl;
        return -1;
    }

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

        if (!have_header) {
            have_header = true;
            continue;
        }

        istringstream ss(s);
        vector <string> record;
        while(ss) {
            string next;
            if(!getline(ss, next, '\t')) 
                break;
            record.push_back( next );
        }

        if(record.size() != 2)
            continue;

        src.push_back(record[0]);
        dst.push_back(record[1]);
    }

    if (!infile.eof())
        cerr << "Failed to read " << test_pairs << "!\n";
    infile.close();

    ofstream outfile(out_connections);
    outfile << "Actor1" << '\t' << "Actor2" << '\t' << "Year" << '\n';

    Timer timer;
    long long end_time;

    if(alg == "bfs") {
        ActorGraph g;
        
        // Build graph using movie_cast data
        auto movie_map = g.prepActorConnections(movie_cast);

        // Run actorconnection algorithm
        timer.begin_timer();
        auto output = g.actorConnections(movie_map, src, dst);
        end_time = timer.end_timer();

        // Write to outfile
        for(auto item : output)
            outfile << item << '\n';

        for(auto item : *movie_map)
            delete item.second;

        delete movie_map;
    } else {
        UpTree u;
        // Build disjoint sets using movie_cast data
        auto movie_map = u.prepActorConnections(movie_cast);

        // Run actorconnection algorithm
        timer.begin_timer();
        auto output = u.actorConnections(movie_map, src, dst);
        end_time = timer.end_timer();

        // Write to outfile
        for (auto item : output)
            outfile << item << '\n';

        for(auto item : *movie_map)
            delete item.second;

        delete movie_map;
    }

    outfile.close();

    cout << "Time for " << alg << ": " << end_time/(1000000.00) << " ms"<< endl;
    return 0;
}
