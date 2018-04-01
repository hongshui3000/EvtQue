/*
 *MIT License
 *
 *Copyright (c) 2017-2018 Wang Ruquan. All rights reserved.
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
*/

#ifndef _DATA_LIST_H_
#define _DATA_LIST_H_


//#include "Resource.h"
//#include "InstCell.h"
#include "Macro.h"
#include "DataPool.h"

#ifdef __cplusplus
extern "C" {
#endif

/*================================================================================*/

#define CN_DATA_LIST_MODE   ( CN_DATA_LOCATE_MODE )   // 0-��̬����ģʽ 1-��̬����ģʽ


// ������������ڵ�
typedef struct ListNode
{
    struct ListNode* pNext;         // ��һ���ڵ�ָ�� 

    void             *pData;        // �ڵ�����
    
}tagListNode;

// ����������ƽṹ��
typedef struct
{
    tagListNode     *pHead;         // ����ͷ��ָ��  
    UINT32          dwNum;          // �ڵ����
#if(CN_DATA_LIST_MODE!=0)
    UINT32          dwBlk;          // ��̬�����ڴ�����    
    tagPoolCtrl     tPoolCtrl;      // ��̬�����ڴ�� 
#endif

}tagListCtrl; 

/*================================================================================
 * ������������
 *================================================================================*/
extern UINT32 DataListBuild(tagListCtrl *ptListCtrl, tagListNode *pRam, UINT32 dwBlk);
extern UINT32 DataListDestory(tagListCtrl *ptListCtrl);

extern UINT32 DataListAdd(tagListCtrl *ptListCtrl, void *pData);
extern UINT32 DataListDel(tagListCtrl *ptListCtrl, void *pData);
extern UINT32 DataListCut(tagListCtrl *ptListCtrl, void *pData);
extern UINT32 DataListTraverse(tagListCtrl *ptListCtrl, UINT32 dwType);

extern tagListNode* DataListSrch(tagListCtrl *ptListCtrl, void *pData);
/*================================================================================*/


#ifdef __cplusplus
}
#endif

#endif
