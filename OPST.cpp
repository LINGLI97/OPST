//
// Created by ling on 12/03/24.
//
#include <iostream>
#include <string>
#include <stack>
#include <cstring>
#include <unordered_set>

#include "OPST.h"

//#define INT2PS
//#define CHECK

using namespace std;
//using namespace sdsl;
//constructor
uint64_t pair2int(pair<int, int> pair_ab, int n){

    return ((uint64_t)pair_ab.first + 1)*((uint64_t)n + 1) + (uint64_t)pair_ab.second +1;

}



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
#ifdef VERBOSE
        //        cout<<"First"<<endl;
//        cout<<"Max: "<<b<< endl;
#endif

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
#ifdef VERBOSE
        //        cout<<"First"<<endl;
//        cout<<"Min: "<<b<< endl;
#endif

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












//LastCode wrapper
/*
pair<int, int> OPST:: LastCode(int a , int b){

    assert(make_pair(predecessorNV(a, b), successorNV(a,b)) == make_pair(predecessorWT(wt.root(), a-1, b-1), successorWT(wt.root(), a-1, b-1)));
    if ((double) b -a < RANGE_THRESHOLD  || (double) n < SiZE_THRESHOLD * log(sigma) ){
        int predecessor_local = predecessorNV(a, b);
        int successor_local =  successorNV(a,b);
        if (predecessor_local != NA){
            predecessor_local = predecessor_local - a;
        }

        if (successor_local != NA){
            successor_local = successor_local - a;
        }

        return make_pair(predecessor_local,successor_local);
//        return make_pair(predecessorWT(wt.root(), a, b), successorWT(wt.root(), a, b));


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

//        return make_pair(predecessorWT(wt.root(), a-1, b-1), successorWT(wt.root(), a-1, b-1));
    }
}




uint64_t OPST::LastCodeInt(int a, int b) {

    if (b == n){
        assert(a== n -1);

        return pair2int(make_pair(n,n), n) +1;
        // return the integer (n+1)(n+2) +1 indicating $
    } else{

        pair<int, int> LastCode;
        LastCode = this->LastCode(a, b);
        return pair2int(LastCode, n);
    }

}


 */
pair<int, int> OPST:: LastCode(int cur_i, int a , int b){

#ifdef CHECK
    assert(make_pair(predecessorNV(a, b), successorNV(a,b)) == make_pair(predecessorWT(wt.root(), a-1, b-1), successorWT(wt.root(), a-1, b-1)));

#endif
    if ((double) b -a < RANGE_THRESHOLD  || (double) n < SiZE_THRESHOLD * log(sigma) ){
        int predecessor_local = predecessorNV(a, b);
        int successor_local =  successorNV(a,b);
        if (predecessor_local != NA){
            predecessor_local = predecessor_local - cur_i;
        }

        if (successor_local != NA){
            successor_local = successor_local - cur_i;
        }

        return make_pair(predecessor_local,successor_local);
//        return make_pair(predecessorWT(wt.root(), a, b), successorWT(wt.root(), a, b));


    } else{

        int predecessor_local = predecessorWT(wt.root(), a-1, b-1);
        int successor_local = successorWT(wt.root(), a-1, b-1);
        if (predecessor_local != NA){
            predecessor_local = predecessor_local - cur_i;
        }
        if (successor_local != NA){
            successor_local = successor_local - cur_i;
        }
        return make_pair(predecessor_local,successor_local);

//        return make_pair(predecessorWT(wt.root(), a-1, b-1), successorWT(wt.root(), a-1, b-1));
    }
}



uint64_t OPST::LastCodeInt(int cur_i, int a, int b) {

    if (b == n){
//        if (a!= n -1)
//        {
//            cout<<"b = "<<b<<endl;
////        }
//        cout<<"a = "<<a<<endl;
//        cout<<"b = "<<b<<endl;
#ifdef CHECK
        assert(pair2int(make_pair(n,n), n) +1 == terminate_label);

#endif
        return pair2int(make_pair(n,n), n) +1;
        // return the integer (n+1)(n+2) +1 indicating $
    } else{
#ifdef CHECK
        if (pair2int(LastCode, n) > terminate_label){
            cout<<LastCode.first<<","<<LastCode.second<<endl;
            cout<<pair2int(LastCode, n)<<endl;
            cout<<terminate_label<<endl;
        }
        assert(pair2int(LastCode, n) < terminate_label);
#endif
        pair<int, int> LastCode;
        LastCode = this->LastCode(cur_i,a, b);


        return pair2int(LastCode, n);
    }

}













void OPST::generateDot(stNode* node, std::ofstream& dotFile, bool suf) {
    if (!node) return;

    // 对于每个子节点，输出一条边的描述
    stNode** children = node->allChild();
    if (children != NULL){
        int numChildren = node->numChild(); // 获取子节点的数量
        for (int i = 0; i < numChildren; ++i) {
            dotFile << "\"" << node << "\" -> \"" << children[i] << "\" [label=\"" << int2ps[children[i]->getLabel()].first<<" , "<<int2ps[children[i]->getLabel()].second << "\"];\n";

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

    // 开始DOT文件
    dotFile << "digraph SuffixTree {\n";
    dotFile << "node [shape=ellipse, style=filled, fillcolor=lightgrey];\n";
    dotFile << "edge [color=black];\n";
    dotFile << "graph [nodesep=0.5, ranksep=1, splines=polyline];\n";



    // 生成并写入节点和边的描述
    generateDot(node, dotFile,suf);

    // 结束DOT文件
    dotFile << "}\n";
    dotFile.close();
}




void OPST::int2psInsert(int cur_i, int a, int b){


    uint64_t i;
    pair<int, int> ps;
    if (b==n){
         i = LastCodeInt(cur_i,a, b);
         ps = make_pair(-2,-2);
    } else{

        i = LastCodeInt(cur_i,a, b);
        ps = LastCode(cur_i,a, b);
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



OPST::OPST(  int_vector<> & w )
{
    this->w = w;
    this->n = w.size();
    // construct wavelet tree for LastCodeInt
    construct_im(wt,w);
    sigma = wt.sigma;
    //set the value of terminate_label
    terminate_label = (uint64_t)(this->n + 1) * (uint64_t)(this->n + 2) + 1;
    cout<<"Sigma of input = "<<sigma<<endl;
    cout<<"Input size wt.size() = "<<wt.size()<<endl;
    cout<<"Input size this->n = "<<wt.size()<<endl;

    cout<<"terminate_label $ = " << terminate_label<<endl;
//    cout<< this->n<<endl;

    assert((int)wt.size() == this->n);

    this->root = new stNode(terminate_label);
    this->root->setSLink( this->root );

    stNode * u = this->root;
//    stNode * v = NULL;
//    stNode * w = NULL;
    int d = 0;
//    stNode * v = NULL;

    for ( int i = 0; i < this->n; i++ )
    {


//        cout<<"Outside The depth of u: "<<u->getDepth()<<endl;
//        cout<<"Outside LastCode: "<<LastCode(i,i,i + d).first<<","<<LastCode(i, i, i + d).second<<endl;
//        cout<<"Outside LastCodeint: "<<LastCodeInt(i,i ,i + d)<<endl;
//        cout<<"(i, i + d): ("<<i<< ","<<i+d<<")"<<endl;
//        cout<<"(P,S): "<<LastCode(i, i, i + d).first<<" , "<<LastCode(i, i, i + d).second<<endl;
//
//
//        cout<<"LastCodeInt: "<<LastCodeInt(i, i, i + d)<<endl;
        while (((i+d) < this -> n) && (d == u->getDepth()) && (u->getChild(LastCodeInt(i, i, i + d)) != NULL)) {
#ifdef INT2PS
            int2psInsert(i, i, i + d);
#endif
//            cout<<"i = " << i<<" ; Inside1 The depth of u: "<<u->getDepth()<<endl;
//            cout<<"Inside1 LastCode: "<<LastCodeInt(i,i,i + d)<<endl;

            u = u->getChild(LastCodeInt(i,i, i + d));
//			u->addOcc( i );
            d = d + 1;
//            cout<<"d "<<d<<endl;
//            cout<<u->getStart()<<endl;
//            cout<<u->getStart() + d<<endl;
//            cout<<i<<endl;
//            cout<<i+d<<endl;
//            cout<<<<endl;
//            assert(i+d< this->n);
//            assert(i+u->getStart()< this->n);

//            cout<<"Inside2 d = "<<d <<";"<<"The depth of u: "<<u->getDepth()<<endl;

//            if ((i + d < this->n) && (i + u->getStart() < this->n)){
//
//                cout<<"Inside2 LastCode: "<<LastCode(i,u->getStart(), u->getStart() + d).first<<" "<<LastCode(i, u->getStart(), u->getStart() + d).second<<endl;
//                cout<<"Inside2 LastCode: "<<LastCode(i , i , i + d).first<<" "<<LastCode(i, i , i + d).second<<endl;
//
//            }
//            cout<<"(P,S)1: "<<LastCode(u->getStart() , u->getStart(), u->getStart() + d).first<<" , "<<LastCode(u->getStart() , u->getStart(), u->getStart() + d).second<<endl;
//            cout<<"(P,S)1: "<<LastCode(i , u->getStart(), u->getStart() + d).first<<" , "<<LastCode(i, u->getStart(), u->getStart() + d).second<<endl;
//
//            cout<<"(P,S)2: "<<LastCode(i ,i , i + d).first<<" , "<<LastCode(i ,i , i + d).second<<endl;

            while ((i + d < this->n) && (u->getStart() +d < this->n) && (d < u->getDepth()) && (LastCode(u->getStart() , u->getStart(), u->getStart() + d) == LastCode(i ,i , i + d))) {
//                cout<<"i = " << i<<" ; Inside3 d = "<<d <<";"<<"The depth of u: "<<u->getDepth()<<endl;
//                cout<<"Inside3 LastCode: "<<LastCodeInt(i ,i , i + d)<<" "<<LastCodeInt(i , u->getStart(), u->getStart() + d)<<endl;
                d = d + 1;

            }



        }


        if ( d < u->getDepth() )
        {

            u= createNode (u, d );
//            u = v;
        }

        createLeaf( i, u, d);

//        cout<<" here "<<endl;

        if ( u->getSLink() == NULL )
        {
            ComputeSuffixLink( u );
        }
        u = u->getSLink();
//        d = u->getSLink()->getDepth();
        d = u->getDepth();
//        d = max( d-1, 0 );
    }
#ifdef INT2PS
    exportSuffixTreeToDot(root,"pic_nosufL", false);
    exportSuffixTreeToDot(root,"pic_sufL", true);
#endif
    cout<<"The number of middle_implicit_max appeared: "<<middle_implicit_max<<endl;

}


//member funct
//void OPST::bfs_leaves()
//{
//    stNode * u = NULL;
//    stNode * v = NULL;
//    stack<stNode*> s;
//    u = this->root;
//    s.push( u );
//    while( !s.empty() )
//    {
//        v = s.top();
//        s.pop();
//        cout << v << endl;
//        if ( v->numChild() == 0 )
//        {
//            pair<int, stNode*> leaf ( v->getStart(), v );
//            this->leaves.insert( leaf );
//        }
//        else
//        {
//            stNode ** children = v->allChild();
//            for ( int i = 0; i < v->numChild(); i++ )
//                s.push( children[i] );
//            delete[] children;
//        }
//    }
//}

//OPST::~OPST() {
////	delete this->root;
//
//    delete (this->root);
//}


void deleteSubtree(stNode* node) {
    if (node != nullptr) {
        // 递归地删除所有子节点
        node->deleteAllChildren();
        // 删除当前节点
        delete node;
    }
}




void OPST::deleteTree(stNode* node) {
    if (node != nullptr) {
        // 假设stNode类有方法来获取其所有子节点
        // 遍历每个子节点并递归地调用deleteTree
        stNode** children = node->allChild();

        if (children != NULL) { // 确保u有子节点
            int numChildren = node->numChild(); // 获取子节点的数量

            // 遍历并删除每个子节点
            for (int i = 0; i < numChildren; ++i) {
                delete children[i]; // 删除子节点
            }
            delete[] children;
        }

        delete node;
    }
}
OPST::~OPST() {
    // 释放root节点以及通过root可达的所有节点的内存
//    deleteTree(this->root);
}




void OPST::ComputeSuffixLink( stNode * u )
{
    int d = u->getDepth();
    stNode * u_copy = u;
    int middle_implicit_tmp = 0;

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
//        cout<<"v->getDepth(): "<<v->getDepth()<<endl;
//        cout<<"d-1 : "<< d- 1<<endl;
//
//        cout<<LastCode(cur_i, u->getStart(), u->getStart() + v->getDepth() + 1).first<<" "<< LastCode(cur_i, u->getStart(), u->getStart() + v->getDepth() + 1).second<<endl;
//
//        cout<<LastCodeInt(cur_i, u->getStart(), u->getStart() + v->getDepth() + 1)<<endl;
//        cout<<LastCodeInt(u->getStart(),u->getStart(), u->getStart() + v->getDepth() + 1)<<endl;
//
//        cout<<LastCodeInt(u->getStart(), u->getStart() + v->getDepth() + 1)<<endl;

//        cout<<u->getStart()+1<<","<<u->getStart() + v->getDepth() + 1<<endl;
//        cout<<LastCodeInt(u->getStart()+1, u->getStart()+1, u->getStart() + v->getDepth() + 1)<<endl;

#ifdef INT2PS
        int2psInsert(u->getStart()+1, u->getStart()+1, u->getStart() + v->getDepth() + 1);
#endif
        v = v->getChild( LastCodeInt(u->getStart()+1, u->getStart()+1, u->getStart() + v->getDepth() + 1) );
//        v = v->getChild( LastCodeLnt(v->getDepth(), u->getStart() + v->getDepth() + 1) );

    }
    if ( v->getDepth() > d-1 )
    {
        v = createNode( v, d-1);
//        v->setSLink ( NULL );
    }
    u->setSLink (v);
    u-> getSLink()->setDepth(d -1);
//    delete u_copy;
}

stNode * OPST::createNode(stNode * u, int d )
{
    int i = u->getStart();
    stNode * p = u->getParent();
//    cout<<LastCodeInt(cur_i, i, i+p->getDepth())<<endl;
//    cout<<i<<" , "<<i+d<<endl;
//    cout<<LastCodeInt(i, i, i+d)<<endl;
#ifdef INT2PS

    int2psInsert(i,i, i+p->getDepth());
    int2psInsert(i,i, i+d);
#endif
    uint64_t v_label = LastCodeInt(i,i, i+p->getDepth());
    uint64_t u_label = LastCodeInt(i, i, i+d);

    stNode * v = new stNode( i, d, v_label);
    v->addChild( u, u_label);
//    u->setParent( v );


//    cout<<i<<" , "<<i+p->getDepth()<<endl;
//    cout<<LastCodeInt(i, i, i+p->getDepth())<<endl;
    p->addChild( v, v_label);
//	v->setParent( p );
    return v;
}


stNode * OPST::createNodeIm(stNode * u, int d )
{
    int i = u->getStart();
    stNode * p = u->getParent();
//    cout<<LastCodeInt(cur_i, i, i+p->getDepth())<<endl;
    cout<<i<<" , "<<i+d<<endl;
    cout<<LastCodeInt(i, i, i+d)<<endl;

    stNode * v = new stNode( i, d, LastCodeInt(i,i, i+p->getDepth()));
    v->addChild( u, LastCodeInt(i, i, i+d));


    cout<<i<<" , "<<i+p->getDepth()<<endl;
    cout<<LastCodeInt(i, i, i+p->getDepth())<<endl;
    p->addChild( v, LastCodeInt(i, i, i+ p->getDepth()) );
    u->setParent( p );

//	v->setParent( p );
    return v;
}


void OPST::createLeaf( int i, stNode * u, int d )
{
    // (n+2)(n+1)+1 denotes $
//    cout<<"Leaf: "<<LastCodeInt(i, i, i+d)<<endl;
//    cout<<i<<" "<<i+d<<endl;
    uint64_t leaf_label = LastCodeInt(i, i, i+d);
    stNode *leaf  = new stNode(i, this->n-i +1, leaf_label) ;
    assert(i == leaf->getStart());
//    cout<<"Create a leaf, depth: "<<this->n-i +1<< " label: "<< LastCodeInt(i, i, i+d) <<endl;
//    cout<<"leaf range: "<<leaf->getStart()<<" "<<leaf->getStart() +d <<endl;
//    cout<<"(P , S): "<< LastCode(i , leaf->getStart(), leaf->getStart()+d).first<< " , "<< LastCode(i , leaf->getStart(), leaf->getStart()+d).second<<endl;
//    cout<<"leaf LastCodeInt: "<<LastCodeInt(i , leaf->getStart(), leaf->getStart()+d)<<endl;
//    cout<<leaf->getStart()<<" , "<<leaf->getStart() +d<<endl;
//    cout<<LastCodeInt(leaf->getStart() , leaf->getStart(), leaf->getStart()+d)<<endl;

    u->addChild( leaf, leaf_label);
#ifdef INT2PS
    int2psInsert(leaf->getStart() , leaf->getStart(), leaf->getStart()+d);
    int2psInsert(i,i, i+d);
#endif
//	leaf->setParent(u);
}


//stNode * OPST::createLeaf( stNode *u, int i )
//{
//	u->addLeaf( i );
//	return w;
//}
/*
int OPST::forward_search( string x )
{
    //TODO find all occurrences
    int m = x.size();
    int d = 0;
    stNode * u = this->root;
    stNode * v = NULL;
    bool match = true;
    while ( match )
    {
        v = u->getChild( x[d] );
        if ( v == NULL )
        {
            match = false;
            break;
        }
        int i, j;
        for ( i = d, j = 0; i < m && j < v->getDepth() ; i++, j++ )
        {
            if ( x[i] != this->T->at( v->getStart()+j ) )
            {
                match = false;
                break;
            }
        }
        d = v->getDepth();
        u = v;
        if ( d >= m )
        {
            if ( !u->printOcc() )
            {
                return u->getStart()+m-1;
            } else{
                cout<<"Here"<<endl;
            }
        }
    }
    return -1;
}

stNode * OPST::forward_search_node( stNode * s, int l )
{
    int start = s->getStart();
    int position = s->getStart() + s->getDepth();
    stNode * u = s;
    int extent = l - u->getDepth();
    while ( extent > 0 )
    {
        u = u->getChild( this->T->at( position ) );
        extent = l - u->getDepth();
        position = u->getStart() + u->getDepth();
    }
    return u;
}
*/
 //
//void OPST::trimST( int * ME )
//{
//    bfs_leaves();
//    stNode * u = this->root;
//    int node_move_type = 0;
//    for ( int i = 0; i < this->n; i++ )
//    {
//        u = forward_search_node( u, ME[i] );
//        if ( ME[i] < u->getDepth() )
//        {
//            u = createNode_trim( u, ME[i] );
//            u->addLeaf( i );
//            auto it = this->leaves.find( i );
//            //	it->second->getParent()->removeChild( this->T->at(i) );
//
//            delete it->second;
//            this->leaves.erase( it );
//            node_move_type = 1;
//        }
//        else
//        {
//            if ( u->numChild() != 0 )
//            {
//                u->addLeaf( i );
//                node_move_type = 2;
//            }
//            else
//            {
//                stNode * v = createNode_trim( u, ME[i]-1 );
//                u = v;
//                u->addLeaf( i );
//                node_move_type = 1;
//            }
//            auto it = this->leaves.find( i );
//            //	it->second->getParent()->removeChild( this->T->at(i) );
//            delete it->second;
//            this->leaves.erase( it );
//        }
//        if ( u == this->root )
//            u = u->getSLink();
//        else if ( node_move_type == 1 )
//            u = u->getParent()->getSLink();
//        else if ( node_move_type == 2 )
//            u = u->getSLink();
//    }
//}


//void OPST::bfs_leaves()
//{
//    stNode * u = NULL;
//    stNode * v = NULL;
//    stack<stNode*> s;
//    u = this->root;
//    s.push( u );
//    while( !s.empty() )
//    {
//        v = s.top();
//        s.pop();
//        cout << v << endl;
//        if ( v->numChild() == 0 )
//        {
//            pair<int, stNode*> leaf ( v->getStart(), v );
//            this->leaves.insert( leaf );
//        }
//        else
//        {
//            stNode ** children = v->allChild();
//            for ( int i = 0; i < v->numChild(); i++ )
//                s.push( children[i] );
//            delete[] children;
//        }
//    }
//}


void OPST::printLeaves()
{
//    this->leaves.clear();
    cout << "________________" << endl;
//    bfs_leaves();
    for ( auto it = this->leaves.begin(); it != this->leaves.end(); it++ )
    {
        cout << it->first << ":" << it->second << ":\t";
        it->second->printOcc();
        cout << endl;
    }
}

