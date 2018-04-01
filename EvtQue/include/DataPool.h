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

#ifndef _DATA_POOL_H_
#define _DATA_POOL_H_


//#include "Resource.h"
//#include "InstCell.h"
#include "Macro.h"

#ifdef __cplusplus
extern "C" {
#endif

/*================================================================================*/

// �ڴ�ؿ��ƽṹ��
typedef struct
{
    void            *pPool;                 // �ڴ��ָ��
    void            *pFree;                 // ���п�ָ��
 
    UINT32          dwNum;                  // ���ݿ����
    UINT32          dwSize;                 // ���ݿ鳤��
                 
    UINT32          dwEmpty;                // �������ݿ����
    UINT32          dwUsed;                 // ��ǰʹ�ÿ����
    UINT32          dwMax;                  // ���ʹ�ÿ����

#if(CN_OS_TYPE==1)
    pthread_mutex_t Mutex;                  // �����ź���-������̶߳�д����
#endif

}tagPoolCtrl;

/*================================================================================
 * ������������
 *================================================================================*/
extern UINT32 DataPoolCreat(tagPoolCtrl *ptPoolCtrl, void *pPool, UINT32 dwNum, UINT32 dwSize);
extern void * DataPoolGet(tagPoolCtrl *ptPoolCtrl, UINT32 *pdwErr);
extern UINT32 DataPoolPut(tagPoolCtrl *ptPoolCtrl, void *pData);

extern void DataPoolShow(tagPoolCtrl *ptPoolCtrl);
extern void DataPoolInfo(tagPoolCtrl *ptPoolCtrl);
/*================================================================================*/


#ifdef __cplusplus
}
#endif

#endif
