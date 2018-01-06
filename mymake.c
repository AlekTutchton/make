/*
 *This program will simulate the make command in UNIX.
 * Author: Alek Tutchton
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "mymake.h"
//#include "/home/cs352/spring17/Assignments/proj09/cs352.h"


int readFile(FILE *);


int main(int argc, char *argv[]) {
    FILE *fp;
    char *makeFile, *targ = NULL;
    
    if(argc != 3) {
        fprintf(stderr, "Usage: ./mymake  makefile target\n");
        return 1;
    }
    
    makeFile = argv[1];
    targ = argv[2];
    
    fp = fopen(makeFile, "r");
    if(fp == NULL) {
        perror(makeFile);
        return 1;
    }
    
    int status = readFile(fp);
    if(status == 1) {
        fclose(fp);
        freeAll();
        return 1;
    }
    fclose(fp);
    
    if (targ == NULL) {
        if(header != NULL) {
            targ = header->str;
        }
        else {
            fprintf(stderr, "Usage: ./mymake  makefile target\n");
            return 1;
        }
    }
    

        POT(targ);
        freeAll();
        return 0;
     
}


    /*
     *readFile() reads the input makefile and creates the dependency graph
     * data structure.
     */
    int readFile(FILE *file) {
        char *line = NULL;
        char *fp, *parsePtr, *temp;
        size_t sz =0;
        node *tailNode = NULL;
        
        //trim if there is any whitspace at the end of the input.
        while(getline(&line, &sz, file) > 0) {
            int end = (int)strlen(line) - 1;
            while(isspace(line[end]) && end >= 0) {
                end--;
            }
            end++;
            line[end] = '\0';

            if(strlen(line) == 0)
                continue;
            fp = line;
            while(isspace(*fp)) {
                ++fp;
            }
            if(line[0] == '\t') {
                if(tailNode == NULL) {
                    fprintf(stderr, "Command with no target\n");
                    free(line);
                    return 1;
                }
                
                addCommand(tailNode, fp);
                continue;
            }
            
            parsePtr = fp;
            while(*parsePtr != '\0' && *parsePtr != ':') {
                parsePtr++;
            }
            
            if(*parsePtr != ':') {
                fprintf(stderr, "Illegal line %s\n", line);
                free(line);
                return 1;
            }
            *parsePtr = '\0';
            temp = parsePtr - 1;
            while(isspace(*temp)) {
                *temp = '\0';
                temp--;
            }
            
            char* checkTargs = fp;
            
            while( *checkTargs) {
                if(isspace(*checkTargs)) {
                    fprintf(stderr, "Too many Targets: %s\n", line);
                    free(line);
                    return 1;		
                }
                checkTargs++;
            }	
            
            tailNode = findNode(fp);
            if(tailNode == NULL) {
                tailNode = addNode(fp);
            }	
            else {
                if( tailNode->target == 1) {
                    fprintf(stderr, "Repeat target: %s\n", fp);
                    free(line);
                    return 1;
                }
            }
            tailNode->target = 1; //mark the new node as a target.
            
            while (1) {
                
                fp = parsePtr + 1;
                
                while (isspace(*fp)) {
                    fp++;
                }
                if (*fp == '\0')
                    break;
                
                parsePtr = fp + 1;
                
                while (*parsePtr != '\0' && !isspace(*parsePtr)) {
                    parsePtr++;
                }
                
                if (*parsePtr == '\0') {
                    addEdge(tailNode, fp);
                    break;
                }
                
                *parsePtr = '\0';
                addEdge(tailNode, fp);
                
            }
        }
        return 0;
}
