#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
const int BASE=257;
const int MOD=1e9+7;

std::mutex m;

plagiarism_checker_t::plagiarism_checker_t(void) {
    thread_running = true;
    worker_thread = std::thread([this] {
        while (thread_running) {
            std::shared_ptr<submission_t> submission;
            double start_time;

            {
                std::unique_lock<std::mutex> lock(m); // Lock the queue for thread safety
                if (!submission_queue.empty()) {
                    submission = submission_queue.front().first;
                    start_time = submission_queue.front().second;
                    submission_queue.pop(); // Remove the submission from the queue
                }
            }

            if (submission) {
                check_plag(submission, start_time); // Process the submission
            } else {
                // If queue is empty, sleep for a short time
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    });
    worker_thread.detach(); // Detach so it runs in the background
}

plagiarism_checker_t::~plagiarism_checker_t(void) {
    thread_running = false;
    if (worker_thread.joinable()) {
        worker_thread.join(); // Optionally, wait for thread to finish
    }
}


class BloomFilter:public plagiarism_checker_t   {
public:
    std::bitset<400000> bitArray;
    BloomFilter(){
    }
    // Simple hash functions for demonstration
    int hash1(int value) { return (value * 17 + 5) % 400000; }
    int hash2(int value) { return (value * 23 + 11) % 400000; }
    int hash3(int value) { return (value * 29 + 7) % 400000; }
    int hash4(int value) { return (value * 31 + 13) % 400000; }
    int hash5(int value) { return (value * 37 + 17) % 400000; }
    int hash6(int value) { return (value * 41 + 19) % 400000; }
    int hash7(int value) { return (value * 53 + 23) % 400000; }
    int hash8(int value) { return (value * 59 + 29) % 400000; }
    int hash9(int value) { return (value * 61 + 31) % 400000; }
    int hash10(int value) { return (value * 67 + 37) % 400000; }
    int hash11(int value) { return (value * 71 + 41) % 400000; }
    int hash12(int value) { return (value * 73 + 43) % 400000; }
    int hash13(int value) { return (value * 79 + 47) % 400000; }
    int hash14(int value) { return (value * 83 + 53) % 400000; }
    int hash15(int value) { return (value * 89 + 59) % 400000; }
    int hash16(int value) { return (value * 97 + 61) % 400000; }
    int hash17(int value) { return (value * 101 + 67) % 400000; }
    int hash18(int value) { return (value * 103 + 71) % 400000; }
    int hash19(int value) { return (value * 107 + 73) % 400000; }
    int hash20(int value) { return (value * 109 + 79) % 400000; }

    void add(int value) {
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
        bitArray[hash14(value)] = 1;
        bitArray[hash15(value)] = 1;
        bitArray[hash16(value)] = 1;
        bitArray[hash17(value)] = 1;
        bitArray[hash18(value)] = 1;
        bitArray[hash19(value)] = 1;
        bitArray[hash20(value)] = 1;
        
    }
    std::bitset<400000> give(){
        return bitArray;
    }

    bool contains(std::bitset<400000>& givenArray,int value) {
        return givenArray[hash1(value)] && givenArray[hash2(value)] && givenArray[hash3(value)] && givenArray[hash4(value)] && givenArray[hash5(value)] && givenArray[hash6(value)] && givenArray[hash7(value)] && givenArray[hash8(value)] && givenArray[hash9(value)] && givenArray[hash10(value)] && givenArray[hash11(value)] && givenArray[hash12(value)] && givenArray[hash13(value)] && givenArray[hash14(value)] && givenArray[hash15(value)] && givenArray[hash16(value)] && givenArray[hash17(value)] && givenArray[hash18(value)] && givenArray[hash19(value)] && givenArray[hash20(value)];
    }
};



std::vector<int> RollHash(std::vector<int> &tokens, int k){
    std::vector<int> hashes;
    long long base_k=1;
        
    for(int i=0; i<k; i++){
        base_k=(1LL*base_k*BASE)%MOD;
    }

    long long hash=0;

    //compute hash for first entry
    for(int i=k-1; i>=0; i++){
        hash = (1LL*hash*BASE+tokens[i])% MOD;
    }
    hashes.push_back(hash);

    //compute for rest of the entries by updating hash
    for(int i=k; i<tokens.size(); i++){
        hash=(hash-tokens[i-k])/BASE;
        hash=(hash+tokens[i]*(base_k/BASE))%MOD;
        hashes.push_back(hash);
    }
    return hashes;
}

void plagiarism_checker_t::len15check(std::vector<int>& submission, double start_time)
{
    // DONT STORE HASHES, USE BITSET DIRECTLY
    std::vector<int> rolled = RollHash(submission,15);
    int curr=bitsets.size();
    // Check how many non-overlapping hashes present in previous submissions, in reverse
    for (int j=curr-1;j>=0;j--) {
        int count=0;
        BloomFilter checker;
        for(int i=0;i<(int)rolled.size();i++)
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
                if(!plagged[curr])
                {
                    submissions[curr]->student->flag_student(submissions[curr]);
                    submissions[curr]->professor->flag_professor(submissions[curr]);
                    plagged[curr]=1;
                }

                // Plag close submissions
                if(std::abs(timestamps[curr]-timestamps[j])<1.0 && !plagged[j])
                {
                    submissions[curr]->student->flag_student(submissions[curr]);
                    submissions[curr]->professor->flag_professor(submissions[curr]);
                    plagged[curr]=1;
                }
                break;
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

        // DONE
        // Also need to add the part of delayed flag call of 1 second window
        // Also need to go reverse on auto submission and iterate till the plag not detected or
        // plag detected but yet under 1 second is satisfied, break otherwise
        // Add the condition to plag both files, if plag detected in 1 second window...
    }

    // Present submission bloom filter
    BloomFilter b;
    for(auto i: rolled){
        b.add(i);
    }
    std::bitset<400000> bitArray = b.give();
    bitsets.push_back(bitArray);
}


void plagiarism_checker_t::check_plag(std::shared_ptr<submission_t> submission,double start_time){
    // can add bitset calculation part here, pass it by reference to the required functions
    // then pushback as required inside that function

    // Store time and pointer
    submissions.push_back(submission);
    timestamps.push_back(start_time);
    plagged.push_back(0);

    std::unique_lock<std::mutex> lock(m);
    // To be placed before mutex???

    tokenizer_t file(submission->codefile);
    std::vector<int> submissionTokens = file.get_tokens();
    

    // PART1
    // FIND 75 MATCHINGS here
    // PART1 END


    //PART 2
    len15check(submissionTokens,start_time);
    // PART 2 END

    // PART 3 
    // USE PATCHWORK ALGORITHM here
    // PART 3 END
}



void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto start_time_since_epoch = start_time.time_since_epoch();
    double start_time_in_seconds = std::chrono::duration<double>(start_time_since_epoch).count();

    // Add submission to the queue
    {
        std::unique_lock<std::mutex> lock(m); // Lock the queue for thread safety
        submission_queue.push({ __submission, start_time_in_seconds });
    }

    // Immediately return, worker thread will process this in the background
}



// //testing purpose
// int main(){
//     std::vector<int> tokens1={1,2,3,4,6,9,4,2,1,5,8,44,2,9};
//     std::vector<int> tokens2={1,2,3,4,6,1,3,5,7,8,9,4,2,44};

//     std::vector<int> hash1 = RollHash(tokens1, 5);
//     std::vector<int> hash2 = RollHash(tokens2, 5);

//     for(int i=0; i<hash1.size(); i++){
//         std::cout<<hash1[i]<<' ';
//     }
//     std::cout<<std::endl;
//     for(int i=0; i<hash2.size(); i++){
//         std::cout<<hash2[i]<<' ';
//     }
//     std::cout<<std::endl;
    
// }
