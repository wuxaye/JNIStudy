#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jint JNICALL
Java_com_xaye_myjni_JNI_passInt(JNIEnv *env, jobject thiz, jint x, jint y) {
    return x + y;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_xaye_myjni_JNI_passString(JNIEnv *env, jobject thiz, jstring str) {
    // 将 jstring 转换为 std::string, std::string 是一个标准库提供的字符串类，
    // 可以这样写 std::string str = "Hello, World!"; 类似于Java String str = "Hello, World!";
    const char *inputCStr = env->GetStringUTFChars(str, nullptr);
    std::string inputString(inputCStr);

    // 处理字符串（例如，转换为大写）
    std::string outputString;
    for (char &c : inputString) {  //& 是引用运算符,&c 代表每个字符的引用
        outputString += toupper(c);//toupper 函数并不会直接改变 inputString 中的字符，而是返回 c 的大写版本
    }

    // 释放 jstring 的资源
    env->ReleaseStringUTFChars(str, inputCStr);

    // 将处理后的 std::string 转换回 jstring
    return env->NewStringUTF(outputString.c_str());
}

extern "C"
JNIEXPORT jintArray JNICALL
Java_com_xaye_myjni_JNI_passArray(JNIEnv *env, jobject thiz, jintArray array) {
    // 获取 jintArray 中的元素指针,也就是第一个元素的内存地址
    jint *p = env->GetIntArrayElements(array, nullptr);
    // 获取数组的长度
    jsize length = env->GetArrayLength(array);

    // 遍历数组元素并将每个元素乘以 10
    for (int i = 0; i < length; ++i) {
        p[i] *= 10;
    }

    // 提交更改并释放数组元素 , 显式调用 ReleaseIntArrayElements，以确保这些更改被反映到原始 Java 数组中。
    env->ReleaseIntArrayElements(array, p, 0); // 第三个参数为 0 表示更改已经提交

    // 返回修改后的数组
    return array;
}