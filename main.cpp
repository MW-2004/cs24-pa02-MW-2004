// Spring'22
// Instructor: Diba Mirza
// Student name: Marcus Wu
#include <iostream>
#include <functional>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "movies.h"
using namespace std;
bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {//alter the order, reduce copy (substr)
        movieName = line.substr(1, commaIndex - 2);
    }else{
        movieName = line.substr(0, commaIndex);
    }
    return true;
}
/*
struct movie: string, double;
struct best: string, reference_wrapper<movie>;
*/
int main(int argc, char** argv){
    //timer();
    ios_base::sync_with_stdio(false);//not useful when -Ofast
    cin.tie(NULL);//not useful when -Ofast
    string file1, file2;
    /* only for debug
    if(argc =  = 1){
        file1 = "input_100_random.csv";
        file2 = "prefix_large.txt";
    }*/
    if(argc == 2) file1 = argv[1];
    if(argc == 3){
        file1 = argv[1];
        file2 = argv[2];
    }
    ifstream movieFile (file1);
    if (movieFile.fail()){
        cerr << "Could not open file " << file1;
        exit(1);
    }
    string line, movieName;
    double movieRating;

    vector<movie> movies;
    movies.reserve(100);
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
        movies.emplace_back(movieName,movieRating);
    }
    sort(movies.begin(), movies.end(), 
        [](const movie &a, const movie &b){return a.name<b.name;});

    movieFile.close();

    if (argc == 2){
        for(movie &e: movies) cout << e.name << ", " << e.rat << '\n';
        return 0;
    }

    //help reduce binary search's range
    int indexes[27];
    char curr = 'a' - 1;
    for(size_t i = 0;i < movies.size(); i++){
        char ch = movies[i].name[0];
        while(curr<ch){
            curr++;
            indexes[curr - 'a'] = i;
        }
    }
    while(curr < 'z' + 1){//let indexes['z"+1-'a'] won't overflow
        curr++;
        indexes[curr - 'a'] = movies.size();
    }

    ifstream prefixFile (file2);
    if (prefixFile.fail()) {
        cerr << "Could not open file " << file2;
        exit(1);
    }

    vector<best> bests;
    bests.reserve(1000);

    getline(prefixFile, line);
    do{// use do-while to reduce if
        string &prefix = line;//no cost after -Ofast
        int lower = indexes[prefix[0] - 'a'];
        int upper = indexes[prefix[0] - 'a' + 1];
        vector<movie>::iterator st = 
            lower_bound(movies.begin() + lower, movies.begin() + upper, prefix, comparer{});
        if(st != movies.end() && st->name.find(prefix) == 0){
            vector<movie>::iterator ed = 
                upper_bound(st, movies.begin() + upper, prefix, comparer{});
            vector<reference_wrapper<movie>> founds(st, ed);

            stable_sort(founds.begin(), founds.end(), 
                [](const movie &a, const movie &b){return a.rat>b.rat;});

            for(movie &e: founds) cout << e.name << ", " << e.rat << '\n';
            cout<<'\n';
            bests.emplace_back(prefix, founds.front());
        }
        else{
            cout<<"No movies found with prefix "<<prefix<<'\n';
        }
        
    }while(getline(prefixFile, line));
    
    for(best &e: bests){
        cout << "Best movie with prefix " << e.pr << " is: " << e.mov.get().name
            << " with rating " << e.mov.get().rat << '\n';
    }
    return 0;
}
/*
runtime:
    1. emplace_back n movies to vector and then sort: O(l*n + n*log(n))
    2. create an array of indexes for the first occur of every letter
        to reduce the constant of binary search: O(n)
    3. for m prefixes:
        binary search for lower bound, upper bound: Olog(n)
        copy the references, then stable_sort: O(k*log(k)) 
        store the best movie's reference, O(1)
        total: O(m(k*log(k)+log(n)))
    4. print all best movies: O(m)
in total: O(n(1+log(n)) + m(k*log(k)+log(n)))

memory:
    1. vector<movie> movies has n movie of each l chars, O(nl)
    2. for each prefix, vector<best> bests stores references of movie 
        and prefix names which is considered O(1), so the vector is O(m)
in total: O(n*l + m)

tradeoff:
    I already optimize the memory complexity
    which is at least O(n*l+m) to store necessary data.
    if movies' number is much larger, and there may be duplicated prefixs,
    I can use unordered map to map from first char to 
    map<second char, sorted vector of movies>,
    or even more layers of mapping
    this may reduce run time, but requires more memory.

*/