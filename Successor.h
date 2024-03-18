//
// Created by ling on 13/03/24.
//

#ifndef SUCCESSOR_H
#define SUCCESSOR_H
#include <sdsl/wavelet_trees.hpp>
#include <iostream>
#include <random> // For std::mt19937 and std::uniform_int_distribution
#include <cstdlib> // For std::exit
using namespace sdsl;
#define RANGE_THRESHOLD 10
#define TIMES_THRESHOLD 5
class DerivedWTInt : public wt_int<> {
public:

    using wt_int<>::wt_int;

    int rank_bit( value_type c, int i,size_type offset) const;

    int select_bit( value_type c, int i, const node_type& node) const;


};



int Min(const DerivedWTInt& wt, const sdsl::wt_int<>::node_type& u, int a, int b);



int successorWT(const DerivedWTInt& wt, const sdsl::wt_int<>::node_type& u, int a, int b);


int successorNV(sdsl::int_vector<>& w, int a, int b);

int successor(int_vector<> w, sdsl::wt_int<>::size_type &size, sdsl::wt_int<>::size_type &sigma, int a , int b);

void run_test(int numsample);

#endif
