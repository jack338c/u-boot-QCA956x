
/*! Copyright(c) 1996-2009 Shenzhen TP-LINK Technologies Co. Ltd.
 * \file    nm_lib.c
 * \brief    api functions for NVRAM manager.
 * \author    Meng Qing
 * \version    1.0
 * \date    24/04/2009
 */


/**************************************************************************************************/
/*                                      CONFIGURATIONS                                            */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      INCLUDE_FILES                                             */
/**************************************************************************************************/
#include <common.h>
#include <malloc.h>
#include "nm_api.h"
#include "nm_lib.h"
#include "sysProductInfo.h"

/**************************************************************************************************/
/*                                      DEFINES                                                   */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      TYPES                                                     */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      EXTERN_PROTOTYPES                                         */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      LOCAL_PROTOTYPES                                          */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      VARIABLES                                                 */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      LOCAL_FUNCTIONS                                           */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      PUBLIC_FUNCTIONS                                          */
/**************************************************************************************************/

/*******************************************************************
 * Name        : nm_api_writePtnToNvram
 * Abstract    : write the value of a partition to NVRAM.
 * Input    : 
 * Output    : 
 * Return    : OK/ERROR
 */
int nm_api_writePtnToNvram(char *name, char *buf, int len)
{
    int ret;
    char *ptr = buf;
    NM_PTN_ENTRY *ptnEntry;

    NM_SEM_TAKE(g_nmReadWriteLock, WAIT_FOREVER);
    
    /* get runtime-partition-entry by name */
    if ((ptnEntry = nm_lib_ptnNameToEntry(g_nmPtnStruct, name)) == NULL)
    {
        NM_ERROR("partition name not found.\r\n");
        goto error;
    }

    /* some partitions don't have partition head(partition-length & checksum) */
    if ((strncmp(NM_PTN_NAME_FS_UBOOT, ptnEntry->name, NM_PTN_NAME_LEN) == 0)
        || (strncmp(NM_PTN_NAME_FACTORY_BOOT, ptnEntry->name, NM_PTN_NAME_LEN) == 0)
        || (strncmp(NM_PTN_NAME_OS_IMAGE, ptnEntry->name, NM_PTN_NAME_LEN) == 0)
        || (strncmp(NM_PTN_NAME_FILE_SYSTEM, ptnEntry->name, NM_PTN_NAME_LEN) == 0)
        || (strncmp(NM_PTN_NAME_RADIO, ptnEntry->name, NM_PTN_NAME_LEN) == 0)
        || (strncmp(NM_PTN_NAME_RADIO_BACKUP, ptnEntry->name, NM_PTN_NAME_LEN) == 0)
        || (strncmp(NM_PTN_NAME_LOG, ptnEntry->name, NM_PTN_NAME_LEN) == 0))
    {
        if (len > ptnEntry->size)
        {
            NM_ERROR("no enough space in this partition.");
            goto error;
        }

        ret = nm_lib_writeHeadlessPtnToNvram((char *)ptnEntry->base, ptr, len);
    }
    else
    {
        if (len > (ptnEntry->size - sizeof(int) - sizeof(int)))
        {
            NM_ERROR("no enough space in this partition.");
            goto error;
        }
        
        ret = nm_lib_writePtnToNvram((char *)ptnEntry->base, ptr, len);
    }

    NM_SEM_GIVE(g_nmReadWriteLock);
    return ret;
    
error:
    NM_SEM_GIVE(g_nmReadWriteLock);
    return -1;
}


#if 1
/*******************************************************************
 * Name        : nm_api_readPtnFromNvram
 * Abstract    : read value of a partition from NVRAM.
 * Input    : 
 * Output    : 
 * Return    : OK/ERROR
 */
int nm_api_readPtnFromNvram(char *name, char *buf, int len)
{
    int ret;
    int readSize = 0;
    char *ptr = buf;
    NM_PTN_ENTRY *ptnEntry;

    NM_SEM_TAKE(g_nmReadWriteLock, WAIT_FOREVER);
        
    /* get runtime-partition-entry by name */
    if ((ptnEntry = nm_lib_ptnNameToEntry(g_nmPtnStruct, name)) == NULL)
    {
        NM_ERROR("partition name not found(name:%s).\r\n", name);
        goto error;
    }

    readSize = ptnEntry->size > len ? len : ptnEntry->size;
    /* some partitions don't have partition head(partition-length & checksum) */
    if ((strncmp(NM_PTN_NAME_FS_UBOOT, ptnEntry->name, NM_PTN_NAME_LEN) == 0)
        || (strncmp(NM_PTN_NAME_FACTORY_BOOT, ptnEntry->name, NM_PTN_NAME_LEN) == 0)
        || (strncmp(NM_PTN_NAME_OS_IMAGE, ptnEntry->name, NM_PTN_NAME_LEN) == 0)
        || (strncmp(NM_PTN_NAME_FILE_SYSTEM, ptnEntry->name, NM_PTN_NAME_LEN) == 0)
        || (strncmp(NM_PTN_NAME_RADIO, ptnEntry->name, NM_PTN_NAME_LEN) == 0)
        || (strncmp(NM_PTN_NAME_RADIO_BACKUP, ptnEntry->name, NM_PTN_NAME_LEN) == 0)
        || (strncmp(NM_PTN_NAME_LOG, ptnEntry->name, NM_PTN_NAME_LEN) == 0))
    {
        ret = nm_lib_readHeadlessPtnFromNvram((char *)ptnEntry->base, ptr, len);
    }
#if 0    
    else if (strncmp(NM_PTN_NAME_USER_CFG, ptnEntry->name, NM_PTN_NAME_LEN) == 0
             || strncmp(NM_PTN_NAME_DEFAULT_CFG, ptnEntry->name, NM_PTN_NAME_LEN) == 0
             || strncmp(NM_PTN_NAME_PROFILE_CFG, ptnEntry->name, NM_PTN_NAME_LEN) == 0)
    {
        ret = nm_lib_readCfgPtnFromNvram((char *)ptnEntry->base, ptr, len);
    }
#endif    
    else
    {
        ret = nm_lib_readPtnFromNvram((char *)ptnEntry->base, ptr, len);
    }

    NM_SEM_GIVE(g_nmReadWriteLock);
    return ret;
    
error:
    NM_SEM_GIVE(g_nmReadWriteLock);
    return -1;
}

#endif


/*******************************************************************
 * Name        : nm_api_writeToNvram
 * Abstract    : write value from a buffer to NVRAM.
 * Input    : 
 * Output    : 
 * Return    : OK/ERROR
 */
int nm_api_writeToNvram(char *base, char *buf, int len)
{
    int ret;
    
    NM_SEM_TAKE(g_nmReadWriteLock, WAIT_FOREVER);
    ret = nm_lib_writeHeadlessPtnToNvram(base, buf, len);
    NM_SEM_GIVE(g_nmReadWriteLock);

    return ret;
}



/*******************************************************************
 * Name        : nm_api_readFromNvram
 * Abstract    : read value from NVRAM to a buffer.
 * Input    : 
 * Output    : 
 * Return    : OK/ERROR
 */
int nm_api_readFromNvram(char *base, char *buf, int len)
{
    int ret;
    
    NM_SEM_TAKE(g_nmReadWriteLock, WAIT_FOREVER);
    ret = nm_lib_readHeadlessPtnFromNvram(base, buf, len);
    NM_SEM_GIVE(g_nmReadWriteLock);

    return ret;
}

/*******************************************************************
 * Name        : nm_api_erasePtn
 * Abstract    : erase a partition.
 * Input    : partition name
 * Output    : 
 * Return    : OK/ERROR
 */
int nm_api_erasePtn(char *name)
{
    char *eraseBuf = NULL;
    NM_PTN_ENTRY *ptnEntry;
    
    /*NM_SEM_TAKE(g_nmReadWriteLock, WAIT_FOREVER);*/
    /* get runtime-partition-entry by name */
    if ((ptnEntry = nm_lib_ptnNameToEntry(g_nmPtnStruct, name)) == NULL)
    {
        NM_ERROR("partition name not found.\r\n");
        return ERROR;
    }
    
    eraseBuf = (char *)malloc(ptnEntry->size);
    NM_DEBUG("eraseBuf size = %x \n", ptnEntry->size);
    if (eraseBuf == NULL)
    {
        NM_ERROR("malloc failed! \n");
        return ERROR;
    }

    /* reset partition content to 0xff */
    memset(eraseBuf, 0xff, ptnEntry->size);

    
    if(nm_lib_writeHeadlessPtnToNvram((char *)ptnEntry->base, eraseBuf, ptnEntry->size) < 0)
    {
        NM_ERROR("erase failed! \n");
        free(eraseBuf);
        return ERROR;
    }

    free(eraseBuf);
    /*NM_SEM_GIVE(g_nmReadWriteLock);*/
    return OK;
}

/*******************************************************************
 * Name        : nm_api_setIntegerFlag
 * Abstract    : set image integer flag.
 * Input    : N/A 
 * Output    : 
 * Return    : 0 if check ok, -1 if check fail
 */
int nm_api_setIntegerFlag(unsigned int flag)
{
    int len = -1;
    NM_PTN_EXTRA_PARA sysParaData;

    memset(&sysParaData, 0, sizeof(NM_PTN_EXTRA_PARA));
    
    NM_INFO("set integer flag to %d.\r\n", flag);
#if 0
    //get partition table
    if (0 != nm_init())   
    {
        NM_ERROR("set integer flag partition init fail.");
        return ERROR;
    }

    //get the sys para data stored in flash
    len = nm_api_readPtnFromNvram(NM_PTN_NAME_EXTRA_PARA, 
                    (char *)&sysParaData, sizeof(NM_PTN_EXTRA_PARA));
    if (len < 0)   
    {
        NM_ERROR("read integer flag from partition failed.");
        return ERROR;
    }
#endif
    //set the integer flag
    sysParaData.integerFlag = flag;
    len = nm_api_writePtnToNvram(NM_PTN_NAME_EXTRA_PARA, 
                    (char *)&sysParaData, sizeof(NM_PTN_EXTRA_PARA));

    if (len < 0)   
    {
        NM_ERROR("write integer flag to partition failed.");
        return ERROR;
    }

    return OK;
}

/*******************************************************************
 * Name        : nm_api_checkInteger
 * Abstract    : Process image integer check.
 * Input    : N/A 
 * Output    : 
 * Return    : 0 if check ok, -1 if check fail
 */
int nm_api_checkInteger(void)
{

    int len = -1;
    NM_PTN_EXTRA_PARA sysParaData;
#if 0
    //get partition table
    if (0 != nm_init())   
    {
        NM_ERROR("factory boot check integer partition init fail.");
        return ERROR;
    }

    len = nm_api_readPtnFromNvram(NM_PTN_NAME_EXTRA_PARA, 
                            (char *)&sysParaData, sizeof(NM_PTN_EXTRA_PARA));
#endif

    len = nm_lib_readPtnFromNvram((char *)(NM_EXTRA_PARA_BASE), 
                                                                  (char *)&sysParaData, sizeof(NM_PTN_EXTRA_PARA));
    if (len < 0)   
    {
        NM_ERROR("factory boot check integer read flag partition fail.");
        return ERROR;
    }

    NM_DEBUG("bootflag[%08x], interflag[%08x].", sysParaData.dbootFlag, sysParaData.integerFlag);

    //if integer flag is 1, mean upgrade no error
    if (sysParaData.integerFlag != NM_FWUP_IS_INTEGER)
    {
        NM_ERROR("factory boot check integer flag is not 1.");
        return ERROR;
    }
    
    NM_INFO("factory boot check integer ok.\r\n");

    return OK;
}

/**************************************************************************************************/
/*                                      GLOBAL_FUNCTIONS                                          */
/**************************************************************************************************/


