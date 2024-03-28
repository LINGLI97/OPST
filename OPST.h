//
// Created by ling on 12/03/24.
//

#ifndef OPST_H
#define OPST_H



#include <string>
#include <map>
#include "node.h"
#include "cmdline.h"

//#include "Predecessor.h"
//#include "Successor.h"
#include <sdsl/wavelet_trees.hpp>
#include <iostream>
#include <random> // For std::mt19937 and std::uniform_int_distribution
#include <cstdlib> // For std::exit
using namespace sdsl;
//#define RANGE_THRESHOLD 10
//#define SiZE_THRESHOLD 5
//#define VERBOSE
#define NA -1

using namespace std;

class DerivedWTInt : public sdsl::wt_int<> {
public:

    using wt_int<>::wt_int;

    int rank_bit( value_type c, int i,size_type offset) const;

    int select_bit( value_type c, int i, const node_type& node) const;


};


class OPST
{
private:
    stNode * root;
    int_vector<> w;
    unordered_map<uint64_t, pair<int, int>> int2ps;

    int rangeThreshold;

    DerivedWTInt wt;




public:
    explicit OPST( int_vector<> & w, int &rangeThreshold, double &time_wavelet);


//predecessor
    int Max(const sdsl::wt_int<>::node_type& u, int a, int b);
    int predecessorNV(int a, int b);
    int predecessorWT(const sdsl::wt_int<>::node_type& u, int a, int b);

//successor
    int Min(const sdsl::wt_int<>::node_type& u, int a, int b);
    int successorNV(int a, int b);
    int successorWT(const sdsl::wt_int<>::node_type& u, int a, int b);

// LastCodeInt
    uint64_t LastCodeInt(int a, int b);
    pair<int, int>  LastCode( int a, int b);



    stNode * createNode(  stNode * u, int d );
    void createLeaf(int i, stNode * u, int d );
    void ComputeSuffixLink( stNode * u );


    // Visualization
    void int2psInsert(int a, int b);
    void exportSuffixTreeToDot(const std::string& filename,bool suf);
    void generateDot(stNode* node, std::ofstream& dotFile, bool suf);
    void generateCount(stNode* node, std::ofstream& dotFile, bool suf);



    // DFS
    void MaxTauDFS(int tau);
    void FindNodes();

    std::vector<stNode*> MaxTauNodes;
    int explicit_k = 0;
    int n; // the length of w
    uint64_t terminate_label;
    int sigma;
    void deleteTreeIteratively();
    ~OPST();
};







#endif
