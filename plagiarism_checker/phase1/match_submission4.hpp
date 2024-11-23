#include <array>
#include <iostream>
#include <span>
#include <vector>
// -----------------------------------------------------------------------------
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <cmath>

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here

//-------------------------------------------------
//------------------------------------------------

// Uses Knuth-Morris-Pratt algorithm to find exact matching
class KMP
{
private:
    // HELPER Functions
    // To avoid overlapping between the matches found in submission2 (text to be matched)
    static int findNextUnmatchedPart(int patsize, int txtsize, int start, const std::unordered_set<int> &matchedIndices2)
    {
        bool found = 0;
        int i = start;
        while (!found && i < txtsize)
        {
            if (matchedIndices2.count(i + patsize - 1))
            {
                i = i + patsize;
                continue;
            }
            else if (matchedIndices2.count(i + 10))
            {
                i = i + 11;
                continue;
            }
            else if (matchedIndices2.count(i))
            {
                i++;
                continue;
            }
            found = 1;
        }
        if (!found)
        {
            return -1;
        }
        return i;
    }

    // Fills h[] for given pattern pat[0..M-1]
    static void computeKMPTable(const std::vector<int> &pat, int M, std::vector<int> &h)
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

public:
    // MAIN Function
    static int KMPSearch(const std::vector<int> &pat, const std::vector<int> &txt, std::unordered_set<int> &matchedIndices2)
    {
        // printf("Found pattern at index %d \n", index_where_found);
        int j = 0;
        int m = pat.size();
        int n = txt.size();
        int i = findNextUnmatchedPart(m, n, 0, matchedIndices2);
        if (i == -1) // Means No remaining place found, where matching can be done.
        {
            return 0;
        }
        std::vector<int> h(m + 1);
        computeKMPTable(pat, m, h);
        while (i < n)
        {
            if (matchedIndices2.count(i)) //
            {
                i = findNextUnmatchedPart(m, n, i, matchedIndices2);
                j = 0;
                continue;
            }
            // printf("Inside search loop\n, i=%d, j=%d", i, j);
            if (pat[j] == txt[i])
            {
                i++;
                j++;
                if (j == m) // j==|Pat|
                {
                    // printf("Found pattern at index %d of len %d \n", i - j, j);

                    // Adding the matched pattern indices in matchedIndices2
                    for (int k = i - j; k < i; k++)
                    {
                        matchedIndices2.insert(k);
                    }
                    return j; // modified to return the length of pattern match
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
};
//---------------------------------------

// Uses Locality-Sensitive Hashing to find approximate matching
class LSH
{
public:
    // Utility function to calculate a simple hash for a window
    static size_t compute_hash(std::vector<int> &vec, int start, int len)
    {
        size_t hash_value = 0;
        for (int i = start; i < start + len; i++)
        {
            hash_value = hash_value * 997 + vec[i]; // Hash multiplier (prime number)
        }
        return hash_value;
    }

    // MAIN Function
    static std::vector<int> longest_approximate_match(std::vector<int> &vec1, std::vector<int> &vec2)
    {
        // good performance
        int max_len = static_cast<int>(1 * std::min(vec1.size(), vec2.size()));
        int min_len = 30;

        int longest_match_len = 0;
        int start_idx1 = -1;
        int start_idx2 = -1;

        // For each possible window length, find matches
        for (int len = max_len; len >= min_len; len--)
        {
            std::unordered_map<size_t, int> hash_map;

            // Compute hashes for each window in vec1
            for (int i = 0; i <= vec1.size() - len; i++)
            {
                size_t hash_val = compute_hash(vec1, i, len);
                hash_map[hash_val] = i;
            }

            // Compute hashes for each window in vec2 and look for matches in vec1
            for (int j = 0; j <= vec2.size() - len; j++)
            {
                size_t hash_val = compute_hash(vec2, j, len);
                if (hash_map.find(hash_val) != hash_map.end())
                {
                    // A match is found
                    int i = hash_map[hash_val];
                    if (len > longest_match_len)
                    {
                        longest_match_len = len;
                        start_idx1 = i;
                        start_idx2 = j;
                    }
                }
            }

            // Break if a match of this length is found, as we are finding the longest match
            if (longest_match_len > 0)
            {
                break;
            }
        }

        return {longest_match_len, start_idx1, start_idx2};
    }
};
//----------------------------------------------

// Using Longest Common Subsequence (with some mismatches) to find approximate matching
class LCS
{
public:
    // MAIN Function
    static std::vector<int> longest_approximate_match(std::vector<int> &vec1, std::vector<int> &vec2, int min_match_len, int max_mismatches)
    {
        int m = vec1.size();
        int n = vec2.size();

        // DP table to store the length of longest common suffix
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        int best_len = 0;
        int start_idx1 = -1;
        int start_idx2 = -1;
        int mismatches = 0;

        // Building the dp table
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                // Check for a match or allow up to max_mismatches in the current subsequence
                if (vec1[i - 1] == vec2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                }
                else if (dp[i - 1][j - 1] >= min_match_len && mismatches < max_mismatches)
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                    mismatches++;
                }
                else
                {
                    dp[i][j] = 0;
                }

                // Update best match if the current match length meets the minimum requirement
                if (dp[i][j] >= min_match_len && dp[i][j] > best_len)
                {
                    best_len = dp[i][j];
                    start_idx1 = i - best_len;
                    start_idx2 = j - best_len;
                }
            }
        }

        return {best_len, start_idx1, start_idx2};
    }
};
//---------------------------------------
//---------------------------------------

// MAIN Function of this file
// Finds matches between two tokenized files
std::array<int, 5> match_submissions(std::vector<int> &submission1,
                                     std::vector<int> &submission2)
{
    // ensure 1 is the smaller one
    if (submission1.size() > submission2.size())
        return match_submissions(submission2, submission1);

    // matchedIndices required to avoid non overlapping
    std::unordered_set<int> matchedIndices1;
    std::unordered_set<int> matchedIndices2;

    // Initialize total match for exact matching
    int totalMatch = 0;

    for (int i = 0; i < (int)submission1.size(); ++i)
    {
        // If index `i` is part of an already matched pattern, skip it
        if (matchedIndices1.count(i))
        {
            continue;
        }

        // Check patterns of length between 20 and 10 (start from larger patterns first)
        for (int len = 20; len >= 10; --len)
        {
            if (i + len > submission1.size())
                continue; // Skip out-of-bound patterns

            // Search for the pattern
            std::vector<int> pattern(submission1.begin() + i, submission1.begin() + i + len);
            int match = KMP::KMPSearch(pattern, submission2, matchedIndices2);
            totalMatch += match;

            if (match > 0)
            {
                // Mark all indices within this pattern as "matched"
                for (int j = i; j < i + len; ++j)
                {
                    matchedIndices1.insert(j);
                }
                break; // Skip smaller patterns if a larger one is found
            }
        }
    }

    // Parameters for
    int min_match_len = 30;                                                                         // Minimum length to consider as a match
    int max_mismatches = std::min(50, int(0.1 * std::min(submission2.size(), submission1.size()))); // Allow some mismatch in a subsequence

    // Approximate matching function
    std::vector<int> approxMatch = LCS::longest_approximate_match(submission1, submission2, min_match_len, max_mismatches);
    // std::vector<int> approxMatch=LSH::longest_approximate_match(submission1, submission2);

    // Report plag if more than half of the tokens are same
    double fracPlag = totalMatch / (double(std::max(submission1.size(), submission2.size())));
    int hasPlag = (fracPlag > 0.5);
    std::array<int, 5> result = {hasPlag, totalMatch, approxMatch[0], approxMatch[1], approxMatch[2]};
    return result;
}
