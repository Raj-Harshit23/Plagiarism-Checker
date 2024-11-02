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
    // To avoid overlapping between the matches found in submission2 (the text vector to be matched)
    // Hence we find the unmatched part, which can accomodate the present pattern
    static int findNextUnmatchedPart(int patsize, int txtsize, int start, const std::unordered_set<int> &matchedIndices2)
    {
        bool found = 0;
        int i = start;

        while (!found && i < txtsize)
        {
            // if current pattern were to be matched, the end index of pattern must be unmatched
            if (matchedIndices2.count(i + patsize - 1))        
            {
                i = i + patsize;
                continue;
            }

            // exhaustive check to find if any pattern matching has already been done, in between start and end index
            else if (matchedIndices2.count(i + 10))
            {
                i = i + 11;
                continue;
            }

            // if present index is already matched
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
        return i;   //Returns starting index of unmatched part
    }

    // Uses Kmp like approach to ci=ompute kmp-table
    // Fills h[] for given pattern pat[0..M-1]
    static void computeKMPTable(const std::vector<int> &pat, int M, std::vector<int> &h)
    {
        h[0] = -1;
        int i = 1;
        int j = 0;

        while (i < M)
        {
            if (pat[i] != pat[j])
            {
                h[i] = j;
                // precompute for next iteration
                while (j >= 0 && pat[i] != pat[j])
                {
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
                    // Adding the matched pattern indices in matchedIndices2
                    for (int k = i - j; k < i; k++)
                    {
                        matchedIndices2.insert(k);
                    }
                    return j; // returns the length of the pattern matched
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
//----------------------------------------------

// Using Longest Common Subsequence to find approximate matching
class LCS
{
public:
    // MAIN Function
    static std::vector<int> longest_approximate_match(std::vector<int> &vec1, std::vector<int> &vec2)
    {
        int m = vec1.size();
        int n = vec2.size();
        // Maintain a table for calculating dynamic programming
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (vec1[i - 1] == vec2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1; // Characters match
                } else {
                    dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]); // Choose the maximum
                }
            }
        }

        // Backtrack to find the LCS elements and their indices
        // std::vector<std::tuple<int, int, int>> lcsWithIndices;
        std::vector<int> ind1, ind2;
        int i = m, j = n;
        while (i > 0 && j > 0) {
            if (vec1[i - 1] == vec2[j - 1]) {
                // lcsWithIndices.emplace_back(vec1[i - 1], i - 1, j - 1); // Append value and indices to LCS
                ind1.push_back(i-1);
                ind2.push_back(j-1);
                i--;
                j--;
            } else if (dp[i - 1][j] > dp[i][j - 1]) {
                i--; // Move up
            } else {
                j--; // Move left
            }
        }

        // Since we constructed lcs backwards, reverse it
        std::reverse(ind1.begin(), ind1.end());
        std::reverse(ind2.begin(), ind2.end());

        // For 80% check
        int ni=ind1.size();
        int ans1=0;
        int ans2=0;
        int ans3=0;

        // 
        for(int k=0; k<ni; k++){
            for(int h=ni-1; h>k; h--){
                int minStringLen=std::min((ind1[h]-ind1[k]),(ind2[h]-ind2[k]));
                if(minStringLen<30)
                {
                    break;
                }
                int maxStringLen=std::max((ind1[h]-ind1[k]),(ind2[h]-ind2[k]));
                double fracMatch=double(h-k+1)/(maxStringLen+1);
                if(fracMatch>0.95)
                {
                    int ans=std::min(std::min((int)((maxStringLen+1)*(fracMatch/0.8)),m-ind1[k]),n-ind2[k]);     //the max matching length can be scaled such that accuracy remains greater than 80%
                    // std::cout<<fracMatch<<" "<<ans<<" "<<maxStringLen<<std::endl;
                    if(ans>ans1)
                    {
                        ans1=ans;
                        ans2=ind1[k];
                        ans3=ind2[k];
                    }
                }
            }
        }
        return {ans1,ans2,ans3};

        
        // for(auto k: lcsWithIndices){
        //     std::cout<<std::get<0>(k)<<" "<<std::get<1>(k)<<" "<<std::get<2>(k)<<"     ";
        // }
        // auto [a,b,c]=lcsWithIndices[0];
        // return {a,b,c};

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
    std::vector<int> approxMatch = LCS::longest_approximate_match(submission1, submission2);
    // std::vector<int> approxMatch=LSH::longest_approximate_match(submission1, submission2);

    // Report plag if more than half of the tokens are same
    double fracPlag = totalMatch / (double(std::max(submission1.size(), submission2.size())));
    int hasPlag = (fracPlag > 0.5);
    std::array<int, 5> result = {hasPlag, totalMatch, approxMatch[0], approxMatch[1], approxMatch[2]};
    return result;
}
