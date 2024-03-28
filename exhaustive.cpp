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






int main(int argc, char *argv[]) {


    cmdline::parser parser;
    parser.add<int>("size", 'n', "the length of the input", false, 16);
    parser.add<int>("sigma", 's', "the size of the sigma", false, 3);
    parser.parse_check(argc, argv);

    std::vector<int> sigmaVec;

    int n = parser.get<int>("size");
    int sigma = parser.get<int>("sigma");

    for (int i = 1; i<sigma+1; i++){

        sigmaVec.push_back(i);
    }
    cout<< "n = "<<n<<"; sigma = "<<sigmaVec.size()<<endl;
    std::vector<int> current;
    std::vector<std::vector<int>> vectors;  // size = sigma^n
    int tau =2;
    double worstRatio = -1;
    std::vector<int> worstVec;

    generateVectors(n, sigmaVec, current, vectors);
    for (auto &vec: vectors){
        double currentRatio = OPSTMethod(vec,tau);
         if(currentRatio > worstRatio){
             worstRatio = currentRatio;
             worstVec.clear();
             worstVec = vec;
             cout<<"Find worse ratio: "<<currentRatio<<" ; Its vector is ";
             for (auto &ele: worstVec){
                 cout<<ele<<" ";
             }
             cout<<endl;
         }
    }
    cout<< "The worst ratio k/n = "<<worstRatio<<" Its worst vector is ";
    for (auto &ele: worstVec){
        cout<<ele<<" ";
    }
    cout<<endl;
    return 0;
}
