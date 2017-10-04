/***************************************************************
 *
 * Copyright(c) 2005-2014 Shenzhen TP-Link Technologies Co. Ltd.
 * All right reserved.
 *
 * Filename		:	make_flash.h
 * Version		:	2.0
 * Abstract		:	Tool for generating LSDK Series Router's firmware
 * Author		:	LI SHAOZHANG (lishaozhang@tp-link.net)
 * Created Date	:	07/11/2007
 *
 * Modified History:
 * 03Mar14, PangXing	Ver 2.0: Improve features, and fix bugs.
 * 						Now, this tool has no Ver 1.0 any longer.
 * 						Just use arg to separate feature.
 ***************************************************************/


#ifndef _MAKE_FLASH_H_
#define _MAKE_FLASH_H_


/************* data **************/

// The tool's version
#define VERSION	"2.0"


#define BUF_LEN (16 * 1024 * 1024)
#define MAX_FILENAME_LEN 256
#define LINEBUFLEN 256

/*
 file tag (head) structure all is in clear text except validationTokens (crc, md5, sha1, etc).
 Total: 128 unsigned chars
*/
#define TAG_LEN         512
#define SIG_LEN         24

/* Original second SIG = 20 is now devided into 14 for SIG_LEN_2 and 6 for CHIP_ID */
#define SIG_LEN_2       12
#define CHIP_ID_LEN		8
#define TOKEN_LEN       20

/* TAG for downloadable image (kernel plus file system)
	integer in the structure is stored in Network-Byte order (BIG-endian) */
#define BOARD_ID_LEN    16

/* rsa signature len. */
#define RSA_SIGN_LEN 128

#define FLASH_PART_NUM 3


//#define TAG_VERSION		0x01000000
//#define COMPANY_INFO	"TP-LINK Technologies"
//#define VERSION_INFO	"ver. 1.0"
#define TAG_VERSION		0x02000000
#define COMPANY_INFO	"5A87AA52D998D0167C5D"
#define VERSION_INFO	"ver. 2.0"


/************ structure. *****************/
enum _11N_ROUTER_PART
{
	BOOT,
	KERNEL,
	FS
};

typedef enum _PARTITION_TYPE
{
	TYPE_A_4M = 0,
	TYPE_A_8M,
	TYPE_A_16M,
	TYPE_NOT_DEFINED_NOW,
}PARTITION_TYPE;

enum _FLASH_SIZE
{
	FLASH_SIZE_4M = (0x400000),
	FLASH_SIZE_8M = (0x800000),
	FLASH_SIZE_16M = (0x1000000),
};

enum _ERROR_TYPE
{
	ERR_NO_ERROR = 0,
	ERR_PARSE_CONF_INPUT,
	ERR_OPEN_CONF_FAIL,
	ERR_BUILD_TIME_STAT,
	ERR_FS_STAT,
	ERR_CHECK,
};


typedef struct _LINUX_FILE_TAG
{
	unsigned long tagVersion;
	char 		  signiture_1[SIG_LEN];         // text line for company info
	char 		  signiture_2[SIG_LEN_2];       // additional info (can be version number)
	char 		  chipId[CHIP_ID_LEN];			// chip id
	char 		  boardId[BOARD_ID_LEN];        // board id
	unsigned long productId;					// product id
	unsigned long productVer;					// product version
	unsigned long reserved1;					// reserved for future

	unsigned char imageValidationToken[TOKEN_LEN];// image validation token - md5 checksum
	unsigned char kernelValidationToken[TOKEN_LEN];	// kernel+tag validation token - md5 checksum

	unsigned long kernelTextAddr;				// text section address of kernel
	unsigned long kernelEntryPoint;				// entry point address of kernel

	unsigned long totalImageLen;				// the sum of kernelLen+rootfsLen+tagLen

	unsigned long kernelAddress;				// starting address (offset from the beginning of FILE_TAG) of kernel image
	unsigned long kernelLen;					// length of kernel image

	unsigned long rootfsAddress;				// starting address (offset) of filesystem image
	unsigned long rootfsLen;					// length of filesystem image

	unsigned long bootloaderAddress;			// starting address (offset) of boot loader image
	unsigned long bootloaderLen;				// length of boot loader image

	//unsigned short swVerMajor;
	//unsigned short swVerMinor;
	//unsigned short swVerRev;
	//unsigned short reserverd2;

	//unsigned char rsaSignature[RSA_SIGN_LEN];
} LINUX_FILE_TAG;

typedef struct _LINUX_FLASH_STRUCT
{
	unsigned long bootOffset;
	unsigned long macAddrOffset;
	unsigned long verifyOffset;
	unsigned long pinOffset;
	unsigned long kernelOffset;
	unsigned long rootfsOffset;
	unsigned long configOffset;
	unsigned long radioOffset;
}LINUX_FLASH_STRUCT;


// TIP: eg. wdr6500v2_cn_1_0_0_flash_140303_203040.bin
// product_id|product_ver|language|major|minor|revision|flash_type|year-month-day|hour-minute-second|bin
typedef struct
{
	unsigned short major;
	unsigned short minor;
	unsigned short revision;
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
	unsigned int product_id;
	unsigned int product_ver;
	unsigned char language[4];
	unsigned int flash_size;
}MODEL_CONFIG;



/*************** function *****************/
#define printf_info printf

#define RETURN(x, info)	\
	{	\
		if ((x) != ERR_NO_ERROR) {	\
			printf_info("[%s, line %d], error: %s\n", __FUNCTION__, __LINE__, info);	\
		}	\
		return (x);	\
	}

#define CHECK(x) \
	do{\
		if ((x))	\
		{\
			printf_info("Line %d:", __LINE__);\
			perror("check ");	\
			free(buf); \
			RETURN(ERR_CHECK, "check");	\
		}\
	}while(0);


#endif

