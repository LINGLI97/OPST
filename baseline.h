#ifndef OPST_BASELINE_H
#define OPST_BASELINE_H

#include <iostream>
#include <vector>

#include <utility> // for std::pair
#include <string> // for std::string
#include <sstream> // for std::stringstream
#include <functional>

#include <sdsl/wavelet_trees.hpp>
#include <stdlib.h>
//#include <stdio.h>
#include "karp_rabin_hashing.hpp"
#include "utils.hpp"

#include <cmath>

using namespace std;
using namespace sdsl;


std::string generatePatternString(const std::vector<std::pair<int, int>>& sortedArray, int x);



int quadraticMethodMax(std::vector<int> &w, int &tau);



int quadraticMethodClosed(std::vector<int> &w, int &tau);



#endif
