#include <array>
#include <iostream>
#include <span>
#include <vector>
#include<cmath>
// -----------------------------------------------------------------------------

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
void KMPSearch(std::vector<int> &pat, std::vector<int> &h, std::vector<int> &txt)
{
    // printf("Found pattern at index %d \n", index_where_found);
    int i = 0;
    int j = 0;
    int m = pat.size();
    // std::vector<int> h (m+1);

    // computeKMPTable(pat, m, h);

    while (i < txt.size())
    {
        // printf("Inside search loop\n, i=%d, j=%d", i, j);
        if (pat[j] == txt[i])
        {
            i++;
            j++;
            if (j==m) //j==|Pat|
            {
                printf("Found pattern at index %d \n", i - j);
                j = h[j];
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
}

std::array<int, 5> match_submissions(std::vector<int> &submission1,
                                     std::vector<int> &submission2)
{
    // TODO: Write your code here
    // ensure 1 is the smaller one
    if(submission1.size()>submission2.size()) 
        return match_submissions(submission2, submission1);

    // // take all substrings/patterns in 1 which have length 10-20 and do kmp on 2
    // for(int i=0; i<submission1.size(); i++){
    //     std::vector<int> kmpTable( submission1.size()-i+1);
    //     std::vector<int> pathelper(submission1.begin()+i, submission1.end());
    //     computeKMPTable(pathelper, submission1.size()-i, kmpTable);
        
    //     //len =10, ...20 and take substr(i, i+len) as pattern
    //     for(int j=i+10; j<i+20 && j<submission1.size() ; j++){ 
    //         std::vector<int> pat(submission1.begin()+i, submission1.begin()+j);
    //         KMPSearch(pat , kmpTable, submission2);
    //     }
    // }
    for (int i = 0; i < submission1.size(); ++i) {
        for (int len = 10; len <= 20 && i + len <= submission1.size(); ++len) {
            std::vector<int> pattern(submission1.begin() + i, submission1.begin() + i + len);

            // Compute KMP table for the current pattern
            std::vector<int> kmpTable(pattern.size());
            computeKMPTable(pattern, kmpTable);

            // Perform KMP search on submission2
            KMPSearch(pattern, kmpTable, submission2);
        }
    }

    std::array<int, 5> result = {0, 0, 0, 0, 0};
    return result; // dummy return
    // End TODO
}
