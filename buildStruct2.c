/*
 *This file contains the methods tho build the data structure
 *in a dependency graph for mymake2.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mymake2.h"
#include <sys/stat.h>
//#include "/home/cs352/spring17/Assignments/proj09/cs352.h"

node *header = NULL;
struct stat targetDate;
extern int toDate;

/*
 * addNode() is a medthod that adds a node at the bottom of
 *structure.
 */
node *addNode(char * newNode) {

	node *curr = header;
    node **temp = &header;

    while(curr != NULL) {
        temp = &(curr->next);
		curr = curr->next;
	}

	//create the memory required for the newNode
	curr = malloc(sizeof(node));
    if(curr == NULL) {
        fprintf(stderr,"Out of memory\n");
        exit(1);
    }
	curr->str = strdup(newNode);
    stat(curr->str, &curr->time);
	curr->marked = 0;
    curr->target = 0;
	curr->firstCmd = NULL;		
	curr->edges = NULL;		
	curr->next = NULL;
     *temp = curr;
	return curr;
}

/*
 * addCommand() adds a new command to the end of the given node.
 */
cmd *addCommand(node *currNode, char *str) {
    cmd *curr, **temp;
    
    temp = &currNode->firstCmd;
    curr = currNode->firstCmd;
    while(curr != NULL) {
        temp = &(curr->next);
        curr = curr->next;
    }
    curr = malloc(sizeof(cmd));
    if (curr == NULL) {
        fprintf(stderr,"Out of memory\n");
        exit(1);
    }
    *temp = curr;
    curr->next = NULL;
    curr->str = strdup(str);
    return curr;
}

/*
 *addEdge() adds an edge to the given Node.
 */
edge *addEdge(node *currNode, char *newEdge) {
    edge *curr, **temp;
    node *edgeNode;
    
    edgeNode = header;
    while (edgeNode != NULL) {
        if (strcmp(edgeNode->str, newEdge) == 0)
            break;
        edgeNode = edgeNode->next;
    }
    if (edgeNode == NULL) {
        edgeNode = addNode(newEdge);
    }
    
    temp = &currNode->edges;
    curr = currNode->edges;
    
    while(curr != NULL) {
        temp = &curr->next;
        curr = curr->next;
    }
    curr = malloc(sizeof(edge));
    if (curr == NULL) {
        fprintf(stderr,"Out of memory\n");
        exit(1);
    }
    
    *temp = curr;
    curr->dependent = edgeNode;
    curr->next = NULL;
    
    return curr;
}


/*
 *helpPOT() traverse the graph and calls the postorder traversal function. 
 */
void helpPOT(node *newNode) {
    edge *dependent;
    cmd *cmd;
    int retVal;
    
    if(newNode->marked == 1) {
        return;
    }
    newNode->marked = 1;
    
    retVal = stat(newNode->str, &newNode->time);
    if(retVal == -1 && newNode->target != 1) {
        fprintf(stderr, "%s file does not exsist\n", newNode->str);
        freeAll();
        exit(1);
    }
    
    for(dependent = newNode->edges; dependent != NULL;  dependent = dependent->next){
        if(!dependent->dependent->marked) {
            helpPOT(dependent->dependent);
        }

        else if (dependent->dependent->marked != 2) {
            fprintf(stderr, "Cirrcular depency dropped\n");
        }
    }
    
    if(retVal == -1 || ( newNode->marked == 1 && newNode->time.st_ctime >= targetDate.st_ctime)) {
        cmd = newNode->firstCmd;
        while (cmd != NULL) {
            printf("%s\n", cmd->str);
            retVal  = system(cmd->str);
            if(retVal != 0) {
                fprintf(stderr, "command '%s' could not be exicuted\n", cmd->str);
                freeAll();
                exit(1);
            }
            toDate = 1; //set that a command was exicuted.
            cmd = cmd->next;
        }
        stat(newNode->str, &newNode->time);
    }
    newNode->marked = 2;
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
    
    stat(curr->str, &targetDate); //store the target date in a global.
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
        curr = curr->next;
	}

	return curr; //returns NULL if not found.
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

