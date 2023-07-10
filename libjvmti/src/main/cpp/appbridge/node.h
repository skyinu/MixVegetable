//
// Created by skyinu on 2020/1/8.
//
#include <jni.h>

#ifndef JVMTIWRAPPER_ANDROID_NODE_H
#define JVMTIWRAPPER_ANDROID_NODE_H

#endif //JVMTIWRAPPER_ANDROID_NODE_H

typedef struct Node {
    short msgType;
    char *msg1;
    char *msg2;
    char *msg3;
    jobject obj;
    struct Node *prev;
    struct Node *next;
} Node;

void init();

Node *newNode(short msgType);

void addNode(Node *node);

Node *getNode();

void freeNode(JNIEnv *jni_env, Node *node);