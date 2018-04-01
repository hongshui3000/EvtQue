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

#ifndef _EVT_THREAD_H_
#define _EVT_THREAD_H_

#include "Macro.h"
#include "EvtData.h"

#ifdef __cplusplus
extern "C" {
#endif

/*================================================================================*/
#define CN_EVT_TIMER_DELAY      (   1 )      // ��ʱ���������ʱ��-����
#define CN_EVT_TASK_DELAY       ( 200 )      // ״̬���������ʱ��-����


// ���߳����в���
typedef struct
{
    UINT32          dwFsm;                   // ״̬�����

    // ��������
    UINT32          dwDlt;                   // �߳�ִ��ʱ��-����
    UINT32          dwTimer;                 // ��ʱ���Ĳ���-����
    UINT32          dwThread;                // 0-���߳�/��0-���߳�����   

}tagEvtThread;


/*================================================================================
 * ������������
 *================================================================================*/

extern void CreateSingleThread(tagEvtThread *ptPara);
extern void CreateMultiThread(tagEvtThread *ptPara);

extern void EvtThreadParaGet(tagEvtThread  *ptPara);
extern void EvtThreadParaSet(tagEvtThread *ptPara);


/*================================================================================*/


#ifdef __cplusplus
}
#endif

#endif
