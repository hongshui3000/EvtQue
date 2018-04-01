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

#include "EvtData.h"
#include "Random.h" 

#if(CN_OS_TYPE==1)
    pthread_mutex_t RandMutex;                  // �����ź���-������̶߳�д����
#endif



//===========================================================
// ��������: �¼���������ʼ��
//===========================================================
UINT32 EvtQueInit(tagQueCtrl *ptQueCtrl, void *pQue, UINT32 dwNum, UINT32 dwSize)
{
    // �����ź�����ʼ��
#if(CN_OS_TYPE==1)
    pthread_mutex_init(&ptQueCtrl->Mutex, 0);
#endif

    return DataQueInit(ptQueCtrl, pQue, dwNum, dwSize);
} 

//===========================================================
// ��������: �¼�����������
//===========================================================
UINT32 EvtQueRead(tagQueCtrl *ptQueCtrl, void *pDst, UINT32 dwRdNum)
{
    UINT32  dwRst;    

    // ����
#if(CN_OS_TYPE==1)
    pthread_mutex_lock(&ptQueCtrl->Mutex);
#endif

    // ����
    dwRst = DataQueRead(ptQueCtrl, pDst, dwRdNum);

    // �ͷ�
#if(CN_OS_TYPE==1)
    pthread_mutex_unlock(&ptQueCtrl->Mutex);
#endif

    return dwRst;
}

//===========================================================
// ��������: �¼�������д��
//===========================================================
UINT32 EvtQueWrite(tagQueCtrl *ptQueCtrl, void *pSrc, UINT32 dwWrNum)
{
    UINT32  dwRst;    

    // ����
#if(CN_OS_TYPE==1)
    pthread_mutex_lock(&ptQueCtrl->Mutex);
#endif

    // ����
    dwRst = DataQueWrite(ptQueCtrl, pSrc, dwWrNum);

    // �ͷ�
#if(CN_OS_TYPE==1)
    pthread_mutex_unlock(&ptQueCtrl->Mutex);
#endif

    return dwRst;
}


//===========================================================
// ��������: �¼���������Ϣ
//===========================================================
void EvtQueInfo(tagQueCtrl *ptQueCtrl)
{
    // ����
#if(CN_OS_TYPE==1)
    pthread_mutex_lock(&ptQueCtrl->Mutex);
#endif

    // ����
    DataQueInfo(ptQueCtrl);

    // �ͷ�
#if(CN_OS_TYPE==1)
    pthread_mutex_unlock(&ptQueCtrl->Mutex);
#endif

    return;
}

//===========================================================
// ��������: �¼��ڴ�س�ʼ��
//===========================================================
UINT32 EvtPoolCreat(tagPoolCtrl *ptPoolCtrl, void *pPool, UINT32 dwNum, UINT32 dwSize)
{
    // �����ź�����ʼ��
#if(CN_OS_TYPE==1)
    pthread_mutex_init(&ptPoolCtrl->Mutex, 0);
#endif

    return DataPoolCreat(ptPoolCtrl, pPool, dwNum, dwSize);
} 

//===========================================================
// ��������: �¼��ڴ�ػ�ȡ
//===========================================================
void * EvtPoolGet(tagPoolCtrl *ptPoolCtrl, UINT32 *pdwErr)
{
    void *pData;    

    // ����
#if(CN_OS_TYPE==1)
    pthread_mutex_lock(&ptPoolCtrl->Mutex);
#endif

    // ����
    pData = DataPoolGet(ptPoolCtrl, pdwErr);

    // �ͷ�
#if(CN_OS_TYPE==1)
    pthread_mutex_unlock(&ptPoolCtrl->Mutex);
#endif

    return pData;
}

//===========================================================
// ��������: �¼��ڴ�طŻ�
//===========================================================
UINT32 EvtPoolPut(tagPoolCtrl *ptPoolCtrl, void *pData)
{
    UINT32  dwRst;    

    // ����
#if(CN_OS_TYPE==1)
    pthread_mutex_lock(&ptPoolCtrl->Mutex);
#endif

    // ����
    dwRst = DataPoolPut(ptPoolCtrl, pData);

    // �ͷ�
#if(CN_OS_TYPE==1)
    pthread_mutex_unlock(&ptPoolCtrl->Mutex);
#endif

    return dwRst;
}

//===========================================================
// ��������: �¼��ڴ�طŻ�
//===========================================================
UINT32 EvtRandSet(void)
{
    // �����ź�����ʼ��
#if(CN_OS_TYPE==1)
    pthread_mutex_init(&RandMutex, 0);
#endif

    M_SetRandomSeed();

    return 0;
}

//===========================================================
// ��������: �¼��ڴ�طŻ�
//===========================================================
UINT32 EvtRandGet(UINT32 dwRange)
{
    UINT32  dwRst;    

    // ����
#if(CN_OS_TYPE==1)
    pthread_mutex_lock(&RandMutex);
#endif

    // ����
    dwRst = M_GetRandom(dwRange);

    // �ͷ�
#if(CN_OS_TYPE==1)
    pthread_mutex_unlock(&RandMutex);
#endif

    return dwRst;
}

