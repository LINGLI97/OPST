//
// Created by ling on 28/03/24.
//

#ifndef OPST_GIT_BASELINE_H
#define OPST_GIT_BASELINE_H
#include <iostream>
#include <vector>

#include <utility> // for std::pair
#include <string> // for std::string
#include <sstream> // for std::stringstream
#include <functional>

#include <sdsl/wavelet_trees.hpp>

#include "karp_rabin_hashing.hpp"
#include "utils.hpp"

#include <cmath>

using namespace std;
using namespace sdsl;





std::string generatePatternString(const std::vector<std::pair<int, int>>& sortedArray, int x);


int cubicMethod(std::vector<int> &w, int &tau);

int quadraticMethod(std::vector<int> &w, int &tau);


















#endif //OPST_GIT_BASELINE_H
