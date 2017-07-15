#include <jni.h>
#include "hello.h"

using namespace com::github::jekinchen::flatjni;
using namespace flatbuffers;

static HelloReplyBuilder *hello(const HelloRequest *request) {
  //implements your code here
  //...
  return new HelloReplyBuilder(/* pass reply argments here */); //or return null
}

static SumResultBuilder *sum(const SumParam *request) {
  //implements your code here
  //...
  return new SumResultBuilder(/* pass reply argments here */); //or return null
}

static UserBuilder *echo(const User *request) {
  //implements your code here
  //...
  return new UserBuilder(/* pass reply argments here */); //or return null
}


