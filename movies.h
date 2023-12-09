#ifndef MOVIES_H
#define MOVIES_H
#include <string>
#include <iostream>
using std::string, std::ostream;
struct movie{
    string name;
    double rating;
    movie(string str="", double r=-1): name(str), rating(r){}
};
inline ostream& operator<<(ostream& o, const movie& m){
    o<<m.name<<", "<<m.rating;
    return o;
}
#endif