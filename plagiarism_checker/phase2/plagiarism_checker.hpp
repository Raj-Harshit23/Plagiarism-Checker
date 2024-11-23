#include "structures.hpp"
#include<set>
#include<unordered_map>
#include<thread>
#include<mutex>
// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and classes here

class plagiarism_checker_t {
    // You should NOT modify the public interface of this class.
public:
    plagiarism_checker_t(void);
    plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions);
    ~plagiarism_checker_t(void);
    void add_submission(std::shared_ptr<submission_t> __submission);

protected:
    // TODO: Add members and function signatures here
    std::vector<std::shared_ptr<submission_t>> submissions;
    std::vector<std::chrono::system_clock::time_point> timestamps;
    void check_plag(std::shared_ptr<submission_t> __submission);
    std::vector<int> tokenize_file(const std::string& file_name);
    std::vector<std::bitset<400000>> bitsets;
    std::thread t();
    t.detach();
    // End TODO
};