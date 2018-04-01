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


static tagEvtThread    g_tThdPara;


//===========================================================
// ��������: �����߳����в���
//===========================================================
void EvtThreadParaSet(tagEvtThread *ptPara)
{       

    // �����߳����в���
    memcpy(&g_tThdPara, ptPara, sizeof(tagEvtThread));
  
    return;
}

//===========================================================
// ��������: ��ȡ�߳����в���
//===========================================================
void EvtThreadParaGet(tagEvtThread *ptPara)
{       

    // ��ȡ�߳����в���
    memcpy(ptPara, &g_tThdPara, sizeof(tagEvtThread));
  
    return;
}

//===========================================================
// ��������: ���߳�״̬������
//===========================================================
void *SingleThread(void *tid)
{       
    UINT32          dwEnd;
    tagEvtThread   *ptPara;

    // ָ���ʼ��
    ptPara = (tagEvtThread *)tid;
    dwEnd  = EvtTimerNow() + ptPara->dwDlt;


    while(1)
    {
        // ��ʱ���������
        EvtTimerDelay(CN_EVT_TIMER_DELAY);

        // ��ʱ����������
        EvtTimerRun();

        // ״̬����������
        FsmRunSingleThread(); 

        // �������ʱ��
        if((ptPara->dwDlt)&&(EvtTimerNow()>dwEnd))
        {
            break;
        }

    }
    
    printf("The single thread has exit(%d)\n", (int)tid);

    pthread_exit(tid);
    
    //return NULL;
}

//===========================================================
// ��������: ���߳�״̬������
//===========================================================
void *MultiThread(void *tid)
{       
    UINT32          dwEnd;
    tagEvtThread   *ptPara;

    // ָ���ʼ��
    ptPara = (tagEvtThread *)tid;
    dwEnd  = EvtTimerNow() + ptPara->dwDlt;

  
    while(ptPara->dwFsm==EN_FSM_TIMER)
    {

        // ��ʱ���������
        EvtTimerDelay(CN_EVT_TIMER_DELAY);

        // ��ʱ����������
        EvtTimerRun();

        // �������ʱ��
        if((ptPara->dwDlt)&&(EvtTimerNow()>dwEnd))
        {
            printf("thread(%d) gotta exit\n", ptPara->dwFsm);
            break;
        }

    }

    while(ptPara->dwFsm!=EN_FSM_TIMER)
    {
        // ״̬���������
        EvtTimerDelay(CN_EVT_TASK_DELAY);
        
        // ״̬����������
        FsmRunMultiThread(ptPara->dwFsm); 

        // �������ʱ��
        if((ptPara->dwDlt)&&(EvtTimerNow()>dwEnd)
)
        {
            printf("thread(%d) gotta exit\n", ptPara->dwFsm);
            break;
        }

    }
    
    pthread_exit(tid);
    
    //return NULL;
}


//===========================================================
// ��������: �������߳�����
//===========================================================
void CreateSingleThread(tagEvtThread *ptPara)
{       
    UINT32      dwRst;
    void        *tid;
    pthread_t   thread;


    printf("Main here. Create single-thread: dwDlt=%d\n", ptPara->dwDlt);

    // �����̲߳���     
    ptPara->dwFsm = EN_FSM_TIMER;    

    // �����߳�����״̬��
    dwRst = pthread_create(&thread, NULL, SingleThread, (void *)ptPara);
    if (dwRst != 0)
    {    
        printf("Oops. thread created error(%d)\n", dwRst);
        exit(-1);
    }

    // �����߳�
    pthread_join(thread,&tid);
    
    printf("Main is ending: tid(%d)\n",(int)tid);

    return;
}


//===========================================================
// ��������: �������߳�����
//===========================================================
void CreateMultiThread(tagEvtThread *ptPara)
{       
    UINT32          dwRst, dwLoop;
    tagEvtThread    tPara[EN_FSM_END];
    void            *tid;
    pthread_t       threads[EN_FSM_END];


    for (dwLoop = 0; dwLoop < EN_FSM_END; dwLoop++)
    {
        // �����̲߳���
        memcpy(&tPara[dwLoop], ptPara, sizeof(tagEvtThread));

        // ״̬��������-�����߳�����״̬��
        if(M_FsmObjFlg(dwLoop)||(dwLoop==EN_FSM_TIMER))
        {
            printf("Main: Create multi-thread(%d)\n", dwLoop);
            tPara[dwLoop].dwFsm = dwLoop;
            if((dwRst=pthread_create(&threads[dwLoop], NULL, MultiThread, (void *)&tPara[dwLoop])))
            {    
                printf("Oops. thread created error(%d)\n", dwRst);
                exit(-1);
            }
        }
    }
        
    for (dwLoop = 0; dwLoop < EN_FSM_END; dwLoop++)
    {   
        // �����߳�
        if(tPara[dwLoop].dwFsm==dwLoop)
        {
            pthread_join(threads[dwLoop],&tid);
            printf("Main: thread(%d) is ending \n", dwLoop);
        }
    }

    printf("Main is ending: tid(%d)\n",(int)tid);
    
    return;
}




