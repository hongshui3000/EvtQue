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

jmp_buf         g_ErrBuf;                       // �׳��쳣��Ϣ

tagEvtTable     g_tEvtTable[CN_EVT_TABLE];      // �¼�ע��� 
tagEvtTimer     g_tEvtTimer[CN_EVT_TIMER];      // ��ʱ��
void *          g_pBuf[EN_FSM_END][CN_EVT_JUMP];// �����¼�������

tagEvtSubObj    g_tEvtSubObj;                   // �¼�ע��� 

tagEvtGenObj    g_tIsrGenObj;                   // �¼�������-�жϵ��� 
tagEvtGenObj    g_tEvtGenObj;                   // �¼�������-�̵߳��� 

/*================================================================================
 * �¼��ڴ������ 
 *================================================================================*/
#define CN_EVT_GEN_EVT_SIZE_S   (sizeof(tagEvent) + CN_EVT_GEN_BLK_SIZE_S)    // �¼���С-S                                 
#define CN_EVT_GEN_EVT_SIZE_M   (sizeof(tagEvent) + CN_EVT_GEN_BLK_SIZE_M)    // �¼���С-M                                 
#define CN_EVT_GEN_EVT_SIZE_L   (sizeof(tagEvent) + CN_EVT_GEN_BLK_SIZE_L)    // �¼���С-L

static UINT8    g_IsrPool_S[CN_EVT_GEN_EVT_SIZE_S * CN_EVT_GEN_BLK_NUM_S];    // �¼����������ڴ��-S 
static UINT8    g_IsrPool_M[CN_EVT_GEN_EVT_SIZE_M * CN_EVT_GEN_BLK_NUM_M];    // �¼����������ڴ��-M 
static UINT8    g_IsrPool_L[CN_EVT_GEN_EVT_SIZE_L * CN_EVT_GEN_BLK_NUM_L];    // �¼����������ڴ��-L 
                               
static UINT8    g_EvtPool_S[CN_EVT_GEN_EVT_SIZE_S * CN_EVT_GEN_BLK_NUM_S];    // �¼����������ڴ��-S 
static UINT8    g_EvtPool_M[CN_EVT_GEN_EVT_SIZE_M * CN_EVT_GEN_BLK_NUM_M];    // �¼����������ڴ��-M 
static UINT8    g_EvtPool_L[CN_EVT_GEN_EVT_SIZE_L * CN_EVT_GEN_BLK_NUM_L];    // �¼����������ڴ��-L 

static tagPoolCtrl  g_PoolCtrl_S[2];
static tagPoolCtrl  g_PoolCtrl_M[2];
static tagPoolCtrl  g_PoolCtrl_L[2];
/*================================================================================
 * �¼��ڴ�����ñ�-ǧ��ע�⣡ 
 *================================================================================*/
const tagPoolTable g_tEvtPool[] = 
{
    // *ptPoolCtrl,       void *pPool,                 UINT32 dwNum,          UINT32 dwSize
    {&g_PoolCtrl_S[0],  (void *)&g_EvtPool_S[0],   CN_EVT_GEN_BLK_NUM_S,  CN_EVT_GEN_EVT_SIZE_S},
    {&g_PoolCtrl_M[0],  (void *)&g_EvtPool_M[0],   CN_EVT_GEN_BLK_NUM_M,  CN_EVT_GEN_EVT_SIZE_M},    
    {&g_PoolCtrl_L[0],  (void *)&g_EvtPool_L[0],   CN_EVT_GEN_BLK_NUM_L,  CN_EVT_GEN_EVT_SIZE_L},

};
const UINT32 CN_EVT_GEN_POOL_NUM = sizeof(g_tEvtPool)/sizeof(tagPoolTable);

const tagPoolTable g_tIsrPool[] = 
{
    // *ptPoolCtrl,       void *pPool,                 UINT32 dwNum,          UINT32 dwSize
    {&g_PoolCtrl_S[1],  (void *)&g_IsrPool_S[0],   CN_EVT_GEN_BLK_NUM_S,  CN_EVT_GEN_EVT_SIZE_S},
    {&g_PoolCtrl_M[1],  (void *)&g_IsrPool_M[0],   CN_EVT_GEN_BLK_NUM_M,  CN_EVT_GEN_EVT_SIZE_M},    
    {&g_PoolCtrl_L[1],  (void *)&g_IsrPool_L[0],   CN_EVT_GEN_BLK_NUM_L,  CN_EVT_GEN_EVT_SIZE_L},

};
const UINT32 CN_ISR_GEN_POOL_NUM = sizeof(g_tIsrPool)/sizeof(tagPoolTable);

// =============================================================================
// �������ܣ���ʼ���¼�������
// �����������
// �����������
// ����ֵ��  ��
// =============================================================================
UINT32 EvtGenerator(void)
{
//    static UINT32         dwErr, dwLoop, dwBuf[16], *pSize;
//    static void           *pBuf[16], **ppEvt;
    UINT32  dwErr;  


    //  ��ʼ���¼�������-�жϺ������� 
    dwErr = EvtGenCreat(&g_tIsrGenObj, (tagPoolTable *)&g_tIsrPool, CN_ISR_GEN_POOL_NUM); 
    if(dwErr)
    {
        printf("PoolTable: dwErrInfo=%d\n",dwErr);
    } 

    //  ��ʼ���¼�������-�̵߳���
    dwErr = EvtGenCreat(&g_tEvtGenObj, (tagPoolTable *)&g_tEvtPool, CN_EVT_GEN_POOL_NUM); 
    if(dwErr)
    {
        printf("PoolTable: dwErrInfo=%d\n",dwErr);
    } 
    
    return 0;
}

// =============================================================================
// �������ܣ���ʼ���¼�ע���
// �����������
// �����������
// ����ֵ��  ��
// ============================================================================= 
UINT32 EvtRegister(void)
{
    static UINT32   dwErr;
    
    dwErr = EvtSubBuild(&g_tEvtSubObj, &g_tEvtTable[0], CN_EVT_TABLE);
    if(dwErr)
    {
        printf("dwErr=%d\n",dwErr);
    } 
        
//  printf("EvtRegInfo\n"); 
//  EvtRegMap(&g_tEvtRegObj, (UINT32 *)&g_dwSubTable01[0], CN_EVT_REG_TBL01, EN_FSM_1);
//  EvtRegInfo(&g_tEvtRegObj);
//  
//  printf("EvtRegInfo\n");
//  EvtRegMap(&g_tEvtRegObj, (UINT32 *)&g_dwSubTable02[0], CN_EVT_REG_TBL02, EN_FSM_2);
//  EvtRegInfo(&g_tEvtRegObj);

//  printf("EvtRegInfo\n");
//  EvtRegDestroy(&g_tEvtRegObj);
//  EvtRegInfo(&g_tEvtRegObj);

    return 0;
} 

// =============================================================================
// �������ܣ�״̬�����ĵ��¼��ṩ��ע���
// �����������
// �����������
// ����ֵ��  ��
// ============================================================================= 
void EvtSubscriber(UINT32 dwFsmNo, UINT32 *pdwSubTable, UINT32 dwSubNum)
{
    // ע�� 
    EvtSubMap(&g_tEvtSubObj, pdwSubTable, dwSubNum, dwFsmNo);

    return;
} 

//===========================================================
// ��������: �¼���ʼ��
//===========================================================
UINT32 EvtTimerInitAll(void)
{
    EvtTimerInit(&g_tEvtTimer[0], CN_EVT_TIMER);
    
    return 0;
} 

//===========================================================
// ��������: �¼���ʼ��
//===========================================================
UINT32 EvtTimerSet(UINT32 dwFsm, UINT32 dwSet,UINT32 dwTimer, UINT32 dwType, UINT32 dwCnt)
{      
    UINT32      dwEnd, dwDlt;
    
    // ���
    if(dwTimer >= CN_EVT_TIMER)
    {
        printf("EvtTimerSet:Err Timer=%d\n",dwTimer);
        return 1;
    }

    if(dwSet)
    {
        // ͳһת��Ϊ���뵥λ����
        EvtTimerCnvt(dwType, dwCnt, &dwEnd, &dwDlt);

        //printf("EvtTimerSet:Fsm(%d),Timer(%d),End=%d,Dlt=%d\n",dwFsm,dwTimer,dwEnd,dwDlt);
            
        // ������ʱ��
        EvtTimerStart(&g_tEvtTimer[dwTimer], dwTimer, 0 , dwEnd, dwDlt);
    }
    else
    {
        // ֹͣ��ʱ��
        EvtTimerStop(&g_tEvtTimer[dwTimer]);
    }
    
    return 0;
} 

//===========================================================
// ��������: ��ʱ������
//===========================================================
UINT32 EvtTimerRun(void)
{
    UINT32          dwNow;
    UINT32          dwLoop,dwErr;
    tagEvtTimer     *pTimer;
    tagTime         *pTime;
    static UINT32   dwNowBak;
    static UINT32   dwCnt;

    // ��ȡ��ǰʱ��
    dwNow = EvtTimerNow();

    if((dwNow - dwNowBak)>=1000)
    {
        dwNowBak = dwNow;

        printf("EvtTimerRun:%d!\n",++dwCnt);
    }
    

    // ������ʱ������
    pTimer = &g_tEvtTimer[0];
    for(dwLoop=0; dwLoop<CN_EVT_TIMER; dwLoop++,pTimer++)
    {
        // ��ʱ���Ƿ�����
        if(pTimer->dwFlag >= CN_EVT_TIMER)
        {               
            continue;
        }
        
        // ���¶�ʱ����Ϣ
        if(EvtTimerUpdt(pTimer, dwNow))
        {
            // ���Ͷ�ʱ���¼�
            //--wangrq for test @ 20180306
/*
printf("EvtTimerRun:Send Time Evt! From timer(%d),Now=%d\n",dwLoop,dwNow);
*/
            //pTime = M_FsmEvtGet(tagTime);
            
            pTime = M_IsrEvtGet(tagTime);            
            dwErr = M_FsmPubEvt(pTime, pTimer->dwType);
        }        
    }
            
    return dwErr;
}



