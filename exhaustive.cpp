//
// Created by ling on 28/03/24.
//
#include <iostream>
#include <vector>
#include "OPST.h"
using namespace std;
using namespace sdsl;


#include <random>
#include <chrono>


void generateVectors(int n, const std::vector<int>& sigma, std::vector<int>& current, std::vector<std::vector<int>>& result) {
    if (n == 0) {

        result.push_back(current);
        return;
    }

    for (int i = 0; i < sigma.size(); ++i) {

        current.push_back(sigma[i]);
        generateVectors(n - 1, sigma, current, result);
        current.pop_back();
    }
}



void readfile(int_vector<> &input_seq_vec,std::vector<int> &w){



    input_seq_vec.resize(w.size());
    for(size_t i = 0; i < w.size(); ++i) {
        input_seq_vec[i] = w[i];
    }
}



double OPSTMethod(std::vector<int> &w, int &tau){
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


    return (double)OP.explicit_k/ (double )OP.n;


}






int main() {
    int n = 16; 
    std::vector<int> sigma = {1, 2, 3}; // sigma
    std::vector<int> current;
    std::vector<std::vector<int>> vectors;  // size = 3^16
    int tau =2;
    double worstRatio = -1;
    generateVectors(n, sigma, current, vectors);
    for (auto &vec: vectors){
        double currentRatio = OPSTMethod(vec,tau);
         if(currentRatio > worstRatio){
             worstRatio = currentRatio;
             cout<<"currentRatio: "<<currentRatio<<endl;
         }
    }
    cout<< "The worst ratio k/n = "<<worstRatio<<endl;


    return 0;
}
