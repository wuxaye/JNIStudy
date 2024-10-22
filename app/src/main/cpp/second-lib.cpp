#include <jni.h>
#include <android/log.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
//#define 是预处理指令，用于定义宏，可以是常量、表达式或代码片段。
#define LOG_TAG "SecondLib"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C"
JNIEXPORT void JNICALL
Java_com_xaye_myjni_JNI2_cCallJavaVoidMethod(JNIEnv *env, jobject thiz) {
   // com/xaye/myjni/JNI2.java

    //1.获取类的字节码
    //Class<Test> clazz = Test.class;
    jclass clazz = env->FindClass("com/xaye/myjni/JNI2");

    //2.通过字节码获取方法
    // Method method = clazz.getDeclaredMethod("print", String.class);
    // jmethodID GetMethodID(jclass clazz, const char* name, const char* sig)
    // 最后一个参数是方法签名，这里为空()，表示无参数，因为返回值为void 类型，void的签名为V,所以参数和返回值的签名为：()V
    jmethodID methodID = env->GetMethodID(clazz, "printHelloWorld", "()V");

    //3.通过字节码创建对象
    // Test test = clazz.newInstance();
    // 这里的 thiz 即对象

    //4.调用方法
    // method.invoke(test, "hello world");
    // void CallVoidMethod(jobject obj, jmethodID methodID, ...)
    env->CallVoidMethod(thiz, methodID);
}

void showToast(JNIEnv *env, jobject thiz, char* str) {
    //1.获取类的字节码
    jclass clazz = env->FindClass("com/xaye/myjni/JNI2");

    //2.获取字段（成员方法）的ID
    jfieldID fieldID = env->GetFieldID(clazz, "context", "Landroid/content/Context;");

    //3.获取context的值
    jobject context = env->GetObjectField(thiz, fieldID);

    //4.创建Toast要显示的内容
    jstring msg = env->NewStringUTF(str);

    //5.获取Toast的字节码
    jclass toastClazz = env->FindClass("android/widget/Toast");

    //6.获取makeText静态方法
    //jni:  jmethodID GetStaticMethodID(jclass clazz, const char* name, const char* sig)
    //java: Toast makeText(Context context, CharSequence text, @Duration int duration)
    // 方法签名使用快捷键shift + entry即可，不用手动输
    jmethodID methodID = env->GetStaticMethodID(toastClazz, "makeText","(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");

    //7.调用makeText 生成一个Toast对象
    // 这个方法需要三个参数：Context对象、CharSequence对象和持续时间(int)
    jobject toast = env->CallStaticObjectMethod(toastClazz, methodID, context, msg, 0);

    //8.获取Toast的show方法ID
    jmethodID showMethodID = env->GetMethodID(toastClazz, "show", "()V");

    //9.调用show方法
    env->CallVoidMethod(toast, showMethodID);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xaye_myjni_JNI2_cCallJavaIntMethod(JNIEnv *env, jobject thiz) {

    //1.获取类的字节码
    jclass clazz = env->FindClass("com/xaye/myjni/JNI2");

    //2.通过字节码获取方法
    jmethodID methodID = env->GetMethodID(clazz, "add", "(II)I");

    //调用方法
    jint result = env->CallIntMethod(thiz, methodID, 1, 2);

    char* message = "Hello from C++!";

    showToast(env, thiz,message);
    LOGI(" cCallJavaIntMethod result: %d",result);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_xaye_myjni_JNI2_cCallJavaStringMethod(JNIEnv *env, jobject thiz) {

    //1.获取类的字节码
    // 使用 JNIEnv 的 FindClass 方法查找名为 "com/xaye/myjni/JNI2" 的 Java 类
    // "com/xaye/myjni/JNI2" 是 Java 中类的完整路径名，使用 '/' 替代 '.'。
    jclass clazz = env->FindClass("com/xaye/myjni/JNI2");

    //2.通过字节码获取方法
    // GetMethodID 方法用于获取指定方法的 ID。此方法的参数包括：
    // clazz: 找到的 Java 类的字节码
    // "printString": 要调用的方法的名称
    // "(Ljava/lang/String;)V": 方法的签名，表示该方法接受一个字符串参数并返回 void
    jmethodID methodID = env->GetMethodID(clazz, "printString", "(Ljava/lang/String;)V");

    //3.调用方法
    // 准备要传递给 Java 方法的字符串
    char * str = "hello world from C++";

    // 将 C 字符串转换为 Java 字符串（jstring）
    // NewStringUTF 方法创建一个新的 Java 字符串对象，并将 C 字符串的内容复制到它中
    jstring jstr = env->NewStringUTF(str);

    // 调用 Java 方法
    // CallVoidMethod 方法用于调用无返回值的方法。此方法的参数包括：
    // thiz: 当前对象的引用，表示调用方法的对象
    // methodID: 获取到的方法 ID
    // jstr: 作为参数传递给 Java 方法的 jstring 对象
    env->CallVoidMethod(thiz, methodID, jstr);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_xaye_myjni_JNI2_encode(JNIEnv *env, jobject thiz, jstring str) {

    char * str_ = (char *) env->GetStringUTFChars(str, NULL);

    int len = strlen(str_);

    for (int i = 0; i < len; i++) {
       *(str_ + i) += 3;
    }

    return env->NewStringUTF(str_);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_xaye_myjni_JNI2_decode(JNIEnv *env, jobject thiz, jstring str) {

    // 将 jstring 类型的 str 转换为 C 字符串 (UTF-8 格式)
    char * str_ = (char *) env->GetStringUTFChars(str, NULL);

    // 获取字符串的长度
    int len = strlen(str_);

    for (int i = 0; i < len; i++) {
        // 将每个字符的 ASCII 值增加 3
        *(str_ + i) -= 3; // *上一个地址 就是拿到该地址上的值
    }

    return env->NewStringUTF(str_);
}

int getPressure() {
    return rand() % 100 + 1;
}

int startMonitor;

extern "C"
JNIEXPORT void JNICALL
Java_com_xaye_myjni_MainActivity_startMonitor(JNIEnv *env, jobject thiz) {
    // 1. 获取字节码 (这里的参数 name 可以将文件的路径复制过来，然后 alt + enter 快捷键即可自动转换)
    jclass clazz = env->FindClass("com/xaye/myjni/MainActivity");

    // 2. 获取方法ID
    jmethodID methodID = env->GetMethodID(clazz, "updatePressure", "(I)V");

    startMonitor = 1;
    while (startMonitor) {
        int pressure = getPressure();
        // 3. 调用方法
        env->CallVoidMethod(thiz, methodID, pressure);
        sleep(2);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_xaye_myjni_MainActivity_stopMonitor(JNIEnv *env, jobject thiz) {
    startMonitor = 0;
}


// 子线程执行的函数
// 参数 void* args 可以用于传递任何类型的数据
void* threadFunction(void* args) {
    LOGI("子线程开始执行");
    // 模拟耗时操作
    for (int i = 0; i < 5; ++i) {
        LOGI("子线程运行中: %d", i);
        sleep(1);//暂停子线程1秒钟，模拟耗时操作。
    }
    LOGI("子线程结束");
    return nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xaye_myjni_JNI2_startCThread(JNIEnv *env, jobject thiz) {
    pthread_t thread;

    /*
     * pthread_create 函数创建一个新线程
     * &thread：传入指针以接收新线程的ID。
     * nullptr：线程属性，设置为 nullptr 使用默认属性。
     * threadFunction：指定线程执行的函数。
     * nullptr：传递给线程函数的参数。
     * */
    int result = pthread_create(&thread, nullptr, threadFunction, nullptr);

    //检查线程创建是否成功，result 返回值为0表示成功，其他值表示错误。
    if (result != 0) {
        LOGI("无法创建线程: %d", result);
    } else {
        char* message = "线程创建成功";
        LOGI("线程创建成功");
        showToast(env, thiz,message);
        pthread_detach(thread);  // 让线程在结束时自动释放资源
    }
}
