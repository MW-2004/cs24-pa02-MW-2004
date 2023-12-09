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
    movieRating = stod(line.substr(commaIndex + 1));
    if (movieName[0] == '\"') {
        movieName = line.substr(1, commaIndex - 2);
    } else {
        movieName = line.substr(0, commaIndex);
    }
    return true;
}

int main(int argc, char **argv) {
    timer();
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string file1, file2, outputFile = "a.out"; // Set the output file to "a.out"

    if (argc == 1) {
        file1 = "input_100_random.csv";
        file2 = "prefix_large.txt";
    }
    if (argc == 2) {
        file1 = argv[1];
    }
    if (argc == 3) {
        file1 = argv[1];
        file2 = argv[2];
    }

    ifstream movieFile(file1);

    if (movieFile.fail()) {
        cerr << "Could not open file " << file1;
        exit(1);
    }

    string line, movieName;
    double movieRating;
    vector<movie> movies;
    movies.reserve(100);

    while (getline(movieFile, line) && parseLine(line, movieName, movieRating)) {
        movies.emplace_back(movieName, movieRating);
    }

    sort(movies.begin(), movies.end(), [](const movie &a, const movie &b) { return a.name < b.name; });

    movieFile.close();

    // Redirect output to "a.out"
    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cerr << "Could not open output file " << outputFile;
        exit(1);
    }

    if (argc == 2) {
        for (movie &e : movies) {
            outFile << e.name << ", " << e.rat << '\n';
        }
        return 0;
    }

    int indexes[27]; // Help reduce binary search's range
    char curr = 'a' - 1;
    for (size_t i = 0; i < movies.size(); i++) {
        char ch = movies[i].name[0];
        while (curr < ch) {
            curr++;
            indexes[curr - 'a'] = i;
        }
    }
    while (curr < 'z' + 1) { // Let indexes['z"+1-'a'] won't overflow
        curr++;
        indexes[curr - 'a'] = movies.size();
    }

    ifstream prefixFile(file2);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << file2;
        exit(1);
    }

    vector<best> bests;
    bests.reserve(1000);
    getline(prefixFile, line);
    do {
        int lower = indexes[line[0] - 'a'];
        int upper = indexes[line[0] - 'a' + 1];
        auto st = lower_bound(movies.begin() + lower, movies.begin() + upper, line, comparer{});
        if (st != movies.end() && st->name.find(line) == 0) {
            auto ed = upper_bound(st, movies.begin() + upper, line, comparer{});
            vector<reference_wrapper<movie>> founds(st, ed);
            stable_sort(founds.begin(), founds.end(), [](const movie &a, const movie &b) { return a.rat > b.rat; });
            for (movie e : founds) outFile << e.name << ", " << e.rat << '\n';
            if (founds.empty()) {
                exit(1);
            }
            bests.emplace_back(line, founds.front());
            outFile << '\n';
        } else {
            outFile << "No movies found with prefix " << line << '\n';
        }

    } while (getline(prefixFile, line));

    for (size_t i = 0; i < bests.size(); i++) {
        outFile << "Best movie with prefix " << bests[i].pr << " is: " << bests[i].mov.get().name << " with rating "
                << bests[i].mov.get().rat << '\n';
    }

    return 0;
}
