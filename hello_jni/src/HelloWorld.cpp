#include "HelloWorld.hpp"

extern "C"
JNIEXPORT void JNICALL Java_HelloWorld_sayHello(JNIEnv* env, jobject obj) {
    (void)env;
    (void)obj;
    std::cout << "Hello, World from JNI1!" << std::endl;
    std::cout << "Hello, World from JNI2!" << std::endl;
}

extern "C"
JNIEXPORT jstring JNICALL Java_HelloWorld_getString(JNIEnv* env, jobject obj) {
    (void)obj;
    return env->NewStringUTF("Hello from JNI!");
}