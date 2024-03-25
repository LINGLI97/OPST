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

using namespace std;
using namespace sdsl;



#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <functional>



void readfile(int_vector<> &input_seq_vec,std::vector<int> &w){



    input_seq_vec.resize(w.size());
    for(size_t i = 0; i < w.size(); ++i) {
        input_seq_vec[i] = w[i];
    }
}






int OPSTMethod(std::vector<int> &w, int &tau){
    int_vector<> input_seq_vec;

    // read vector from file
    readfile(input_seq_vec, w);
//    for (auto it: input_seq_vec){
//        cout<<it<<endl;
//    }
    double time_wavelet =0.0;
    int rangeThreshold = 512;

    OPST OP(input_seq_vec, rangeThreshold, time_wavelet);

    OP.MaxTauDFS(tau);
    OP.FindNodes();
//    cout<<"The number of found patterns is "<<OP.MaxTauNodes.size()<<endl;


    return OP.MaxTauNodes.size();


}






std::string generatePatternString(const std::vector<std::pair<int, int>>& sortedArray, int x) {
    std::vector<int> patternString;
    std::stringstream patternStream;
    int n = sortedArray.size();

    for (int i = 0; i < n - 1; ++i) {

        int diff_a = sortedArray[i].first - x;

        patternString.push_back(diff_a);
        patternStream << diff_a;

        if (sortedArray[i].second == sortedArray[i + 1].second) {

            patternString.push_back(1);
            patternStream << ",1,";
        } else {

            patternString.push_back(0);
            patternStream << ",0,";
        }
    }

    int diff_last = sortedArray[n - 1].first - x;
    patternString.push_back(diff_last);
    patternStream << diff_last;

    return patternStream.str(); 
}





int Method2(std::vector<int> &w, int &tau){


//    std::vector<int> w = {1,2,4,1,2,4,1,2,4};
    int cnt_maximal = 0;
    int n = w.size();

    uint64_t H[2][n-1];
//    int HT[2][n];
    std::unordered_map<uint64_t,int> HT[2];


    std::hash<std::string> hashFunc;
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



            //
            uint64_t hashValue = hashFunc(patternString);

            H[k % 2][i] = hashValue;
            int x = ++HT[k % 2][H[k % 2][i]];
            if (x >= tau) {

//                std::cout<<"Killed"<<std::endl;

                HT[(k-1)%2][H[(k-1)%2][i]] =0;
                HT[(k-1)%2][H[(k-1)%2][i+1]] =0;
            }
        }

        if (k > 1) {
//            for(auto p :H[(k - 1) % 2]){
//
//            }

//            for (int p = 0; p < n; p++) {
//                if (HT[(k - 1) % 2][H[(k - 1) % 2][p]] >= tau) {
//
//                    std::cout << "max frequent patterns of  length " << k << " appeared " << HT[(k - 1) % 2][H[(k - 1) % 2][p]]
//                              << " times, one witness:" << p << std::endl;
//                    std::cout<< "The interval is "<<p+1<<","<<p+k<<std::endl;
////                    continue;
//                }
//
//            }


            for (auto it :HT[(k - 1) % 2]) {
                if (it.second >= tau) {

                    cnt_maximal ++;
//                    continue;
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

void run_test(std::vector<int>& vec, int tau) {

    int numOPST = OPSTMethod(vec,tau);
    int numMethod2 = Method2(vec,tau);

    if (numMethod2 != numOPST) {
        std::cerr << "Error: Results differ between methods for tau = " << tau << std::endl;
        exit(-1);
    }


}




int main() {
    const size_t num_tests = 100000;
    const size_t vector_size = 100; // Example vector size
    const int min_val = 1, max_val = 100; // Value range for vector elements
    const int tau_min =2, tau_max = 10; // Range for tau

    for (size_t i = 0; i < num_tests; ++i) {
        auto vec = generate_random_vector(vector_size, min_val, max_val);
        int tau = generate_random_tau(tau_min, tau_max);
//        std::vector<int> vec = {1,2,4,4,2,5,5,1};
//        int tau = 2;
        cout<<"Tau is "<<tau<<endl;
        run_test(vec, tau); // You'll need to adapt this to your specific method calls
        cout<<i+1<< "th test passed!"<<endl;
        cout<<"------------------------------------------------"<<endl;
    }
}

