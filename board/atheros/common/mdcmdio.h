#ifndef _MDCMDIO_H_
#define _MDCMDIO_H_

int32_t smiRead(uint32_t, uint32_t, uint32_t *);
int32_t smiWrite(uint32_t, uint32_t, uint32_t);
int32_t smiReadBit(uint32_t, uint32_t, uint32_t, uint32_t *);
int32_t smiWriteBit(uint32_t, uint32_t, uint32_t, uint32_t);

#endif
