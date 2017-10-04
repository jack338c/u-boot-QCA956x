
/*! Copyright(c) 1996-2009 Shenzhen TP-LINK Technologies Co. Ltd.
 * \file    ipifcfg_cfg.h
 * \brief   Protos for ipnet ifconfig's config mod.
 * \author  Meng Qing
 * \version 1.0
 * \date    21/1/2009
 */

#ifndef NM_FWUP_H
#define NM_FWUP_H


#ifdef __cplusplus
extern "C"{
#endif


/**************************************************************************************************/
/*                                      CONFIGURATIONS                                            */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      INCLUDE_FILES                                             */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      DEFINES                                                   */
/**************************************************************************************************/
#define NM_FWUP_FRAGMENT_SIZE 0x10000
#define NM_FWUP_SUPPORT_LIST_NUM_MAX 16
#define NM_FWUP_SOFTWARE_VERSION_LEN    (256)

#define IMAGE_SIZE_LEN  (0x04)
#define IMAGE_SIZE_MD5  (0x10)
#define IMAGE_SIZE_PRODUCT  (0x1000)
#define IMAGE_SIZE_BASE (IMAGE_SIZE_LEN + IMAGE_SIZE_MD5 + IMAGE_SIZE_PRODUCT)
#define IMAGE_SIZE_FWTYPE	(IMAGE_SIZE_LEN + IMAGE_SIZE_MD5)
#define IMAGE_SIZE_RSA_SIG	(IMAGE_SIZE_FWTYPE + 0x11C)
#define IMAGE_LEN_RSA_SIG	(0x80)
#define IMAGE_CLOUD_HEAD_OFFSET (252)

#define IMAGE_SIZE_MAX  (IMAGE_SIZE_BASE + 0x800 + FLASH_SIZE * (1 << 20))
#define IMAGE_SIZE_MIN  (IMAGE_SIZE_BASE + 0x800)

#define	FW_TYPE_NAME_LEN_MAX	0x100

/**************************************************************************************************/
/*                                      TYPES                                                     */
/**************************************************************************************************/
typedef int STATUS;

typedef enum _FWUP_ERROR_TYPE
{
	NM_FWUP_ERROR_NONE 				 =  0,
    NM_FWUP_ERROR_NORMAL			 = -1,	/* inner error */
    NM_FWUP_ERROR_INVALID_FILE		 = -2,	/* invalid firmware file */
    NM_FWUP_ERROR_INCORRECT_MODEL	 = -3,	/* the firmware is not for this model */
    NM_FWUP_ERROR_BAD_FILE			 = -4,	/* an valid firmware, but something wrong in file */
    NM_FWUP_ERROR_UNSUPPORT_VER		 = -5,	/* firmware file not compatible with current version */
    NM_FWUP_ERROR_UNSUPPORT_BOOT_MOD = -6,	/* firmware file not compatible with boot mode */
} FWUP_ERROR_TYPE;

enum fw_type
{
    FW_TYPE_INVALID = 0x0,
    FW_TYPE_COMMON = 0x1,
    FW_TYPE_CLOUD = 0x2,
    FW_TYPE_MAX = 0xff
};

struct fw_type_option
{
	char *name;
	enum fw_type type;
	int (*func)(unsigned char *buf, int buf_len);
};

#define MD5_ALGORITHM 			0x05 
#define MD5_CHECKSUM_OFFSET		4
#define MD5_BLOCK_SIZE			32768	// 只计算 前32KB内容与secret的 MD5 checksum
#define MD5_DIGEST_LEN			16		// length of MD5 digest result
#define MD5_CALC_SIZE			(MD5_BLOCK_SIZE + MD5_DIGEST_LEN)
/*
char plcMd5Key[16] = 
{
	0xcc, 0x96, 0x28, 0xee, 0x8d, 0xfb, 0x21, 0xbb,
	0x3d, 0xef, 0x6c, 0xb5, 0x9f, 0x77, 0x4c, 0x7c
};
*/
typedef unsigned char   BYTE,*PBYTE;

typedef struct _MD5_HEADER
{
	BYTE checksum[MD5_DIGEST_LEN];
	BYTE fileLength[4];
}MD5_HEADER;

typedef struct _UPGRADE_FLAG
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
	BYTE upgradeAP	:1;
	BYTE upgradePLC :1;
	BYTE checksum	:1;
	BYTE reset		:1;
	BYTE nvmflag	:1;
	BYTE pibflag	:1;
	BYTE gglflag	:1;
	BYTE reserved4	:1;
#else
	BYTE reserved4	:1;
	BYTE gglflag	:1;
	BYTE pibflag	:1;
	BYTE nvmflag	:1;
	BYTE reset		:1;			/* 1: reset PLC after upgrade */
	BYTE checksum	:1;			/* 1: Check MD5 for PLC firmware */
	BYTE upgradePLC :1;			/* 1: Upgrade PLC	 0: Don't upgrade PLC */
	BYTE upgradeAP	:1;			/* 1: Upgrade AP	 0: Don't upgrade AP */
#endif
}UPGRADE_FLAG;

typedef struct _UPGRADE_FILE_HEADER
{
	MD5_HEADER md5Header;
	UPGRADE_FLAG upgradeFlag;
	BYTE reserved[3];		/* Must be 'N' 'E' 'W', to distinguish between the old firmware and the new one. */
	BYTE apFileOffset[4];
	BYTE nvmFileOffset[4];
	BYTE pibFileOffset[4];
	BYTE gglFileOffset[4];
}UPGRADE_FILE_HEADER, *PUPGRADE_FILE_HEADER;

/**************************************************************************************************/
/*                                      FUNCTIONS                                                 */
/**************************************************************************************************/


STATUS nm_initFwupPtnStruct(void);

STATUS nm_upgradeFwupFile(char *pAppBuf, int nFileBytes);
STATUS nm_tpFirmwareCheck(unsigned char *ptr,int bufsize);
//STATUS nm_tpFirmwareRecovery(unsigned char *ptr,int bufsize);
STATUS nm_tpFirmwareVerify(unsigned char *ptr,int bufsize);
void nm_NewFirmwareDivide(unsigned char * fw_data, int* ap_offset, int*ap_len);

/**************************************************************************************************/
/*                                      VARIABLES                                                 */
/**************************************************************************************************/

extern int g_nmCountFwupAllWriteBytes;
extern int g_nmCountFwupCurrWriteBytes;
extern int g_nmUpgradeResult;
/*extern SOFT_VER_STRUCT curSoftVer;*/




#ifdef __cplusplus 
}
#endif

#endif


