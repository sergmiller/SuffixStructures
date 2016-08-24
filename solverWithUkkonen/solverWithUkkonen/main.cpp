//
//  main.cpp
//  Ukkonen
//
//  Created by Сергей Миллер on 22.11.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//



#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory.h>

using std::cin;
using std::cout;
using std::string;


#define MAX_SIZE 2002
//#define size_t int
#define nullptr NULL


#ifndef ukkonen_hpp
#define ukkonen_hpp


class SuffixTree {
public:
    void buildTree(std::string& s);
    bool findString(std::string& s);
    long long calcSum();
    //~SuffixTree();
private:
    class Node {
    public:
        Node (size_t startPos, size_t length, int parent, size_t index);//make cutting vert
        Node (size_t* lastPos, int parent, size_t index);//make leaf
        size_t startPos;
        int parent;
        int link;
        size_t index;
        bool isLeaf;
        size_t getLength();
        size_t length;
        size_t* lastPos;//pointer to last char in str
        //Node* getNext(char c);
        //void setNext(char c, Node* node);
        int next[27];
    };
    
    class UkkonenBuilder {
    public:
        UkkonenBuilder(std::string& s, SuffixTree& suffTree);
        bool emptyPhase;
        size_t lastChar;
        size_t firstPos;
        void addIntoTree(char c);
        void addNewLeafs();
        void normolizeOffsetLength();
        void continueTree();
        void addBranchFromVert();
        void addBranchBetweenVert();
        size_t currentOffset;
        int currentPoint;
        int prevLinkPoint;
        int currentLinkPoint;
        
        std::string& str;
        SuffixTree& suffTree;
        //void outTree(Node* node);
        std::string getEdge(std::string& s, size_t start, size_t len);
        std::vector <Node>& tree;
    };
    
    size_t lastPos;
    std::vector <Node> tree;
public:
    size_t root;
    size_t dummy;
};

#endif /* ukkonen_hpp */

//inline SuffixTree::Node* SuffixTree::Node::getNext(char c) {
//    return next[c - 'a' + 1];
//}
//
//inline void SuffixTree::Node::setNext(char c, Node* node) {
//    next[c - 'a' + 1] = node;
//}

//SuffixTree::~SuffixTree() {
//    Node* cur = &tree[0];
//    Node* last = &tree[tree.size() - 1] + 1;
//    for(;cur != last; ++cur) {
//        delete cur;
//    }
//}

//long long SuffixTree::calcSum() {
//    long long sum = 0;
//    //Node** cur = &tree[2];
//    //Node** last = &tree[tree.size() - 1];
//    for(size_t i = 2; i < tree.size();++i) {
//        sum += tree[i]->getLength();
//        //std::cout << i << " " << tree[i]->getLength() << std::endl;
//        //        if(tree[i]->isLeaf) {
//        //            --sum;
//        //        }
//    }
//    return sum;
//}

long long SuffixTree::calcSum() {
    long long sum = 0;
    Node* cur = &tree[2];
    Node* last = &tree[tree.size() - 1] + 1;
    for(; cur != last ;++cur) {
        sum += (*cur).getLength();
    }
    return sum;
}

//std::string SuffixTree::UkkonenBuilder::getEdge(std::string& s, size_t start, size_t len) {
//    std::string str;
//    for(size_t i = 0;i < len; ++i) {
//        str += s[start + i];
//    }
//    return str;
//}

inline size_t SuffixTree::Node::getLength() {
    return (isLeaf ? (*lastPos - startPos) : length);
}

//void SuffixTree::UkkonenBuilder::outTree(Node* node) {
//    std::cout << node->index << ": ";
//    for(size_t c = 0; c < 27; ++c) {
//        if(node->next[c]) {
//            std::string st = getEdge(str, node->next[c]->startPos, node->next[c]->getLength());
//            std::cout << node->index << ": " <<  st << " " << node->next[c]->index << " ";
//        }
//    }
//    
//    std::cout << std::endl;
//    
//    for(size_t c = 0; c < 27; ++c) {
//        if(node->next[c])
//            this->outTree(node->next[c]);
//    }
//}

SuffixTree::UkkonenBuilder::UkkonenBuilder(std::string& s, SuffixTree& suffTree):
str(s), suffTree(suffTree), tree(suffTree.tree) {
    tree.reserve(MAX_SIZE);
    
    Node dummy = Node(0, 0, nullptr, 0);
    Node root =  Node(0, 1, 0, 1);
    tree.push_back(dummy);
    tree.push_back(root);
    
    for(size_t c = 0; c < 27; ++c) {
        tree[0].next[c] = 1;
    }
    
    tree[1].link = 0;
    
    currentOffset = 1;
    currentPoint = 1;
    prevLinkPoint = 1;
    suffTree.lastPos = 0;
}

SuffixTree::Node::Node(size_t startPos, size_t length, int parent, size_t index):
startPos(startPos), length(length), parent(parent), index(index), isLeaf(false) {
    memset(next, -1, 27 * sizeof(int));
}

//SuffixTree::Node::Node(size_t _startPos, size_t _length, Node* _parent, size_t _index){
//    isLeaf = false;
//    startPos = _startPos;
//    length = _length;
//    parent = _parent;
//    index = _index;
//    memset(next, 0, 27 * sizeof(Node*));
//}

SuffixTree::Node::Node(size_t* lastPos, int parent, size_t index):
startPos(*lastPos - 1), lastPos(lastPos), parent(parent), index(index), isLeaf(true) {
    //memset(next, 0, 27 * sizeof(Node*));
}

//SuffixTree::Node::Node(size_t* _lastPos, Node* _parent, size_t _index) {
//    isLeaf = true;
//    startPos = *_lastPos - 1;
//    lastPos = _lastPos;
//    parent = _parent;
//    index = _index;
//    //memset(next, 0, 27 * sizeof(Node*));
//}

void SuffixTree::buildTree(std::string& s) {
    UkkonenBuilder builder(s, *this);
    for(size_t i = 0;i < s.size(); ++i) {
        builder.addIntoTree(s[i]);
        //        std::cout << "phase " << i << "\n";
        //        builder.outTree(root);
        //        std::cout << std::endl;
    }
}

inline void SuffixTree::UkkonenBuilder::addIntoTree(char c) {
    lastChar = c;
    //1 Step: increase leafs from last phase
    ++suffTree.lastPos;
    
    //2 Step: calc new nodes in tree
    addNewLeafs();
}

inline void SuffixTree::UkkonenBuilder::addNewLeafs() {
    
    emptyPhase = false;
    
    while(true) {
        normolizeOffsetLength();
        continueTree();
        
        if(prevLinkPoint != 1 && currentLinkPoint) {
            tree[prevLinkPoint].link = currentLinkPoint;
            prevLinkPoint = 1;
        }
        
        if(emptyPhase) {
            break;
        }
        
        prevLinkPoint = currentLinkPoint;
        currentLinkPoint = nullptr;
        currentPoint = tree[tree[currentPoint].parent].link;
        if(currentOffset) {
            currentPoint = tree[currentPoint].next[str[firstPos]];
        } else {
            currentOffset = tree[currentPoint].length;
        }
    }
}

inline void SuffixTree::UkkonenBuilder::normolizeOffsetLength() {
    while(currentOffset > tree[currentPoint].getLength()) {
        currentOffset -= tree[currentPoint].getLength();
        firstPos += tree[currentPoint].getLength();
        currentPoint = tree[currentPoint].next[str[firstPos]];
    }
}

void SuffixTree::UkkonenBuilder::continueTree() {
    if(currentOffset == tree[currentPoint].getLength()) {
        if(tree[currentPoint].next[lastChar] == -1) {
            addBranchFromVert();
            currentOffset = 0;
        } else {
            currentPoint = tree[currentPoint].next[lastChar];
            emptyPhase = true;
            currentOffset = 1;
        }
        currentLinkPoint = tree[currentPoint].parent;
    } else {
        if(str[tree[currentPoint].startPos + currentOffset] == lastChar) {
            ++currentOffset;
            emptyPhase  = true;
            return;
        } else {
            addBranchBetweenVert();
        }
    }
}

void SuffixTree::UkkonenBuilder::addBranchBetweenVert() {
    Node cuttingVert = Node(tree[currentPoint].startPos, currentOffset, tree[currentPoint].parent, tree.size());
    tree.push_back(cuttingVert);
    int cutVertNumber = (int)tree.size() - 1;
    Node& curPointer = tree[currentPoint];
    
    curPointer.startPos += currentOffset;
    curPointer.length -= currentOffset;
    curPointer.parent = cutVertNumber;
    Node newLeaf = Node(&suffTree.lastPos, cutVertNumber, tree.size());
    tree.push_back(newLeaf);
    
    Node& cutPointer = tree[cutVertNumber];
    tree[cutPointer.parent].next[str[tree[cutVertNumber].startPos]] = cutVertNumber;
    cutPointer.next[str[tree[currentPoint].startPos]] = currentPoint;
    cutPointer.next[str[newLeaf.startPos]] = (int)newLeaf.index;
    
    currentLinkPoint = cutVertNumber;
    currentPoint = cutVertNumber;
    firstPos = cutPointer.startPos;
}

inline void SuffixTree::UkkonenBuilder::addBranchFromVert() {
    Node newLeaf = Node(&suffTree.lastPos, currentPoint, tree.size());
    tree.push_back(newLeaf);
    int newLeafIndex = (int)tree.size() - 1;
    
    tree[currentPoint].next[lastChar] = newLeafIndex;
    
    currentPoint = newLeafIndex;
    firstPos = tree[currentPoint].startPos;
}

void test();//test for http://acm.timus.ru/problem.aspx?space=1&num=1706

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int main(int argc, const char * argv[]) {
    std::ios::sync_with_stdio(false);
    //srand(time(NULL));
    freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
//    string s;
//    cin >> s;
//    
//    for(size_t i = 0;i < s.size(); ++i) {
//        s[i] -= ('a' - 1);
//    }
//    
//    SuffixTree st;
//    st.buildTree(s);
//    
//    cout << st.calcSum();
//    cout << 1000 << std::endl;
//    for(size_t i = 0;i < 4000; ++i) {
//        cout << (char)('a' + rand()%2);
//    }
    //double t = clock();
    test();
   // cout << "\n" << (clock() -t)/CLOCKS_PER_SEC;
    return 0;
}

inline long long countNotEmptySubstr(size_t len,
                           size_t offset,
                           string& s) {
    string curSubstr = s.substr(offset, len);
    
    SuffixTree st;
    st.buildTree(curSubstr);
    
    return st.calcSum();
}

void test() {
    size_t k;
    string s;
    cin >> k >> s;
    
    for(size_t i = 0;i < s.size(); ++i) {
        s[i] -= ('a' - 1);
    }
    
    s += s;
    
    std::vector <long long> ans;
    ans.resize(s.size()/2);
    
    for(size_t i = 0;i < s.size()/2; ++i) {
        ans[i] = countNotEmptySubstr(k, i, s);
    }
    
    for(size_t i = 0;i < s.size()/2; ++i) {
        cout << ans[i] << " ";
    }
}
