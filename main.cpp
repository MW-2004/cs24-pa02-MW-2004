// Spring'22
// Instructor: Diba Mirza
// Student name: Marcus Wu
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include "movies.h"
#include <array>
using namespace std;
bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}
/*bool buffered_getline(std::istream& input, std::string& output) {
    static char buffer[4096];
    static size_t bufferIndex = 0;
    static size_t bytesRead = 0;

    output.clear();  // Clear the output string

    while (true) {
        if (bufferIndex == bytesRead) {
            input.read(buffer, 4096);
            bytesRead = input.gcount();  // Number of bytes read
            bufferIndex = 0;

            if (bytesRead == 0) {
                // End of file
                return false;
            }
        }

        char currentChar = buffer[bufferIndex++];
        if (currentChar == '\n') {
            // Found newline character, stop reading
            return true;
        } else {
            // Append character to the output string
            output.push_back(currentChar);
        }
    }
}*/
#define buffered_getline getline
int main(int argc, char** argv){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << '\n';
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << '\n';
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    string line, movieName;
    double movieRating;
    vector<movie> movies;
    movies.reserve(1000);
    while (buffered_getline(movieFile, line) && parseLine(line, movieName, movieRating)){
        movies.emplace_back(movieName, movieRating);
    }
    sort(movies.begin(), movies.end(), [](const movie&a, const movie&b){return a.name<b.name;});

    movieFile.close();

    if (argc == 2){
        for(movie e: movies){
            cout<<e<<'\n';
        }
        return 0;
    }
    int indexes[28];
    char curr='a'-1;
    for(size_t i=0;i<movies.size();i++){
        char ch=movies[i].name[0];
        while(curr<ch){
            curr++;
            indexes[curr-'a']=i;
        }
    }
    while(curr<'z'+1){
        curr++;
        indexes[curr-'a']=movies.size();
    }
    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    prefixes.reserve(100);
    while (buffered_getline(prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }
    vector<movie> bests;
    struct comparer{
        bool operator()(const movie& movie, const string& prefix){return movie.name.compare(0, prefix.size(), prefix)<0;}
        bool operator()(const string& prefix, const movie& movie){return movie.name.compare(0, prefix.size(), prefix)>0;}
    } cmp;
    for(string pr: prefixes){
        char ch=pr[0]-'a';
        int lb=indexes[(int)ch];
        int ub=indexes[(int)ch+1];
        auto st=lower_bound(movies.begin()+lb, movies.begin()+ub, pr, cmp);
        if(st!=movies.end() && st->name.find(pr)!=string::npos){
            auto ed=upper_bound(st, movies.begin()+ub, pr, cmp);
            vector<movie> res(st,ed);
            stable_sort(res.begin(), res.end(), [](const movie &a, const movie &b){return a.rating>b.rating;});
            for(auto iter=st;iter!=ed;iter++) cout<<*iter<<'\n';
            cout<<'\n';
            bests.push_back(*st);
        }
        else{
            bests.push_back(movie{});
            cout << "No movies found with prefix "<<pr<< '\n';
        }
    }
    cout<<std::fixed << std::setprecision(1);
    for(size_t i=0;i<prefixes.size();i++){
        if(bests[i].name!="")
            cout << "Best movie with prefix " << prefixes[i] << " is: " << bests[i].name << " with rating " << bests[i].rating << '\n';
    }
    return 0;
}

/* Add your run time analysis for part 3 of the assignment here as commented block*/