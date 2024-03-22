//
// Created by ling on 12/03/24.
//
#include <iostream>
#include <string>
#include <unordered_map>
#include "node.h"

using namespace std;


stNode::stNode()
{
    this->start = 0;
    this->depth = 0;
    this->label = 0;
    this->parent = NULL;
    this->slink = NULL;
}



stNode::stNode(uint64_t terminate_label)
{
    this->start = 0;
    this->depth = 0;
    this->label = terminate_label;
    this->parent = NULL;
    this->slink = NULL;
}

stNode::stNode( int i, int d, uint64_t l)
{
    this->start = i;
    this->depth = d;
    this->label = l;
    this->parent = NULL;
    this->slink = NULL;
}

int stNode::getStart()
{
    return this->start;
}

int stNode::getDepth()
{
    return this->depth;
}

void stNode::setDepth( int d){

    this->depth = d;
}


int stNode::getLabel()
{
    return this->label;
}
stNode * stNode::getChild( uint64_t l )
{
#ifdef UNORDERED_DENSE
    ankerl::unordered_dense::map<uint64_t, stNode*>::iterator it;

#else
    unordered_map<uint64_t, stNode*>::iterator it;

#endif

    it = this->child.find(l);
    if ( it == this->child.end() )
        return NULL;
    else
        return it->second;
}

stNode * stNode::getParent()
{
    return this->parent;
}

stNode * stNode::getSLink()
{
    return this->slink;
}

void stNode::setSLink( stNode * slinkNode )
{
    this->slink = slinkNode;
}

void stNode::addChild( stNode * childNode, uint64_t l)
{
    auto it = this->child.find( l );
    if ( it == this->child.end() )
    {
        pair <uint64_t , stNode*> insertChild( l, childNode );


        this->child.insert( insertChild );
    }
    else
    {
        it->second = childNode;
    }
    childNode->label = l;
    childNode->parent = this;
}
/*
void stNode::addLeaf( int i , uint64_t terminate_label)
{
    auto it = this->child.find( terminate_label );
    if ( it == this->child.end() )
    {
        stNode * leaf = new stNode( i, this->depth+1, terminate_label );
        leaf->parent = this;
        leaf->Occ.push_back( i );
        pair<uint64_t , stNode*> insertLeaf( terminate_label, leaf );
        this->child.insert( insertLeaf );
    }
    else
    {
        it->second->Occ.push_back( i );
    }
}

void stNode::removeChild( uint64_t l )
{
    auto it = this->child.find( l );
    if ( it != this->child.end() )
    {
        this->child.erase( it );
    }
}
*/

void stNode::setParent( stNode * parentNode )
{
    this->parent = parentNode;
}
/*
stNode::~stNode()
{
    auto it = this->parent->child.find( this->label );
    this->parent->child.erase( it );
    for ( auto it = this->child.begin(); it != this->child.end(); ++it )
    {
        cout << "delete " << it->second << endl;
        delete it->second;
    }
    this->child.clear();
    if ( this->parent->child.empty() )
    {
        cout << "delete " << this->parent << endl;
        delete this->parent;
    }
}
*/
//int cnt = 0;
stNode::~stNode()
{

/*
// recursion
    // Delete all child nodes
    for (auto it = this->child.begin(); it != this->child.end(); )
    {

        delete it->second; // Delete child node
        it = this->child.erase(it); // Erase from map and move to next

    }

    // Check if this node is not the root (has a parent)
//    cout<<"Delete the node"<<endl;
    if (this->parent != nullptr)
    {
        // If parent's child map is empty after erasing, delete the parent; otherwise deleting this child from its parent's children map
        if (this->parent->child.empty())
        {
            delete this->parent;
        }
    }

*/

}



int stNode::numChild()
{
    if (this->child.empty()){
        return 0;
    }
    return this->child.size();
}

//void stNode::printChild()
//{
//    for ( auto it = this->child.begin(); it != this->child.end(); it++ )
//        cout << it->first << '\t' << it->second << endl;
//}


/*
stNode ** stNode::allChild()
{
    int n = this->child.size();
    if ( n == 0 )
        return NULL;
    else
    {
        stNode ** allChild = new stNode * [n];
        int i = 0;
        auto it = this->child.begin();
        for ( ; it != this->child.end(); ++it, ++i )
        {
            allChild[i] = it->second;
        }
        return allChild;
    }
}
*/
//std::vector<stNode*> stNode::allChildren() {
//    std::vector<stNode*> children;
//    for (auto& pair : this->child) {
//        children.push_back(pair.second);
//    }
//    return children;
//}


std::vector<stNode*> stNode::allChild() {
    std::vector<stNode*> allChildren;
    for (auto& pair : this->child) {
        allChildren.push_back(pair.second);
    }
    return allChildren;
}

/*
stNode ** stNode::allChild()
{






    int n = this->child.size();
    if ( n == 0 )
        return NULL;
    else
    {
        stNode ** allChild = new stNode * [n];
        int i = 0;
        auto it = this->child.begin();
        for ( ; it != this->child.end(); ++it, ++i )
        {
            allChild[i] = it->second;
        }
        return allChild;
    }
}

*/
