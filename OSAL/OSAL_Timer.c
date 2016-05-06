/******************************************************************************
* File       : OSAL_Timer.c
* Function   : Provide timer service.
* description: To be done.          
* Version    : V1.00
* Author     : Ian
* Date       : 6th May 2016
* History    :  No.  When           Who           What
*               1    06/May/2016    Ian           Create
******************************************************************************/

#include "type_def.h"
#include "common_head.h"
#include "OSAL.h"
#include "OSAL_Timer.h"

static T_TIMER_NODE *sg_ptTmHead = NULL;
static T_TIMER_NODE *sg_ptTmTail = NULL;

static PF_TIMER_SRC  sg_pfSysTm  = NULL;



/******************************************************************************
* Name       : uint8 Osal_Timer_Init(PF_TIMER_SRC pfSysTm)
* Function   : Init OSAL timer
* Input      : PF_TIMER_SRC pfSysTm   Funtion which returns system time
* Output:    : None
* Return     : SW_OK   Successful.
*              SW_ERR  Failed.
* description: To be done.
* Version    : V1.00
* Author     : Ian
* Date       : 6th May 2016
******************************************************************************/
uint8 Osal_Timer_Init(PF_TIMER_SRC pfSysTm)
{   /* Check if the input function is NULL or NOT */
    if (NULL == pfSysTm)
    {   /* If invalid, return error */
        return SW_ERR;    
    }
    /* If the input function is OK, then go on */

    sg_pfSysTm = pfSysTm;      /* Save the system time function for further using */

    return SW_OK;
}

/******************************************************************************
* Name       : uint8 Osal_Timer_Init(PF_TIMER_SRC pfSysTm)
* Function   : Init OSAL timer
* Input      : PF_TIMER_SRC pfSysTm   Funtion which returns system time
* Output:    : None
* Return     : SW_OK   Successful.
*              SW_ERR  Failed.
* description: To be done.
* Version    : V1.00
* Author     : Ian
* Date       : 6th May 2016
******************************************************************************/
T_TIMER_NODE* Osal_Timer_Add()
{
    T_TIMER_NODE* ptNode;

    /* ����ͷ����Ѿ����� */
    ptNode = (T_TIMER_NODE*)malloc(sizeof(T_TIMER_NODE)); /* ���붨ʱ����� */
    if (NULL == ptNode)
    {
        return NULL; /* ����Ƿ�����ɹ� */
    }

    /* �������ɹ� */
    ptNode->next              = NULL;                   /* �¸�����ַ�ÿ� */

    if(NULL == sg_ptTmTail)
    {
        sg_ptTmHead = ptNode;
    }
    else
    {
        sg_ptTmTail->Next = ptNode;
    }
    sg_ptTmTail = ptNode;

    return ptNode;

}


/******************************************************************************
* Name       : uint8 Osal_Timer_Init(PF_TIMER_SRC pfSysTm)
* Function   : Init OSAL timer
* Input      : PF_TIMER_SRC pfSysTm   Funtion which returns system time
* Output:    : None
* Return     : SW_OK   Successful.
*              SW_ERR  Failed.
* description: To be done.
* Version    : V1.00
* Author     : Ian
* Date       : 6th May 2016
******************************************************************************/
T_TIMER_NODE* Osal_Timer_Start(uint8 u8TaskID, uint16 u16Evt, uint16 u16Cnt, uint16 u16TmOut)
{
    T_TIMER_NODE* ptNode;

    ptNode = Osal_Timer_Add();
    if (NULL == ptNode)
    {
        return NULL; /* ����Ƿ�����ɹ� */
    }

    /* �������ɹ� */
    ptNode->tTimer.u16TmStart = sg_pfSysTm();           /* ��ȡ��ʱ��ʼʱ�� */
    ptNode->tTimer.u16TmNow   = sg_pfSysTm();           /* ��ȡ��ǰʱ�� */
    ptNode->tTimer.u16TmOut   = u16TmOut;                      /* �Ѿ�����ʱ�� */
    ptNode->tTimer.u8Cnt      = u16Cnt;
    ptNode->tTimer.u16Evt     = u16Evt;
    ptNode->tTimer.u8TaskID   = u8TaskID;

    return ptNode;

}


/*************************************************************************
* �������ƣ�int KillTimer(T_TIMER_NODE* ptNode)
* ����˵����ɾ����ʱ�����
* ���������T_TIMER_NODE* ptNode ��ʱ������ַ
* �����������
* �� �� ֵ��SW_ERR: ����ʧ��
           SW_OK �����ɹ�
* ����˵������
**************************************************************************/
T_TIMER_NODE* Osal_Timer_Del(T_TIMER_NODE* ptNode)
{
    T_TIMER_NODE* ptFind;

    if (NULL == ptNode)
    {
        return NULL; /* ��鶨ʱ������Ƿ�Ϊ�� */
    }

    if(ptNode == sg_ptTmHead)
    {
        if(ptNode == sg_ptTmTail)
        {
            sg_ptTmHead = NULL;
            sg_ptTmTail = NULL;
        }
        else
        {
            sg_ptTmHead = sg_ptTmHead->ptNext;
        }
        free(ptNode);
        return ptNode;
    }    

    ptFind = sg_ptTmHead; /* ���ҵ�ͷ��� */
    while (ptFind) /* �������ĩβ��� */
    {
        if (ptFind->next == ptNode)      /* �����һ����Ƿ�Ϊ��ɾ����� */
        {
            ptFind->next = ptNode->next; /* �����������½�� */
            free(ptNode);                /* ɾ����� */
            if(NULL == ptFind->next)
            {
                sg_ptTmTail = ptFind;
            }
            return SW_OK;                /* �����ɹ����˳����� */
        }
        ptFind = ptFind->next;           /* ����������һ��� */
    }
    return NULL;                     /* δ�ҵ�������ʧ�� */
}


T_TIMER_NODE* Osal_Timer_Find(T_TIMER_NODE* ptNode)
{
    T_TIMER_NODE *ptFind = sg_ptTmHead;

    while(ptFind)
    {
        if(ptFind == ptNode)
        {
            break;
        }
        ptFind = ptFind->ptNext;
    }
    return ptFind;
}


/*************************************************************************
* �������ƣ�int ResetTimer(T_TIMER_NODE* ptNode)
* ����˵����������ʱ�����
* ���������T_TIMER_NODE* ptNode ��ʱ������ַ
* �����������
* �� �� ֵ��SW_ERR: ����ʧ��
           SW_OK �����ɹ�
* ����˵������
**************************************************************************/
T_TIMER_NODE* Osal_Timer_Restart(T_TIMER_NODE* ptNode)
{
    if (NULL == ptNode)
    {
        return NULL;                /* ��鶨ʱ������Ƿ�Ϊ�� */
    }

    if(NULL == Osal_Timer_Find())
    {
        return NULL;
    }

    ptNode->tTimer.start = sg_pfSysTm(); /* ���¶�ʱ����ʼʱ�� */
    return ptNode;                       /* �����ɹ� */
}


/*************************************************************************
* �������ƣ�int ProcessTimer(void)
* ����˵�������¶�ʱ�����
* �����������
* �����������
* �� �� ֵ��SW_ERR: ����ʧ��
            SW_OK �����ɹ�
* ����˵������
**************************************************************************/
uint8 Osal_Timer_Process(void)
{
    T_TIMER_NODE* ptFind;
    T_TIMER_NODE* ptNodeFree;
    if (NULL == sg_ptTmHead)
    {
        return SW_OK; /* û�ж�ʱ����Ҫ���� */
    }
    ptFind = sg_ptTmHead;          /* �ҵ���һ����Ч��� */
    while(ptFind)                         /* �������ĩβ��� */
    {
        ptFind->tTimer.now = sg_pfSysTm(); /* ����ʱ�� */

        /* ����˿�ʱ������ʼʱ���ʱ��� */    
        if((ptFind->tTimer.now - ptFind->tTimer.start) >= ptFind->tTimer.timeout)          /* ���ʱ����ڵ����趨�ļ�ʱʱ�� */
        {
            Osal_Event_Set(ptFind->tTimer.u8TaskID,ptFind->tTimer.u16Evt);

            if(ptFind->tTimer.u16Cnt)
            {
                if(ptFind->tTimer.u16Cnt != FOREVER_PERIOD)
                {
                    ptFind->tTimer.u16Cnt--;
                }
                
                Osal_Timer_Restart(ptFind);                              /* ����������Դ�����������ʱ�� */
            }
            else
            {                                                    /* ����ǵ��δ�����ɾ����ʱ�� */
                ptNodeFree = ptFind;
                ptFind = ptFind->next;
                Osal_Timer_Del(ptNodeFree);
                continue;
            }            
        }
        ptFind = ptFind->next;                                   /* ����������һ����ʱ����� */
    }
    return SW_OK;                                                /* �����ɹ� */
}

/* end of file */

