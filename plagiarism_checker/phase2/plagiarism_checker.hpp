// #include "structures.hpp"
// // -----------------------------------------------------------------------------
// #include<set>
// #include<unordered_map>
// #include<thread>
// #include<mutex>
// #include<chrono>
// #include<queue>
// #include<bitset>
// // You are free to add any STL includes above this comment, below the --line--.
// // DO NOT add "using namespace std;" or include any other files/libraries.
// // Also DO NOT add the include "bits/stdc++.h"

// // OPTIONAL: Add your helper functions and classes here

// class plagiarism_checker_t {
//     // You should NOT modify the public long longerface of this class.
// public:
//     plagiarism_checker_t(void);
//     plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions){
//     };
//     ~plagiarism_checker_t(void);
//     void add_submission(std::shared_ptr<submission_t> __submission);

// protected:
//     // TODO: Add members and function signatures here
//     std::vector<std::shared_ptr<submission_t>> submissions;
//     std::vector<double> timestamps;
//     // Should take difference for timestamps?store initial time for constructor
//     std::queue<std::pair<std::shared_ptr<submission_t>, double>> submission_queue; // Queue for submissions
//     std::mutex m;  // Mutex to protect access to the queue
//     std::thread worker_thread; // Single worker thread
//     bool thread_running; // Flag to check if thread is running
//     void check_plag(std::shared_ptr<submission_t> __submission, double start_time);
//     void len15check(std::vector<long long>& submission, double start_time);
//     void len75check(std::vector<long long>& submission, double start_time);
//     void patchCheck(std::vector<long long>& submission, double start_time);

//     // to check if the file is already plagged
//     std::vector<bool> plagged;

//     // Queue of submissions in present 1 second window
//     // updated whenever a new submission is received
//     std::queue<std::pair<std::shared_ptr<submission_t>, double>> present_window;
//     std::thread t;
//     std::vector<long long> tokenize_file(const std::string& file_name);
//     std::vector<std::bitset<400000>> bitsets;
    
//     // End TODO
// };
#include "structures.hpp"
// -----------------------------------------------------------------------------
#include<set>
#include<unordered_map>
#include<unordered_set>
#include<thread>
#include<mutex>
#include<chrono>
#include<queue>
#include<bitset>
// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and classes here

class plagiarism_checker_t {
    // You should NOT modify the public long longerface of this class.
public:
    plagiarism_checker_t(void);
    plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions);
    ~plagiarism_checker_t(void);
    void add_submission(std::shared_ptr<submission_t> __submission);

protected:
    // TODO: Add members and function signatures here
    std::vector<std::shared_ptr<submission_t>> submissions;
    std::vector<double> timestamps;
    // Should take difference for timestamps?store initial time for constructor
    std::queue<std::pair<std::shared_ptr<submission_t>, double>> submission_queue; // Queue for submissions
    std::mutex m;  // Mutex to protect access to the queue
    std::thread worker_thread; // Single worker thread
    bool thread_running; // Flag to check if thread is running
    void check_plag(std::shared_ptr<submission_t> __submission, double start_time);
    void len15check(std::vector<int>& submission, double start_time);
    void len75check(std::vector<int>& submission, double start_time);
    void patchCheck(std::vector<int>& submission, double start_time);

    // to check if the file is already plagged
    std::vector<bool> plagged;
    std::vector<long long> tokenize_file(const std::string& file_name);
    std::vector<std::bitset<400000>> bitsets;
    std::vector<std::bitset<400000>> bitsets75;
    std::vector<std::unordered_set<long long>> totalMatches;
    // End TODO
};
