//
// Created by skyinu on 2020/1/8.
//
#ifndef JVMTIWRAPPER_ANDROID_NODE_H
#define JVMTIWRAPPER_ANDROID_NODE_H

#endif //JVMTIWRAPPER_ANDROID_NODE_H

typedef struct Node {
    short msgType;
    char *msg1;
    char *msg2;
    char *msg3;
    struct Node *prev;
    struct Node *next;
} Node;

void init();

Node *newNode();

void addNode(Node *node);

Node *getNode();

void freeNode(Node *node);