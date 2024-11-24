#include "structures.hpp"
// -----------------------------------------------------------------------------
#include<set>
#include <functional>  // Add this header to resolve the error with std::function
#include<unordered_map>
#include<unordered_set>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<chrono>
#include<queue>
#include<bitset>
// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and classes here

class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();
    void Start();
    void QueueJob(const std::function<void()>& job);
    bool busy();
    void Stop();

private:
    void ThreadLoop();

    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
    std::mutex queue_mutex;
    std::condition_variable mutex_condition;

    std::atomic<size_t> remaining_jobs{0};  // Use atomic size_t for thread-safe job count
    std::mutex job_counter_mutex;  // Protects condition variable for remaining jobs
    std::condition_variable all_jobs_done_condition;

    bool should_terminate = false;
};
class plagiarism_checker_t {
    // You should NOT modify the public long longerface of this class.
public:
    plagiarism_checker_t(void);
    plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions);
    ~plagiarism_checker_t(void);
    void add_submission(std::shared_ptr<submission_t> __submission);

protected:
    ThreadPool worker;
    // TODO: Add members and function signatures here
    std::vector<std::shared_ptr<submission_t>> submissions;
    int numOrigs;
    std::vector<double> timestamps;
    // Should take difference for timestamps?store initial time for constructor
    std::queue<std::pair<std::shared_ptr<submission_t>, double>> submission_queue; // Queue for submissions
    std::mutex m;  // Mutex to protect access to the queue
    std::condition_variable cv;  // Condition variable to notify worker thread
    std::thread worker_thread; // Worker thread
    std::atomic<bool> thread_running; // Flag to check if thread is running
    bool checkq=1;
    // bool destructed=1;
    // bool thread_running;
    void check_plag(std::shared_ptr<submission_t> __submission, double start_time);
    void len15check(std::vector<int>& submission, double start_time);
    void len75check(std::vector<int>& submission, double start_time);
    void patchCheck(std::vector<int>& submission, double start_time);
    // void worker();  // Worker thread function

    // to check if the file is already plagged
    std::vector<bool> plagged;
    std::vector<long long> tokenize_file(const std::string& file_name);
    std::vector<std::bitset<400000>> bitsets;
    std::vector<std::bitset<400000>> bitsets75;
    std::vector<std::unordered_set<long long>> totalMatches;
    // End TODO
};
