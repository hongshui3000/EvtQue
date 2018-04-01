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

#ifndef _DATA_QUE_H_
#define _DATA_QUE_H_


//#include "Resource.h"
//#include "InstCell.h"
#include "Macro.h"

#ifdef __cplusplus
extern "C" {
#endif

/*================================================================================*/

//#define CN_NUM_CAN_DATA                 ( 2 )      // CAN������������1��������=1�����ջ�����+1�����ͻ�������

//#define CN_LEN_DATA_FRAME               ( 13  )    // ÿ������֡�ĳ���
//#define CN_NUM_BUF_RECV                 ( 100 )    // ���ջ�������֡��
//#define CN_NUM_BUF_SEND                 ( 30  )    // ���ͻ�������֡��

// ���ζ��п��ƽṹ��
typedef struct
{
    void            *pQue;                  // ����ָ��
    UINT32          dwNum;                  // ���ݸ���
    UINT32          dwSize;                 // ���ݳ���
                                            
    UINT32          dwWr;                   // ����д��
    UINT32          dwRd;                   // ���ݶ���
                 
    UINT32          dwEmpty;                // �������ݸ���
    UINT32          dwUsed;                 // ��ǰд�����
    UINT32          dwMax;                  // ���д�����      

#if(CN_OS_TYPE==1)
    pthread_mutex_t Mutex;                  // �����ź���-������̶߳�д����
#endif

}tagQueCtrl;                                
                                            
              
/*================================================================================
 * ������������
 *================================================================================*/
extern BOOL DataQueIsFull(tagQueCtrl *ptQueCtrl);
extern void DataQueInfo(tagQueCtrl *ptQueCtrl); 
extern void DataQueShow(tagQueCtrl *ptQueCtrl); 
extern UINT32 DataQueInit(tagQueCtrl *ptQueCtrl, void *pQue, UINT32 dwNum, UINT32 dwSize);
extern UINT32 DataQueRead(tagQueCtrl *ptQueCtrl, void *pDst, UINT32 dwRdNum);
extern UINT32 DataQueWrite(tagQueCtrl *ptQueCtrl, void *pSrc, UINT32 dwWrNum);
/*================================================================================*/


#ifdef __cplusplus
}
#endif

#endif
