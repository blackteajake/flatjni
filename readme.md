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
   cd build && cmake ..
   ```

   If everything goes well, an executive file named `flatc` generated in `build` directory.

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

   Note: `FlatJNI` supports scalar and string filed types: 

   ```shell
   8 bit: byte, ubyte, bool
   16 bit: short, ushort
   32 bit: int, uint, float
   64 bit: long, ulong, double
   string: UTF-8 or 7-bit ASCII
   ```

4. Generate JNI Code template:

   ```shell
   ./flatc --java --cpp -o jni hello.fbs
   ```

   Then a `jni` directory will create, and it's contents as follows:

   ```shell
   jni/
   ├── Android.mk
   ├── Application.mk
   ├── com
   │   └── github
   │       └── jekinchen
   │           └── flatjni
   │               ├── Greeter.java
   │               ├── HelloReply.java
   │               ├── HelloRequest.java
   │               ├── SumParam.java
   │               └── SumResult.java
   ├── hello.h
   └── hello_jni.cpp
   ```

   The `hello_jni.cpp` let you implements all RPC calls naturally:

   ```c++
   //...
   using namespace com::github::jekinchen::flatjni;
   using namespace flatbuffers;

   static HelloReplyBuilder *hello(const HelloRequest *request) {
     //implements your code here
     std::string greeting("Hello ");
     greeting.append(request->name() == NULL ? "NULL" : request->name()->c_str());
     return new HelloReplyBuilder(greeting.c_str()); //or return null
   }

   static SumResultBuilder *sum(const SumParam *request) {
     //implements your code here
     //return new SumResultBuilder(request->i() + request->j()); //or return null
     return NULL;
   }

   //JNI code you don't need to care about ...
   //...
   ```

   ​

5. Build `.so` file which can be used in Android project: 

   ```shell
   cp ../include/flatbuffers jni/
   cd jni && ndk-build  //ndk-build is sheeped by Android NDK
   ```

   Here is the outputs:

   ```shell
   [armeabi] Compile++ thumb: Greeter <= hello_jni.cpp
   [armeabi] SharedLibrary  : libGreeter.so
   [armeabi] Install        : libGreeter.so => libs/armeabi/libGreeter.so
   [armeabi-v7a] Compile++ thumb: Greeter <= hello_jni.cpp
   [armeabi-v7a] SharedLibrary  : libGreeter.so
   [armeabi-v7a] Install        : libGreeter.so => libs/armeabi-v7a/libGreeter.so
   ```

   We copy `libs/armeabi/libGreeter.so` and `libs/armeabi-v7a/libGreeter.so` to the library directory of Android project, for example: `AndroidProjectRoot/app/src/main/jniLibs`.

6. Copy the contents in `jni/com` to Android project, and make native calls in Java, lets says in `MainActivity.java`:

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
                   String text = replay != null ? replay.greeting() : "hello replay null";
                   text += result != null ? ("sum(1,2)=" + result.result()) : ", sum reply null";
                   tv.setText(text);
               }
           });
       }
   }
   ```

   The contents in `flatjni/java/com` are need to copy to Android project too. 

   In later version, I'll to make the distribution more easier!
