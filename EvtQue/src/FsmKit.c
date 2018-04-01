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
#include "FsmKit.h"
#include "Demo.h"

tagFsmObj   g_tFsmTable[EN_FSM_END];            // ״̬���ܱ� 
void *      g_pPvt[EN_FSM_END][CN_EVT_JUMP];    // ��״̬������д�� 

//===========================================================
// ��������: ״̬������ 
//===========================================================
void FsmRunSingleThread(void)
{
    UINT32      dwLoop;
    
    // ״̬������ 
    for(dwLoop=0;dwLoop<EN_FSM_END;dwLoop++)
    {
        if(FsmObjRdy(&g_tFsmTable[dwLoop]))
        {
            FsmObjRun(&g_tFsmTable[dwLoop]);
            break;      
        }       
    }

    return;

} 

//===========================================================
// ��������: ״̬������ 
//===========================================================
void FsmRunMultiThread(UINT32 dwFmsNo)
{
    
    // ״̬������ 
    if(FsmObjRdy(&g_tFsmTable[dwFmsNo]))
    {
        FsmObjRun(&g_tFsmTable[dwFmsNo]);
    }  

    return;

} 

//===========================================================
// ��������: ״̬����ʼ��
//===========================================================
UINT32 FsmObjInitAll(void)
{
    FsmObjInit(&g_tFsmTable[0], EN_FSM_END);
    
    return 0;
} 

//===========================================================
// ��������: ״̬����ʼ��
//===========================================================
UINT32 FsmPrivtInitAll(void)
{
    FsmPrivtInit(&g_tFsmTable[0], EN_FSM_END);
    
    return 0;
} 

//===========================================================
// ��������: ״̬����ʼ��
//===========================================================
UINT32 FsmObjBuildAll(void)
{
#if(0)
    Fsm01_Build();
    Fsm02_Build();
#else

    FsmBuild_Philo();

    FsmBuild_Table();


#endif

    M_EvtRegInfo();

    return 0;
} 

//===========================================================
// ��������: ״̬������ 
//===========================================================
UINT32 FsmBuilder(UINT32 dwFsmNo,void *pFsmQue,UINT32 dwFsmNum,void *pIsrQue,UINT32 dwIsrNum)
{
    UINT32      dwErr;
    tagFsmObj   *pMe;

   
    // ��� 
    if(dwFsmNo>=EN_FSM_END)
    {
        printf("FsmBuilder:FsmNo Err\n");
        return 1;
    }

    // ״̬��ָ���ʼ��
    pMe = &g_tFsmTable[dwFsmNo];

    // �¼���������ʼ��
    if((dwErr = FsmObjBuild(&pMe->tFsmQue, pFsmQue, dwFsmNum)))
    {
        printf("FsmBuilder:EvtQueInit dwErr=%d\n",dwErr);
    }
    
    if((dwErr = FsmObjBuild(&pMe->tIsrQue, pIsrQue, dwIsrNum)))
    {
        printf("FsmBuilder:IsrQueInit dwErr=%d\n",dwErr);
    }
    
    if((dwErr = FsmObjBuild(&pMe->tJmpQue, g_pBuf[dwFsmNo], CN_EVT_JUMP)))
    {
        printf("FsmBuilder:PvtQueInit dwErr=%d\n",dwErr);
    }   
   
    return dwErr;    
}

//===========================================================
// ��������: ״̬���任 
//===========================================================
UINT32 FsmConstructor(UINT32 dwFsmNo,char *cName,FUNCPTR pInit,void *pData)
{
    tagFsmObj   *pMe;

   
    // ��� 
    if(dwFsmNo>=EN_FSM_END)
    {
        printf("FsmBuilder:FsmNo Err\n");
        return 1;
    }

    // ״̬��ָ���ʼ��
    pMe = &g_tFsmTable[dwFsmNo];

    pMe->dwFsm = dwFsmNo;
    pMe->cName = cName;
    
    pMe->tPrivate.pInit = pInit;
    pMe->tPrivate.pData = pData;
       
    return 0;
}


