//
// Created by ling on 25/03/24.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <utility> // for std::pair
#include <string> // for std::string
#include <sstream> // for std::stringstream
#include <functional>

#include <sdsl/wavelet_trees.hpp>
#include <fstream>


//#include "Successor.h"
//#include "Predecessor.h"
//
#include "OPST.h"
#include "karp_rabin_hashing.hpp"
#include "utils.hpp"
#include <math.h>
#include <cmath>

using namespace std;
using namespace sdsl;





#include <random>
#include <chrono>




namespace karp_rabin_hashing {

//=============================================================================
// Base and exponent used in Karp-Rabin hashing.
//=============================================================================
    std::uint64_t hash_variable;
    std::uint64_t mersenne_prime_exponent;

//=============================================================================
// Return (a * b) mod p, where p = (2^k) - 1.
// Requires a, b <= 2^k. Tested for k = 1, .., 63.
//=============================================================================
    std::uint64_t mul_mod_mersenne(
            const std::uint64_t a,
            const std::uint64_t b,
            const std::uint64_t k) {
        const std::uint64_t p = ((std::uint64_t)1 << k) - 1;
        __extension__ const unsigned __int128 ab =
                (unsigned __int128)a *
                (unsigned __int128)b;
        std::uint64_t lo = (std::uint64_t)ab;
        const std::uint64_t hi = (ab >>  (uint64_t) 64);
        lo = (lo & p) + ((lo >> k) + (hi << ( (uint64_t) 64 - k)));
        lo = (lo & p) + (lo >> k);
        return lo == p ?  (uint64_t) 0 : lo;
    }

//=============================================================================
// Return a mod p, where p = (2^k) - 1.
// Works for any a in [0..2^64).
// Tested for k = 1, .., 63.
//=============================================================================
    std::uint64_t mod_mersenne(
            std::uint64_t a,
            const std::uint64_t k) {
        std::uint64_t p = ((std::uint64_t)1 << k) -  (uint64_t) 1;
        if (k < (uint64_t) 32) {

            // We need to check if a <= 2^(2k).
            const std::uint64_t threshold = ((std::uint64_t)1 << (k <<  (uint64_t) 1));
            if (a <= threshold) {
                a = (a & p) + (a >> k);
                a = (a & p) + (a >> k);
                return a == p ?  (uint64_t) 0 : a;
            } else return a % p;
        } else {

            // We are guaranteed that a < 2^(2k)
            // because a < 2^64 <= 2^(2k).
            a = (a & p) + (a >> k);
            a = (a & p) + (a >> k);
            return a == p ?  (uint64_t) 0 : a;
        }
    }

//=============================================================================
// Return random number x in [0..p), where p = (2^k) - 1.
//=============================================================================
    std::uint64_t rand_mod_mersenne(const std::uint64_t k) {
        const std::uint64_t p = ((std::uint64_t)1 << k) -  (uint64_t) 1;
        return utils::random_int<std::uint64_t>(
                (std::uint64_t)0, (std::uint64_t(p -  (uint64_t) 1)));
    }

//=============================================================================
// Return (a^n) mod p, where p = (2^k) - 1.
//=============================================================================
    std::uint64_t  pow_mod_mersenne(
            const std::uint64_t a,
            std::uint64_t n,
            const std::uint64_t k) {
        std::uint64_t pow = mod_mersenne(a, k);
        std::uint64_t ret = mod_mersenne( (uint64_t) 1, k);
        while (n >  (uint64_t) 0) {
            if (n &  (uint64_t) 1)
                ret = mul_mod_mersenne(ret, pow, k);
            pow = mul_mod_mersenne(pow, pow, k);
            n >>=  (uint64_t) 1;
        }
        return ret;
    }

//=============================================================================
// Given Karp-Rabin hashes of two substrings, return
// the Karp-Rabin hash of their concatenation.
//=============================================================================
    std::uint64_t concat(
            const std::uint64_t left_hash,
            const std::uint64_t right_hash,
            const std::uint64_t right_len) {
        const std::uint64_t pow = pow_mod_mersenne(
                hash_variable, right_len, mersenne_prime_exponent);
        const std::uint64_t tmp = mul_mod_mersenne(
                left_hash, pow, mersenne_prime_exponent);
        const std::uint64_t ret = mod_mersenne(
                tmp + right_hash, mersenne_prime_exponent);
        return ret;
    }
    std::uint64_t power(const std::uint64_t k)
    {
        return pow((uint64_t)2,k);
    }
    std::uint64_t subtract(const std::uint64_t long_hash, const std::uint64_t short_hash, const std::uint64_t right_len)
    {
        const std::uint64_t pow = pow_mod_mersenne(
                hash_variable, right_len, mersenne_prime_exponent);


        //mul_mod_mersenne requires a, b <= 2^k
        uint64_t max_pow=power(mersenne_prime_exponent);
        if(short_hash > max_pow || pow > max_pow || mersenne_prime_exponent > 63)
            cout<<"Error2";

        const std::uint64_t tmp = mul_mod_mersenne(
                short_hash, pow, mersenne_prime_exponent);

        if(long_hash < tmp)cout<<"error\n";
        const std::uint64_t ret = mod_mersenne(
                long_hash-tmp, mersenne_prime_exponent);

        return ret;
    }

//=============================================================================
// Initialize the base and exponent for Karp-Rabin hashing.
//=============================================================================
    void init() {
        mersenne_prime_exponent = 61;
        hash_variable = rand_mod_mersenne(mersenne_prime_exponent);
    }

}  // namespace karp_rabin_kashing







void readfile(int_vector<> &input_seq_vec,std::vector<int> &w){



    input_seq_vec.resize(w.size());
    for(size_t i = 0; i < w.size(); ++i) {
        input_seq_vec[i] = w[i];
    }
}






int OPSTMethodMax(std::vector<int> &w, int &tau){
    int_vector<> input_seq_vec;
    std::vector<stNode*> MaxTauNodes;

    // read vector from file
    readfile(input_seq_vec, w);
//    for (auto it: input_seq_vec){
//        cout<<it<<endl;
//    }
    double time_wavelet =0.0;
    int rangeThreshold = 512;

    OPST OP(input_seq_vec, rangeThreshold, time_wavelet);
//    OP.exportSuffixTreeToDot("count22", true);

    OP.MaxTauDFS(tau);
    OP.MaxFindNodes(MaxTauNodes);
//    cout<<"The number of found patterns is "<<OP.MaxTauNodes.size()<<endl;


    return MaxTauNodes.size();


}


int OPSTMethodClosed(std::vector<int> &w, int &tau){
    int_vector<> input_seq_vec;

    // read vector from file
    readfile(input_seq_vec, w);
//    for (auto it: input_seq_vec){
//        cout<<it<<endl;
//    }
    std::vector<stNode*> ClosedTauNodes;

    double time_wavelet =0.0;
    int rangeThreshold = 512;

    OPST OP(input_seq_vec, rangeThreshold, time_wavelet);
//    OP.exportSuffixTreeToDot("count1", false);
    OP.exportSuffixTreeToDot("count12", false);
    OP.ClosedTauDFS(tau);
    OP.exportSuffixTreeToDot("count2", false);
//    OP.exportSuffixTreeToDot("count22", true);
    OP.ClosedFindNodes(ClosedTauNodes);
//    cout<<"The number of found patterns is "<<OP.MaxTauNodes.size()<<endl;

//    OP.exportSuffixTreeToDot("count3", false);
//    OP.exportSuffixTreeToDot("count32", true);
    return ClosedTauNodes.size();


}





std::string generatePatternString(const std::vector<std::pair<int, int>>& sortedArray, int x) {
    std::stringstream patternStream;
    int n = sortedArray.size();

    for (int i = 0; i < n - 1; ++i) {

        int diff_a = sortedArray[i].first - x;

        patternStream << diff_a;

        if (sortedArray[i].second == sortedArray[i + 1].second) {

            patternStream << ",1,";
        } else {

            patternStream << ",0,";
        }
    }

    int diff_last = sortedArray[n - 1].first - x;
    patternStream << diff_last;

    return patternStream.str();
}





int cubicMethodMax(std::vector<int> &w, int &tau){
    karp_rabin_hashing::init();

//    std::vector<int> w = {1,2,4,1,2,4,1,2,4};
    int cnt_maximal = 0;
    int n = w.size();

    uint64_t H[2][n-1];
//    int HT[2][n];
    std::unordered_map<uint64_t,int> HT[2];

//
    for(int i = 0; i < n; ++i) {
        H[0][i] = 0;
    }
    HT[0][0] = n;

    for(int k = 1; k < n; ++k) {
        for (int i = 0; i < n - k; ++i) {

//            std::cout << "the range of w " << "(" << i << "," << k + i << ")" << std::endl;
            std::vector<std::pair<int, int>> subsequence;
            for (int j = i; j <= i + k; ++j) {
                subsequence.push_back({j, w[j]});
            }

            std::sort(subsequence.begin(), subsequence.end(),
                      [](const std::pair<int, int> &a, const std::pair<int, int> &b) {
                          if (a.second != b.second) {
                              return a.second < b.second;
                          } else {
                              return a.first < b.first;
                          }
                      });

            std::string patternString = generatePatternString(subsequence, i);

//            cout<<patternString<<" "<<patternString.c_str()<<" "<<patternString.size()<<endl;

            uint64_t hashValue  = karp_rabin_hashing::hash_string(patternString.c_str(),patternString.size());


            H[k % 2][i] = hashValue;
            int x = ++HT[k % 2][H[k % 2][i]];
            if (x >= tau) {

                HT[(k-1)%2][H[(k-1)%2][i]] =0;
                HT[(k-1)%2][H[(k-1)%2][i+1]] =0;
            }
        }




            for (auto it :HT[(k - 1) % 2]) {
                if (it.second >= tau) {

                    cnt_maximal ++;
//                    continue;
                }

            }


        HT[(k - 1) % 2].clear();

    }
    return cnt_maximal;
}





int quadraticMethodMax(std::vector<int> &w, int &tau){
    karp_rabin_hashing::init();


    int cnt_maximal = 0;
    int n = w.size();

    uint64_t H[2][n];

    std::unordered_map<uint64_t,int> HT[2];
    std::set<std::pair<int, int>> S;

    for(int i = 0; i < n; ++i) {
        H[0][i] = 0;
    }
    HT[0][0] = n;

    uint64_t odd_hashValue  = 2 * n;

    for(int k = 1; k < n; ++k) {

        //initialization of Set S
        S.clear();
        for (int j = 0; j < k; ++j) {
            S.insert({w[j], j});
        }

        for (int i = 0; i < n - k; ++i) {

            std::pair<std::set<pair<int, int>>::iterator, bool> current = S.insert({w[i+k] , i+k});

            auto it = current.first;
            if (it != S.begin()) { // make it is not the first element
                --it; // the iterator points to the element before current

                // the position of the element before current
                //whether its first coordinate is equal to w[i+k].

                uint64_t appendix = 2*(it->second - i) + (int)(it->first == w[i+k]);

                H[k % 2][i] = karp_rabin_hashing::concat(H[(k - 1) % 2][i], appendix, 1);

            } else {


                H[k % 2][i] = karp_rabin_hashing::concat(H[(k - 1) % 2][i], odd_hashValue, 1);
            }

            int x = ++HT[k % 2][H[k % 2][i]];
            if (x >= tau) {

//                std::cout<<"Killed"<<std::endl;

                HT[(k-1)%2][H[(k-1)%2][i]] =0;
                HT[(k-1)%2][H[(k-1)%2][i+1]] =0;
            }

            // window sliding
            if (i + k + 1 < n) {
                S.erase({w[i], i});

            }

        }



        for (auto it :HT[(k - 1) % 2]) {
            if (it.second >= tau) {

                cnt_maximal ++;
            }
        }

        HT[(k - 1) % 2].clear();//        std::vector<int> vec = {1,2,4,4,2,5,5,1};

    }
    return cnt_maximal;
}



int cubicMethodClosed(std::vector<int> &w, int &tau,std::unordered_map<int, vector<vector<int>>>&result){
    karp_rabin_hashing::init();

//    std::vector<int> w = {1,2,4,1,2,4,1,2,4};
    int cnt_maximal = 0;
    int n = w.size();

    uint64_t H[2][n];
    std::unordered_map<uint64_t,int> HT[2];

//
    for(int i = 0; i < n; ++i) {
        H[0][i] = 0;
    }
    HT[0][0] = n;

    for(int k = 1; k < n; ++k) {
        for (int i = 0; i < n - k; ++i) {

//            std::cout << "the range of w " << "(" << i << "," << k + i << ")" << std::endl;
            std::vector<std::pair<int, int>> subsequence;
            for (int j = i; j <= i + k; ++j) {
                subsequence.push_back({j, w[j]});
            }

            std::sort(subsequence.begin(), subsequence.end(),
                      [](const std::pair<int, int> &a, const std::pair<int, int> &b) {
                          if (a.second != b.second) {
                              return a.second < b.second;
                          } else {
                              return a.first < b.first;
                          }
                      });

            std::string patternString = generatePatternString(subsequence, i);


            uint64_t hashValue  = karp_rabin_hashing::hash_string(patternString.c_str(),patternString.size());

//            cout<<"k = "<<k <<"; hashvalue: "<<hashValue<<endl;
            H[k % 2][i] = hashValue;
            HT[k % 2][H[k % 2][i]]++;
            if (HT[(k-1)%2][H[(k-1)%2][i]] == HT[k%2][H[k%2][i]]) {

//                std::cout<<"Killed"<<std::endl;

                HT[(k-1)%2][H[(k-1)%2][i]] =0;

            }



            if (HT[(k-1)%2][H[(k-1)%2][i+1]] == HT[k%2][H[k%2][i]]){

//                std::cout<<"Killed"<<std::endl;

                HT[(k-1)%2][H[(k-1)%2][i+1]] =0;

            }
        }




        for (auto it :HT[(k - 1) % 2]) {
            if (it.second >= tau) {
//                cout<<it.first<<" pattern happened "<< it.second<<"times"<<endl;

                cnt_maximal ++;
                vector<int> vec;
                for (int p = 0; p< n; p++){
                    if (H[(k - 1) % 2][p] == it.first ){
//                        cout<<"witness"<< p <<" ";
                        vec.push_back(p);
                    }
                }
//                cout<<endl;


//              cout<<k<<" ";
                auto it = result.find(k);
                if (it == result.end()){
                    vector<vector<int>> vec2D;
                    vec2D.push_back(vec);
                    result.insert({k,vec2D});
                } else{

                    result[k].push_back(vec);

                }

            }

        }


        HT[(k - 1) % 2].clear();

    }
    return cnt_maximal;
}


int quadraticMethodClosed(std::vector<int> &w, int &tau, std::unordered_map<int, vector<vector<int>>> &result){
    karp_rabin_hashing::init();


    int cnt_maximal = 0;
    int n = w.size();

    uint64_t H[2][n];

    std::unordered_map<uint64_t,int> HT[2];
    std::set<std::pair<int, int>> S;

    for(int i = 0; i < n; ++i) {
        H[0][i] = 0;
    }
    HT[0][0] = n;
    uint64_t odd_hashValue = 2*n;


    for(int k = 1; k < n; ++k) {

        //initialization of Set S
        S.clear();
        for (int j = 0; j < k; ++j) {
            S.insert({w[j], j});
        }

        for (int i = 0; i < n - k; ++i) {

            std::pair<std::set<pair<int, int>>::iterator, bool> current = S.insert({w[i+k] , i+k});

            auto it = current.first;
            if (it != S.begin()) { // make it is not the first element
                --it; // the iterator points to the element before current

                // the position of the element before current
                //whether its first coordinate is equal to w[i+k].
                uint64_t appendix = 2*(it->second - i) + (int)(it->first == w[i+k]);

                H[k % 2][i] = karp_rabin_hashing::concat(H[(k - 1) % 2][i], appendix, 1);


            } else {


                H[k % 2][i] = karp_rabin_hashing::concat(H[(k - 1) % 2][i], odd_hashValue, 1);

            }

            HT[k % 2][H[k % 2][i]]++;
            if (HT[(k-1)%2][H[(k-1)%2][i]] == HT[k%2][H[k%2][i]]) {

//                std::cout<<"Killed"<<std::endl;

                HT[(k-1)%2][H[(k-1)%2][i]] =0;

            }



            if (HT[(k-1)%2][H[(k-1)%2][i+1]] == HT[k%2][H[k%2][i]]){

                HT[(k-1)%2][H[(k-1)%2][i+1]] =0;

            }
            // window sliding
            if (i + k + 1 < n) {
                S.erase({w[i], i});

            }

        }



        for (auto it :HT[(k - 1) % 2]) {
            if (it.second >= tau) {
//                    cout<<"times" <<it.second<<
                cnt_maximal ++;
//                    continue;
//                cout<<it.first<<" pattern happened "<< it.second<<"times"<<endl;
//                cout<<k<<" ";

                vector<int> vec;
                for (int p = 0; p< n; p++){
                    if (H[(k - 1) % 2][p] == it.first ){
//                        cout<<"witness"<< p <<" ";
                        vec.push_back(p);
                    }
                }
                auto it = result.find(k);
                if (it == result.end()){
                    vector<vector<int>> vec2D;
                    vec2D.push_back(vec);
                    result.insert({k,vec2D});
                } else{

                    result[k].push_back(vec);

                }


            }
        }










        HT[(k - 1) % 2].clear();

    }
    return cnt_maximal;
}




// Utility to generate a random integer within a range
int generate_random_int(int min, int max) {
    static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

// Function to generate a random vector
std::vector<int> generate_random_vector(size_t size, int min_val, int max_val) {
    std::vector<int> vec(size);
    for (auto& val : vec) {
        val = generate_random_int(min_val, max_val);
    }
    return vec;
}

// Function to generate a random tau
int generate_random_tau(int min_val, int max_val) {
    return generate_random_int(min_val, max_val);
}

void run_test_max(std::vector<int>& vec, int tau) {

    // test the number of tau-maximal patterns
    int numOPST = OPSTMethodMax(vec,tau);

    cout<<"numOPST: "<<numOPST<<endl;

    int numCubic= cubicMethodMax(vec,tau);
    cout<<"numcubic: "<<numCubic<<endl;

    int numQuadratic= quadraticMethodMax(vec,tau);
    cout<<"numquadratic: "<<numQuadratic<<endl;

    if (numCubic != numOPST || numQuadratic != numOPST) {
        std::cerr << "Error: Results differ between methods for tau = " << tau << std::endl;
        exit(-1);
    }


}

void run_test_closed(std::vector<int>& vec, int tau) {

    // test the number of tau-maximal patterns
    int numOPST = OPSTMethodClosed(vec,tau);
    cout<<"numOPST: "<<numOPST<<endl;

    std::unordered_map<int, vector<vector<int>>> cubicresult;

    std::unordered_map<int, vector<vector<int>>> quadraticresult;

//    std::vector<int> vec1 = {7,5,5,9,3,1,7,1};
//    std::vector<int> vec2 = {8,6,6,9,3,1,1,8};



    int numCubic= cubicMethodClosed(vec,tau,cubicresult);
    cout<<"numcubic: "<<numCubic<<endl;

    int numQuadratic= quadraticMethodClosed(vec,tau, quadraticresult);
//    int numQuadratic1= quadraticMethodClosed(vec2,tau, quadraticresult);

    cout<<"numquadratic: "<<numQuadratic<<endl;

    if (numCubic != numQuadratic || numCubic != numOPST) {
//    if (numCubic != numQuadratic ) {

        for (auto i: vec){
            cout<<i<<",";
        }
        cout<<endl;
        std::cerr << "Error: Results differ between methods for tau = " << tau << std::endl;
        exit(-1);
    }


}



int main() {
    const size_t num_tests = 100000;
    const size_t vector_size = 100; // Example vector size
    const int min_val = 1, max_val = 10; // Value range for vector elements
    const int tau_min =2, tau_max = 10; // Range for tau

    for (size_t i = 0; i < num_tests; ++i) {
        auto vec = generate_random_vector(vector_size, min_val, max_val);
        int tau = generate_random_tau(tau_min, tau_max);
//        std::vector<int> vec = {10,6,4,2,8,5,9,5,6,10,3,6,5,8,2,1,10,10,3,3,4,8,6,3,5,4,7,4,9,3,1,7,6,5,2,6,9,2,5,1,4,7,6,6,1,10,7,8,8,9,9,10,3,7,7,3,7,1,7,7,5,5,9,3,1,7,1,7,3,3,4,5,3,6,1,2,9,2,9,6,9,8,6,6,9,3,1,1,8,8,9,2,6,8,2,1,8,4,2,4,8,4,9,5,5,10,10,8,4,1,8,10,4,4,2,1,9,8,3,7,4,2,2,1,4,5,4,10,6,3,3,2,8,1,1,5,6,3,8,1,9,1,7,5,10,9,4,8,1,5,10,5,6,6,5,6,10,8,4,8,8,9,5,7,1,4,6,3,8,2,2,2,1,9,5,6,10,8,4,8,1,4,2,8,8,6,5,5,7,1,1,6,1,8,9,6,3,8,6,3};
//        int tau = 2;
//                std::vector<int> vec = {0,1,2,3,4,5,6};
//        std::vector<int> vec = {4,5,8,3,8,4,5,9,1,6};
//
//        int tau = 2;
//        std::vector<int> vec = {9,8,6,4,3,6,10,3,9,2};
//        int tau = 2;

//        std::vector<int> vec = {11,10,21,25,12,14,18,19,26,13,16,20,24,30,15,17};
//        int tau = 3;
//        cout<<"Tau is "<<tau<<endl;

//        run_test_max(vec, tau); // You'll need to adapt this to your specific method calls
        run_test_closed(vec, tau); // You'll need to adapt this to your specific method calls


        cout<<i+1<< "th test passed!"<<endl;
        cout<<"------------------------------------------------"<<endl;
    }
}

