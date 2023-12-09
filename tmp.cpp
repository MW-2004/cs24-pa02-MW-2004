#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::pair<int, int> findIndicesWithPrefix(const std::vector<std::string>& strings, const std::string& prefix) {
    // Use std::equal_range to find the range of elements matching the prefix
    auto range = std::equal_range(strings.begin(), strings.end(), prefix, 
        [](const std::string& s, const std::string& p) {
            return s.compare(0, p.length(), p) < 0;
        }
    );

    // Check if the range is empty (no matching elements)
    if (range.first == range.second) {
        return {-1, -1};  // No matching elements
    }

    // Calculate begin and end indices based on the range
    int beginIndex = std::distance(strings.begin(), range.first);
    int endIndex = std::distance(strings.begin(), range.second) - 1;

    return {beginIndex, endIndex};
}

int main() {
    // Example usage
    std::vector<std::string> vectorOfStrings = {"apple", "apricot", "apricot", "banana", "orange", "grape"};
    std::string givenPrefix = "ap";

    auto [beginIndex, endIndex] = findIndicesWithPrefix(vectorOfStrings, givenPrefix);

    // Output the results
    std::cout << "Begin index: " << beginIndex << std::endl;
    std::cout << "End index: " << endIndex << std::endl;

    return 0;
}
