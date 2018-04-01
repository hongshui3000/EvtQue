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

#ifndef _FSM_BOJ_H_
#define _FSM_BOJ_H_

#include "Macro.h"
#include "EvtData.h"


#ifdef __cplusplus
extern "C" {
#endif


/*================================================================================*/

// ״̬����״̬
typedef struct
{   
    char *          cName;                  // ״̬������
    FUNCPTR         pHandle;                // ״ִ̬�к���ָ�� 
    
}tagState;

// ״̬����˽������
typedef struct
{   
    FUNCPTR         pInit;                  // ˽�����ݵĳ�ʼ������
    void *          pData;                  // ˽�����ݽṹ��

}tagPrivate;


// ״̬������ṹ��
typedef struct
{   
    UINT32          dwFsm;                  // ״̬���ı�� 
    char *          cName;                  // ״̬������

    tagState        tNow;                   // ״̬����״̬
    tagState        tNext;                  // ״̬����״̬

    tagPrivate      tPrivate;               // ״̬����˽������

    tagQueCtrl      tIsrQue;                // �¼����ջ�����-���ж�ִ�к���д�� 
    tagQueCtrl      tFsmQue;                // �¼����ջ�����-������״̬��д�� 
    tagQueCtrl      tJmpQue;                // �¼����ջ�����-��״̬������д�� 

    UINT32          dwJump;

}tagFsmObj;

/*================================================================================
 * ������������
 *================================================================================*/
extern UINT32 FsmObjInit(tagFsmObj *pMe, UINT32 dwNum);
extern UINT32 FsmPrivtInit(tagFsmObj *pMe, UINT32 dwNum);
extern UINT32 FsmObjBuild(tagQueCtrl *ptQueCtrl,void *pQue,UINT32 dwNum);
extern UINT32 FsmObjRun(tagFsmObj *pMe);

extern UINT32 FsmObjNow(tagFsmObj *pMe, FUNCPTR pHandle, char *cName);
extern UINT32 FsmObjNext(tagFsmObj *pMe, FUNCPTR pHandle, char *cName);

extern UINT32 FsmObjRdy(tagFsmObj *pMe);


/*================================================================================*/


#ifdef __cplusplus
}
#endif

#endif
