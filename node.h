//
// Created by ling on 12/03/24.
//

#ifndef NODE_H
#define NODE_H


#include <string>
#include <unordered_map>
#include <vector>
#include "unordered_dense.h"
#include <iostream>
using namespace std;
//#define UNORDERED_DENSE

class stNode
{
private:
    int start;
    int depth;
    uint64_t label; // label is the label of edge which links its parent and itself


    stNode * parent;
    stNode * slink;
public:
    stNode();
    explicit stNode(uint64_t terminate_label);
    stNode( int i, int d, uint64_t l );
    int getStart();
    int getDepth();
    void setDepth( int d);
    int getLabel();

//DFS
    bool isCandidate;
    bool leftMax;
    bool visited;
    int leafCount;



#ifdef UNORDERED_DENSE
    ankerl::unordered_dense::map<uint64_t, stNode*> child;

#else
    unordered_map<uint64_t, stNode*> child;
#endif


    stNode * getChild( uint64_t l );
    stNode * getParent();
    stNode * getSLink();
    void setSLink( stNode * slinkNode );
    void addChild( stNode * childNode,  uint64_t l );

    void setParent ( stNode * parentNode );
    int numChild();
    std::vector<stNode*> allChild();


    ~stNode();
};

#endif


