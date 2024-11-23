#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
const int BASE=257;
const int MOD=1e9+7;

std::mutex m;

class BloomFilter:public plagiarism_checker_t   {
public:
    std::bitset<400000> bitArray(0);
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

    bool contains(int value) {
        return bitArray[hash1(value)] && bitArray[hash2(value)] && bitArray[hash3(value)] && bitArray[hash4(value)] && bitArray[hash5(value)] && bitArray[hash6(value)] && bitArray[hash7(value)] && bitArray[hash8(value)] && bitArray[hash9(value)] && bitArray[hash10(value)] && bitArray[hash11(value)] && bitArray[hash12(value)] && bitArray[hash13(value)] && bitArray[hash14(value)] && bitArray[hash15(value)] && bitArray[hash16(value)] && bitArray[hash17(value)] && bitArray[hash18(value)] && bitArray[hash19(value)] && bitArray[hash20(value)];
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



void plagiarism_checker_t::check_plag(vector<int> submission){
    std::unique_lock<mutex> lock(m);
    std::vector<int> rolled = RollHash(submission,15);
    BloomFilter b();
    for(auto i: rolled){
        b.add(i);
    }
    std::bitset<400000> bitArray = b.give();
    for(auto prevSubmission: bitsets){
        int count=0;
        for(auto rolledHashValue:prevSubmission){
            if(b.contains(rolledHashValue)){
                count++;
            }
        }
        if(count>=10){
            _submission.student_t.flag_student(_submission);
            _submission.professor_t.flag_professor(_submission);
        }
    }
    bitsets.push_back(bitArray);
}


void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission){
    tokenizer_t file(__submission.codefile);
    std::vector<int> submission = file.get_tokens();
    t(check_plag(submission));
    return;
}




//testing purpose
int main(){
    std::vector<int> tokens1={1,2,3,4,6,9,4,2,1,5,8,44,2,9};
    std::vector<int> tokens2={1,2,3,4,6,1,3,5,7,8,9,4,2,44};

    std::vector<int> hash1 = RollHash(tokens1, 5);
    std::vector<int> hash2 = RollHash(tokens2, 5);

    for(int i=0; i<hash1.size(); i++){
        std::cout<<hash1[i]<<' ';
    }
    std::cout<<std::endl;
    for(int i=0; i<hash2.size(); i++){
        std::cout<<hash2[i]<<' ';
    }
    std::cout<<std::endl;
    
}