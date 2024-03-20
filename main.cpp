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


//void readfile(const string &filename, int_vector<>& input_seq_vec){
//    std::ifstream f(filename);
//    uint64_t c;
//    uint64_t cnt = 0;
//    while (f>>c)
//    {
//        input_seq_vec[cnt] =c;
//
//        cnt++;
////        cout<<cnt<<endl;
////        input_seq_vec.push_back(c);
//    }
//    f.close();
//
//}




















int main(int argc, char *argv[])
{
//    int_vector<> iv = {1, 0, 0, 3, 3, 1, 0, 2};

    cmdline::parser parser;
    parser.add<string>("filepath", 'f', "the path to input file", false, "discretization.txt");
    parser.add<int>("rangeThreshold", 'r', "the range of (a,b) determines call the naive or wavelet tree method", false, 10);
    parser.add<int>("sizeThreshold", 's', "the time relations between size and sigma determines call the naive or wavelet tree method", false, 5);
//    parser.add<bool>("unordered_dense", 'u', "Use unordered_dense map or not", false, false);


    parser.parse_check(argc, argv);

    string filename = parser.get<string>("filepath");

    cout<< "Suffix tree is constructed based on "<<filename<<endl;
#ifdef VERBOSE

    cout<<"In detailed log mode."<<endl;
#else
    cout<<"In succinct log mode."<<endl;


#endif

    int rangeThreshold = parser.get<int>("rangeThreshold");
    int sizeThreshold = parser.get<int>("sizeThreshold");
cout<<"---------------------------------------Some Parameters Info Board----------------------------------------------------"<<endl;
    cout<< "If the range of LastCode input (a, b) , namely b - a < "<<rangeThreshold <<", or input size n < "<<sizeThreshold<<" * log(sigma), it utilizes the naive way to compute (p(w), s(w))"<<endl;


#ifdef UNORDERED_DENSE

    cout<<"Using unordered dense map"<<endl;
#else
    cout<<"Using std::unordered_map"<<endl;

#endif

#ifdef INT2PS
    cout<<"Run the following cmd to visualize the constructed suffix tree:"<<endl;
    cout<<"dot -Tpdf pic_nosufL -o suffix_tree_nosuf.pdf"<<endl;
    cout<<"dot -Tpdf pic_sufL -o suffix_tree_suf.pdf"<<endl;

#endif


#ifdef CHECK

    cout<<"Using safe check"<<endl;
#else
    cout<<"Not using safe check"<<endl;

#endif

    cout<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
    int_vector<> input_seq_vec;
    readfile(filename, input_seq_vec);
//    for(int i= 0 ;i< input_seq_vec.size(); i++) {
//        std::cout << input_seq_vec[i]<<" ";
//    }
//
//    std::cout << std::endl;

    auto OP_start = std::chrono::high_resolution_clock::now();

    OPST OP(input_seq_vec, rangeThreshold, sizeThreshold);
    auto OP_end = std::chrono::high_resolution_clock::now();
    double time_OP = std::chrono::duration_cast < std::chrono::milliseconds > (OP_end - OP_start).count()*0.001;
    cout<<"Runtime for construction  = "<<time_OP<<" s"<<endl;


////	ST.forward_search( "ssi" );
//    int * ME = new int [10];
//    ME[0]=ME[1]=ME[3]=ME[4]=ME[6]=ME[7]= 3;
//    ME[2]=ME[5]= 2;
//    ME[8]=1;
//    ME[9]=0;
////	ST.trimST( ME );
//    OP.printLeaves();
//    delete [] ME;
    return 0;
}
