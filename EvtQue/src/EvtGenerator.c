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

     
/*================================================================================
 * �ڲ��ṹ������
 *================================================================================*/
 


/*================================================================================
 * �ڲ���������
 *================================================================================*/
                    
                          


/*================================================================================
 * �ڲ���������
 *================================================================================*/

//===========================================================
// ��������: �ڴ����Ϣ
//===========================================================
void EvtGenInfo(tagEvtGenObj *pMe)
{
    UINT32      dwLoop; 

    printf("EvtGenInfo:\n");

    for(dwLoop=0;dwLoop<pMe->dwPoolNum;dwLoop++)
    {
        DataPoolInfo(pMe->ptPoolTable[dwLoop].ptPoolCtrl);
        //DataPoolShow(pMe->ptPoolTable[dwLoop].ptPoolCtrl);
    }

    return;    
}

//===========================================================
// ��������: �¼���������ʼ��
//===========================================================
UINT32 EvtGenCreat(tagEvtGenObj *pMe, tagPoolTable *ptPoolTable, UINT32 dwPoolNum)
{
    UINT32          dwErr, dwLoop;
    
    // ��� 
    if(dwPoolNum < 3)
    {
        printf("EvtGenCreat:Num Err\n");
        return 1;
    }
    if(!ptPoolTable)
    {
        printf("EvtGenCreat:Pool Err\n");
        return 2;
    }

    // ��ʼ���ڴ��     
    for(dwLoop=0;dwLoop<dwPoolNum;dwLoop++)
    {
        dwErr = EvtPoolCreat(ptPoolTable[dwLoop].ptPoolCtrl,\
                             ptPoolTable[dwLoop].pPool,     \
                             ptPoolTable[dwLoop].dwNum,     \
                             ptPoolTable[dwLoop].dwSize      );
                                            
        if(dwErr)
        {
            printf("EvtGenCreat: Err=%d,Loop=%d\n",dwErr,dwLoop);       
            return 3;
        }
    }

    // �¼���������Ϣ 
    pMe->ptPoolTable = ptPoolTable;         // �ڴ��
    pMe->dwPoolNum   = dwPoolNum;           // �ڴ������

    return 0;    
}

//===========================================================
// ��������: �ڴ�ػ�ȡ���ݿ�ָ��
//===========================================================
void * EvtGenGet(tagEvtGenObj *pMe, UINT32 dwSize)
{
    UINT32          dwErr, dwLoop;
    tagEvent        *ptEvt = NULL;

    // ѡ���С���ʵ��ڴ����Ϊ�¼�ָ��   
    for(dwLoop=0;dwLoop<pMe->dwPoolNum;dwLoop++)
    {
        if(pMe->ptPoolTable[dwLoop].dwSize >= dwSize)
        {   
            ptEvt = (tagEvent *)EvtPoolGet(pMe->ptPoolTable[dwLoop].ptPoolCtrl, &dwErr); 
            if(ptEvt)
            {
                // �����ɵ��¼���Ҫ����ڴ�ر��
                ptEvt->dwPool = dwLoop;
                
                return ptEvt;
            }
        }
    }

    //--need fix ���е��˴�Ҫ�����쳣�澯��Ϣ
    //--�׳��쳣����
    printf("EvtGenGet: Pool buffer is empty, pool info is : \n");

    M_EvtGenInfo();
    longjmp(g_ErrBuf, 1);
    
    return ptEvt;
}

//===========================================================
// ��������: �ڴ�طŻ����ݿ�ָ��
//===========================================================
UINT32 EvtGenPut(tagEvtGenObj *pMe, void *pData)
{
    UINT32          dwErr=0, dwLoop;
    tagEvent        *ptEvt;
    
    // �¼�ָ��
    ptEvt = (tagEvent *)pData;

    // �����¼����ڴ�ر�Ż����ڴ��
    for(dwLoop=0;dwLoop<pMe->dwPoolNum;dwLoop++)
    {
        if(ptEvt->dwPool == dwLoop)
        {
            //printf("EvtGenPut:Loop=%d,PS=%d,ES=%d\n",dwLoop,pMe->ptPoolTable[dwLoop].dwSize,dwSize);
            dwErr = EvtPoolPut(pMe->ptPoolTable[dwLoop].ptPoolCtrl, pData);

            return dwErr;
        }
    }
    
    //--need fix ���е��˴�Ҫ�����쳣�澯��Ϣ
    //--�׳��쳣����
    printf("EvtGenPut: Pool buffer is unnormal, pool info is \n");

    M_EvtGenInfo();
    longjmp(g_ErrBuf, 2);

    return dwErr;
}
