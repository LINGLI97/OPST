//
// Created by ling on 12/03/24.
//


#include <iostream>
#include <string>
#include <sdsl/wavelet_trees.hpp>
#include <fstream>


//#include "Successor.h"
//#include "Predecessor.h"
//
#include "OPST.h"

using namespace std;
using namespace sdsl;

//#define terminate_label 0
//#include "cmdline.h"


void readfile(const string &filename, int_vector<>& input_seq_vec){
    std::ifstream f(filename);
    if (!f) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::vector<uint64_t> temp_vec;
    uint64_t c;
    while (f >> c) {
        temp_vec.push_back(c);
    }
    f.close();

    // Then set input_seq_vec size otherwise input_seq_vec will have memory problem
    input_seq_vec.resize(temp_vec.size());
    for(size_t i = 0; i < temp_vec.size(); ++i) {
        input_seq_vec[i] = temp_vec[i];
    }
}





int main(int argc, char *argv[])
{


    cmdline::parser parser;
    parser.add<string>("filepath", 'f', "the path to input file", false, "discretization.txt_discretized");
    parser.add<int>("rangeThreshold", 'r', "the range of (a,b) determines call the naive or wavelet tree method", false, 512);
    parser.add<int>("tau", 't', "the value of minimal support, tau > 1", false, 2);


    parser.parse_check(argc, argv);

    string filename = parser.get<string>("filepath");

    cout<< "Suffix tree is constructed based on "<<filename<<endl;

    int tau = parser.get<int>("tau");

    int rangeThreshold = parser.get<int>("rangeThreshold");

    double time_wavelet =0.0;
//    int sizeThreshold = parser.get<int>("sizeThreshold");
cout<<"--------------------------------------------Information Board--------------------------------------------------------"<<endl;



    int_vector<> input_seq_vec;

    // read vector from file
    readfile(filename, input_seq_vec);

    auto OP_start = std::chrono::high_resolution_clock::now();

    OPST OP(input_seq_vec, rangeThreshold, time_wavelet);

    auto OP_end = std::chrono::high_resolution_clock::now();
    double time_OP = std::chrono::duration_cast < std::chrono::milliseconds > (OP_end - OP_start).count()*0.001;

    auto DFS_start = std::chrono::high_resolution_clock::now();

    OP.MaxTauDFS(tau);
    OP.FindNodes();
    auto DFS_end = std::chrono::high_resolution_clock::now();
    double time_DFS = std::chrono::duration_cast < std::chrono::milliseconds > (DFS_end - DFS_start).count()*0.001;


//    OP.exportSuffixTreeToDot("count", false);
//    OP.exportSuffixTreeToDot("count2", true);

#ifdef VERBOSE

    cout<<"In verbose log mode."<<endl;
#else
    cout<<"In succinct log mode."<<endl;


#endif

#ifdef UNORDERED_DENSE

    cout<<"Using unordered dense map"<<endl;
#else
    cout<<"Using std::unordered_map"<<endl;

#endif

#ifdef VISUALIZATION
    cout<<"Run the following cmd to visualize the constructed suffix tree:"<<endl;
    cout<<"dot -Tpdf pic_nosufL -o suffix_tree_nosuf.pdf"<<endl;
    cout<<"dot -Tpdf pic_sufL -o suffix_tree_suf.pdf"<<endl;

#endif


#ifdef CHECK

    cout<<"Using safe check"<<endl;
#else
    cout<<"Not using safe check"<<endl;

#endif


    cout<<"Tau is set as "<< tau<<"."<<endl;

    cout<< "If the range of LastCode input (a, b) , namely b - a < "<<rangeThreshold <<", it utilizes the naive way to compute (p(w), s(w))."<<endl;

    cout<< "Runtime for wavelet tree construction  = "<<time_wavelet<<" s."<<endl;
    cout<<"Runtime for suffix tree construction  = "<<time_OP - time_wavelet<<" s."<<endl;
    cout<<"Total runtime for wavelet tree and suffix tree construction  = "<<time_OP<<" s."<<endl;
    cout<<"Runtime used for find "<<tau<<"-maximal order-preserving "<<tau<<"-frequent patterns: "<<time_DFS<<" s."<<endl;
    for (const auto& node : OP.MaxTauNodes) {
        std::cout << "Pattern found at interval: [" << node->getStart() << ", " << node->getStart() + node->getDepth() -1 << "]" << std::endl;
    }

    cout<<"---------------------------------------------------------------------------------------------------------------------"<<endl;

// clear the vector of MaxTauNodes for future use
    OP.MaxTauNodes.clear();
    return 0;
}
