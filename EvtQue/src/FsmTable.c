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

#include "PhiloDemo.h"


/*================================================================================
 * ״̬������
 *================================================================================*/


// ����״̬
typedef enum _TBL_FORK_STATE_
{
    EN_TBL_FORK_FREE = 0,
    EN_TBL_FORK_USE,
    
}enTblFork;

// ����˽�б���
typedef struct
{
    enTblFork   enForks[CN_PHILO_NUM];
        
}tagTblObj;

//  ����˽�б���
tagTblObj g_tTbl;

/*================================================================================
 * 
 *================================================================================*/

// �����������ɵ��¼�����
#define  CN_EVT_BUF_TBL   ( 20  )
#define  CN_ISR_BUF_TBL   ( 20  )

// ������������
tagEvent  *ptEvtBufTbl[CN_EVT_BUF_TBL];
tagEvent  *ptIsrBufTbl[CN_ISR_BUF_TBL];

// ���ĵ��¼���
const UINT32 g_dwSubTableTbl[]={
    EN_PUB_PHILO_GET,
    EN_PUB_PHILO_PUT,

};
const UINT32 CN_EVT_REG_TBL = sizeof(g_dwSubTableTbl)/sizeof(UINT32); 

/*================================================================================
 * ״̬��ִ��
 *================================================================================*/
static UINT32 FsmTbl_State(tagEvent *ptEvt,tagFsmObj *pMe);


static UINT32 FsmTbl_Init(tagFsmObj *pMe)
{
    UINT32        dwLoop;
    tagTblObj     *ptTbl;

    // ��ȡ˽�б���ָ��
    ptTbl = M_FsmGetPvt(tagTblObj);

    // ˽�б�����ʼ��
    for(dwLoop=0;dwLoop<CN_PHILO_NUM;dwLoop++)
    {
        ptTbl->enForks[dwLoop] = EN_TBL_FORK_FREE;
    }

    // ����״̬��״̬
    M_FsmSet(FsmTbl_State, "STATE");


    return 0;
} 


static UINT32 FsmTbl_State(tagEvent *ptEvt,tagFsmObj *pMe)
{
    UINT32          dwIndx1,dwIndx2;
    tagPhiloGet     *ptEvtGet;
    tagPhiloPut     *ptEvtPut;
    tagTblAccept    *ptAccept;
    tagTblRefuse    *ptRefuse;
    tagTblObj       *ptTbl;

    // ��ȡ˽�б���ָ��
    ptTbl = M_FsmGetPvt(tagTblObj);

    // �¼�����״̬��
    switch(ptEvt->dwType)
    {
        case EN_PUB_PHILO_GET:

            ptEvtGet = (tagPhiloGet *)ptEvt;
            
            dwIndx1 = ptEvtGet->dwForks[0];
            dwIndx2 = ptEvtGet->dwForks[1];

            if((ptTbl->enForks[dwIndx1]==EN_TBL_FORK_FREE)
             &&(ptTbl->enForks[dwIndx2]==EN_TBL_FORK_FREE))
            {
                ptTbl->enForks[dwIndx1] = EN_TBL_FORK_USE;
                ptTbl->enForks[dwIndx2] = EN_TBL_FORK_USE;

                ptAccept = M_FsmEvtGet(tagTblAccept);
                ptAccept->dwPhilo = ptEvtGet->dwForks[0];

                M_FsmPubEvt(ptAccept, EN_PUB_TBL_ACCEPT);

            }
            else
            {
                ptRefuse= M_FsmEvtGet(tagTblRefuse);
                ptRefuse->dwPhilo = ptEvtGet->dwForks[0];

                M_FsmPubEvt(ptRefuse, EN_PUB_TBL_REFUSE);

            }

            break;

        case EN_PUB_PHILO_PUT:

            ptEvtPut = (tagPhiloPut *)ptEvt;
            
            dwIndx1 = ptEvtPut->dwForks[0];
            dwIndx2 = ptEvtPut->dwForks[1];

            ptTbl->enForks[dwIndx1] = EN_TBL_FORK_FREE;
            ptTbl->enForks[dwIndx2] = EN_TBL_FORK_FREE;
                            
            break;
 
        default:        
            break;      
    }

    return 0;
} 

void FsmBuild_Table(void)
{
    UINT32  dwFsm;

    // ����״̬�����
    dwFsm = EN_FSM_TBL;

    // ״̬������ 
    FsmBuilder(dwFsm, (void *)&ptEvtBufTbl[0], CN_EVT_BUF_TBL,\
                      (void *)&ptIsrBufTbl[0], CN_ISR_BUF_TBL);

    // �¼����� 
    EvtSubscriber(dwFsm, (UINT32 *)&g_dwSubTableTbl[0], CN_EVT_REG_TBL);


    // ��ʼ״̬����
    FsmConstructor(dwFsm, "Table", (FUNCPTR)FsmTbl_Init, (void *)&g_tTbl);
        

    return;
} 

