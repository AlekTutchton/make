/*
 *This file contains the methods tho build the data structure
 *in a dependency graph.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mymake.h"
//#include "/home/cs352/spring17/Assignments/proj09/cs352.h"

node *header = NULL;

/*
 * addNode() is a medthod that adds a node at the bottom of
 *structure.
 */
node *addNode(char * newNode) {

	node *curr = header;
	
	while(curr != NULL) {
		curr = curr->next;
	}

	//create the memory required for the newNode
	curr = malloc(sizeof(node));
	curr->str = strdup(newNode);
	curr->marked = 0;
	curr->firstCmd = NULL;		
	curr->edges = NULL;		
	curr->next = NULL;		
	return curr;
}

/*
 * addCommand() adds a new command to the end of the given node.
 */
cmd *addCommand(node *target, char *command) {
	cmd *curr;
	curr = target->firstCmd;
	while(curr != NULL) {
		curr = curr->next;
	}
	
	curr= malloc(sizeof(cmd));
	curr->str = strdup(command);
	curr->next = NULL;
	
	return curr;
}

/*
 *addEdge() adds a new edge to the given node.
 */
edge *addEdge(node *target, char *newEdge) {
	edge *curr;
    node *currNode = header;

	while(currNode != NULL) {
		if(strcmp(currNode->str, newEdge) == 0)
			break;
		currNode = currNode->next;
	}
	
	//The node was not found, so add it.
	if(currNode == NULL) {
		currNode = addNode(newEdge);
	}

	curr = currNode->edges;
	while(curr != NULL) {
		curr = curr->next;
	}
	
	
	curr = malloc(sizeof(edge));
	curr->dependent = currNode;
	curr->next = NULL;
	
	return curr;
}

/*
 *helpPOT() traverse the graph and calls the postorder traversal function. 
 */
void helpPOT(node *newNode) {
    edge *dependent;
    cmd *cmd;
    
    newNode->marked = 1;
    dependent = newNode->edges;
    while(dependent != NULL) { 
        if (!dependent->dependent->marked) {
          helpPOT(dependent->dependent);
	}
	dependent = dependent->next;	
    }
    printf("%s\n", newNode->str); 

    cmd = newNode->firstCmd;
    while(cmd != NULL) {
        printf("  %s\n", cmd->str);
	cmd = cmd->next;
	}
}

/*
 *POT() preforms a post order traversal of the data structure. 
 */
void POT(char *t) {
    node *curr;
    
    for (curr = header; curr != NULL; curr = curr->next)
        if (!strcmp(curr->str, t))
            break;
    if (curr == NULL) {
        fprintf(stderr,"Target %s not found\n", t);
        return;
    }
    
   helpPOT(curr);
    return;
}

/*
 *findNode() finds the node that corresponds to the given target.
 */
node *findNode(char *target) {
	node *curr;
	curr = header;
	while(curr != NULL) {
		if(strcmp(target, curr->str) == 0)
			return curr;
	}

	return curr;
}

/*
 *freeAll() frees all the allocated memory of the data structure.
 */
void freeAll() {
	node *nodeBk, *nodeFrt;
	edge *edgeBk, *edgeFrt;
	cmd *cmdBk, *cmdFrt;

	nodeBk = header;
	while(nodeBk != NULL) {
		nodeFrt = nodeBk->next;
		cmdBk = nodeBk->firstCmd;
		while(cmdBk != NULL) {
			cmdFrt = cmdBk->next;
			free(cmdBk->str);
			free(cmdBk);
            cmdBk = cmdFrt;
		}
		edgeBk = nodeBk->edges;
		while(edgeBk != NULL) {
			edgeFrt= edgeBk->next;
			free(edgeBk);
			edgeBk = edgeFrt;
		}
		free(nodeBk->str);
		free(nodeBk);
		nodeBk = nodeFrt;
	}
	return;		
}

