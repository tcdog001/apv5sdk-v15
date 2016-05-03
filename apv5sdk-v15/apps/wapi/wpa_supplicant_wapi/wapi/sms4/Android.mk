LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:=  sms4c.c  wpi_pcrypt.c

ifeq ($(TARGET_ARCH), arm)
LOCAL_CFLAGS += -DLE
endif

LOCAL_CFLAGS += -DWN_ECC_GCCINT64 -DASUE

LOCAL_MODULE := libsms4

include $(BUILD_STATIC_LIBRARY)

