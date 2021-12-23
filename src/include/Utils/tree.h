#ifndef TREE_H
#define TREE_H

typedef struct{
    int *leaf;
    int n;
    int *parent;
    int *child;
    int *group;
    char **name;

    int groups;
    int *groupSize;
    int *groupOffset;
} Tree;

#endif