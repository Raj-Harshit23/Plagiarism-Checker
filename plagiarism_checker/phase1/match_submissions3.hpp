#include <array>
#include <iostream>
#include <span>
#include <vector>
// -----------------------------------------------------------------------------
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
// #include <tuple>
#include <cmath>

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here

// Fills h[] for given pattern pat[0..M-1]
void computeKMPTable(std::vector<int> &pat, int M, std::vector<int> &h)
{
    h[0] = -1;
    int i = 1;
    int j = 0;

    while (i < M)
    {
        // printf("Inside table loop\n, i=%d, j=%d", i, j);
        if (pat[i] != pat[j])
        {
            h[i] = j;
            // precompute for next iteration
            while (j >= 0 && pat[i] != pat[j])
            {
                // printf("Inside table precompute loop\n, i=%d, j=%d", i, j);
                j = h[j];
            }
        }
        else
        {
            h[i] = h[j];
        }
        i++;
        j++;
    }
    h[i] = j;
}

// Prints occurrences of txt[] in pat[]
int KMPSearch(std::vector<int> &pat, std::vector<int> &txt)
{
    // printf("Found pattern at index %d \n", index_where_found);
    int i = 0;
    int j = 0;
    int m = pat.size();
    std::vector<int> h (m+1);
    computeKMPTable(pat, m, h);

    // std::vector<int> patMatch;

    while (i < txt.size())
    {
        // printf("Inside search loop\n, i=%d, j=%d", i, j);
        if (pat[j] == txt[i])
        {
            i++;
            j++;
            if (j==m) //j==|Pat|
            {
                printf("Found pattern at index %d of len %d \n", i - j, j);
                // patMatch.push_back(i-j);
                // j = h[j];
                return j; //modified to return the length of pattern match
            }
        }
        else
        {
            j = h[j];
            if (j < 0)
            {
                i++;
                j++;
            }
        }
    }
    return 0;
}

// // Utility function to calculate a simple hash for a window
// size_t compute_hash(std::vector<int>& vec, int start, int len) {
//     size_t hash_value = 0;
//     for (int i = start; i < start + len; i++) {
//         hash_value = hash_value * 31 + vec[i];  // Hash multiplier (prime number)
//     }
//     return hash_value;
// }

// std::vector<int> longest_approximate_match(std::vector<int>& vec1, std::vector<int>& vec2) {
//     // for 0.8
//     // Testcase 1: 4.32953 / 5.0
//     // Testcase 2: 4.80012 / 5.0
//     // Testcase 3: 3.79155 / 5.0
//     // Total score: 12.9212 / 15.0
//     int max_len = static_cast<int>(0.8 * std::max(vec1.size(), vec2.size()));
//     int min_len = 30;
    
//     int longest_match_len = 0;
//     int start_idx1 = -1;
//     int start_idx2 = -1;

//     // For each possible window length, find matches
//     for (int len = max_len; len >= min_len; len--) {
//         std::unordered_map<size_t, int> hash_map;

//         // Compute hashes for each window in vec1
//         for (int i = 0; i <= vec1.size() - len; i++) {
//             size_t hash_val = compute_hash(vec1, i, len);
//             hash_map[hash_val] = i;
//         }

//         // Compute hashes for each window in vec2 and look for matches in vec1
//         for (int j = 0; j <= vec2.size() - len; j++) {
//             size_t hash_val = compute_hash(vec2, j, len);
//             if (hash_map.find(hash_val) != hash_map.end()) {
//                 // A match is found
//                 int i = hash_map[hash_val];
//                 if (len > longest_match_len) {
//                     longest_match_len = len;
//                     start_idx1 = i;
//                     start_idx2 = j;
//                 }
//             }
//         }
        
//         // Break if a match of this length is found, as we are finding the longest match
//         if (longest_match_len > 0) {
//             break;
//         }
//     }

//     return {longest_match_len, start_idx1, start_idx2};
// }

std::vector<int> longest_approximate_match(std::vector<int>& vec1, std::vector<int>& vec2, int min_match_len,  int max_mismatches) {
    int m = vec1.size();
    int n = vec2.size();

    // DP table to store the length of longest common suffix
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    int best_len = 0;
    int start_idx1 = -1;
    int start_idx2 = -1;

    // Building the dp table
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            // Check for a match or allow up to max_mismatches in the current subsequence
            if (vec1[i - 1] == vec2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else if (dp[i - 1][j - 1] >= min_match_len - max_mismatches) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = 0;
            }

            // Update best match if the current match length meets the minimum requirement
            if (dp[i][j] >= min_match_len && dp[i][j] > best_len) {
                best_len = dp[i][j];
                start_idx1 = i - best_len;
                start_idx2 = j - best_len;
            }
        }
    }

    // Build the DP table
    // for (int i = 1; i <= m; i++) {
    //     for (int j = 1; j <= n; j++) {
    //         if (vec1[i - 1] == vec2[j - 1]) {
    //             dp[i][j] = dp[i - 1][j - 1] + 1;

    //             // Check if this match is the longest and meets the 80% condition
    //             int match_len = dp[i][j];
    //             int required_len = std::max(i, j);  // Required length for 80% match

    //             if (match_len >= 0.8 * required_len && match_len > best_len) {
    //                 best_len = match_len;
    //                 start_idx1 = i - match_len;
    //                 start_idx2 = j - match_len;
    //             }
    //         } else {
    //             dp[i][j] = 0;  // No match, reset
    //         }
    //     }
    // }

    return {best_len, start_idx1, start_idx2};
}



std::array<int, 5> match_submissions(std::vector<int> &submission1,
                                     std::vector<int> &submission2)
{
    // TODO: Write your code here
    // ensure 1 is the smaller one
    if(submission1.size()>submission2.size()) 
        return match_submissions(submission2, submission1);

    std::unordered_set<int> matchedIndices;
    int totalMatch=0;

    for (int i = 0; i < submission1.size(); ++i) {
        // If index `i` is part of an already matched pattern, skip it
        if (matchedIndices.count(i)) {
            continue;
        }
        // Check patterns of length between 20 and 10 (start from larger patterns first)
        for (int len = 30; len >= 10; --len) {
            if (i + len > submission1.size()) continue;  // Skip out-of-bound patterns

            std::vector<int> pattern(submission1.begin() + i, submission1.begin() + i + len);
            int match=KMPSearch(pattern, submission2);
            totalMatch+=match;

            if (match>0) {
                // Mark all indices within this pattern as "matched"
                for (int j = i; j < i + len; ++j) {
                    matchedIndices.insert(j);
                }
                break;  // Skip smaller patterns if a larger one is found
            }
        }
        // for (int len = 10; len <= 20 && i + len <= submission1.size(); ++len) {
        //     std::vector<int> pattern(submission1.begin() + i, submission1.begin() + i + len);

        //     // Perform KMP search on submission2
        //     KMPSearch(pattern, submission2);
        // }
    }
    std::cout<<totalMatch<<std::endl;
    std::cout<<"Subm1.size(): "<<submission1.size()<<" Subm2.size(): "<<submission2.size()<<std::endl;

    // Parameters for approximation (for lcs algorithm)
    int min_match_len = 30;   // Minimum length to consider as a match
    int max_mismatches = 0.01*std::min(submission2.size(), submission1.size());  // Allow up to 1 mismatch in a subsequence

    std::vector<int> approxMatch=longest_approximate_match(submission1, submission2, min_match_len, max_mismatches);
    std::cout<<"Approx match length: "<<approxMatch[0]<<" starting at "<<approxMatch[1]<<" and "<<approxMatch[2]<<std::endl;

    double fracPlag =totalMatch/(double(std::max(submission1.size(), submission2.size())));
    int hasPlag=(fracPlag>0.5);
    std::array<int, 5> result = {hasPlag, totalMatch, approxMatch[0], approxMatch[1], approxMatch[2]};
    return result; // dummy return
    // End TODO
}
