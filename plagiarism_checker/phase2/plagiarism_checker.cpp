#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
const long long BASE=37;
const long long MOD=1e7+7;



ThreadPool::ThreadPool() {
    // Initialization if needed (constructor logic)
}

ThreadPool::~ThreadPool() {
    Stop();  // Ensure everything is cleaned up and threads are joined
}

void ThreadPool::Start() {
    const uint32_t num_threads = 1;  // Or dynamically adjust
    for (uint32_t ii = 0; ii < num_threads; ++ii) {
        threads.emplace_back(&ThreadPool::ThreadLoop, this);
    }
}

void ThreadPool::QueueJob(const std::function<void()>& job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);

        // Increment the job counter atomically
        remaining_jobs.fetch_add(1, std::memory_order_relaxed);
    }
    mutex_condition.notify_one();  // Notify worker threads
}

bool ThreadPool::busy() {
    std::unique_lock<std::mutex> lock(queue_mutex);
    return !jobs.empty() || remaining_jobs.load(std::memory_order_relaxed) > 0;
}

void ThreadPool::Stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();

    // Wait for all jobs to complete before stopping threads
    {
        std::unique_lock<std::mutex> counter_lock(job_counter_mutex);
        all_jobs_done_condition.wait(counter_lock, [this] { return remaining_jobs.load() == 0; });
    }

    // Join all threads before returning
    for (std::thread& active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
}

void ThreadPool::ThreadLoop() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] {
                return !jobs.empty() || should_terminate;
            });

            if (should_terminate && jobs.empty()) {
                return; // Exit if termination is requested and the job queue is empty
            }
            job = jobs.front();
            jobs.pop();
        }

        job();  // Execute the job

        // After the job is done, decrement the job counter atomically
        remaining_jobs.fetch_sub(1, std::memory_order_relaxed);

        // Notify when all jobs are done
        {
            std::unique_lock<std::mutex> counter_lock(job_counter_mutex);
            if (remaining_jobs.load() == 0) {
                all_jobs_done_condition.notify_all();
            }
        }
    }
}


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
        // return givenArray[hash1(value)] && givenArray[hash2(value)] && givenArray[hash3(value)] && givenArray[hash4(value)] && givenArray[hash5(value)] && givenArray[hash6(value)] && givenArray[hash7(value)] && givenArray[hash8(value)] && givenArray[hash9(value)] && givenArray[hash10(value)] && givenArray[hash11(value)] && givenArray[hash12(value)] && givenArray[hash13(value)] && givenArray[hash14(value)] && givenArray[hash15(value)] && givenArray[hash16(value)] && givenArray[hash17(value)] && givenArray[hash18(value)] && givenArray[hash19(value)] && givenArray[hash20(value)];
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
    // We are going to use 2-layer hashes, since we are tagging plagged on a single 
    std::vector<long long> rolled1 = RollHash(submission,50);
    std::vector<int> rolled1int;
    for (auto i : rolled1) {
        i = static_cast<int>(i);
        rolled1int.push_back(i);
    }
    std::vector<long long> rolled = RollHash(rolled1int,25);

    // Corner case check
    if (rolled.empty()) return;

    long long curr=bitsets75.size();
    BloomFilter checker;
    

    // Check how many non-overlapping hashes present in previous submissions, in reverse
    for (long long j=curr-1;j>=0;j--) {
        for(long long i=0;i<(long long)rolled.size();i++)
        {
            if(checker.contains(bitsets75[j],rolled[i])){
                // Plag present
                if(curr>numOrigs && !plagged[curr])
                {
                    submissions[curr]->student->flag_student(submissions[curr]);
                    submissions[curr]->professor->flag_professor(submissions[curr]);
                    plagged[curr]=1;

                }

                // Plag close submissions
                if(j>numOrigs && std::abs(timestamps[curr]-timestamps[j])<1.0 && !plagged[j])
                {
                    submissions[j]->student->flag_student(submissions[j]);
                    submissions[j]->professor->flag_professor(submissions[j]);
                    plagged[j]=1;
                }
                break;
            }
        }

        // Continue to check for close submissions
        if(plagged[curr])
        {
            while(j>0 && plagged[j])
            {
                j--;
                if(std::abs(timestamps[curr]-timestamps[j])>1.0) break;
            }
        }
    }
    
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

    tokenizer_t file(submission->codefile);
    std::vector<int> submissionTokens = file.get_tokens();
    // PART1
    len75check(submissionTokens,start_time);
    //PART 2
    len15check(submissionTokens,start_time);
}



plagiarism_checker_t::plagiarism_checker_t(void) {}
plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions) {

        // std::cerr<<"dvdgd"<<std::endl;

    numOrigs = __submissions.size();
    for(int i=0;i<numOrigs;i++){
        // std::cerr<<i<<numOrigs<<std::endl;

        totalMatches.push_back(std::unordered_set<long long>());
        plagged.push_back(0);
        timestamps.push_back(0);


        tokenizer_t origTokenizer(__submissions[i]->codefile);
        std::vector<int> submissionTokens = origTokenizer.get_tokens();

        std::vector<long long> rolled1 = RollHash(submissionTokens,10);
        std::vector<int> rolled1int;
        for (auto jkl : rolled1) {
            jkl = static_cast<int>(jkl);
            rolled1int.push_back(jkl);
        }
        std::vector<long long> rolled = RollHash(rolled1int,5);

        std::vector<long long> rolled70 = RollHash(submissionTokens,50);
        std::vector<int> rolled70int;
        for (auto jk : rolled70) {
            jk = static_cast<int>(jk);
            rolled70int.push_back(jk);
        }
        std::vector<long long> rolled75 = RollHash(rolled70int,25);

        BloomFilter orig;
        for(auto j: rolled){
            orig.add(j);
        }
        std::bitset<400000> origBitarr = orig.give();
        bitsets.push_back(origBitarr);
       
        BloomFilter orig75;
        for(auto j: rolled75){
            orig75.add(j);
        }
        std::bitset<400000> origBitarr75 = orig75.give();
        bitsets75.push_back(origBitarr75);
        submissions.push_back(__submissions[i]);
        // std::cerr<<i<<numOrigs<<std::endl;
    }
        // std::cerr<<"dvdgd"<<std::endl;
    worker.Start();
}

plagiarism_checker_t::~plagiarism_checker_t() {
    while (worker.busy()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Sleep a bit to avoid busy-waiting
    }
    // All jobs are finished, stop the pool
    worker.Stop();
} 

void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto start_time_since_epoch = start_time.time_since_epoch();
    double start_time_in_seconds = std::chrono::duration<double>(start_time_since_epoch).count();

    {
        worker.QueueJob([this, __submission, start_time_in_seconds]() {
            std::cerr << "Starting plagiarism check for submission " << __submission->id << std::endl;
            check_plag(__submission, start_time_in_seconds);
        });
    }
}
