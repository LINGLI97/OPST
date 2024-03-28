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
#include "baseline.h"
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

    std::vector<int> temp_vec;
    int c;
    while (f >> c) {
        temp_vec.push_back(c);
    }
    f.close();

    // Then set input_seq_vec size otherwise input_seq_vec will have memory problem
    input_seq_vec.resize(temp_vec.size());
    for(size_t i = 0; i < temp_vec.size(); ++i) {
        input_seq_vec[i] = (uint64_t)temp_vec[i];
    }
}




void readfile(const string &filename, std::vector<int> &normalVec){
    std::ifstream f(filename);
    if (!f) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }


    int c;
    while (f >> c) {
        normalVec.push_back(c);
    }
    f.close();

}





int main(int argc, char *argv[])
{


    cmdline::parser parser;
    parser.add<string>("filepath", 'f', "the path to input file", false, "discretization.txt_discretized");
    parser.add<int>("method", 'm', "Three methods: 0 -> OPST; 1 -> quadraticMethod; 2 -> cubicMethod", false, 0);
    parser.add<int>("rangeThreshold", 'r', "the range of (a,b) determines call the naive or wavelet tree method", false, 512);
    parser.add<int>("tau", 't', "the value of minimal support, tau > 1", false, 2);
    parser.add<bool>("printPattern", 'p', "Print out the pattern or not", false, false);


    parser.parse_check(argc, argv);

    string filename = parser.get<string>("filepath");
    int method = parser.get<int>("method");
    int tau = parser.get<int>("tau");

    cout<<"--------------------------------------------Information Board--------------------------------------------------------"<<endl;


    cout<< "Processing "<<filename<<endl;

    cout<<"Tau is set as "<< tau<<"."<<endl;


    if (method ==0){

        cout<<"Using OPST method"<<endl;
        int rangeThreshold = parser.get<int>("rangeThreshold");

        double time_wavelet =0.0;
//    int sizeThreshold = parser.get<int>("sizeThreshold");

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

        cout<<"Sigma of input = "<<OP.sigma<<endl;
        cout<<"n = "<<OP.n<<endl;
        cout<<"Terminate_label $ = " << OP.terminate_label<<endl;
        cout<< "If the range of LastCode input (a, b) , namely b - a < "<<rangeThreshold <<", it utilizes the naive way to compute (p(w), s(w))."<<endl;

        cout<< "Runtime for wavelet tree construction  = "<<time_wavelet<<" s."<<endl;
        cout<<"Runtime for suffix tree construction  = "<<time_OP - time_wavelet<<" s."<<endl;
        cout<<"Total runtime for wavelet tree and suffix tree construction  = "<<time_OP<<" s."<<endl;
        cout<<"Runtime used for find "<<tau<<"-maximal order-preserving "<<tau<<"-frequent patterns: "<<time_DFS<<" s."<<endl;
        cout<<"Total runtime: "<<time_OP+time_DFS<<" s."<<endl;

        cout<<"The total number of explicit nodes k we traverse: "<<OP.explicit_k<<endl;
        cout<<"The ratio of explicit nodes k / n: "<<(double)OP.explicit_k/ (double )OP.n<<endl;

        cout<<"The number of found maximal patterns is "<<OP.MaxTauNodes.size()<<endl;
        if (parser.get<bool>("printPattern")){
            for (const auto& node : OP.MaxTauNodes) {
                std::cout << "Pattern found at interval: [" << node->getStart() << ", " << node->getStart() + node->getDepth() -1 << "]" << std::endl;
            }
        }


        cout<<"---------------------------------------------------------------------------------------------------------------------"<<endl;

// clear the vector of MaxTauNodes for future use
        OP.MaxTauNodes.clear();

    }else if(method==1){
        cout<<"Using n^2*log(n) method"<<endl;

        vector<int> input_vec;
        readfile(filename, input_vec);
        auto quadratic_start = std::chrono::high_resolution_clock::now();

        int numQuadratic= quadraticMethod(input_vec,tau);
        auto quadratic_end = std::chrono::high_resolution_clock::now();
        double time_quadratic = std::chrono::duration_cast < std::chrono::milliseconds > (quadratic_end - quadratic_start).count() * 0.001;
        cout<<"The input size: "<<input_vec.size()<<endl;

        cout<<"Runtime used for find "<<tau<<"-maximal order-preserving "<<tau<<"-frequent patterns: "<<time_quadratic<<" s."<<endl;
        cout<<"The number of found maximal patterns is "<<numQuadratic<<endl;


    } else if (method==2){
        cout<<"Using n^3*log(n) method"<<endl;
        vector<int> input_vec;
        readfile(filename, input_vec);
        auto Cubic_start = std::chrono::high_resolution_clock::now();

        int numCubic= cubicMethod(input_vec,tau);
        auto Cubic_end = std::chrono::high_resolution_clock::now();

        double time_Cubic = std::chrono::duration_cast < std::chrono::milliseconds > (Cubic_end - Cubic_start).count() * 0.001;
        cout<<"The input size: "<<input_vec.size()<<endl;

        cout<<"Runtime used for find "<<tau<<"-maximal order-preserving "<<tau<<"-frequent patterns: "<<time_Cubic<<" s."<<endl;
        cout<<"The number of found maximal patterns is "<<numCubic<<endl;

    } else{


        cout<<"Your input is out of the bound of -m option"<<endl;
        cout<<"Three methods: 0 -> OPST; 1 -> quadraticMethod; 2 -> cubicMethod"<<endl;

    }












    return 0;
}
