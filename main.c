#include "astar.h"
#include <stdio.h>


int main(){
	int i, j;
	TileBlk start_tile,goal_tile;
	Ty_nodeptr node,openNode,closedNode;
	LinkedList open,closed,path;
	Ty_linknodeptr suss,temp=NULL;
	//����Ʈ�� ����
	CreateList(&open);
	CreateList(&closed);
	printf("start node�� �Է��ϼ���\n");
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			scanf("%d", &start_tile[i][j]);
		}
	}
	printf("goal node�� �Է��ϼ���\n");
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			scanf("%d", &goal_tile[i][j]);
		}
	}
	node=InsertStartnode(start_tile, goal_tile);
	InsertList(&open, node);

	while (open.head != NULL){
		node = MinOnOpen(&open);
		DeleteList(&open, node);
		InsertList(&closed, node);
		if (node->hhat == 0){
			CreateList(&path);
			while (node != NULL){
				AppendFromFront(&path, node);
				node = node->pred;
			}
			break;//path �����
		}
		//pred�� ���� ������ ���ϴ°�
		suss=Getsuccessor(node,goal_tile);
		while (suss != NULL){ //��� ���m�� ���ؼ�
			openNode = IsOnList(&open, suss->nodeptr);  //���¿� �ִ���
			closedNode = IsOnList(&closed, suss->nodeptr); //Ŭ��� �ִ���
			if (openNode==NULL&&closedNode==NULL){ //��𿡵� ���ٸ�
				suss->nodeptr->pred = node;
				InsertList(&open, suss->nodeptr);
			}
			else if (openNode != NULL){
				if (suss->nodeptr->fhat < openNode->fhat){
					// ���¿��մ� ��� ã�Ƽ� �ٲ���
					openNode->fhat = suss->nodeptr->fhat;
					openNode->ghat = suss->nodeptr->ghat;
					openNode->pred = node;
					temp = suss;
				}
			}
			else{
				if (suss->nodeptr->fhat < closedNode->fhat){
					closedNode->fhat = suss->nodeptr->fhat;
					closedNode->ghat = suss->nodeptr->ghat;
					closedNode->pred = node;
				}
				DeleteList(&closed, closedNode);
				InsertList(&open, closedNode);
			}
			suss = suss->next;
			if (temp!=NULL){  //���θ���� �̹� ���¿� ����ִٸ� �Ҵ�� ���� ����
				free(temp->nodeptr);
				free(temp);
				temp = NULL;
			}
		}
		
	}
	
	PrintPath(&path);
	while (open.head != NULL){
		node = DeleteFromFront(&open);
		free(node);
	}
	while (closed.head != NULL){
		node = DeleteFromFront(&closed);
		free(node);
	}
	while (path.head != NULL){
		node = DeleteFromFront(&path);
		free(node);
	}
	return 0;
}