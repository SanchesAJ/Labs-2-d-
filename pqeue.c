

// Очередь с приоритетом .с
#include <stdio.h>
#include <stdlib.h>
#include "pqeue.h"


struct pqueue {

	hufftreeNode *value;

	// Lower values indicate higher priority
	int priority;

	struct pqueue *next;

};

int isLast(struct pqueue **head) {
	return (*head)->next == NULL;
}

 hufftreeNode *newHuffNode(unsigned char d, int p, hufftreeNode *left, hufftreeNode *right) {
	hufftreeNode *newNode = (hufftreeNode *)
		malloc(sizeof(hufftreeNode));
	newNode->value = d;
	newNode->priority = p;
	newNode->left = left;
	newNode->right = right;

	return newNode;
}




// Создание новой очереди.
struct pqueue *newPqueue(unsigned char d, int p, hufftreeNode *left, hufftreeNode *right) {

	hufftreeNode *node = newHuffNode(d, p, left, right);
	struct pqueue *temp = (struct pqueue *) malloc(sizeof(struct pqueue));
	temp->value = node;
	temp->priority = p;
	temp->next = NULL;

	return temp;
}







// Removes the element with the
// highest priority form the list
hufftreeNode *pop(struct pqueue **head) {

	hufftreeNode *res = (*head)->value;

	struct pqueue *temp = *head;
	(*head) = (*head)->next;
	free(temp);

	return res;
}

// Function to push according to priority
void push(struct pqueue **head, unsigned char d, int p, hufftreeNode *left, hufftreeNode *right) {
	struct pqueue *start = (*head);

	if (start == NULL) {
		*head = newPqueue(d, p, left, right);
		return;
	}

	// Create new Pqueue
	struct pqueue *temp = newPqueue(d, p, left, right);

	// Special Case: The head of list has lesser
	// priority than new node. So insert new
	// node before head node and change head node.
	if ((*head)->priority > p) {

		// Insert New Pqueue before head
		temp->next = *head;
		(*head) = temp;

	}
	else {

		// Traverse the list and find a
		// position to insert new node
		while (start->next != NULL &&
			start->next->priority < p) {
			start = start->next;
		}

		// Either at the ends of the list
		// or at required position
		temp->next = start->next;
		start->next = temp;
	}
}

