APP_MODULES := Greeter

#不指定debug或release将使用AndroidManifest.xml设定
APP_OPTIM := debug
#APP_OPTIM := release

APP_ABI := armeabi-v7a 
#APP_ABI := armeabi x86

APP_PLATFORM := android-19

#将此变量定义为 4.9 或 4.8 以选择 GCC 编译器的版本。 64 位 ABI 默认使用版本 4.9 ，32 位 ABI 默认使用版本 4.8
#要选择 Clang 的版本，请将此变量定义为 clang3.4、clang3.5 或 clang。 指定 clang 会选择 Clang 的最新版本
NDK_TOOLCHAIN_VERSION := clang

#APP_STL := stlport_shared
#APP_STL := gnustl_shared
APP_STL := gnustl_static
APP_GNUSTL_FORCE_CPP_FEATURES := rtti exceptions
APP_CPPFLAGS += -std=c++11
