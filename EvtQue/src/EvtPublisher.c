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
 
#include <stdio.h>

#include "FsmKit.h"
#include "EvtKit.h"


//===========================================================
// ��������: ״̬�������¼�
//===========================================================
UINT32 FsmPubEvt(void *pEvt, UINT32 dwType)
{
    UINT32          dwFsm;
    tagEvent        *ptEvt;
    tagListCtrl     *ptList;
    tagListNode     **ppIndex;          // ����ͷ��ָ��  
    tagQueCtrl      *ptQue;             // �¼����ջ����� 
    tagFsmObj       *ptFsm;
    tagEvtTable     *ptTable;
    
    if(dwType >= CN_EVT_TABLE)
    {
        return 1;
    }

    // �¼�ָ��
    ptEvt = (tagEvent *)pEvt;

    // ����¼�����
    ptEvt->dwType = dwType;
    
    // �¼��ܱ�     
    ptTable = &g_tEvtTable[dwType];
    ptList  = &ptTable->tList;
    
    
    if(!ptList->dwNum)
    {
        return 2;       
    }       

    // ����¼�ע����� 
    ptEvt->dwCnt = ptList->dwNum;

    // �������� 
    ppIndex = &ptList->pHead;
    while(*ppIndex)
    {

        // ״̬�����           
        dwFsm = (int)(*ppIndex)->pData;             
        if(dwFsm >= EN_FSM_END)
        {
            return 3;   
        }
        ptFsm = &g_tFsmTable[dwFsm]; 

        // ״̬���¼�������ָ��
        if(ptEvt->dwType < CN_EVT_TIMER)
        {
            ptQue = &ptFsm->tIsrQue;                        
        }
        else
        {
            ptQue = &ptFsm->tFsmQue;                        
        }

        // д���¼�������
        if(EvtQueWrite(ptQue, (void *)&ptEvt, 1))
        {
            //--wangrq for test @ 20180306
            //printf("FsmSigEvt: E%d,F%d\n",ptEvt->dwType,dwFsm);
            
            // ����ָ����� 
            ppIndex = &(*ppIndex)->pNext;

        }
        else
        {
            //--need fix ���е��˴�Ҫ�����쳣�澯��Ϣ
            //--�׳��쳣����
            printf("FsmPubEvt: Fsm(%d) buffer is full, queue info is \n",dwFsm);

            EvtQueInfo(&ptFsm->tIsrQue);
            EvtQueInfo(&ptFsm->tFsmQue);
            longjmp(g_ErrBuf, 3);
            
        }
        
    }
        
    return 0;    
}

