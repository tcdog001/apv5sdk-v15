/*
*                Copyright 2003, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/

/*!
 * \file    idList.c
 * \brief   station id and 802.11 aid management
 */

/*=============================================================================
 *                               INCLUDE FILES
 *=============================================================================
 */

#include "wltypes.h"
#include "List.h"
#include "osif.h"
#include "buildModes.h"

UINT32 AssocStationsCnt= 0;
/*=============================================================================
 *                                DEFINITIONS
 *=============================================================================
*/


typedef struct IdListElem_t

{
    struct IdListElem_t *nxt;
    struct IdListElem_t *prv;
    UINT16 Id;
}
IdListElem_t;


/*=============================================================================
 *                         IMPORTED PUBLIC VARIABLES
 *=============================================================================
 */
/*=============================================================================
 *                          MODULE LEVEL VARIABLES
 *=============================================================================
 */

IdListElem_t AidList[MAX_AID + 1];

IdListElem_t *StnIdList= NULL;


static List FreeStaIdList;
static List StaIdList;

static List FreeAIDList;
static List AIDList;

extern void ListPutItemFILO(List *me, ListItem *Item);
/*============================================================================= 
 *                   PRIVATE PROCEDURES (ANSI Prototypes) 
 *=============================================================================
 */

/*============================================================================= 
 *                         CODED PROCEDURES 
 *=============================================================================
 */

/*
 *Function Name:InitAidList
 *
 *Parameters:
 *
 *Description:
 *
 *Returns:
 *
 */
void InitAidList(void)
{
    UINT32 i;
    ListInit(&FreeAIDList);
    ListInit(&AIDList);
    for (i = 0; i < MAX_AID; i++)
    {
        AidList[i].nxt = NULL;
        AidList[i].prv = NULL;
		AidList[i].Id = MAX_AID - i;
        ListPutItemFILO(&FreeAIDList, (ListItem*)(AidList+i));
    }
}

/*
 *Function Name:AssignAid
 *
 *Parameters:
 *
 *Description:
 *
 *Returns:
 *
 */
UINT32 AssignAid(void)
{
    ListItem *tmp;
    IdListElem_t *tmp1;
    tmp = ListGetItem(&FreeAIDList); 
    if (tmp)
    {
        tmp1 = (IdListElem_t *)tmp;
        ListPutItemFILO(&AIDList,tmp);
        AssocStationsCnt++;
        return tmp1->Id;
    }

    return 0;   /* List is empty */
}

/*
 *Function Name:FreeAid
 *
 *Parameters:
 *
 *Description:
 *
 *Returns:
 *
 */

void FreeAid(UINT32 Aid)
{
    ListItem *search;
    IdListElem_t *search1;
    search = AIDList.head;
    while (search)
    {
        search1 = (IdListElem_t *)search;
        if ((search1->Id == Aid))
        {
            ListPutItemFILO(&FreeAIDList,ListRmvItem(&AIDList,search));
            AssocStationsCnt--;
            return ;
        }
        search = search->nxt;
    }
}
Status_e ResetAid(UINT16 StnId, UINT16 Aid)
{
   return FAIL;
}
/*
 *Function Name:InitStnId
 *
 *Parameters:
 *
 *Description:
 *
 *Returns:
 *
 */

WL_STATUS InitStnIdList(int max_stns)
{
    UINT32 i;
    ListInit(&FreeStaIdList);
    ListInit(&StaIdList);
    if (StnIdList == NULL)
    {
		StnIdList = malloc((max_stns + 1)*sizeof(IdListElem_t));
	if (StnIdList == NULL) 
		return (OS_FAIL);
	memset(StnIdList, 0, (max_stns + 1)*sizeof(IdListElem_t));
    }
    for (i = 0; i < max_stns; i++)
    {
        StnIdList[i].nxt = NULL;
        StnIdList[i].prv = NULL;
        StnIdList[i].Id = max_stns - i;
        ListPutItemFILO(&FreeStaIdList, (ListItem*)(StnIdList+i));
    }
	return(OS_SUCCESS);
}

void StnIdListCleanup(void)
{
    if (StnIdList){
		free(StnIdList);
	StnIdList = 0;
    	}
}

/*
 *Function Name:AssignStnId
 *
 *Parameters:
 *
 *Description:
 *
 *Returns:
 *
 */

UINT32 AssignStnId(void)
{
    ListItem *tmp;
    IdListElem_t *tmp1;
    tmp = ListGetItem(&FreeStaIdList); 
    if (tmp)
    {
        tmp1 = (IdListElem_t *)tmp;
        ListPutItemFILO(&StaIdList,tmp);
        return tmp1->Id;
    }
    return 0;   /* List is empty */
}

/*
 *Function Name:FreeStnId
 *
 *Parameters:
 *
 *Description:
 *
 *Returns:
 *
 */

void FreeStnId(UINT32 StnId)
{
    ListItem *search;
    IdListElem_t *search1;

    search = StaIdList.head;
    while (search)
    {
        search1 = (IdListElem_t *)search;
        if ((search1->Id == StnId))
        {
            ListPutItemFILO(&FreeStaIdList,ListRmvItem(&StaIdList,search));
            return ;
        }
        search = search->nxt;
    }

}

