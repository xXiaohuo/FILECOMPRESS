//#include "SListNode.h"
//#include <stdlib.h>
//#include  <stdio.h>
//#include <assert.h>
//
//
//////////////////////////////////////////////// 
//// 不带头节点的单链表 
////初始化
////创建新节点
//SListNode* BuySLNode(SLDataType data){
//	SListNode* pNewHead = (SListNode*)malloc(sizeof(SListNode));
//	if (pNewHead == NULL){
//		exit(1);
//	}
//	else{
//		pNewHead->_data = data;
//		pNewHead->_pNext = NULL;
//	}
//	return pNewHead;
//}
//
//void SListInit(SListNode** pHead){
//
//	*pHead = NULL;
//
//}
////尾插
//void SListPushBack(SListNode** pHead, SLDataType data){
//	SListNode*pCur = *pHead;
//	SListNode*pNewNode = BuySLNode(data);
//	if (*pHead == NULL){
//		*pHead = pNewNode;
//		return ;
//	}
//	else
//	{
//		while (pCur->_pNext){
//			pCur = pCur->_pNext;
//
//		}
//		pCur->_pNext = pNewNode;
//
//	}
//}
//
////尾删
//void SListPopBack(SListNode** pHead){
//	SListNode*pCur = *pHead;
//	SListNode*plowst = *pHead;
//	if (*pHead == NULL){
//		return;
//	}
//	else {
//		pCur = pCur->_pNext;
//		while (pCur->_pNext){
//			pCur = pCur->_pNext;
//			plowst = plowst->_pNext;
//		}
//		free(pCur);
//		plowst->_pNext = NULL;
//	}
//
//}
//
////头插
//void SListPushFront(SListNode** pHead, SLDataType data){
//	assert(pHead);
//	SListNode*pNewNode = BuySLNode(data);
//	pNewNode->_pNext = *pHead;
//	*pHead = pNewNode;
//
//}
////头删
//
//void SListPopFront(SListNode** pHead){
//	assert(pHead);
//	SListNode*pCur = *pHead;
//	if (*pHead == NULL){
//		return;
//	}
//	else{
//		*pHead = pCur->_pNext;
//		free(pCur);
//	}
//
//}
////查找data
//SListNode* SListFind(SListNode* pHead, SLDataType data){
//	SListNode *pCur = pHead;
//	while (pCur->_pNext){
//		if (pCur->_data == data){
//			return pCur;
//		}
//		pCur = pCur->_pNext;
//	}
//	return NULL;
//
//}
////任意位置插入data
//void SListInsert(SListNode* pos, SLDataType data){
//	SListNode *pnewdata = NULL;
//	SListNode*pCur = pos;
//	if (pCur == NULL){
//		return;
//	}
//	else{
//
//		pnewdata = BuySLNode(data);
//		pnewdata->_pNext = pCur->_pNext;
//		pCur->_pNext = pnewdata;
//	}
//}
////删除任意位置上的节点
//void SListErase(SListNode** pHead, SListNode* pos){
//	assert(*pHead);
//	SListNode*pCur = *pHead;
//	SListNode*pslow = *pHead;
//	SListNode* pnew = pos;
//	if (pos == NULL || *pHead == NULL)
//	{
//		return;
//	}
//	if (pos == *pHead){
//		*pHead = pCur->_pNext;
//		free(pCur);
//	}
//	else{
//		pCur = pCur->_pNext;
//		while (pCur->_pNext)
//		{
//			if (pCur == pnew){
//				break;
//			}
//
//			pCur = pCur->_pNext;
//			pslow = pslow->_pNext;
//		}
//		pslow->_pNext = pCur->_pNext;
//		free(pCur);
//	}
//}
//
////摧毁表
//void SListDestroy(SListNode** pHead){
//  SListNode* pCur = *pHead;
//  while (*pHead){
//	     pCur = *pHead;
//		*pHead = pCur->_pNext;
//		free(pCur);
//	}
//
//}
//
////表中结点个数
//int SListSize(SListNode* pHead){
//	SListNode*pCur = pHead;
//	int count = 0;
//	while (pCur){
//		pCur = pCur->_pNext;
//		count++;
//	}
//	return count;
//}
////判断是否为空
//int SListEmpty(SListNode* pHead){
//	if (pHead == NULL){
//		return 0;
//	}
//	else{
//		return 1;
//	}
//
//}
//
////返回第一个节点中的数
//SLDataType Front(SListNode* pHead)
//{
//	SListNode*pCur = pHead;
//	SLDataType data = pCur->_data;
//	return data;
//}
//
////返回最后一个结点中的数
//SLDataType Back(SListNode* pHead){
//	SListNode*pCur = pHead;
//	SLDataType data = 0;
//	while (pCur->_pNext){
//		pCur = pCur->_pNext;
//	}
//	data = pCur->_data;
//	return data;
//}
////删除值第一个为data的结点;;;
//void SListRemove(SListNode** pHead, SLDataType data){
//	SListNode *pCur = *pHead;
//	SListNode *pslow = *pHead;
//	if (data == pCur->_data){
//		*pHead = pCur->_pNext;
//		free(pCur);
//	}
//	else{
//		while (pCur->_pNext){
//
//			if (pCur->_data == data){
//				pslow->_pNext = pCur->_pNext;
//				free(pCur);
//				break;
//			}
//			pslow = pCur;
//			pCur = pCur->_pNext;
//		}
//	}
//}
////删除所有data 
//void SListRemoveAll(SListNode** pHead, SLDataType data){
//	SListNode *pCur = *pHead;
//	SListNode *pslow = *pHead;
//	
//	while (pCur){
//		if (pCur->_data == data){
//			pslow->_pNext = pCur->_pNext;
//			free(pCur);
//			pCur = pslow->_pNext;
//			continue;
//		}
//		pslow = pCur;
//		pCur = pCur->_pNext;
//	}
//
//}
//
////冒泡排序
//void SlistBubbleSort(SListNode* pHead){
//	SListNode*pCur = pHead;
//	SListNode*pfast = pHead->_pNext;
//	SListNode*ptial = NULL;
//	SListNode* sign = NULL;
//	/*while (pslow)
//	{
//		pCur = pHead;
//		pfast = pslow;
//		pnew = pHead->_pNext;
//		while (pCur->_pNext)
//		{
//			if ((pCur->_data) > (pnew->_data))
//			{
//				SLDataType data = pnew->_data;
//				pnew->_data = pCur->_data;
//				pCur->_data = data;
//			}
//			pCur = pCur->_pNext;
//			pnew = pnew->_pNext;
//		}
//		pslow = pfast->_pNext;
//		
//	}*/
//	
//	while (pCur)
//	{
//		sign = pCur;
//		pCur = pHead;
//		pfast = pHead->_pNext;
//		while (pfast!=ptial)
//		{
//			if ((pCur->_data) > (pfast->_data))
//			{
//				SLDataType data = pfast->_data;
//				pfast->_data = pCur->_data;
//				pCur->_data = data;
//			}
//			pCur = pCur->_pNext;
//			pfast = pfast->_pNext;
//			
//		}
//		ptial = pCur;
//		pCur = sign->_pNext;
//	}
//}
//
//
//
//
//void  PrintSList(SListNode* pHead){
//	SListNode*pCur=pHead;
//	while (pCur){
//		printf("%d->", pCur->_data);
//		pCur = pCur->_pNext;
//	}
//	printf("NULL\n");
//
//}
//
//
//
//
//void TestList(){
//	SListNode *pList;
//	SListInit(&pList);
//	SListPushBack(&pList, 1);
//	SListPushBack(&pList, 6);
//	SListPushBack(&pList, 3);
//	SListPushBack(&pList, 4);
//	SListPushBack(&pList, 4);
//	SListPushFront(&pList, 4);
//	PrintSList(pList);
//	SlistBubbleSort(pList);
//	//SListDestroy(&pList);
//	//SListPushBack(&pList, 4);
//	//SListPushBack(&pList, 4);
//	//SListEmpty(&pList);
//	//SListPushBack(&pList, 4);
//	//SListPushBack(&pList, 4);
//	//printf("%d\n", Back(pList));
//	//SListErase(&pList, SListFind(pList, 1));
//	SListRemoveAll(&pList, 4);
//	//SListInsert(SListFind(pList, 4), 6);
//	///SListPopFront(&pList);
//	//PrintSList(pList);
//	//SListPopFront(&pList);
//	//SListEmpty(pList);
//	//PrintSList(pList);
//	//SListPopBack( &pList);
//	//SListPushFront(&pList,1);
//	//SListPushFront(&pList, 3);
//	PrintSList(pList);
//	//printf("%d\n", SListEmpty(pList));
//}
