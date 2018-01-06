/*
 *the is the header file for the program mymake2
 *
 */

#ifndef MYMAKE2_HEADER
#define MYMAKE2_HEADER
#include <sys/stat.h>

typedef struct c {
	char *str;
	struct c *next;
} cmd;

typedef struct n {
	char *str;
	cmd *firstCmd;
	int marked;
    int target;
    struct stat time;
	struct e *edges;
	struct n *next;
} node;

typedef struct e {
	node *dependent;
	struct e *next;
} edge;


void freeAll();
node *addNode(char *);
edge *addEdge(node *, char *);
cmd *addCommand(node *, char *);
void POT(char *);
node *findNode(char *);
char *trimInput(char *);

extern node *header;
extern struct stat targetDate;
extern int toDate;
#endif	
		
