#include "movies.h"
movie::movie(string& s, double d) : name(s), rat(d) {}
best::best(string& s, movie& m) : pr(s), mov(m) {}
bool comparer::operator()(const movie& movie, const string& prefix) {
    return movie.name.compare(0, prefix.size(), prefix) < 0;
}
bool comparer::operator()(const string& prefix, const movie& movie) {
    return movie.name.compare(0, prefix.size(), prefix) > 0;
}
void timer() {
    static Timer t;
}