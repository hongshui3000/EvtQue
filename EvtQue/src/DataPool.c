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
#include "DataPool.h"

//===========================================================
// ��������: �ڴ����Ϣ
//===========================================================
BOOL DataPoolIsFull(tagPoolCtrl *ptPoolCtrl)
{
    return (ptPoolCtrl->dwMax >= ptPoolCtrl->dwNum);    
}

//===========================================================
// ��������: �ڴ����Ϣ
//===========================================================
void DataPoolInfo(tagPoolCtrl *ptPoolCtrl)
{

    printf("DataPoolInfo:\n");
    printf("Addr=%p,Free=%p,Num=%d,Size=%d\nEmpty=%d,Used=%d,Max=%d\n\n",
                                                ptPoolCtrl->pPool  ,\
                                                ptPoolCtrl->pFree  ,\
                                                ptPoolCtrl->dwNum  ,\
                                                ptPoolCtrl->dwSize ,\
                                                ptPoolCtrl->dwEmpty,\
                                                ptPoolCtrl->dwUsed ,\
                                                ptPoolCtrl->dwMax);

    return;    
}

//===========================================================
// ��������: �ڴ������
//===========================================================
void DataPoolShow(tagPoolCtrl *ptPoolCtrl)
{
    UINT8           *pbyPool,*pbyData;
    UINT32          dwNum, dwSize, dwLen;

    // ָ���ʼ��
    pbyPool  = (UINT8 *)ptPoolCtrl->pPool;
    dwLen   = ptPoolCtrl->dwNum * ptPoolCtrl->dwSize;
    pbyData = &pbyPool[dwLen - 1];

    printf("Pool:Top\n");
    dwNum  = ptPoolCtrl->dwNum   ;
    while(dwNum)
    {
        printf("Pool[%04d]:",dwNum-1);
        dwSize = ptPoolCtrl->dwSize  ;   
        while(dwSize)
        {
            printf("%02x ",*pbyData);
            
            dwSize--;      
              
            if(!dwSize)
                printf("<--Addr:0x%p\n",pbyData);   
            
            pbyData--;
        }
        
        dwNum--;     
    }
    
    printf("Pool:Bottom\n\n\n");


    return;    
}

//===========================================================
// ��������: �ڴ�س�ʼ��
//===========================================================
UINT32 DataPoolCreat(tagPoolCtrl *ptPoolCtrl, void *pPool, UINT32 dwNum, UINT32 dwSize)
{
    void        **ppNode;
    UINT8       *pNext;
    UINT32      dwLoop;

    
    // ��� 
    if(dwNum < 2)
    {
        printf("DataPoolCreat:Num Err\n");
        return 1;
    }
    if(dwSize < sizeof(void *))
    {
        printf("DataPoolCreat:Size Err\n");
        return 2;
    }
    if(!pPool)
    {
        printf("DataPoolCreat:Pool Err\n");
        return 3;
    }
    if (((UINT32)pPool & (sizeof(void *) - 1)))
    {
        // Trick ��So elegant ��
        printf("DataPoolCreat:Alignment Err\n");
        return 4;
    }

    ppNode = (void **)pPool;
    pNext  = (UINT8 *)pPool + dwSize;
    
    for(dwLoop=1;dwLoop<dwNum;dwLoop++)
    {
        // ������һ�����ݿ�ĵ�ַ
        *ppNode = (void *)pNext;
        // ����ָ�����ݿ��ָ��
        ppNode = (void **)pNext;
        // ������һ�����ݿ�ĵ�ַ
        pNext += dwSize;
        
    }

    // �ڴ�ؽṹ
    ptPoolCtrl->pPool  = pPool    ;                 // �ڴ��ָ��
    ptPoolCtrl->pFree  = pPool    ;                 // ���п�ָ��
    ptPoolCtrl->dwNum  = dwNum   ;                  // ���ݸ���
    ptPoolCtrl->dwSize = dwSize  ;                  // ���ݳ���

    // �ڴ����Ϣ
    ptPoolCtrl->dwEmpty= dwNum   ;                  // �������ݸ���
    ptPoolCtrl->dwUsed = 0       ;                  // ��ǰд�����
    ptPoolCtrl->dwMax  = 0       ;                  // ���д�����  

    return 0;    
}

//===========================================================
// ��������: �ڴ�ػ�ȡ���ݿ�ָ��
//===========================================================
void * DataPoolGet(tagPoolCtrl *ptPoolCtrl, UINT32 *pdwErr)
{
    void    *pData = NULL;
    

    if(!ptPoolCtrl)
    {
        *pdwErr = 1;         
    }
    else if(!ptPoolCtrl->dwEmpty)
    {
        *pdwErr = 2;    
    }
    else
    {
        pData = ptPoolCtrl->pFree;
    
        ptPoolCtrl->pFree = *((void **)pData);
                
        ptPoolCtrl->dwUsed++;
        ptPoolCtrl->dwEmpty--;
            
        if(ptPoolCtrl->dwMax < ptPoolCtrl->dwUsed)
        {
            ptPoolCtrl->dwMax = ptPoolCtrl->dwUsed;           
        }
    }

    return pData;
}

//===========================================================
// ��������: �ڴ�طŻ����ݿ�ָ��
//===========================================================
UINT32 DataPoolPut(tagPoolCtrl *ptPoolCtrl, void *pData)
{
    UINT32  dwRt = 0;


    if(!ptPoolCtrl)
    {
        dwRt = 1;         
    }
    else if(!pData)
    {
        dwRt = 2;    
    }
    else if(!ptPoolCtrl->dwUsed)
    {      
        dwRt = 3;   
    }
    else
    {        
        *((void **)pData) = ptPoolCtrl->pFree;
        
        ptPoolCtrl->pFree = pData;                
        ptPoolCtrl->dwUsed--;
        ptPoolCtrl->dwEmpty++;
        
    }

    return dwRt;
}
