LOCAL_PATH:= $(call my-dir)
subdirs := $(addprefix $(LOCAL_PATH)/, $(addsuffix /Android.mk, \
		ECC2.2-2008 \
		sms4        \
		libiwnwai_asue \
		wapic ))
include $(subdirs)
