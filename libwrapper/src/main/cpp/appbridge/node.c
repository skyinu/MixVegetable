//
// Created by skyinu on 2020/1/8.
//

#include "node.h"
#include "pthread.h"
#include "malloc.h"

pthread_mutex_t nodeMutex;
Node *head, *tail;

void init() {
    head = malloc(sizeof(Node));
    tail = malloc(sizeof(Node));
    head->prev = NULL;
    head->next = tail;
    tail->prev = head;
    tail->next = NULL;
    pthread_mutex_init(&nodeMutex, NULL);
}

Node *newNode() {
    Node *node = malloc(sizeof(Node));
    node->msg1 = NULL;
    node->msg2 = NULL;
    node->msg3 = NULL;
    return node;
}

void addNode(Node *node) {
    pthread_mutex_lock(&nodeMutex);
    Node *preTail = tail->prev;
    preTail->next = node;
    node->prev = preTail;
    node->next = tail;
    tail->prev = node;
    pthread_mutex_unlock(&nodeMutex);
}

Node *getNode() {
    if (head->next == tail) {
        return NULL;
    } else {
        pthread_mutex_lock(&nodeMutex);
        Node *current = head->next;
        Node *next = current->next;
        head->next = next;
        next->prev = head;
        pthread_mutex_unlock(&nodeMutex);
        return current;
    }
}

void freeNode(Node *node) {
    if (node->msg1 != NULL) {
        free(node->msg1);
    }
    if (node->msg2 != NULL) {
        free(node->msg2);
    }
    if (node->msg3 != NULL) {
        free(node->msg3);
    }
    free(node);
}