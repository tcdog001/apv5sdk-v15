LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= cert.c \
	common.c \
        hmac_sha256.c \
        interface.c \
        wapi.c
#LOCAL_STATIC_LIBRARIES := libecc
LOCAL_MODULE := libiwnwai_asue

include $(BUILD_STATIC_LIBRARY)
