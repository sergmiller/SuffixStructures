//
//  ukkonen.cpp
//  Ukkonen
//
//  Created by Сергей Миллер on 22.11.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#include "ukkonen.hpp"
#include <cassert>
#include <iostream>
#include <memory.h>

SuffixTree::Node* SuffixTree::Node::getNext(char c) {
    return next[c - 'a' + 1];
}

void SuffixTree::Node::setNext(char c, Node* node) {
    next[c - 'a' + 1] = node;
}

SuffixTree::~SuffixTree() {
    for(size_t i = 0;i < tree.size(); ++i) {
        delete tree[i];
    }
}

long long SuffixTree::calcSum() {
    long long sum = 0;
    for(size_t i = 2;i < tree.size(); ++i) {
        sum += tree[i]->getLength();
        //std::cout << i << " " << tree[i]->getLength() << std::endl;
//        if(tree[i]->isLeaf) {
//            --sum;
//        }
    }
    return sum;
}

std::string SuffixTree::UkkonenBuilder::getEdge(std::string& s, size_t start, size_t len) {
    std::string str;
    for(size_t i = 0;i < len; ++i) {
        str += s[start + i];
    }
    return str;
}

size_t SuffixTree::Node::getLength() {
//    if(isLeaf) {
//        std::cout << index << ": " << lastPos << " " << startPos << std::endl;
//    }
    return (isLeaf ? (*lastPos - startPos) : length);
}

void SuffixTree::UkkonenBuilder::outTree(Node* node) {
    std::cout << node->index << ": ";
    for(char c = 'a'; c <= 'z'; ++c) {
        if(node->getNext(c)) {
            std::string st = getEdge(str, node->getNext(c)->startPos, node->getNext(c)->getLength());
            std::cout << node->index << ": " <<  st << " " << node->getNext(c)->index << " ";
        }
    }
    
    std::cout << std::endl;
    
    for(char c = 'a'; c <= 'z'; ++c) {
        if(node->getNext(c))
            this->outTree(node->getNext(c));
    }
}

SuffixTree::UkkonenBuilder::UkkonenBuilder(std::string& s, SuffixTree& suffTree):
str(s), suffTree(suffTree) {
    suffTree.dummy = new Node(0, 0, nullptr, 0);
    suffTree.tree.push_back(suffTree.dummy);
    suffTree.dummy->parent = suffTree.dummy;
    suffTree.root = new Node(0, 1, suffTree.dummy, 1);
    
    for(char c = 'a'; c <= 'z'; ++c) {
        suffTree.dummy->setNext(c, suffTree.root);
    }
    //suffTree.dummy->next[1] = suffTree.root;
    
    suffTree.root->link = suffTree.dummy;
    //suffTree.root->next.clear();
    suffTree.tree.push_back(suffTree.root);
    currentOffset = 1;
    currentPoint = suffTree.root;
    prevCutPoint = suffTree.root;
    suffTree.lastPos = 0;
}

SuffixTree::Node::Node(size_t startPos, size_t length, Node* parent, size_t index):
startPos(startPos), length(length), parent(parent), index(index){
    isLeaf = false;
    memset(next, 0, 27 * sizeof(Node*));
}

SuffixTree::Node::Node(size_t* lastPos, Node* parent, size_t index):
startPos(*lastPos - 1), lastPos(lastPos), parent(parent), index(index) {
    isLeaf = true;
    memset(next, 0, 27 * sizeof(Node*));
}

void SuffixTree::buildTree(std::string& s) {
    UkkonenBuilder builder(s, *this);
    for(size_t i = 0;i < s.size(); ++i) {
        builder.addIntoTree(s[i]);
        std::cout << "phase " << i << "\n";
        builder.outTree(root);
        std::cout << std::endl;
    }
}

void SuffixTree::UkkonenBuilder::addIntoTree(char c) {
    lastChar = c;
    //1 Step: increase leafs from last phase
    ++suffTree.lastPos;
    
    //2 Step: calc new nodes in tree
    addNewLeafs();
}

void SuffixTree::UkkonenBuilder::addNewLeafs() {
    
    emptyPhase = false;
    
    while(true) {
        normolizeOffsetLength();
        continueTree();
        
        if(prevCutPoint != suffTree.root && currentCutPoint) {
            prevCutPoint->link = currentCutPoint;
            prevCutPoint = suffTree.root;
        }
        
        if(emptyPhase) {
            break;
        }
        
        prevCutPoint = currentCutPoint;
        currentCutPoint = nullptr;
        currentPoint = currentPoint->parent->link;
        if(currentOffset) {
            currentPoint = currentPoint->getNext(str[firstPos]);
        } else {
            currentOffset = currentPoint->length;
        }
    }
}

void SuffixTree::UkkonenBuilder::normolizeOffsetLength() {
    while(currentOffset > currentPoint->getLength()) {
        currentOffset -= currentPoint->getLength();
        firstPos += currentPoint->getLength();
        currentPoint = currentPoint->getNext(str[firstPos]);
    }
}

void SuffixTree::UkkonenBuilder::continueTree() {
    if(currentOffset == currentPoint->getLength()) {
        if(!currentPoint->getNext(lastChar)) {
            addBranchFromVert();
            currentOffset = 0;
        } else {
            currentPoint = currentPoint->getNext(lastChar);
            emptyPhase = true;
            currentOffset = 1;
        }
        currentCutPoint = currentPoint->parent;
    } else {
        if(str[currentPoint->startPos + currentOffset] == lastChar) {
            ++currentOffset;
            emptyPhase  = true;
            return;
        } else {
            addBranchBetweenVert();
        }
    }
}

void SuffixTree::UkkonenBuilder::addBranchBetweenVert() {
    Node* cuttingVert = new Node(currentPoint->startPos, currentOffset, currentPoint->parent, suffTree.tree.size());
    suffTree.tree.push_back(cuttingVert);
    
    currentPoint->startPos += currentOffset;
    currentPoint->length -= currentOffset;
    currentPoint->parent = cuttingVert;
    Node* newLeaf = new Node(&suffTree.lastPos, cuttingVert, suffTree.tree.size());
    suffTree.tree.push_back(newLeaf);
    
    cuttingVert->parent->setNext(str[cuttingVert->startPos], cuttingVert);
    cuttingVert->setNext(str[currentPoint->startPos], currentPoint);
    cuttingVert->setNext(str[newLeaf->startPos], newLeaf);
    
    currentCutPoint = cuttingVert;
    currentPoint = cuttingVert;
    firstPos = cuttingVert->startPos;
}

void SuffixTree::UkkonenBuilder::addBranchFromVert() {
    Node* newLeaf = new Node(&suffTree.lastPos, currentPoint, suffTree.tree.size());
    suffTree.tree.push_back(newLeaf);
    
//    if(currentPoint->next.empty()) {
//        currentPoint->length = currentOffset;
//    }
    
    currentPoint->setNext(lastChar, newLeaf);
    
    currentPoint = newLeaf;
    firstPos = currentPoint->startPos;
}