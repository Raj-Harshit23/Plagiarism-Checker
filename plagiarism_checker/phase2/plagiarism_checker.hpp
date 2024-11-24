#include "structures.hpp"
// -----------------------------------------------------------------------------
#include <bitset>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <set>
#include <thread>
#include <unordered_map>
#include <unordered_set>

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and classes here
std::vector<long long> RollHash(std::vector<int> &tokens, long long k);

class BloomFilter{
public:
    std::bitset<400000> bitArray;
    BloomFilter(){}
    // Simple hash functions
    long long hash1(long long value) ;
    long long hash2(long long value) ;
    long long hash3(long long value) ;
    long long hash4(long long value) ;
    long long hash5(long long value) ;
    long long hash6(long long value) ;
    long long hash7(long long value) ;
    long long hash8(long long value) ;
    long long hash9(long long value) ;
    long long hash10(long long value) ;
    long long hash11(long long value) ;
    long long hash12(long long value) ;
    long long hash13(long long value) ;
    void add(long long value);
    std::bitset<400000> give();
    bool contains(std::bitset<400000>& givenArray,long long value);
};

class ThreadPool {
public:
  ThreadPool();
  ~ThreadPool();
  void Start();
  void QueueJob(const std::function<void()> &job);
  bool busy();
  void Stop();

private:
  void ThreadLoop();

  std::vector<std::thread> threads;
  std::queue<std::function<void()>> jobs;
  std::mutex queue_mutex;
  std::condition_variable mutex_condition;

  std::atomic<size_t> remaining_jobs{
      0}; // Use atomic size_t for thread-safe job count
  std::mutex
      job_counter_mutex; // Protects condition variable for remaining jobs
  std::condition_variable all_jobs_done_condition;

  bool should_terminate = false;
};

// ---------------------------------------------------------------------------
class plagiarism_checker_t {
  // You should NOT modify the public long longerface of this class.
public:
  plagiarism_checker_t(void);
  plagiarism_checker_t(
      std::vector<std::shared_ptr<submission_t>> __submissions);
  ~plagiarism_checker_t(void);
  void add_submission(std::shared_ptr<submission_t> __submission);

protected:
  // TODO: Add members and function signatures here
  ThreadPool worker;
  std::vector<std::shared_ptr<submission_t>> submissions;
  int numOrigs; // Number of original submissions
  std::vector<double> timestamps;
  std::queue<std::pair<std::shared_ptr<submission_t>, double>>
      submission_queue;       // Queue for submissions
  void check_plag(std::shared_ptr<submission_t> __submission,
                  double start_time);
  void len15check(std::vector<int> &submission, double start_time);
  void len75check(std::vector<int> &submission, double start_time);
  void patchCheck(std::vector<int> &submission, double start_time);

  // to check if the file is already plagged
  std::vector<bool> plagged;
  std::vector<std::bitset<400000>> bitsets;
  std::vector<std::bitset<400000>> bitsets75;
  std::vector<std::unordered_set<long long>> totalMatches;
  // End TODO
};
