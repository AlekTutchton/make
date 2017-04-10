/*
 *the is the header file for the program mymake
 *
 */

#ifndef MYMAKE_HEADER
#define MYMAKE_HEADER
typedef struct c {
	char *str;
	struct c *next;
} cmd;

typedef struct n {
	char *str;
	cmd *firstCmd;
	int marked;
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

node *header;
int errNum;
#endif	
		
