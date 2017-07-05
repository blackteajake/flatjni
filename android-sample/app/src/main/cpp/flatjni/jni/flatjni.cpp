#include <stdio.h>
#include <jni.h>

#include <android/log.h>

#define TAG "Greeter"
#define LOGD(...)          __android_log_print(ANDROID_LOG_DEBUG, TAG, ##__VA_ARGS__)
#define LOGI(...)          __android_log_print(ANDROID_LOG_INFO, TAG, ##__VA_ARGS__)
#define LOGE(...)          __android_log_print(ANDROID_LOG_ERROR, TAG, ##__VA_ARGS__)

#define VERSION_STR "libGreeter.so 0.1 Build " __DATE__
#define JAVA_CLASS_PATH "com/jk/greeter/Greeter"

#include "hello_generated.h"

using namespace com::jk::greeter;
using namespace flatbuffers;


static FlatBufferBuilder *sayHello(const HelloRequest *request) {
    return CreateHelloReplyBufferBuilder(true, 1, request->message()->c_str(), 100, 888.0,
                                         9999.9999);
}

static FlatBufferBuilder *play(const PlayRequest *request) {
    return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////

static jbyteArray _sayHello(JNIEnv *env, jclass cls, jbyteArray req) {
    const char *buf = (const char *) env->GetByteArrayElements(req, NULL);
    const HelloRequest *request = GetHelloRequest(buf);
    const FlatBufferBuilder *bb = sayHello(request);
    jbyteArray byteArray = env->NewByteArray(bb->GetSize());
    env->SetByteArrayRegion(byteArray, 0, bb->GetSize(), (const jbyte *) bb->GetBufferPointer());
    delete bb;
    env->ReleaseByteArrayElements(req, (jbyte *) buf, JNI_ABORT);
    return byteArray;
}

static jbyteArray _play(JNIEnv *env, jclass cls, jbyteArray req) {
    return nullptr;
}

static JNINativeMethod methods[] = {
        {"_sayHello", "([B)[B", (void *) _sayHello},
        {"_play",     "([B)[B", (void *) _play}};

jint JNICALL JNI_OnLoad(JavaVM *vm, void *unused) {
    JNIEnv *env = NULL;
    LOGI(VERSION_STR);

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("GetEnv failed!");
        return -1;
    }

    jclass cls = env->FindClass(JAVA_CLASS_PATH);
    if (!cls) {
        LOGE("FindClass(\"%s\") failed!", JAVA_CLASS_PATH);
        return -1;
    }

    int native_count = sizeof(methods) / sizeof(methods[0]);
    LOGD("register native method for class %s", JAVA_CLASS_PATH);
    for (int i=0; i<native_count; ++i) {
        LOGD("method: %s %s", methods[i].name, methods[i].signature);
    }

    if (env->RegisterNatives(cls, methods, native_count) < 0) {
        LOGE("RegisterNatives failed!");
        return -1;
    }

    // LOGI("find onData method id");
    // _player_onData = (*env)->GetStaticMethodID(env, player_cls, "onData", "(ZI[B)V");
    // if (!_player_onData) {
    //     LOGE("can not get onData method id!");
    //     abort();
    // }

    // LOGI("find onComplete method id");
    // _player_onComplete = (*env)->GetStaticMethodID(env, player_cls, "onComplete", "()V");

    // _vm = vm;
    // _player_cls = (*env)->NewGlobalRef(env, player_cls);
    env->DeleteLocalRef(cls);
    LOGI("JNI_OnLoad ok.");
    return JNI_VERSION_1_6;
}
