//
// Created by ling on 12/03/24.
//

#ifndef NODE_H
#define NODE_H


#include <string>
#include <unordered_map>
#include <vector>

using namespace std;


class stNode
{
private:
    int start;
    int depth;
    uint64_t label; // label is the label of edge which links its parent and itself
    unordered_map<uint64_t, stNode*> child;
    vector<int> Occ;
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

    stNode * getChild( uint64_t l );
    stNode * getParent();
    stNode * getSLink();
    void setSLink( stNode * slinkNode );
    void addChild( stNode * childNode,  uint64_t l );
    void addLeaf( int i, uint64_t terminate_label );
    void removeChild( uint64_t l );
    int printOcc();
    void setParent ( stNode * parentNode );
    int numChild();
    void printChild();
    stNode** allChild();
    void deleteAllChildren();

    ~stNode();
};

#endif


