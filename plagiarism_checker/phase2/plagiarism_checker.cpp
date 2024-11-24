#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
const long long BASE=37;
const long long MOD=1e7+7;



ThreadPool::ThreadPool() {}

// destructor to stop the thread pool and clean up resources
ThreadPool::~ThreadPool() {
    Stop();
}

// starts the threads and initiates the thread loop
void ThreadPool::Start() {
    const uint32_t num_threads = 1;
    for (uint32_t ii = 0; ii < num_threads; ++ii) {
        threads.emplace_back(&ThreadPool::ThreadLoop, this);
    }
}

// adds a job to the job queue and increments the job counter
void ThreadPool::QueueJob(const std::function<void()>& job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
        remaining_jobs.fetch_add(1, std::memory_order_relaxed);
    }
    mutex_condition.notify_one();
}

// checks if the thread pool is busy processing jobs
bool ThreadPool::busy() {
    std::unique_lock<std::mutex> lock(queue_mutex);
    return !jobs.empty() || remaining_jobs.load(std::memory_order_relaxed) > 0;
}

// stops the thread pool, waits for all jobs to finish, and joins threads
void ThreadPool::Stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();

    {
        std::unique_lock<std::mutex> counter_lock(job_counter_mutex);
        all_jobs_done_condition.wait(counter_lock, [this] { return remaining_jobs.load() == 0; });
    }

    for (std::thread& active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
}

// the main loop for each thread to process jobs from the queue
void ThreadPool::ThreadLoop() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] {
                return !jobs.empty() || should_terminate;
            });

            if (should_terminate && jobs.empty()) {
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        job();
        remaining_jobs.fetch_sub(1, std::memory_order_relaxed);
        {
            std::unique_lock<std::mutex> counter_lock(job_counter_mutex);
            if (remaining_jobs.load() == 0) {
                all_jobs_done_condition.notify_all();
            }
        }
    }
}

//https://stackoverflow.com/questions/15752659/thread-pooling-in-c11 
//BASED ON THIS DISCUSSION ON STACKOVERFLOW. CHANGED TO MAKE QUEUE EMPTY FOR DESTRUCTOR TO INITIATE DESTRUCTION. ALSO ONLY 1 THREAD USED FOR THE POOL.






























std::vector<long long> RollHash(std::vector<int> &tokens, long long k){
    std::vector<long long> hashes;
    long long base_k=1;
    for(long long i=0; i<k; i++){
        base_k=(base_k*BASE)%MOD;
    }

    long long hash=0;

    //compute hash for first entry
    for(long long i=k-1;i<tokens.size() && i>=0; i--){
        hash = (hash*BASE+tokens[i])% MOD;
    }
    hashes.push_back(hash);
    //compute for rest of the entries by updating hash
    for(long long i=k; i<tokens.size(); i++){
        hash=(hash-tokens[i-k])/BASE;
        hash=(hash+tokens[i]*(base_k/BASE))%MOD;
        hashes.push_back(hash);
    }
    return hashes;
}


class BloomFilter{
public:
    std::bitset<400000> bitArray;
    BloomFilter(){
    }
    // Simple hash functions for demonstration
    long long hash1(long long value) { return (value * 17 + 5) % 400000; }
    long long hash2(long long value) { return (value * 23 + 11) % 400000; }
    long long hash3(long long value) { return (value * 29 + 7) % 400000; }
    long long hash4(long long value) { return (value * 31 + 13) % 400000; }
    long long hash5(long long value) { return (value * 37 + 17) % 400000; }
    long long hash6(long long value) { return (value * 41 + 19) % 400000; }
    long long hash7(long long value) { return (value * 53 + 23) % 400000; }
    long long hash8(long long value) { return (value * 59 + 29) % 400000; }
    long long hash9(long long value) { return (value * 61 + 31) % 400000; }
    long long hash10(long long value) { return (value * 67 + 37) % 400000; }
    long long hash11(long long value) { return (value * 71 + 41) % 400000; }
    long long hash12(long long value) { return (value * 73 + 43) % 400000; }
    long long hash13(long long value) { return (value * 79 + 47) % 400000; }

    void add(long long value) {
        bitArray[hash1(value)] = 1;
        bitArray[hash2(value)] = 1;
        bitArray[hash3(value)] = 1;
        bitArray[hash4(value)] = 1;
        bitArray[hash5(value)] = 1;
        bitArray[hash6(value)] = 1;
        bitArray[hash7(value)] = 1;
        bitArray[hash8(value)] = 1;
        bitArray[hash9(value)] = 1;
        bitArray[hash10(value)] = 1;
        bitArray[hash11(value)] = 1;
        bitArray[hash12(value)] = 1;
        bitArray[hash13(value)] = 1;
        
    }
    std::bitset<400000> give(){
        return bitArray;
    }

    bool contains(std::bitset<400000>& givenArray,long long value) {
        return givenArray[hash1(value)] && givenArray[hash2(value)] && givenArray[hash3(value)] && givenArray[hash4(value)] && givenArray[hash5(value)] && givenArray[hash6(value)] && givenArray[hash7(value)] && givenArray[hash8(value)] && givenArray[hash9(value)] && givenArray[hash10(value)] && givenArray[hash11(value)] && givenArray[hash12(value)] && givenArray[hash13(value)] ;
    }
};





void plagiarism_checker_t::len15check(std::vector<int>& submission, double start_time)
{
    // std::vector<long long> rolled = RollHash(submission,15);
    std::vector<long long> rolled1 = RollHash(submission,10);
    std::vector<int> rolled1int;
    for (auto i : rolled1) {
        i = static_cast<int>(i);
        rolled1int.push_back(i);
    }
    std::vector<long long> rolled = RollHash(rolled1int,5);
    long long curr=bitsets.size();
    // Check how many non-overlapping hashes present in previous submissions, in reverse
    std::unordered_set<long long> totalMatch;
    for (long long j=curr-1;j>=0;j--) {
        long long count=0;
        BloomFilter checker;
        for(long long i=0;i<(long long)rolled.size();i++)
        {
            if(checker.contains(bitsets[j],rolled[i])){
                count++;
                i+=15;  // NON OVERLAPPING INCREMENT CONDITION
            }
            // CHECK COUNT>=10 HERE ONLY
            // Need to adjust the THRESHOLD according to false positive rate
            if(count>=10)
            {
            
                // Plag present
                if(curr>numOrigs && !plagged[curr])
                {

                    submissions[curr]->student->flag_student(submissions[curr]);
                    submissions[curr]->professor->flag_professor(submissions[curr]);
                    plagged[curr]=1;
                }

                // Plag close submissions
                if(curr>numOrigs && std::abs(timestamps[curr]-timestamps[j])<1.0 && !plagged[curr])
                {
                    submissions[curr]->student->flag_student(submissions[curr]);
                    submissions[curr]->professor->flag_professor(submissions[curr]);
                    plagged[curr]=1;
                }
                break;
            }
        }
        
        //PATCHWORK STARTS NOW
        for(long long i=0;i<(long long)rolled.size();i++)
        {
            if(checker.contains(bitsets[j],rolled[i])){
                totalMatch.insert(rolled[i]);
                totalMatches[j].insert(rolled[i]);
                if(j>numOrigs && totalMatches[j].size()>=20 && std::abs(timestamps[curr]-timestamps[j])<1.0 && !plagged[j]){
                    submissions[j]->student->flag_student(submissions[j]);
                    submissions[j]->professor->flag_professor(submissions[j]);
                    plagged[j]=1;
                }
            }
            if(totalMatch.size()>=20){
                if(curr>numOrigs && !plagged[curr])
                {
                    submissions[curr]->student->flag_student(submissions[curr]);
                    submissions[curr]->professor->flag_professor(submissions[curr]);
                    plagged[curr]=1;
                }
                if(std::abs(timestamps[curr]-timestamps[j])>1.0){
                    break;
                }
            }
            
        }

        if(plagged[curr])
        {
            while(j>0 && plagged[j])
            {
                j--;
                if(std::abs(timestamps[curr]-timestamps[j])>1.0) break;
            }
        }

    
    }

    // Present submission bloom filter
    BloomFilter b;
    for(auto i: rolled){
        b.add(i);
    }
    std::bitset<400000> bitArray = b.give();
    bitsets.push_back(bitArray);
    totalMatches.push_back(totalMatch);
}





















void plagiarism_checker_t::len75check(std::vector<int>& submission, double start_time)
{
    // were going to use 2-layer rolled hashes since we are tagging plagged on a single index so we need to make sure of it
    // the logic is if a hash collisioon caused a value to be set, it shouldnt be counted... so in doing 50+25 we make sure hash collisions have to occur 25 times over!! 
    std::vector<long long> rolled1 = RollHash(submission,50);
    std::vector<int> rolled1int;
    for (auto i : rolled1) {
        i = static_cast<int>(i);
        rolled1int.push_back(i);
    }
    std::vector<long long> rolled = RollHash(rolled1int,25);
    //final rolled hash


    // corner case check
    if (rolled.empty()) return;

    long long curr=bitsets75.size();

    BloomFilter checker;
    //object for contains function call

    // check how many non-overlapping hashes present in previous submissions, in reverse
    for (long long j=curr-1;j>=0;j--) {
        for(long long i=0;i<(long long)rolled.size();i++)
        {
            if(checker.contains(bitsets75[j],rolled[i])){
                // plag present
                if(curr>numOrigs && !plagged[curr])
                {
                    submissions[curr]->student->flag_student(submissions[curr]);
                    submissions[curr]->professor->flag_professor(submissions[curr]);
                    plagged[curr]=1;

                }

                // plag close submissions
                if(j>numOrigs && std::abs(timestamps[curr]-timestamps[j])<1.0 && !plagged[j])
                {
                    submissions[j]->student->flag_student(submissions[j]);
                    submissions[j]->professor->flag_professor(submissions[j]);
                    plagged[j]=1;
                }
                break;
            }
        }

        // continue to check for close submissions
        if(plagged[curr])
        {
            while(j>0 && plagged[j])
            {
                j--;
                if(std::abs(timestamps[curr]-timestamps[j])>1.0) break;
            }
        }
    }
    
    //object for adding new file as bitset to bitsets75 vector
    BloomFilter b;
    for(auto i: rolled){
        b.add(i);
    }
    std::bitset<400000> bitArray = b.give();
    bitsets75.push_back(bitArray);
}

void plagiarism_checker_t::check_plag(std::shared_ptr<submission_t> submission,double start_time){
    submissions.push_back(submission);
    timestamps.push_back(start_time);
    plagged.push_back(0);
    // add file such that start time is set and originallly assumed not plagged(innocent until proven guilty :))
    tokenizer_t file(submission->codefile);
    std::vector<int> submissionTokens = file.get_tokens();
    // check for large(75 tokens)
    len75check(submissionTokens,start_time);
    // check for small(15 tokens) 10 matches between 2 files OR 20 matches between a file and all other previous(+1 second) files
    len15check(submissionTokens,start_time);
}

//void constructor made
plagiarism_checker_t::plagiarism_checker_t(void) {}

//IMPORTANT CONSTRUCTOR
plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions) {
    // taking numOrigs to precompute for original files
    numOrigs = __submissions.size();
    for(int i=0;i<numOrigs;i++){
        totalMatches.push_back(std::unordered_set<long long>());
        plagged.push_back(0);
        timestamps.push_back(0);
        //setting empty sets, bools, int wherever required

        //now rolled hash to bitset using RollHash and BloomFilter to get bitsets of orig filse
        tokenizer_t origTokenizer(__submissions[i]->codefile);
        std::vector<int> submissionTokens = origTokenizer.get_tokens();


        // to make RollHash for 15 by using 10+5 (double hash for lower prob of collisison)
        std::vector<long long> rolled1 = RollHash(submissionTokens,10);
        std::vector<int> rolled1int;
        for (auto jkl : rolled1) {
            jkl = static_cast<int>(jkl);
            rolled1int.push_back(jkl);
        }
        std::vector<long long> rolled = RollHash(rolled1int,5);

        // to make RollHash for 75 by using 50+25 (double hash for lower prob of collisison)
        std::vector<long long> rolled70 = RollHash(submissionTokens,50);
        std::vector<int> rolled70int;
        for (auto jk : rolled70) {
            jk = static_cast<int>(jk);
            rolled70int.push_back(jk);
        }
        std::vector<long long> rolled75 = RollHash(rolled70int,25);

        //for adding original bitsets(storing rolled hashes of 15 tokens) and bitsets75(storing rolled hashes of 75 tokens)
        BloomFilter orig;
        for(auto j: rolled){
            orig.add(j);
        }
        std::bitset<400000> origBitarr = orig.give();
        bitsets.push_back(origBitarr);
        //adding 15 token bitsets


        BloomFilter orig75;
        for(auto j: rolled75){
            orig75.add(j);
        }
        std::bitset<400000> origBitarr75 = orig75.give();
        bitsets75.push_back(origBitarr75);
        //adding 75 token bitsets



        submissions.push_back(__submissions[i]);
        //finally also adding submsission pointer into submissions vector
        
    }
    //starting threadpool object(initialized running in infintie loop until job in queue)
    worker.Start();
}

plagiarism_checker_t::~plagiarism_checker_t() {
    while (worker.busy()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // sleep to stop weight on ram
    }
    // all jobs finished stop the pool
    worker.Stop();
} 

void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto start_time_since_epoch = start_time.time_since_epoch();
    double start_time_in_seconds = std::chrono::duration<double>(start_time_since_epoch).count();
    //time stored for allowing functionality of marking older submissions ass plag if plagged with a submission <1 seconds
    {
        // adding job to queue(very quick)
        worker.QueueJob([this, __submission, start_time_in_seconds]() {
            check_plag(__submission, start_time_in_seconds);
        });
    }
    //exits in nearly no time
}
