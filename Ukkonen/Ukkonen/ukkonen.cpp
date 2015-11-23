//
//  ukkonen.cpp
//  Ukkonen
//
//  Created by Сергей Миллер on 22.11.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#include "ukkonen.hpp"

size_t SuffixTree::Node::getLength() {
    return (length == 0 ? (*lastPos - startPos + 1) : length);
}

SuffixTree::UkkonenBuilder::UkkonenBuilder(std::string& s, SuffixTree& suffTree):
str(s), suffTree(suffTree) {}


SuffixTree::Node::Node(size_t startPos, size_t length, size_t parent, size_t index):
startPos(startPos), length(length), parent(parent) {}

SuffixTree::Node::Node(size_t& lastPos, size_t parent, size_t index):
startPos(lastPos), lastPos(&lastPos), parent(parent) {}

void SuffixTree::buildTree(std::string& s) {
    UkkonenBuilder builder(s, *this);
    for(size_t i = 0;i < s.size(); ++i) {
        builder.addIntoTree(s[i]);
    }
}

void SuffixTree::UkkonenBuilder::addIntoTree(char c) {
    ++sizeStr;
    lastChar = c;
    //1 Step: increase leafs from last phase
    ++lastPos;
    
    //2 Step: calc new nodes in tree
    addNewLeafs();
}

void SuffixTree::UkkonenBuilder::addNewLeafs() {
    
    emptyPhase = false;
    
    while(!emptyPhase) {
        continueTree();
        nextSuffix = suffTree.tree[(currentSuffix->parent)]->link->next[lastChar];
        
        
        
        prevSuffix = currentSuffix;
        currentSuffix = nextSuffix;
    }
}

void SuffixTree::UkkonenBuilder::continueTree() {
    if(currentSuffix->startPos + currentOffset + 1 < currentSuffix->getLength()) {
        if(str[currentSuffix->startPos + currentOffset + 1] == lastChar) {
            ++currentOffset;
            emptyPhase = true;
            return;
        } else {
            addBranchBetweenVert();
        }
    }else {
        if(currentSuffix->next[lastChar] != nullptr) {
            currentSuffix = currentSuffix->next[lastChar];
        } else {
            addBranchFromVert();
        }
    }
}

void SuffixTree::UkkonenBuilder::addBranchBetweenVert() {
    Node* cuttingVert = new Node(currentSuffix->startPos, currentOffset, currentSuffix->parent, suffTree.tree.size());
    suffTree.tree.push_back(cuttingVert);
    
    currentSuffix->startPos += currentOffset;
    currentSuffix->length -= currentOffset;
    
    Node* newLeaf = new Node(lastPos, cuttingVert->index, suffTree.tree.size());
    suffTree.tree.push_back(newLeaf);

}

void SuffixTree::UkkonenBuilder::addBranchFromVert() {
    Node* newLeaf = new Node(lastPos, currentSuffix->index, suffTree.tree.size());
    suffTree.tree.push_back(newLeaf);
    
    if(currentSuffix->next.empty()) {
        currentSuffix->length = currentOffset;
    }
    
    currentSuffix->next[lastChar] = newLeaf;
}