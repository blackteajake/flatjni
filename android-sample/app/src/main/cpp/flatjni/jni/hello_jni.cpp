#include <stdio.h>
#include <jni.h>
#include <string>
#include <android/log.h>
#include "hello.h"

#define TAG "hello_jni"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, ##__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, ##__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, ##__VA_ARGS__)

#define VERSION_STR "libGreeter.so 0.1 Build " __DATE__
#define JAVA_CLASS_PATH "com/github/jekinchen/flatjni/Greeter"
using namespace com::github::jekinchen::flatjni;
using namespace flatbuffers;


static HelloReplyBuilder *hello(const HelloRequest *request) {
  //implements your code here
  //...
    std::string greeting("Hello ");
    greeting.append(request->name() == NULL ? "NULL" : request->name()->c_str());
  return new HelloReplyBuilder(greeting.c_str()); //or return null
}

static SumResultBuilder *sum(const SumParam *request) {
  //implements your code here
  //...
  //return new SumResultBuilder(request->i() + request->j()); //or return null
  return NULL;
}



static jbyteArray _hello(JNIEnv *env, jclass cls, jbyteArray req) {
  const char *buf = (const char *) env->GetByteArrayElements(req, NULL);
  if (buf == nullptr) return nullptr;
  HelloReplyBuilder *builder = hello(GetRoot<HelloRequest>(buf));
  if (builder == nullptr) {
    env->ReleaseByteArrayElements(req, (jbyte *) buf, JNI_ABORT);
    return nullptr;
  }
  const FlatBufferBuilder &bb = builder->fbb();
  jbyteArray byteArray = env->NewByteArray(bb.GetSize());
  env->SetByteArrayRegion(byteArray, 0, bb.GetSize(), (const jbyte *) bb.GetBufferPointer());
  env->ReleaseByteArrayElements(req, (jbyte *) buf, JNI_ABORT);
  delete builder;
  return byteArray;
}

static jbyteArray _sum(JNIEnv *env, jclass cls, jbyteArray req) {
  const char *buf = (const char *) env->GetByteArrayElements(req, NULL);
  if (buf == nullptr) return nullptr;
  SumResultBuilder *builder = sum(GetRoot<SumParam>(buf));
  if (builder == nullptr) {
    env->ReleaseByteArrayElements(req, (jbyte *) buf, JNI_ABORT);
    return nullptr;
  }
  const FlatBufferBuilder &bb = builder->fbb();
  jbyteArray byteArray = env->NewByteArray(bb.GetSize());
  env->SetByteArrayRegion(byteArray, 0, bb.GetSize(), (const jbyte *) bb.GetBufferPointer());
  env->ReleaseByteArrayElements(req, (jbyte *) buf, JNI_ABORT);
  delete builder;
  return byteArray;
}



static JNINativeMethod methods[] = {
  {"_hello", "([B)[B", (void *) _hello },
  {"_sum", "([B)[B", (void *) _sum }
};

jint JNICALL JNI_OnLoad(JavaVM *vm, void *unused) {
  JNIEnv *env = NULL;
  LOGI(VERSION_STR);

  if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
    LOGE("GetEnv failed!");
    return -1;
  }

  jclass cls = env->FindClass(JAVA_CLASS_PATH);
  if (!cls) {
    LOGE("FindClass %s failed!", JAVA_CLASS_PATH);
    return -1;
  }

  int native_count = sizeof(methods) / sizeof(methods[0]);
  LOGD("register native method for class %s", JAVA_CLASS_PATH);
  for (int i = 0; i < native_count; ++i) {
    LOGD("method: %s %s", methods[i].name, methods[i].signature);
  }

  if (env->RegisterNatives(cls, methods, native_count) < 0) {
    LOGE("RegisterNatives failed!");
    env->DeleteLocalRef(cls);
    return -1;
  }

  env->DeleteLocalRef(cls);
  LOGI("JNI_OnLoad ok.");
  return JNI_VERSION_1_6;
}

