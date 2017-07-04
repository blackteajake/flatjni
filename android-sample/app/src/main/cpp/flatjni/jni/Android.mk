LOCAL_PATH := $(call my-dir)

# ####################################################
# include $(CLEAR_VARS)

# LOCAL_MODULE :=

# LOCAL_SRC_FILES := 

# include $(PREBUILT_STATIC_LIBRARY)
    
# ####################################################
# include $(CLEAR_VARS)

# LOCAL_MODULE :=

# LOCAL_SRC_FILES := 

# include $(PREBUILT_STATIC_LIBRARY)
    
#####################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := flatjni.cpp fjlog.cpp

LOCAL_MODULE := Greeter

LOCAL_C_INCLUDES := .
					
LOCAL_CFLAGS := -D__ANDROID__

LOCAL_LDFLAGS := -llog

LOCAL_SHARED_LIBRARIES :=

LOCAL_STATIC_LIBRARIES :=

include $(BUILD_SHARED_LIBRARY)

