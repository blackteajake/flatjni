#include <stdio.h>
#include <jni.h>

#include "fjlog.h"

#define VERSION_BUILD   				"0.1"
#define JAVA_BGM_PLAYER_CLASS_PATH    	"com/jk/greeter/HelloRequest"

// static JavaVM *_vm = NULL;
// static player_t _player = NULL;
// static JNIEnv *_env = NULL;

// static jclass _player_cls = NULL;
// static jmethodID _player_onData = NULL;
// static jmethodID _player_onComplete = NULL;

// static void pcm_output(int is_first_chunk, int sample_rate, unsigned char *pcm, size_t len) {
//     ASSERT(_env != NULL, "env from decoder thread should not be NULL after AttachCurrentThread");
//     jbyteArray byteArray = (*_env)->NewByteArray (_env, len);
//     (*_env)->SetByteArrayRegion(_env, byteArray, 0, len, (const jbyte *)pcm);
//     (*_env)->CallStaticVoidMethod(_env, _player_cls, _player_onData, is_first_chunk, sample_rate, byteArray);
//     (*_env)->DeleteLocalRef(_env, byteArray);
// }

// static void decode_complete() {
//     (*_env)->CallStaticVoidMethod(_env, _player_cls, _player_onComplete);
// }

// static void attach_thread() {
//     if ((*_vm)->AttachCurrentThread(_vm, &_env, NULL) != JNI_OK) {
//         LOGE("AttachCurrentThread failed!");
//         abort();
//     }
// }

// static void detach_thread() {
//     (*_vm)->DetachCurrentThread(_vm);
// }

// static void native_set_log(JNIEnv *env, jclass cls, jstring jpath) {
//     const char *path = (*env)->GetStringUTFChars (env, jpath, NULL);
//     debug_init(LOG_LEVEL_DEBUG, "bgm", path);
//     (*env)->ReleaseStringUTFChars (env, jpath, path);
// }

// static jint native_create(JNIEnv *env, jclass cls, jint outputSampleRate, jint srcQuality) {
//     if (_player == NULL) {
//         _player = player_create(outputSampleRate, srcQuality, pcm_output, decode_complete, attach_thread, detach_thread);
//         if (_player == NULL)
//             return -1;
//     }
//     return 0;
// }

// static void native_release(JNIEnv *env, jclass cls) {
//     if (_player != NULL) {
//         player_release(_player);
//         _player = NULL;
//     }
// }

// static jint native_set_source(JNIEnv *env, jclass cls, jstring juri) {
//     int ret = 0;
//     const char *uri = (*env)->GetStringUTFChars (env, juri, NULL);
//     ret = player_set_source(_player, uri);
//     (*env)->ReleaseStringUTFChars (env, juri, uri);
//     return ret;
// }

// static jint native_play(JNIEnv *env, jclass cls) {
//     return player_play(_player);
// }

// static jint native_pause(JNIEnv *env, jclass cls) {
//     return player_pause(_player);
// }

// static void native_stop(JNIEnv *env, jclass cls) {
//     player_stop(_player);
// }

static jbyteArray _sayHello(JNIEnv *env, jclass cls, jbyteArray req) {
    LOGI("say hello from c++");

    jbyteArray byteArray = env->NewByteArray (0);
    return nullptr;
}

JNIEXPORT jint JNICALL JNI_OnLoad (JavaVM *vm, void *unused)
{
    JNIEnv* env = NULL;
    jclass player_cls;

    static JNINativeMethod methods[] = {
        {"_sayHello",      "([B)[B",                   (void*)_sayHello}
    };

    LOGI("loading flatjni version:%s", VERSION_BUILD);

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("get env error, abort!");
        abort ();
    }

    LOGI("find class");
    player_cls = env->FindClass(JAVA_BGM_PLAYER_CLASS_PATH);
    if (!player_cls) {
        LOGE("native registration unable to find class '%s'!", JAVA_BGM_PLAYER_CLASS_PATH);
        abort();
    }

    LOGI("register native methods");
    if(env->RegisterNatives(player_cls, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        LOGE("native registration failed!");
        abort();
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
    env->DeleteLocalRef (player_cls);
    LOGI("bgm player loaded!");
    return JNI_VERSION_1_6;
}
