#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <cmath>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <functional>
#include <limits>
// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here
const int MIN_PATTERN_LENGTH = 10;  // Minimum length for matches
const int LONG_PATTERN_THRESHOLD = 30; // Maximum length to search for long matches

// Structure to hold match information
struct Match {
    int start1; // Start index in submission 1
    int start2; // Start index in submission 2
    int length; // Length of the match
};

// Function to find non-overlapping matches
std::vector<Match> find_matches(const std::vector<int>& submission1, const std::vector<int>& submission2) {
    int n = submission1.size();
    int m = submission2.size();

    // Create a DP table to store lengths of longest matches
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    // Fill the DP table
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (submission1[i - 1] == submission2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
        }
    }

    // Vector to store matches found
    std::vector<Match> matches;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            // If there is a match and it's long enough
            if (dp[i][j] >= MIN_PATTERN_LENGTH) {
                matches.push_back({i - dp[i][j], j - dp[i][j], dp[i][j]});
            }
        }
    }

    return matches;
}

// Function to filter non-overlapping matches
std::vector<Match> filter_non_overlapping_matches(std::vector<Match>& matches) {
    std::vector<Match> filtered;
    int last_end1 = -1;  // Last end index in submission1

    // Sort matches by start index in submission1
    std::sort(matches.begin(), matches.end(), [](const Match& a, const Match& b) {
        return a.start1 < b.start1;
    });

    // Iterate over matches and keep non-overlapping ones
    for (const auto& match : matches) {
        if (match.start1 >= last_end1) {
            filtered.push_back(match);
            last_end1 = match.start1 + match.length;  // Update last end
        }
    }

    return filtered;
}

// Main function to match submissions
std::array<int, 5> match_submissions(std::vector<int>& submission1, std::vector<int>& submission2) {
    std::array<int, 5> result = {0, 0, 0, -1, -1}; // Initialize results
    auto matches = find_matches(submission1, submission2);
    
    // Filter non-overlapping matches
    matches = filter_non_overlapping_matches(matches);

    // Calculate results based on found matches
    for (const auto& match : matches) {
        result[1] += match.length; // Total length of matched patterns
        result[0] = 1; // Flag as plagiarized if any matches found
        if (match.length > result[2]) {
            result[2] = match.length; // Update longest match length
            result[3] = match.start1; // Start index in submission 1 for longest match
            result[4] = match.start2; // Start index in submission 2 for longest match
        }
    }

    // Debug information
    std::cout << "Debug: Total matches found: " << matches.size() 
              << ", Total matched length: " << result[1] 
              << ", Longest match length: " << result[2] 
              << "start: " << result[3]
              << "end: " << result[4] << std::endl;

    return result;
}


// std::array<int, 5> match_submissions(std::vector<int> &submission1, std::vector<int> &submission2) {
    
    
//     // TODO: Write your code here
//     std::array<int, 5> result = {0, 0, 0, 0, 0};
//     return result; // dummy return
//     // End TODO
// }