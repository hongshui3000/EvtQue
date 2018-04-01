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
#include "EvtKit.h"
#include "FsmKit.h"



//===========================================================
// ��������: �¼�ע���Ķ�����Ϣ
//===========================================================
void EvtSubInfo(tagEvtSubObj *pMe)
{
    UINT32 dwLoop;

    printf("EvtSubInfo\n"); 
    
    for(dwLoop=0;dwLoop<pMe->dwEvtNum;dwLoop++)
    {
        DataListTraverse(&pMe->ptEvtTable[dwLoop].tList, 0);
    }
    
    return;    
}


//===========================================================
// ��������: �¼�ע����� 
//===========================================================
UINT32 EvtSubBuild(tagEvtSubObj *pMe, tagEvtTable *ptEvtTable, UINT32 dwEvtNum)
{  
    UINT32          dwErr, dwLoop;  
    tagEvtTable     *ptTable;

    
    // ��� 
    if(!dwEvtNum)
    {
        printf("EvtSubBuild:Num Err\n");
        return 1;
    }
    if(!ptEvtTable)
    {
        printf("EvtSubBuild:Table Err\n");
        return 2;
    }

    // �¼�ע����ʼ�� 
    for(ptTable=&ptEvtTable[0],dwLoop=0;dwLoop<dwEvtNum;dwLoop++,ptTable++)
    {
        // ��ʼ������ 
#if(CN_EVT_REG_MODE!=0)
        dwErr = DataListBuild( &ptTable->tList, &ptTable->tNode[0], EN_FSM_END);                                         
#else
        dwErr = DataListBuild( &ptTable->tList, NULL, 0);                                         
#endif
        if(dwErr)
        {
            printf("EvtSubBuild: Err=%d,Loop=%d\n",dwErr,dwLoop);       
            return 3;
        }
    }

    pMe->ptEvtTable = ptEvtTable;              // �¼�ע��� 
    pMe->dwEvtNum   = dwEvtNum;                // �¼�����   

    return 0;    
}

//===========================================================
// ��������: �¼�ע������״̬��������Ϣ 
//===========================================================
UINT32 EvtSubMap(tagEvtSubObj *pMe, UINT32 *pdwSubTable, UINT32 dwSubNum, UINT32 dwFsmNo)
{
    UINT32          dwErr, dwLoop;  
    UINT32          dwEvt;
    
    // ��� 
    if(!dwSubNum)
    {
        printf("EvtSubMap:Num Err\n");
        return 1;
    }
    if(!pdwSubTable)
    {
        printf("EvtSubMap:Table Err\n");
        return 2;
    }
    
    // �������ı� 
    for(dwLoop=0;dwLoop<dwSubNum;dwLoop++)
    {
        dwEvt = pdwSubTable[dwLoop];        
        dwErr = DataListAdd(&pMe->ptEvtTable[dwEvt].tList, (void *)dwFsmNo);        
        if(dwErr)
        {
            printf("EvtSubMap:Map Err=%d, Loop=%d\n",dwErr,dwLoop);
            return 3;           
        }  
    } 
    
    return 0;
}

//===========================================================
// ��������: �ڴ�طŻ����ݿ�ָ��
//===========================================================
UINT32 EvtSubDestroy(tagEvtSubObj *pMe)
{
    UINT32          dwErr, dwLoop;  

    // �����¼��� 
    for(dwLoop=0;dwLoop<pMe->dwEvtNum;dwLoop++)
    {
        dwErr = DataListDestory(&pMe->ptEvtTable[dwLoop].tList);
        if(dwErr)
        {
            printf("EvtSubDestroy: Err=%d, Loop=%d\n",dwErr,dwLoop);
            return 1;           
        }  
    } 

    return 0;
}

