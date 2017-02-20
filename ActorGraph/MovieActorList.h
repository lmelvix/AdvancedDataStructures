/* MovieActorList.h
 * Structure used to store mappings between a movie and its starring actors.
 * Used in movie_map of pathfinder and actorconnections algorithm.
 */

#ifndef MOVIEACTORLIST_H
#define MOVIEACTORLIST_H

#include <vector>

struct MovieActorList {
    std::string movie;
    int year;
    mutable std::vector<string> actorList;

    MovieActorList(std::string movie, int year) : movie(movie), year(year) {}
};
#endif // MOVIEACTORLIST_H
