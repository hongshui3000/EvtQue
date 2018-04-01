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


//===========================================================
// ��������: �������
//===========================================================
inline UINT32 EvtTimerDelay(UINT32 dwMicroSecond)
{    
    //˯��΢��      
    return usleep(dwMicroSecond);  
}

//===========================================================
// ��������: �������
//===========================================================
inline UINT32 EvtTimerSleep(UINT32 dwSecond)
{
    //˯����  
    return sleep(dwSecond);    
}


//===========================================================
// ��������: ��ȡ��ǰ�������
//===========================================================
inline UINT32 EvtTimerNow(void)
{           
    // ��ȡ��ǰʱ��
    struct timeval tv;
    gettimeofday(&tv,NULL);

    // ��ǰ�������
    return (tv.tv_sec*1000 + tv.tv_usec/1000);
}


//===========================================================
// ��������: ��ʱ����ʼ��
//===========================================================
UINT32 EvtTimerInit(tagEvtTimer *pMe, UINT32 dwNum)
{
    UINT32      dwLoop;
    tagEvtTimer *pTimer;

    for(pTimer=&pMe[0],dwLoop=0; dwLoop<dwNum; pTimer++,dwLoop++)
    {
        // ����ʱ������������ֵ��Ϊֹͣ��־
        pTimer->dwFlag = dwNum;

        // ���ö�ʱ����Ӧ���¼�����
        pTimer->dwType = dwLoop;
    }

    return 0;    
}

//===========================================================
// ��������: ��ʱ������
//===========================================================
UINT32 EvtTimerStart(tagEvtTimer *pMe,UINT32 dwTimer,UINT32 dwAttr,UINT32 dwEnd,UINT32 dwDlt)
{

    pMe->dwFlag = dwTimer;  
    pMe->dwAttr = dwAttr; 
       
    pMe->dwEnd  = dwEnd; 
    pMe->dwDlt  = dwDlt; 
        
    return 0;
}

//===========================================================
// ��������: ��ʱ��ֹͣ
//===========================================================
UINT32 EvtTimerStop(tagEvtTimer *pMe)
{

    pMe->dwFlag = CN_EVT_TIMER;  
    pMe->dwAttr = 0; 
       
    pMe->dwEnd  = 0; 
    pMe->dwDlt  = 0 ; 
        
    return 0;
}

//===========================================================
// ��������: ͳһת��Ϊ���뵥λ����
//===========================================================
UINT32 EvtTimerCnvt(UINT32 dwType, UINT32 dwCnt, UINT32 *pdwEnd, UINT32 *pdwDlt)
{      
    UINT32      dwEnd, dwDlt;

    // ���
    if((!pdwEnd)||(!pdwDlt))
    {
        return 1;
    }

    
    // ��ȡ��ǰʱ��
    struct timeval tv;
    gettimeofday(&tv,NULL);

    // ͳһת��Ϊ���뵥λ����
    switch(dwType)
    {
        case EN_TIMER_MS:
            dwDlt = dwCnt;
            dwEnd = dwDlt + tv.tv_sec*1000 + tv.tv_usec/1000;
            break;
        case EN_TIMER_S:
            dwDlt = dwCnt*1000;
            dwEnd = dwDlt + tv.tv_sec*1000;
            break;
        case EN_TIMER_M:      
            dwDlt = dwCnt*1000*60;    
            dwEnd = dwDlt + tv.tv_sec*1000;
            break;
        case EN_TIMER_H:           
            dwDlt = dwCnt*1000*60*60;  
            dwEnd = dwDlt + tv.tv_sec*1000;
            break;
        case EN_TIMER_D:           
            dwDlt = dwCnt*1000*60*60*24;
            dwEnd = dwDlt + tv.tv_sec*1000;
            break;
        
        default:
            return 3;    
    }         
         
    // ת�����ʱ��
    *pdwEnd = dwEnd;
    *pdwDlt = dwDlt;

    return 0;
} 

//===========================================================
// ��������: ��ʱ������
//===========================================================
UINT32 EvtTimerUpdt(tagEvtTimer *pMe, UINT32 dwNow)
{
    UINT32      dwRst = 0;


    // ��ʱ������
    switch(pMe->dwAttr)
    {
        case 0:
            // ����Ƿ�ʱ
            if(dwNow > pMe->dwEnd)
            {
                // ��ʱ��ֹͣ��־
                pMe->dwFlag = CN_EVT_TIMER;

                dwRst = 1;
            }
            break;
        case 1:
            // ����Ƿ�ʱ

                // ���¶�ʱ��������ʱ
            
            break;
        default:
            break;               
    }

            
    return dwRst;
}

