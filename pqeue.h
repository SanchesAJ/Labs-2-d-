#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct hufftreeNode {
	unsigned char value;
	size_t priority;
	struct hufftreeNode *left;
	struct hufftreeNode *right;
} hufftreeNode;


typedef struct pqueue Pqueue;

void push(Pqueue **head, unsigned char d, int p, hufftreeNode *left, hufftreeNode *right);

Pqueue *newPqueue(unsigned char d, int p, hufftreeNode *left, hufftreeNode *right);
 hufftreeNode *newHuffNode(unsigned char d, int p, hufftreeNode *left, hufftreeNode *right);
hufftreeNode *pop(Pqueue **head);

int isLast(Pqueue **head);

