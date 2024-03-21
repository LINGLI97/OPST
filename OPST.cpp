//
// Created by ling on 12/03/24.
//
#include <iostream>
#include <string>
#include <stack>
#include <cstring>
#include <unordered_set>

#include "OPST.h"
//#define VISUALIZATION
//#define CHECK

//#define VERBOSE
using namespace std;
//using namespace sdsl;


int DerivedWTInt::rank_bit( value_type c, int i, size_type offset) const{

    size_type ones_before_i   = m_tree_rank(offset + i + 1) - m_tree_rank(offset);

    if (c){
        return  ones_before_i;
    }
    else{
//        size_type zero_before_i = i + 1 - ones_before_i;

        return i + 1 - ones_before_i;
    }
}


int DerivedWTInt::select_bit(value_type c, int i, const node_type& node) const{

    size_type offset = node.offset;
    size_type node_size = node.size;

    size_type ones_before_o   = m_tree_rank(offset);
    size_type ones_in_node = m_tree_rank(offset + node_size) - ones_before_o;


    if (0ULL == node_size or node_size < i) {
        throw std::logic_error("select("+util::to_string(c)+","+util::to_string(i)+"): "+util::to_string(c)+" does not occur "+ util::to_string(i) +" times in the bit vector of this node");
    }

    if (c){
        // detect error
        if (ones_in_node < i) {
            throw std::logic_error("select("+util::to_string(c)+","+util::to_string(i)+"): "+util::to_string(c)+" does not occur "+ util::to_string(i) +" times in the bit vector of this node");
        }

        return  m_tree_select1(ones_before_o + i) - offset;
    }
    else{
        if (node_size - ones_in_node< i) {
            // detect error
            throw std::logic_error("select("+util::to_string(c)+","+util::to_string(i)+"): "+util::to_string(c)+" does not occur "+ util::to_string(i) +" times in the bit vector of this node");
        }
        return m_tree_select0(offset - ones_before_o + i) - offset ;
    }
}




int OPST::Max(const sdsl::wt_int<>::node_type& u, int a, int b) {

    if (wt.is_leaf(u)){

        return b;
    }else if (wt.rank_bit(1, a, u.offset) == wt.rank_bit(1, b, u.offset)) {
#ifdef VERBOSE
        cout<<"Max: a: "<<(int )wt.rank_bit(0, a, u.offset)<<endl;
        cout<<"Max: b: "<<(int )wt.rank_bit(0, b, u.offset)<<endl;
        cout<<"Max: "<< Max(wt.expand(u)[0], wt.rank_bit(0, a, u.offset) - 1, wt.rank_bit(0, b, u.offset) - 1)<<endl;

#endif

        return wt.select_bit(0, Max( wt.expand(u)[0], wt.rank_bit(0, a, u.offset) - 1, wt.rank_bit(0, b, u.offset) - 1) + 1, u);
    } else {
#ifdef VERBOSE

        cout<<"Max: a: "<<(int )wt.rank_bit(1, a, u.offset)<<endl;
        cout<<"Max: b: "<<(int )wt.rank_bit(1, b, u.offset)<<endl;
        cout<<"Max: "<< Max(wt.expand(u)[1], wt.rank_bit(1, a, u.offset) - 1, wt.rank_bit(1, b, u.offset) - 1)<<endl;

#endif

        return wt.select_bit(1, Max(wt.expand(u)[1], wt.rank_bit(1, a, u.offset) - 1, wt.rank_bit(1, b, u.offset) - 1) + 1, u);
    }
}





int OPST::predecessorWT(const sdsl::wt_int<>::node_type& u, int a, int b) {

    if (a == b) return NA; // predecessor does not exist

    if (!wt.is_leaf(u)) {
        auto B_u = wt.bit_vec(u); // get the bit vector of u

        auto x = B_u[b + 1];

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


        int pos = predecessorWT(wt.expand(u)[x], wt.rank_bit(x, a, u.offset)-1, wt.rank_bit(x, b, u.offset)-1);// For counting 1s.


        if (pos != NA) {
#ifdef VERBOSE
            cout<<"select("<<x<<", "<<pos+1<<") = "<<wt.select_bit(x, pos + 1, u)<<endl;

#endif

            return wt.select_bit(x, pos + 1, u);
        } else if (x == 0 || wt.rank_bit(0, a, u.offset) == wt.rank_bit(0, b, u.offset)) {

            return NA;
        } else {
#ifdef VERBOSE
            cout<<"select("<<0<<", "<<Max(wt, wt.expand(u)[0], wt.rank_bit(0, a, u.offset)-1, wt.rank_bit(0, b, u.offset) - 1) + 1<<") = "<<wt.select_bit(0, Max(wt, wt.expand(u)[0], wt.rank_bit(0, a, u.offset)-1, wt.rank_bit(0, b, u.offset) - 1) + 1, u)<<endl;
#endif

            return wt.select_bit(0, Max( wt.expand(u)[0], wt.rank_bit(0, a, u.offset)-1, wt.rank_bit(0, b, u.offset) - 1) + 1, u);
        }
    } else {
        return b;
    }
}


int OPST::predecessorNV(int a, int b) {
    if (w.empty()) return -1; // Handle empty vector case

    int aimElement = w[b];
    int predecessorIndex = NA; // Default to "not found"
    // Find the rightmost occurrence of the smallest element greater than or equal to the aimelement
    if (b < 0){
        return predecessorIndex;
    }
    if (a < 0){
        for (int j = 0; j < b ; ++j) {
            if (w[j] <= aimElement) {
                if (predecessorIndex == NA || w[j] >= w[predecessorIndex]) {
                    predecessorIndex = j;
                }
            }
        }

    } else{

        for (int j = a; j < b ; ++j) {
            if (w[j] <= aimElement) {
                if (predecessorIndex == NA || w[j] >= w[predecessorIndex]) {
                    predecessorIndex = j;
                }
            }
        }

    }

    return predecessorIndex;
}




int OPST::Min(const sdsl::wt_int<>::node_type& u, int a, int b) {

    if (wt.is_leaf(u)){

        return b;
    }else if (wt.rank_bit(0, a, u.offset) == wt.rank_bit(0, b, u.offset)) {
#ifdef VERBOSE
        cout<<"Min: a: "<<(int )wt.rank_bit(1, a, u.offset)<<endl;
        cout<<"Min: b: "<<(int )wt.rank_bit(1, b, u.offset)<<endl;
        cout<<"Min: "<< Min(wt.expand(u)[1], wt.rank_bit(1, a, u.offset) - 1, wt.rank_bit(1, b, u.offset) - 1)<<endl;

#endif

        return wt.select_bit(1, Min(wt.expand(u)[1], wt.rank_bit(1, a, u.offset) - 1, wt.rank_bit(1, b, u.offset) - 1) + 1, u);
    } else {
#ifdef VERBOSE

        cout<<"Min: a: "<<(int )wt.rank_bit(0, a, u.offset)<<endl;
        cout<<"Min: b: "<<(int )wt.rank_bit(0, b, u.offset)<<endl;
        cout<<"Min: "<< Min(wt.expand(u)[0], wt.rank_bit(0, a, u.offset) - 1, wt.rank_bit(0, b, u.offset) - 1)<<endl;

#endif


        return wt.select_bit(0, Min(wt.expand(u)[0], wt.rank_bit(0, a, u.offset) - 1, wt.rank_bit(0, b, u.offset) - 1) + 1, u);
    }
}



// Successor func: pos = NA <=> successor does not exist
int OPST::successorWT(const sdsl::wt_int<>::node_type& u, int a, int b) {

    if (a == b) return NA; // successor does not exist

    if (!wt.is_leaf(u)) {
        auto B_u = wt.bit_vec(u); // get the bit vector of u

        auto x = B_u[b + 1];

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


        int pos = successorWT(wt.expand(u)[x], wt.rank_bit(x, a, u.offset)-1, wt.rank_bit(x, b, u.offset)-1);// For counting 1s.


        if (pos != NA) {
#ifdef VERBOSE
            cout<<"select("<<x<<", "<<pos+1<<") = "<<wt.select_bit(x, pos + 1, u)<<endl;

#endif

            return wt.select_bit(x, pos + 1, u);
        } else if (x == 1 || wt.rank_bit(1, a, u.offset) == wt.rank_bit(1, b, u.offset)) {

            return NA;
        } else {
#ifdef VERBOSE
            cout<<"select("<<1<<", "<<Min(wt.expand(u)[1], wt.rank_bit(1, a, u.offset)-1, wt.rank_bit(1, b, u.offset) - 1) + 1<<") = "<<wt.select_bit(1, Min(wt, wt.expand(u)[1], wt.rank_bit(1, a, u.offset)-1, wt.rank_bit(1, b, u.offset) - 1) + 1, u)<<endl;
#endif

            return wt.select_bit(1, Min(wt.expand(u)[1], wt.rank_bit(1, a, u.offset)-1, wt.rank_bit(1, b, u.offset) - 1) + 1, u);
        }
    } else {
        return b;
    }
}


int OPST::successorNV(int a, int b) {
    if (w.empty()) return -1; // Handle empty vector case

    int aimElement = w[b];
    int successorIndex = NA; // Default to "not found"


    if (b < 0){
        return successorIndex;
    }


    if(a < 0){

        for (int j = 0; j < b ; ++j) {
            if (w[j] >= aimElement) {
                if (successorIndex == NA || w[j] <= w[successorIndex]) {
                    successorIndex = j;
                }
            }
        }

    } else{

        // Find the rightmost occurrence of the smallest element greater than or equal to the aimelement
        for (int j = a; j < b; ++j) {
            if (w[j] >= aimElement) {
                if (successorIndex == NA || w[j] <= w[successorIndex]) {
                    successorIndex = j;
                }
            }
        }
    }

    return successorIndex;
}




pair<int, int> OPST:: LastCode(int a , int b){

#ifdef CHECK
    assert(make_pair(predecessorNV(a, b), successorNV(a,b)) == make_pair(predecessorWT(wt.root(), a-1, b-1), successorWT(wt.root(), a-1, b-1)));
#endif
// (double) n < rangeThreshold * log(sigma) need to be changed
    if ( b -a < rangeThreshold){
        int predecessor_local = predecessorNV(a, b);
        int successor_local =  successorNV(a,b);
        if (predecessor_local != NA){
            predecessor_local = predecessor_local - a;
        }

        if (successor_local != NA){
            successor_local = successor_local - a;
        }

        return make_pair(predecessor_local,successor_local);


    } else{

        int predecessor_local = predecessorWT(wt.root(), a-1, b-1);
        int successor_local = successorWT(wt.root(), a-1, b-1);
        if (predecessor_local != NA){
            predecessor_local = predecessor_local - a;
        }
        if (successor_local != NA){
            successor_local = successor_local - a;
        }
        return make_pair(predecessor_local,successor_local);

    }
}



uint64_t OPST::LastCodeInt(int a, int b) {
    // call LastCode
    // b = n, returns $ (n+1)(n+2) +1
    if (b != n){
#ifdef CHECK
        assert(((uint64_t)this->LastCode(a, b).first + 1)*((uint64_t)n + 1) + (uint64_t)this->LastCode(a, b).second +1 < terminate_label);
#endif

        return ((uint64_t)this->LastCode(a, b).first + 1)*((uint64_t)n + 1) + (uint64_t)this->LastCode(a, b).second +1;


    } else{

#ifdef CHECK
        assert(((uint64_t)n + 1)*((uint64_t)n + 2) + 1 == terminate_label);
#endif
        return ((uint64_t)n + 1)*((uint64_t)n + 2) + 1;


    }

}



void OPST::generateDot(stNode* node, std::ofstream& dotFile, bool suf) {
    if (!node) return;

    std::vector<stNode*> children = node->allChild();
    if (children.empty()) {

            dotFile << "\"" << node << "\" [shape=ellipse, style=filled, fillcolor=green, label=\" Start: " << node->getStart() << "\"];\n";

    }
    if (!children.empty()){

        int numChildren = children.size();

        for (int i = 0; i < numChildren; ++i) {

            if (int2ps[children[i]->getLabel()].first == -1){
                if (int2ps[children[i]->getLabel()].second == -1){
                    dotFile << "\"" << node << "\" -> \"" << children[i] << "\" [label=\"" << "("<<"⊥"<<" , "<<"⊥"<<")" << "\"];\n";
                } else{
                    assert(int2ps[children[i]->getLabel()].second != -2);
                    dotFile << "\"" << node << "\" -> \"" << children[i] << "\" [label=\"" << "("<<"⊥"<<" , "<<int2ps[children[i]->getLabel()].second<<")" << "\"];\n";

                }

            } else if (int2ps[children[i]->getLabel()].first == -2){
                assert(int2ps[children[i]->getLabel()].second == -2);
                dotFile << "\"" << node << "\" -> \"" << children[i] << "\" [label=\"" << "$" << "\"];\n";


            } else{

                if (int2ps[children[i]->getLabel()].second == -1){
                    dotFile << "\"" << node << "\" -> \"" << children[i] << "\" [label=\"" << "("<<int2ps[children[i]->getLabel()].first<<" , "<<"⊥"<<")" << "\"];\n";
                } else {
                    dotFile << "\"" << node << "\" -> \"" << children[i] << "\" [label=\"" << "("
                            << int2ps[children[i]->getLabel()].first << " , " << int2ps[children[i]->getLabel()].second
                            << ")" << "\"];\n";
                }
            }

            generateDot(children[i], dotFile, suf);


        }
    }
    if (suf){
        if (node->getSLink()!=NULL){
            dotFile << "\"" << node << "\" -> \"" << node->getSLink() << "\" [label=\"" << "" << "\",color=\"red\"];\n";
        }

    }



}

void OPST::exportSuffixTreeToDot(stNode * node, const std::string& filename,bool suf) {
    std::ofstream dotFile(filename);
    if (!dotFile.is_open()) {
        std::cerr << "Unable to open file for writing: " << filename << std::endl;
        return;
    }

    //DOT setting
    dotFile << "digraph SuffixTree {\n";
    dotFile << "node [shape=ellipse, style=filled, fillcolor=lightgrey];\n";
    dotFile << "edge [color=black];\n";
    dotFile << "graph [nodesep=0.5, ranksep=1, splines=polyline];\n";




    generateDot(node, dotFile,suf);


    dotFile << "}\n";
    dotFile.close();
}




void OPST::int2psInsert(int a, int b){

    uint64_t i;
    pair<int, int> ps;
    if (b==n){
         i = LastCodeInt(a, b);
         ps = make_pair(-2,-2);
    } else{

        i = LastCodeInt(a, b);
        ps = LastCode(a, b);
    }

    auto it = this->int2ps.find( i );
    if ( it == this->int2ps.end() )
    {
        this->int2ps.insert({i,ps});
    }
    else
    {
        assert(it->second.first == ps.first);
        assert(it->second.second == ps.second);

    }

}




OPST::OPST(int_vector<> & w, int rangeThreshold)
{
    this->w = w;
    this->n = w.size();
    // construct wavelet tree for LastCodeInt
    construct_im(wt,w);
    sigma = wt.sigma;
    //set the value of terminate_label
    terminate_label = (uint64_t)(this->n + 1) * (uint64_t)(this->n + 2) + 1;
    cout<<"Sigma of input = "<<sigma<<endl;
    cout<<"Wavelet tree size = "<<wt.size()<<endl;
    cout<<"Suffix tree size = "<< this->n <<endl;

    cout<<"terminate_label $ = " << terminate_label<<endl;


    this->rangeThreshold = rangeThreshold;

//    this->unordered_denseFlag = parser.get<bool>("unordered_dense");


#ifdef CHECK

    assert((int)wt.size() == this->n);
#endif
    this->root = new stNode(terminate_label);
    this->root->setSLink( this->root );

    stNode * u = this->root;
    int d = 0;


    for ( int i = 0; i < this->n; i++ )
    {
        while (((i+d) < this -> n) && (d == u->getDepth()) && (u->getChild(LastCodeInt(i, i + d)) != NULL)) {

#ifdef VISUALIZATION
            int2psInsert( i, i + d);
#endif

            u = u->getChild(LastCodeInt(i, i + d));
            d = d + 1;
            while ((i + d < this->n) && (u->getStart() +d < this->n) && (d < u->getDepth()) && (LastCode(u->getStart(), u->getStart() + d) == LastCode(i , i + d))) {
                d = d + 1;
            }
        }

        if ( d < u->getDepth() )
        {
            u= createNode (u, d );
        }
        createLeaf( i, u, d);

        if ( u->getSLink() == NULL )
        {
            ComputeSuffixLink( u );
        }
        u = u->getSLink();

//        d = u->getDepth(); // same as d = max( d-1, 0 );
        d = max( d-1, 0 );
    }


#ifdef VISUALIZATION
    exportSuffixTreeToDot(root,"pic_nosufL", false);
    exportSuffixTreeToDot(root,"pic_sufL", true);
#endif

    cout<<"The number of middle_implicit_max appeared: "<<middle_implicit_max<<endl;

}

void OPST::deleteTree(stNode * node) {
    std::stack<stNode*> toDelete;
    toDelete.push(node);

    while (!toDelete.empty()) {
        stNode* current = toDelete.top();
        toDelete.pop();

        // 将所有子节点添加到栈中
        for (auto child : current->allChild()) {
            if (child) {
                toDelete.push(child);
            }
        }

        // 删除当前节点
        delete current;
    }
}




OPST::~OPST() {

//    deleteTree(this->root);

}




void OPST::ComputeSuffixLink( stNode * u )
{
    int d = u->getDepth();
    stNode * u_copy = u;
    int middle_implicit_tmp = 0;

    //count the num of implicit node between two branching nodes
    while (!u_copy->getParent()->getSLink()){
        u_copy = u_copy->getParent();
        middle_implicit_tmp += 1;
    }
    if (middle_implicit_tmp > middle_implicit_max){
        middle_implicit_max = middle_implicit_tmp;
    }

    stNode * v = u_copy->getParent()->getSLink();


    while ( v->getDepth() < d-1 )
    {

#ifdef VISUALIZATION
        int2psInsert(u->getStart()+1, u->getStart() + v->getDepth() + 1);
#endif
        // go down
        v = v->getChild( LastCodeInt(u->getStart()+1, u->getStart() + v->getDepth() + 1) );

    }
    if ( v->getDepth() > d-1 )
    {
        //create a new node as a suffix link node
        v = createNode( v, d-1);
    }
    u->setSLink (v);
    u-> getSLink()->setDepth(d -1);
}

stNode * OPST::createNode(stNode * u, int d )
{

    // add a new node v between p and u
    int i = u->getStart();
    stNode * p = u->getParent();


#ifdef VISUALIZATION
    int2psInsert(i, i+p->getDepth());
    int2psInsert(i, i+d);
#endif
    // get the label of u and v
    uint64_t v_label = LastCodeInt(i, i+p->getDepth());
    uint64_t u_label = LastCodeInt(i, i+d);

    stNode * v = new stNode( i, d, v_label);
    v->addChild( u, u_label);
    p->addChild( v, v_label);

    return v;
}


void OPST::createLeaf( int i, stNode * u, int d )
{
    // create a leaf node connected to u
    // (n+2)(n+1)+1 denotes $

    uint64_t leaf_label = LastCodeInt( i, i+d);
    stNode *leaf  = new stNode(i, this->n-i +1, leaf_label) ;
    assert(i == leaf->getStart());
    u->addChild( leaf, leaf_label);

#ifdef VISUALIZATION
    int2psInsert( leaf->getStart(), leaf->getStart()+d);
    int2psInsert(i, i+d);
#endif
}

