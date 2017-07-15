FlatJNI
===========

**FlatJNI** frees you from the troublesome JNI programming, which automatically generates NDK templates based on flatbuffers' schame files for easy integration into Android projects. Just like using flatbuffers, call C ++ functions in Java, so you do not need to take the time to study the JNI specification,and not to write error-prone JNI-related code.

Getting Started
=================

1. Clone **FlatJNI** :

   ```shell
   git clone https://github.com/jekinchen/flatjni.git
   ```


2. Compile, `cmake` installation are required. On Linux:

   ```shell
   cd flatjni
   mkdir build
   cd build && cmake -DCMAKE_INSTALL_PREFIX=target .. && make install
   ```

   If everything goes well, an executable file `flatc` generated in `build/target/bin` directory.

3. Write a schema file, for example:

   ```shell
   namespace com.github.jekinchen.flatjni;

   table HelloRequest {
     name:string;
   }

   table HelloReply {
     greeting:string;
   }

   table SumParam {
     i:int;
     j:int;
   }

   table SumResult {
     result:int;
   }

   rpc_service Greeter {
     hello(HelloRequest):HelloReply;
     sum(SumParam):SumResult;
   }
   ```

   Save as file `hello.fbs`. 

   Note: all supported filed types: 

   ```
   8 bit: byte, ubyte, bool
   16 bit: short, ushort
   32 bit: int, uint, float
   64 bit: long, ulong, double
   string: UTF-8 or 7-bit ASCII
   [byte]: byte array
   ```

4. Generate JNI Code template:

   ```shell
   cd target
   bin/flatc --java --cpp -o jni /path/to/hello.fbs
   cp -a include/flatbuffers java/com jni/
   ```

   `flatc` create `jni` directory, now the contents of `jni` as follows:

   ```shell
   jni
   ├── Android.mk
   ├── Application.mk
   ├── com
   │   ├── github
   │   │   └── jekinchen
   │   │       └── flatjni
   │   │           ├── Greeter.java
   │   │           ├── HelloReply.java
   │   │           ├── HelloRequest.java
   │   │           ├── SumParam.java
   │   │           └── SumResult.java
   │   └── google
   │       └── flatbuffers
   │           ├── Constants.java
   │           ├── FlatBufferBuilder.java
   │           ├── Struct.java
   │           └── Table.java
   ├── flatbuffers
   │   ├── base.h
   │   └── flatbuffers.h
   ├── hello.h
   └── hello_jni.cpp
   └── hello_rpc.h
   ```

   Open and edit `hello_rpc.h`, which let you implement all RPC calls naturally:

   ```c++
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
   ```

5. Build `.so` file which can be used in Android project: 

   ```shell
   cd jni && ndk-build
   ```

   `ndk-build` is shipped by Android NDK. Note `ndk-build` require current executing directory called `jni`.

   Here is the outputs:

   ```shell
   [armeabi] Compile++ thumb: Greeter <= hello_jni.cpp
   [armeabi] SharedLibrary  : libGreeter.so
   [armeabi] Install        : libGreeter.so => libs/armeabi/libGreeter.so
   [armeabi-v7a] Compile++ thumb: Greeter <= hello_jni.cpp
   [armeabi-v7a] SharedLibrary  : libGreeter.so
   [armeabi-v7a] Install        : libGreeter.so => libs/armeabi-v7a/libGreeter.so
   ```

   Copy `libs/armeabi/libGreeter.so` and `libs/armeabi-v7a/libGreeter.so` to the library directory of Android project, for example: `AndroidProjectRoot/app/src/main/jniLibs`.

6. Move `jni/com` to Android project, now let's make native calls in Java, say in `MainActivity.java`:

   ```java
   public class MainActivity extends AppCompatActivity {
       @Override
       protected void onCreate(Bundle savedInstanceState) {
           super.onCreate(savedInstanceState);
           setContentView(R.layout.activity_main);

           final TextView tv = (TextView) findViewById(R.id.tv);
           tv.setOnClickListener(new View.OnClickListener() {
               @Override
               public void onClick(View v) {
                   HelloReply replay = Greeter.hello("jk"); //call C++ method
                   SumResult result = Greeter.sum(1, 2); //call C++ method
                   String text = replay != null ? replay.greeting() : "hello()=null";
                   text += "\n";
                   text += result != null ? ("sum(1,2)=" + result.result()) : "sum(1,2)=null";
                   tv.setText(text);
               }
           });
       }
   }
   ```

   As you can see above, you just only care about the interface definition (a schema file like RPC definition), and use some helper class to make a C++ native call from Java!




Android Test
=================

There is an Android example, use Android Studio to open `flatjni/android-sample`. 

The schema file is `android-sample/app/src/main/cpp/flatjni/hello.fbs`, you can change it, then execute flatc to re-generate all files, replace the relative files in Android project with new ones, and see what happens. 

The example tested with Android Studio v2.3.3, which support build NDK/JNI (achived by adding `externalNativeBuild` to `android-sample/app/build.gradle`), and set breakpoint in C++ file.

Why to bypass JNI?
=================

Because writing JNI-relatvie code is a trivial job, it's so error-prone! I spent a lot of time to debug JNI, and usually the bug is just some literal errors or use the wrong function, which not easily figure out at the coding or compile time. I'll show some examples:

- FindClass

  ```c++
  env->FindClass("com.github.jekinchen.flatjni.Greeter");
  ```
  Failed to find the class because of the name space separator, here's the right style:
  ```c++
  env->FindClass("com/github/jekinchen/flatjni/Greeter");
  ```

- Type Signatures

  ```c++
  //Java Native method
  long f (int n, String s, int[] arr);
  //You need this signature to register in C++
  (ILjava/lang/String;[I)J
  ```

  You'd better to spend dozens of times to check signature. One may say there is a tool `javap` which can prints the signatures of all methods for you, but if someone make a little change to method later ? The error occur at run time and it's expensive to figure out then at compile time.

- A wide variety of functions

  ```c++
  //call java method which return type is void
  CallVoidMethod()
  CallVoidMethodA()
  CallVoidMethodV()
  //return type is Object
  CallObjectMethod()
  CallObjectMethodA()
  CallObjectMethodV()
  //return type is Boolean
  CallBooleanMethod()
  CallBooleanMethodA()
  CallBooleanMethodV()
  ...
  //call java static method
  CallStaticVoidMethod()
  CallStaticObjectMethod()
  ...
  ```

- Local reference

  C++ has no GC, when you create a local reference ( to jobject type) , remember to release it before the method return, or you'll leak local reference and finally trigger a fatal error says `Local reference table overflow`. The local reference table allow maximum 512 entries.

- Thread context

  In C++ thread, you must call `AttachCurrentThread` before calling Java method, and require to call `DetachCurrentThread` before thread exiting.

In short, in order to avoid dealing with the above complex rules, one way is to use few JNI functions as possible. Let developers focus on theirs business. I achieved this by using `google's flatbuffer`, a `flatbuffer` is a byte array that represent a business data structure, all Java native method can take a `flatbuffer` as argument and return value. Developers just use some helper classes to create or access `flatbuffer` easily. That's the `flatJNI` do.


Todo
=================
Call Java native method in C++.

Contribution
=================
Jekin Chen : jekinchen@qq.com
Any suggestions are Welcome!