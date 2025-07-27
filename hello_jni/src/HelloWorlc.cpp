#include<iostream>
#include "HelloWorld.h"

extern "C"
JNIEXPORT void JNICALL Java_HelloWorld_sayHello
  (JNIEnv* env, jobject) {
    std::cout << "Hello, World from JNI!" << std::endl;
}

extern "C"
JNIEXPORT jstring JNICALL Java_HelloWorld_getString
  (JNIEnv* env, jobject) {
        return env->NewStringUTF("Hello from JNI!");
}
