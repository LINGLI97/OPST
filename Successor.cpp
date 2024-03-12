#include <sdsl/wavelet_trees.hpp>
#include <iostream>
#include <random> // For std::mt19937 and std::uniform_int_distribution
#include <cstdlib> // For std::exit

using namespace sdsl;
using namespace std;

//#define VERBOSE
#define NA -1
//Derived wavelet tree
class DerivedWTInt : public wt_int<> {
public:

    using wt_int<>::wt_int;

    int rank_bit( value_type c, int i,size_type offset) const;

    int select_bit( value_type c, int i, const node_type& node) const;


};



int DerivedWTInt::rank_bit( value_type c, int i, size_type offset) const{

    size_type ones_before_i   = m_tree_rank(offset + i + 1) - m_tree_rank(offset);

    if (c){
        return  ones_before_i;
    }
    else{
        size_type zero_before_i = i + 1 - ones_before_i;

        return zero_before_i;
    }
}


int DerivedWTInt::select_bit(value_type c, int i, const node_type& node) const{
//        for (int k =0;k<this->m_tree_rank.size(); k++){
//
//            std::cout << "The content of m_tree_rank " << this->m_tree_rank(k)<< std::endl;
//        }

    size_type offset = node.offset;
    size_type node_size = node.size;

    size_type ones_before_o   = m_tree_rank(offset);
    size_type ones_in_node = m_tree_rank(offset + node_size) - ones_before_o;


    if (0ULL == node_size or node_size < i) {
        throw std::logic_error("select("+util::to_string(i)+","+util::to_string(c)+"): "+util::to_string(c)+" does not occur "+ util::to_string(i) +" times in the bit vector of this node");
    }

    if (c){
        // detect error
        if (ones_in_node < i) {
            throw std::logic_error("select("+util::to_string(i)+","+util::to_string(c)+"): "+util::to_string(c)+" does not occur "+ util::to_string(i) +" times in the bit vector of this node");
        }

        return  m_tree_select1(ones_before_o + i) - offset;
    }
    else{
        if (node_size - ones_in_node< i) {
            // detect error
            throw std::logic_error("select("+util::to_string(i)+","+util::to_string(c)+"): "+util::to_string(c)+" does not occur "+ util::to_string(i) +" times in the bit vector of this node");
        }
        return m_tree_select0(offset - ones_before_o + i) - offset ;
    }
}





int Min(const DerivedWTInt& wt, const sdsl::wt_int<>::node_type& u, int a, int b) {

    if (wt.is_leaf(u)){
#ifdef VERBOSE
        //        cout<<"First"<<endl;
//        cout<<"Min: "<<b<< endl;
#endif

        return b;
    }else if (wt.rank_bit(0, a, u.offset) == wt.rank_bit(0, b, u.offset)) {
#ifdef VERBOSE
        cout<<"Min: a: "<<(int )wt.rank_bit(1, a, u.offset)<<endl;
        cout<<"Min: b: "<<(int )wt.rank_bit(1, b, u.offset)<<endl;
        cout<<"Min: "<< Min(wt, wt.expand(u)[1], wt.rank_bit(1, a, u.offset) - 1, wt.rank_bit(1, b, u.offset) - 1)<<endl;

#endif

        return wt.select_bit(1, Min(wt, wt.expand(u)[1], wt.rank_bit(1, a, u.offset) - 1, wt.rank_bit(1, b, u.offset) - 1) + 1, u);
    } else {
#ifdef VERBOSE

        cout<<"Min: a: "<<(int )wt.rank_bit(0, a, u.offset)<<endl;
        cout<<"Min: b: "<<(int )wt.rank_bit(0, b, u.offset)<<endl;
        cout<<"Min: "<< Min(wt, wt.expand(u)[0], wt.rank_bit(0, a, u.offset) - 1, wt.rank_bit(0, b, u.offset) - 1)<<endl;

#endif


        return wt.select_bit(0, Min(wt, wt.expand(u)[0], wt.rank_bit(0, a, u.offset) - 1, wt.rank_bit(0, b, u.offset) - 1) + 1, u);
    }
}




// Successor func: pos = NA <=> successor does not exist
int successorWT(const DerivedWTInt& wt, const sdsl::wt_int<>::node_type& u, int a, int b) {
    if (a == b) return NA; // successor does not exist

    if (!wt.is_leaf(u)) {
        auto B_u = wt.bit_vec(u); // get the bit vector of u



        auto x = B_u[b + 1];
//        const sdsl::wt_int<>::node_type expandU_x = wt.expand(u)[x];

// need to convert uint64_t to int (a b can be negative)
#ifdef VERBOSE
        cout<<"Bit vector of node u: ";
        for(int i= 0 ;i< B_u.size(); i++) {
            std::cout << B_u[i];
        }

        std::cout << std::endl;

        int a_value = wt.rank_bit(x, a, u.offset)-1;
        int b_value = wt.rank_bit(x, b, u.offset)-1;

        cout<<"a: "<<a_value<<endl;
        cout<<"b: "<<b_value<<endl;
        cout<<"expand("<<x<<")"<< endl;
#endif


        int pos = successorWT(wt, wt.expand(u)[x], wt.rank_bit(x, a, u.offset)-1, wt.rank_bit(x, b, u.offset)-1);// For counting 1s.


        if (pos != NA) {
#ifdef VERBOSE
            cout<<"select("<<x<<", "<<pos+1<<") = "<<wt.select_bit(x, pos + 1, u)<<endl;

#endif

            return wt.select_bit(x, pos + 1, u);
        } else if (x == 1 || wt.rank_bit(1, a, u.offset) == wt.rank_bit(1, b, u.offset)) {

            return NA;
        } else {
#ifdef VERBOSE
            cout<<"select("<<1<<", "<<Min(wt, wt.expand(u)[1], wt.rank_bit(1, a, u.offset)-1, wt.rank_bit(1, b, u.offset) - 1) + 1<<") = "<<wt.select_bit(1, Min(wt, wt.expand(u)[1], wt.rank_bit(1, a, u.offset)-1, wt.rank_bit(1, b, u.offset) - 1) + 1, u)<<endl;
#endif

            return wt.select_bit(1, Min(wt, wt.expand(u)[1], wt.rank_bit(1, a, u.offset)-1, wt.rank_bit(1, b, u.offset) - 1) + 1, u);
        }
    } else {
        return b;
    }
}


int successorNV(int_vector<>& w, int a, int b) {
    if (w.empty()) return -1; // Handle empty vector case

    int aimElement = w[b + 1];
    int successorIndex = NA; // Default to "not found"

    // Find the rightmost occurrence of the smallest element greater than or equal to the aimelement
    for (int j = a; j < b+1 ; ++j) {
        if (w[j] >= aimElement) {
            if (successorIndex == NA || w[j] <= w[successorIndex]) {
                successorIndex = j;
            }
        }
    }


    return successorIndex;
}


/*
int successor(int_vector<> iv, sdsl::wt_int<>::size_type &size, sdsl::wt_int<>::size_type &sigma, int a , int b){

    if ((double) size < 5 * log(sigma) ){
//        successorNV();

    } else{

//        successorWT();
    }
}

*/

void run_test(int numsample){
    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator

    for (int i = 0; i < numsample; i++) {
        cout<<"Now is sample No."<<i<<endl;

        // Generate a random vector size between 1 and 1000
        std::uniform_int_distribution<> distr_size(1, 50);
        int vec_size = distr_size(eng);

        // Generate random vector values between 0 and 100
        std::uniform_int_distribution<> distr_val(0, 100);

        int_vector<> iv(vec_size);
        for (auto val : iv) {
            val = distr_val(eng);
        }


        // Construct DerivedWT
        DerivedWTInt wt;
        construct_im(wt, iv);

        // Pick random a and b
        if (vec_size > 1) { // Ensure there's a range to search within
            std::uniform_int_distribution<> distr_range(0, vec_size - 2); // -2 to leave space for b
            int a = distr_range(eng);
            int b = distr_range(eng);
            if (a > b) std::swap(a, b); // Ensure a <= b
            int u = b+1;
            int successorIndexWT = successorWT(wt, wt.root(), a-1, b);
            int successorIndexNV = successorNV(iv, a, b);

            // does not exist
            if (successorIndexWT == NA && successorIndexNV == NA){
                continue;
            }
            // Check for discrepancies

            if (successorIndexWT != successorIndexNV ||
                (iv[successorIndexWT] != iv[successorIndexNV])) {
                std::cerr << "Discrepancy found: WT index = " << successorIndexWT
                          << ", NV index = " << successorIndexNV << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }
    }

    std::cout << "All " << numsample << " samples verified successfully." << std::endl;
}



int main(int argc, char* argv[])
{

    run_test(1000000);
//    int_vector<> iv = {1, 0, 1, 3, 3, 1, 2, 2};

    // 通过 input


    /*
    int_vector<> iv = {1, 1, 4, 1, 1, 2, 3, 2};


    // construct DerivedWT
    DerivedWTInt wt;
    construct_im(wt, iv);


     */
/*
    auto bitvector = wt.bit_vec(wt.root());

    for(int i= 0 ;i< bitvector.size(); i++) {
        std::cout << bitvector[i];
    }

    std::cout << std::endl;

    std::array<sdsl::wt_int<>::node_type, 2> children = wt.expand(wt.root());
// 左右子节点
    auto left_child = children[0];
    auto right_child = children[1];

    auto xx = wt.sym(wt.expand(right_child)[0]);
    auto yy = wt.is_leaf(wt.expand(right_child)[0]);

    auto left_bitvector = wt.bit_vec(left_child);
    auto right_bitvector = wt.bit_vec(right_child);


    std::cout << "Left child bitvector: ";
    for(auto bit : left_bitvector) {
        std::cout << bit;
    }
    std::cout << std::endl;


    std::cout << "right child bitvector: ";


    for(auto bit : right_bitvector) {
        std::cout << bit;
    }
    std::cout << std::endl;










*/

/*
    cout << "wt.size()="<< wt.size() << endl;
    cout << "wt.sigma ="<< wt.sigma << endl;


    int u = 6;
    int a = 2;
    int b = u-1;
    // bug
    cout<<"The index of the successor got by Wavelet tree = "<<successorWT(wt,wt.root(),a-1, b)<<endl;

//    cout<<"Successor got by Wavelet tree = "<<iv[successorWT(wt,wt.root(),a-1, b)]<<endl;

    int successorIndex = successorNV(iv,a,b);

    std::cout << "The index of the successor got by Wavelet tree = " << successorIndex << std::endl;

*/


}