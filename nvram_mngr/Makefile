# Makefile for nvrammngr lib.
# This lib is needed for tplink firmware recovery function support.
#

include $(TOPDIR)/config.mk

PROD_FILE_BIN = $(IMAGEPATH)/FlashMaker/$(PID)_$(RELEASE)/product-info.bin
PROD_FILE_C = product_info.c

LIB	= libnvrammngr.a

OBJS =  nm_api.o nm_fwup.o nm_lib.o sysProductInfo.o md5.o product_info.o #fwrecovery_api.o fwrecovery_gpio.o md5.o

#CFLAGS += -DCONFIG_LZMA=1

$(LIB): $(PROD_FILE_C) .depend $(OBJS)
	$(AR) crv $@ $(OBJS)

$(PROD_FILE_C): $(PROD_FILE_BIN)
	xxd -i $< > $@
	sed -i 's/_.*product_info_bin/product_info_bin/g' $@

.depend: Makefile $(OBJS:.o=.c)
		$(CC) -M $(CFLAGS) $(OBJS:.o=.c) > $@

sinclude .depend
