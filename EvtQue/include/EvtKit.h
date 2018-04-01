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


#ifndef _EVT_KIT_H_
#define _EVT_KIT_H_

#include "FsmObject.h"
#include "EvtGenerator.h"
#include "EvtSubscriber.h"
#include "EvtRecycler.h"
#include "EvtTimer.h"
#include "EvtThread.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
 * �¼�����������
 *==============================================================================*/
#define CN_EVT_GEN_BLK_SIZE_S   (  2 )    // ���ݿ��С-S
#define CN_EVT_GEN_BLK_SIZE_M   (  4 )    // ���ݿ��С-M
#define CN_EVT_GEN_BLK_SIZE_L   (  8 )    // ���ݿ��С-L
                                                         
#define CN_EVT_GEN_BLK_NUM_S    (  12 )    // ���ݿ������������2��-S
#define CN_EVT_GEN_BLK_NUM_M    (  22 )    // ���ݿ������������2��-M
#define CN_EVT_GEN_BLK_NUM_L    (  32 )    // ���ݿ������������2��-L


/*==============================================================================
 * ״̬���Ĺ����¼����� 
 *==============================================================================*/
enum _EVT_TYPE_PUBLIC_
{
    // ʱ���¼�
    EN_EVT_BEGIN = 0,                        //--�����޸ġ����ɶ���

    EN_PUB_TIMER_T10,                        // 
    EN_PUB_TIMER_T15,                        // 

    EN_PUB_TIMER_P1,                         // 
    EN_PUB_TIMER_P2,                         // 
    EN_PUB_TIMER_P3,                         // 
    EN_PUB_TIMER_P4,                         // 
    EN_PUB_TIMER_P5,                         // 

    EN_TIM_END,                              //--�����޸ġ����ɶ���

    // Ӧ���¼�
    EN_PUB_A,                                // 
    EN_PUB_B,                                // 
    EN_PUB_C,                                // 

    EN_PUB_PHILO_GET,                        // 
    EN_PUB_PHILO_PUT,                        // 
    EN_PUB_TBL_ACCEPT,                       // 
    EN_PUB_TBL_REFUSE,                       // 

    EN_EVT_END,                              //--�����޸ġ����ɶ���
};
/*==============================================================================
 * ״̬���ı����¼����� 
 *==============================================================================*/
enum _EVT_TYPE_JMP_
{
    // �����¼�
    EN_JMP_BEGIN = EN_EVT_END,         //--�����޸ġ����ɶ���

    EN_JMP_ENTER,                            //--�����޸ġ����ɶ���
    EN_JMP_EXIT,                             //--�����޸ġ����ɶ���

    EN_JMP_END,                              //--�����޸ġ����ɶ���

};

#define CN_EVT_TABLE    (EN_EVT_END - EN_EVT_BEGIN)
#define CN_EVT_TIMER    (EN_TIM_END - EN_EVT_BEGIN)
#define CN_EVT_JUMP     (EN_JMP_END - EN_JMP_BEGIN)


/*==============================================================================*/
// �����¼�����ṹ��
typedef struct
{
    UINT32          dwType;                  // �¼����� 
    UINT32          dwCnt;                   // �¼���ע��Ĵ���
    UINT32          dwPool;                  // �¼�Դ�ڵ��ڴ�ر��
    
}tagEvent;

// ʱ���¼�����ṹ��
typedef struct
{
    tagEvent        tEvent;                  // �¼�ͷ
    UINT32          dwRst;                   // Ԥ��ʱ����Ϣ
  
}tagTime;

/*==============================================================================
 * ��������ȫ�ֱ��� 
 *==============================================================================*/
extern jmp_buf         g_ErrBuf;                       // �׳��쳣��Ϣ

extern tagEvtTable     g_tEvtTable[CN_EVT_TABLE];      // �¼�ע��� 
extern tagEvtTimer     g_tEvtTimer[CN_EVT_TIMER];      // ��ʱ������
extern void *          g_pBuf[EN_FSM_END][CN_EVT_JUMP];// �����¼�������

extern tagEvtSubObj    g_tEvtSubObj;                   // �¼�ע��� 
extern tagEvtGenObj    g_tIsrGenObj;                   // �¼�������-�жϺ������� 
extern tagEvtGenObj    g_tEvtGenObj;                   // �¼�������-�̵߳��� 

/*==============================================================================*/



/*==============================================================================
 * ��������ȫ�ֱ��� 
 *==============================================================================*/
extern UINT32 EvtGenerator(void);
extern UINT32 EvtRegister(void);
extern UINT32 EvtTimerInitAll(void);
extern UINT32 EvtTimerRun(void);
extern void   EvtSubscriber(UINT32 dwFsmNo, UINT32 *pdwSubTable, UINT32 dwSubNum);
extern UINT32 EvtTimerSet(UINT32 dwFsm, UINT32 dwSet,UINT32 dwTimer, UINT32 dwType, UINT32 dwCnt);


// �¼�������-�жϵ��� 
#define M_IsrEvtGet(tType)      (tType *)EvtGenGet(&g_tIsrGenObj, sizeof(tType))
#define M_IsrEvtPut(ptEvt)      EvtGenPut(&g_tIsrGenObj, (void *)ptEvt)

// �¼�������-�̵߳���  
#define M_FsmEvtGet(tType)      (tType *)EvtGenGet(&g_tEvtGenObj, sizeof(tType))
#define M_FsmEvtPut(ptEvt)      EvtGenPut(&g_tEvtGenObj, (void *)ptEvt)

// �¼�������-��ѯ��
#define M_EvtGenInfo()          EvtGenInfo(&g_tEvtGenObj)

// �¼�ע���-��ѯ��
#define M_EvtRegInfo()          EvtSubInfo(&g_tEvtSubObj)


// ��ʱ������
#define M_EvtTimerSet(dwSet, dwTimer, dwType, dwCnt)  EvtTimerSet(pMe->dwFsm, dwSet, dwTimer, dwType, dwCnt)


/*===============================================================================*/


#ifdef __cplusplus
}
#endif

#endif
