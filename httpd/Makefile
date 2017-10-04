#
# Makefile for http stuff
#

include $(TOPDIR)/config.mk

LIB   = libhttpd.a
#OBJS += uip.o uip_arch.o uip_arp.o httpd.o fs.o
OBJS += uip.o uip_arch.o uip_arp.o httpd.o fsdata.o

all:    $(LIB)

#fs.c:   fsdata.c
WEB_PATH=$(CONFIG_WEBPAGES_PATH)/web_pages_newVI_temp/uboot/uboot_upgrade_without_%.html

ifneq (, $(findstring $(RELEASE), fr))
PID_FR=$(subst 0,5,$(PID))
PROD_NAME = TL-$(shell echo $(PID_FR) | tr 'a-z' 'A-Z')_$(RELEASE)
else
PROD_NAME = TL-$(shell echo $(PID) | tr 'a-z' 'A-Z')_$(RELEASE)
endif

#根据RELEASE变量中的内容，自动选择uboot页面语言类型
RELEASE_UP = $(shell echo $(RELEASE) | tr a-z A-Z)
LANGUAGE_LIB = en_US de_DE es_ES es_LA fr_FR it_IT jp_JP ko_KR pt_BR pt_PT nl_NL
ifneq (, $(filter %_$(RELEASE_UP), $(LANGUAGE_LIB)))
LANGUAGE_TYPE = $(strip $(filter %_$(RELEASE_UP), $(LANGUAGE_LIB)))
else
LANGUAGE_TYPE=en_US
endif


fsdata.c:
	xxd -i ./image/upgrade.html | sed -e s/_.*upgrade_html/upgrade_html/ > ./fsdata.c
#	    cp -f $(WEB_PATH) ./image/upgrade.tmp
#	    sed -i 's/PRODUCT_NAME/$(PROD_NAME)/g' ./image/upgrade.tmp
#	    sed -i 's/LAN_TYPE_HOLDER/$(LANGUAGE_TYPE)/g' ./image/upgrade.tmp
#	    xxd -i ./image/upgrade.tmp | sed -e s/_.*upgrade_tmp/upgrade_html/ > ./fsdata.c
#		rm ./image/upgrade.tmp

$(LIB): $(START) $(OBJS)
	    $(AR) crv $@ $(OBJS)

#########################################################################

.depend: Makefile $(OBJS:.o=.c)
	     $(CC) -M $(CFLAGS) $(OBJS:.o=.c) > $@

sinclude .depend

#########################################################################

