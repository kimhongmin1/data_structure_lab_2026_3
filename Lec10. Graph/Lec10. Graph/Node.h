#pragma once
#include <cstdio>
#define MAX_VTXS 20

class Node {
protected:
    int id;
    Node* link;
public:
    Node(int i, Node* l = NULL) : id(i), link(l) {}
    int getId() { return id; }
    Node* getLink() { return link; }
    void setLink(Node* l) { link = l; }
};
