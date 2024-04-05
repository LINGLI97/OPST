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

    parser.add<bool>("OPST", 0, "Enable Maximal and Closed OPST method", false, true);

    parser.add<bool>("MaximalQuadratic", 0, "Enable Maximal-Quadratic method", false, false);
    parser.add<bool>("ClosedQuadratic", 0, "Enable Closed-Quadratic method", false, false);


    parser.add<bool>("MaximalCubic", 0, "Enable Maximal-Cubic method", false, false);
    parser.add<bool>("ClosedCubic", 0, "Enable Closed-Cubic method", false, false);

    parser.add<int>("rangeThreshold", 'r', "the range of (a,b) determines call the naive or wavelet tree method", false, 512);
    parser.add<int>("tau", 't', "the value of minimal support, tau > 1", false, 2);
    parser.add<bool>("printPattern", 'p', "Print out the pattern or not", false, false);


    parser.parse_check(argc, argv);

    string filename = parser.get<string>("filepath");
    int tau = parser.get<int>("tau");

    cout<<"--------------------------------------------Information Board--------------------------------------------------------"<<endl;


    cout<< "Processing "<<filename<<endl;
#ifdef VERBOSE

    cout<<"In verbose log mode."<<endl;
#else
    cout<<"In succinct log mode."<<endl;


#endif


    cout<<"Tau is set as "<< tau<<"."<<endl;

    // read file to input_vec
    vector<int> input_vec;
    readfile(filename, input_vec);



    if (parser.get<bool>("OPST")){

        cout<<"Enabling OPST method"<<endl;
        int rangeThreshold = parser.get<int>("rangeThreshold");

        // OPST construction
        auto Construction_start = std::chrono::high_resolution_clock::now();

        OPST OP(input_vec, rangeThreshold);

        auto Construction_end = std::chrono::high_resolution_clock::now();
        double time_Construction = std::chrono::duration_cast < std::chrono::milliseconds > (Construction_end - Construction_start).count()*0.001;



        //Maximal patterns mining

        std::vector<stNode*> MaxTauNodes;

        auto Maximal_start = std::chrono::high_resolution_clock::now();


        OP.MaxTauDFS(tau);
        OP.MaxFindNodes(MaxTauNodes);


        auto Maximal_end = std::chrono::high_resolution_clock::now();
        double time_Maximal = std::chrono::duration_cast < std::chrono::milliseconds > (Maximal_end - Maximal_start).count()*0.001;




        //Closed patterns mining
        std::vector<stNode*> ClosedTauNodes;

        auto Closed_start = std::chrono::high_resolution_clock::now();

        OP.ClosedTauDFS(tau);
        OP.ClosedFindNodes(ClosedTauNodes);


        auto Closed_end = std::chrono::high_resolution_clock::now();
        double time_Closed = std::chrono::duration_cast < std::chrono::milliseconds > (Closed_end - Closed_start).count()*0.001;






        cout<<"The input size: "<<OP.n<<endl;

        cout<<"Terminate_label (2 * n + 1) $ = " << OP.terminate_label<<endl;
//        cout<< "If the range of LastCode input (a, b) , namely b - a < "<<rangeThreshold <<", it utilizes the naive way to compute (p(w), s(w))."<<endl;

//
//        cout<<"Runtime for suffix tree construction  = "<<time_Construction- time_wavelet<<" s."<<endl;
        cout<<"Runtime only for OPST construction  = "<<time_Construction<<" s."<<endl;

        cout<<"---------------------------------------------------------------------------------------------------------------------"<<endl;

        cout<<"Runtime only used for mining "<<tau<<"-maximal order-preserving "<<tau<<"-frequent patterns: "<<time_Maximal<<" s."<<endl;
        cout<<"The number of found maximal patterns is "<<MaxTauNodes.size()<<endl;
        cout<<"Total runtime for OPST construction and Maximal patterns mining : "<<time_Construction+time_Maximal<<" s."<<endl;


        cout<<"---------------------------------------------------------------------------------------------------------------------"<<endl;

        cout<<"Runtime only used for mining "<<tau<<"-closed order-preserving "<<tau<<"-frequent patterns: "<<time_Closed<<" s."<<endl;
        cout<<"The number of found closed patterns is "<<ClosedTauNodes.size()<<endl;

        cout<<"Total runtime for OPST construction and Closed patterns mining : "<<time_Construction+time_Closed<<" s."<<endl;


//        cout<<"The total number of explicit nodes k we traverse: "<<OP.explicit_k<<endl;
//        cout<<"The ratio of explicit nodes k / n: "<<(double)OP.explicit_k/ (double )OP.n<<endl;

//        cout<<"The number of times of using wavelet to compute LastCode: "<<OP.cnt_wt<<endl;
        if (parser.get<bool>("printPattern")){
            for (const auto& node : MaxTauNodes) {
                std::cout << "Maximal Patterns found at interval: [" << node->start << ", " << node->start + node->depth -1 << "]" << std::endl;
            }
            cout<<"---------------------------------------------------------------------------------------------------------------------"<<endl;

            for (const auto& node : ClosedTauNodes) {
                std::cout << "Closed Patterns found at interval: [" << node->start << ", " << node->start + node->depth -1 << "]" << std::endl;
            }

        }


        cout<<"---------------------------------------------------------------------------------------------------------------------"<<endl;

// clear the vector of MaxTauNodes for future use
//        MaxTauNodes.clear();
//        ClosedTauNodes.clear();

    }






    // Baselines

    if(parser.get<bool>("MaximalQuadratic")){
        cout<<"Enabling O(n^2*log(n)) baseline - Maximal patterns mining: "<<endl;



//        std::vector<int> input_vec = {10,6,4,2,8,5,9,5,6,10,3,6,5,8,2,1,10,10,3,3,4,8,6,3,5,4,7,4,9,3,1,7,6,5,2,6,9,2,5,1,4,7,6,6,1,10,7,8,8,9,9,10,3,7,7,3,7,1,7,7,5,5,9,3,1,7,1,7,3,3,4,5,3,6,1,2,9,2,9,6,9,8,6,6,9,3,1,1,8,8,9,2,6,8,2,1,8,4,2,4,8,4,9,5,5,10,10,8,4,1,8,10,4,4,2,1,9,8,3,7,4,2,2,1,4,5,4,10,6,3,3,2,8,1,1,5,6,3,8,1,9,1,7,5,10,9,4,8,1,5,10,5,6,6,5,6,10,8,4,8,8,9,5,7,1,4,6,3,8,2,2,2,1,9,5,6,10,8,4,8,1,4,2,8,8,6,5,5,7,1,1,6,1,8,9,6,3,8,6,3};

        auto quadratic_start = std::chrono::high_resolution_clock::now();

        int numQuadratic= quadraticMethodMax(input_vec,tau);
        auto quadratic_end = std::chrono::high_resolution_clock::now();
        double time_quadratic = std::chrono::duration_cast < std::chrono::milliseconds > (quadratic_end - quadratic_start).count() * 0.001;
        cout<<"The input size: "<<input_vec.size()<<endl;

        cout<<"Runtime used for mining "<<tau<<"-maximal order-preserving "<<tau<<"-frequent patterns: "<<time_quadratic<<" s."<<endl;
        cout<<"The number of found maximal patterns is "<<numQuadratic<<endl;
        cout<<"---------------------------------------------------------------------------------------------------------------------"<<endl;


    }


    if(parser.get<bool>("ClosedQuadratic")){
        cout<<"Enabling O(n^2*log(n)) baseline - Closed patterns mining: "<<endl;

//        std::vector<int> input_vec = {10,6,4,2,8,5,9,5,6,10,3,6,5,8,2,1,10,10,3,3,4,8,6,3,5,4,7,4,9,3,1,7,6,5,2,6,9,2,5,1,4,7,6,6,1,10,7,8,8,9,9,10,3,7,7,3,7,1,7,7,5,5,9,3,1,7,1,7,3,3,4,5,3,6,1,2,9,2,9,6,9,8,6,6,9,3,1,1,8,8,9,2,6,8,2,1,8,4,2,4,8,4,9,5,5,10,10,8,4,1,8,10,4,4,2,1,9,8,3,7,4,2,2,1,4,5,4,10,6,3,3,2,8,1,1,5,6,3,8,1,9,1,7,5,10,9,4,8,1,5,10,5,6,6,5,6,10,8,4,8,8,9,5,7,1,4,6,3,8,2,2,2,1,9,5,6,10,8,4,8,1,4,2,8,8,6,5,5,7,1,1,6,1,8,9,6,3,8,6,3};
//        vector<int> input_vec = {};
//        readfile(filename, input_vec);
//

        auto quadratic_start = std::chrono::high_resolution_clock::now();

        int numQuadratic= quadraticMethodClosed(input_vec,tau);
        auto quadratic_end = std::chrono::high_resolution_clock::now();
        double time_quadratic = std::chrono::duration_cast < std::chrono::milliseconds > (quadratic_end - quadratic_start).count() * 0.001;
        cout<<"The input size: "<<input_vec.size()<<endl;

        cout<<"Runtime used for mining "<<tau<<"-closed order-preserving "<<tau<<"-frequent patterns: "<<time_quadratic<<" s."<<endl;
        cout<<"The number of found closed patterns is "<<numQuadratic<<endl;
        cout<<"---------------------------------------------------------------------------------------------------------------------"<<endl;


    }




    if (parser.get<bool>("MaximalCubic")){
        cout<<"Enabling O(n^3*log(n)) baseline - Maximal patterns mining: "<<endl;
//        vector<int> input_vec;
//        readfile(filename, input_vec);

//        std::vector<int> input_vec = {10,6,4,2,8,5,9,5,6,10,3,6,5,8,2,1,10,10,3,3,4,8,6,3,5,4,7,4,9,3,1,7,6,5,2,6,9,2,5,1,4,7,6,6,1,10,7,8,8,9,9,10,3,7,7,3,7,1,7,7,5,5,9,3,1,7,1,7,3,3,4,5,3,6,1,2,9,2,9,6,9,8,6,6,9,3,1,1,8,8,9,2,6,8,2,1,8,4,2,4,8,4,9,5,5,10,10,8,4,1,8,10,4,4,2,1,9,8,3,7,4,2,2,1,4,5,4,10,6,3,3,2,8,1,1,5,6,3,8,1,9,1,7,5,10,9,4,8,1,5,10,5,6,6,5,6,10,8,4,8,8,9,5,7,1,4,6,3,8,2,2,2,1,9,5,6,10,8,4,8,1,4,2,8,8,6,5,5,7,1,1,6,1,8,9,6,3,8,6,3};

        auto Cubic_start = std::chrono::high_resolution_clock::now();

        int numCubic= cubicMethodMax(input_vec,tau);
        auto Cubic_end = std::chrono::high_resolution_clock::now();

        double time_Cubic = std::chrono::duration_cast < std::chrono::milliseconds > (Cubic_end - Cubic_start).count() * 0.001;
        cout<<"The input size: "<<input_vec.size()<<endl;

        cout<<"Runtime used for mining "<<tau<<"-maximal order-preserving "<<tau<<"-frequent patterns: "<<time_Cubic<<" s."<<endl;
        cout<<"The number of found maximal patterns is "<<numCubic<<endl;
        cout<<"---------------------------------------------------------------------------------------------------------------------"<<endl;

    }




    if (parser.get<bool>("ClosedCubic")){
        cout<<"Enabling O(n^3*log(n)) baseline - Closed patterns mining: "<<endl;
//        vector<int> input_vec;
//        readfile(filename, input_vec);


//        std::vector<int> input_vec = {10,6,4,2,8,5,9,5,6,10,3,6,5,8,2,1,10,10,3,3,4,8,6,3,5,4,7,4,9,3,1,7,6,5,2,6,9,2,5,1,4,7,6,6,1,10,7,8,8,9,9,10,3,7,7,3,7,1,7,7,5,5,9,3,1,7,1,7,3,3,4,5,3,6,1,2,9,2,9,6,9,8,6,6,9,3,1,1,8,8,9,2,6,8,2,1,8,4,2,4,8,4,9,5,5,10,10,8,4,1,8,10,4,4,2,1,9,8,3,7,4,2,2,1,4,5,4,10,6,3,3,2,8,1,1,5,6,3,8,1,9,1,7,5,10,9,4,8,1,5,10,5,6,6,5,6,10,8,4,8,8,9,5,7,1,4,6,3,8,2,2,2,1,9,5,6,10,8,4,8,1,4,2,8,8,6,5,5,7,1,1,6,1,8,9,6,3,8,6,3};

        auto Cubic_start = std::chrono::high_resolution_clock::now();

        int numCubic= cubicMethodClosed(input_vec,tau);
        auto Cubic_end = std::chrono::high_resolution_clock::now();

        double time_Cubic = std::chrono::duration_cast < std::chrono::milliseconds > (Cubic_end - Cubic_start).count() * 0.001;
        cout<<"The input size: "<<input_vec.size()<<endl;

        cout<<"Runtime used for mining "<<tau<<"-closed order-preserving "<<tau<<"-frequent patterns: "<<time_Cubic<<" s."<<endl;
        cout<<"The number of found closed patterns is "<<numCubic<<endl;
        cout<<"---------------------------------------------------------------------------------------------------------------------"<<endl;


    }
















    return 0;
}
