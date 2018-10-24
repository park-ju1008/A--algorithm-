#include "astar.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//��ŸƮ����� �����Լ�
Ty_nodeptr InsertStartnode(TileBlk start_tile, TileBlk goal_tile){
	int count;
	Ty_nodeptr startnode = (Ty_nodeptr)malloc(sizeof(Ty_node));
	memcpy(startnode->tile, start_tile, sizeof(TileBlk));
	count = CompareTile(start_tile, goal_tile);
	startnode->fhat = count;
	startnode->ghat = 0;
	startnode->hhat = count;
	startnode->pred = NULL;
	return startnode;
}

//�������� ���ϱ����� ��� �����Լ�
Ty_nodeptr CreateNode(TileBlk tile, TileBlk goal_tile, double precost){
	int count=0;
	Ty_nodeptr node = (Ty_nodeptr)malloc(sizeof(Ty_node));
	memcpy(node->tile, tile, sizeof(TileBlk));
	count = CompareTile(tile, goal_tile);
	node->ghat = precost + 1;
	node->hhat = count;
	node->fhat = node->ghat + node->hhat;
	node->direction = 0;
	node->pred = NULL;
	return node;
}

//��ũ�帮��Ʈ�� �ʱ�ȭ 
void CreateList(LinkedList* list){
	list->head = NULL;
	list->tail = NULL;
}

//����Ʈ�� ��� �Է��ϴ� �Լ�
Ty_nodeptr InsertList(LinkedList* list, Ty_nodeptr node){
	Ty_linknodeptr index = (Ty_linknodeptr)malloc(sizeof(Ty_linknode));
	index->nodeptr = node;
	index->next = NULL;
	if (list->head != NULL){
		list->tail->next = index;
		list->tail = index;
	}
	else{
		list->head = index;
		list->tail = index;
	}
	return list->tail->nodeptr;
}

//����Ʈ�� ������ �����ϴ� �Լ�
Ty_nodeptr AppendFromFront(LinkedList* list, Ty_nodeptr node){
	Ty_linknodeptr index = (Ty_linknodeptr)malloc(sizeof(Ty_linknode));
	index->nodeptr = node;
	index->next = NULL;
	if (list->head != NULL){
		index->next = list->head;
		list->head = index;
	}
	else{
		list->head = index;
		list->tail = index;
	}
	return node;
}

//����Ʈ�� ���ʺ��� �����ϴ� �Լ�
Ty_nodeptr DeleteFromFront(LinkedList* list){
	Ty_linknodeptr temp=NULL;
	Ty_nodeptr node=NULL;
	if (list->head != NULL){
		temp = list->head;
		node = list->head->nodeptr;
		list->head = list->head->next;
		free(temp);
	}
	return node;
}

//����Ʈ���� ���ϴ� ��带 �����ϴ� �Լ�
Ty_nodeptr DeleteList(LinkedList* list, Ty_nodeptr node){
	Ty_linknodeptr temp;
	Ty_linknodeptr pre=NULL;
	Ty_nodeptr del=NULL;
	temp = list->head;
	if (temp!=NULL){
		while (temp->nodeptr != node){
			pre = temp;
			temp = temp->next;
		}
		if (temp != list->head&&temp != list->tail){
			pre->next = temp->next;
		}
		if (temp == list->head){
			list->head = list->head->next;
		}
		if (temp == list->tail){
			list->tail = pre;
			if (pre != NULL){
				pre->next = NULL;
			}
		}
		del=temp->nodeptr;
		if (temp != NULL){
			free(temp);
		}
	}
	return del;
}

//���¿��� ���� ���� ��θ� ������ ��带 ���¿��� ������ �Լ�
Ty_nodeptr MinOnOpen(LinkedList* list){
	Ty_linknodeptr cur;
	Ty_nodeptr	minNode;

	minNode = list->head->nodeptr;
	cur = list->head;
	while (cur != NULL){
		if (minNode->fhat > cur->nodeptr->fhat){
			minNode = cur->nodeptr;
		}
		cur = cur->next;
	}
	return minNode;
}

//�Է��� ��尡 ����Ʈ�� �ִ��� Ȯ���ϴ� �Լ�
Ty_nodeptr IsOnList(LinkedList* list, Ty_nodeptr node){
	Ty_nodeptr ret=NULL;
	Ty_linknodeptr index;
	index = list->head;
	while (index!=NULL){
		if (CompareTile(index->nodeptr->tile,node->tile)==0){
			ret =index->nodeptr;
			break;
		}
		index = index->next;
	}
	return ret;
}

//pred�� successor�� ���ϴ� �Լ�
Ty_linknodeptr Getsuccessor(Ty_nodeptr pred,TileBlk goal){
	int i,j;
	int row=-1, colum=-1;
	Ty_nodeptr tempNode;
	TileBlk tempTile;
	LinkedList list;
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			if (pred->tile[i][j] == -1){
				row = i;
				colum = j;
				break;
			}
		}
		if (row != -1 && colum != -1){
			break;
		}
	}
	CreateList(&list);
	if (row>0){
		//���ιб�
		memcpy(tempTile, pred->tile, sizeof(TileBlk));
		tempTile[row][colum] = tempTile[row - 1][colum];
		tempTile[row - 1][colum] = -1;
		tempNode = CreateNode(tempTile, goal, pred->ghat);
		tempNode->direction = 1;
		InsertList(&list, tempNode);
	}
	if (row < N - 1){
		//�Ʒ��� �б�
		memcpy(tempTile, pred->tile, sizeof(TileBlk));
		tempTile[row][colum] = tempTile[row + 1][colum];
		tempTile[row + 1][colum] = -1;
		tempNode = CreateNode(tempTile, goal, pred->ghat);
		tempNode->direction = 2;
		InsertList(&list, tempNode);
	}
	if (colum>0){
		//�������� �б�
		memcpy(tempTile, pred->tile, sizeof(TileBlk));
		tempTile[row][colum] = tempTile[row][colum - 1];
		tempTile[row][colum - 1] = -1;
		tempNode = CreateNode(tempTile, goal, pred->ghat);
		tempNode->direction = 3;
		InsertList(&list, tempNode);
	}
	if (colum < N - 1){
		//���������� �б�
		memcpy(tempTile, pred->tile, sizeof(TileBlk));
		tempTile[row][colum] = tempTile[row][colum + 1];
		tempTile[row][colum + 1] = -1;
		tempNode = CreateNode(tempTile, goal, pred->ghat);
		tempNode->direction = 4;
		InsertList(&list, tempNode);
	}
	return list.head;
}

//�ΰ��� Ÿ���� �ٸ� ������ ����ϴ� �Լ�
int CompareTile(TileBlk start_tile, TileBlk dest_tile){
	int i, j, count = 0;
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			if (start_tile[i][j] != -1 && start_tile[i][j] != dest_tile[i][j]){
			count++;
		}
	}
}
	return count;
}

//���� ������ �ܼ�â�� ���Ϸ� ����ϴ� �Լ�
void PrintPath(LinkedList* list){
	Ty_nodeptr node=NULL;
	int i, k,fhat;
	FILE *fp = fopen("path.txt", "wt");


	node=DeleteFromFront(list);
	fhat = node->fhat;
	printf("This is the start state.\n");
	fprintf(fp, "This is the start state.\n");
	while (node != NULL){
		if (node->direction == 1){
			printf("==> move up\n");
			fprintf(fp, "==> move up\n");
		}
		else if (node->direction == 2){
			printf("==> move down\n");
			fprintf(fp, "==> move down\n");
		}
		else if (node->direction == 3){
			printf("==> move left\n");
			fprintf(fp, "==> move left\n");
		}
		else if (node->direction == 4){
			printf("==> move right\n");
			fprintf(fp, "==> move right\n");
		}
		for (i = 0; i < N; i++) {
			for ( k = 0; k < N; k++) {
				
				if (node->tile[i][k] < 10 && node->tile[i][k] != -1) {
					printf(" ");
					fprintf(fp, " ");
				}
				if (node->tile[i][k] != -1){
					printf("%d ", node->tile[i][k]);
					fprintf(fp, "%d ", node->tile[i][k]);
				}
				else {
					printf("   ");
					fprintf(fp, "   ");
				}
			}
			printf("\n");
			fprintf(fp, "\n");
		}
		printf("\n");
		fprintf(fp, "\n");
		node = DeleteFromFront(list);
	}
	printf("This sequence is a path to the goal !!\n");
	fprintf(fp, "This sequence is a path to the goal !!\n");
	printf("Length of the path =  %d\n",fhat);
	fprintf(fp, "Length of the path = %d\n",fhat);
}