#include <array>
#include <iostream>
#include <span>
#include <vector>
// -----------------------------------------------------------------------------
#include <unordered_set>
#include <algorithm>
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
        for (int len = 20; len >= 10; --len) {
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

    double fracPlag =totalMatch/(double(std::max(submission1.size(), submission2.size())));
    int hasPlag=(fracPlag>0.5);
    std::array<int, 5> result = {hasPlag, totalMatch, 0, 0, 0};
    return result; // dummy return
    // End TODO
}
