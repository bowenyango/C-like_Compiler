#pragma once
#include "mid_code.h"

#define MAX_BASIC_BLOCK 5000
#define MAX_PARENT_BLOCK 1000
#define MAX_PREARRAY 1000
#define MAX_CONNECT 1000
#define MAX_NODE_NUMBER 1000


typedef struct{
	string *name;
	int index;
}Node_List;

typedef struct {
	int prearray[MAX_PREARRAY]; //存放所有的前驱块
	int prenum; //前驱块数量
	int next_follow; //直接后继
	int next_jump; //跳转后继
}Block;

typedef struct {
	int edge_number;
	bool conflict[MAX_CONNECT];
}Conflict_Node;

