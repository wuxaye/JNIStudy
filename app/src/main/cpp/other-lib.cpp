#include <jni.h>
#include <string>
#include "md5.h" // 引入MD5头文件
#include <android/log.h>

//
// Created by Administrator on 2025/7/2.
//
#define LOG_TAG "other-lib"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


static char *EXTRA_SIGNATURE = "XAYE";
using namespace std;

//校验签名
static int is_verify = 0;
static char *PACKAGE_NAME = "com.xaye.myjni";
// debug 包的签名，打正式包  需要将 APP_SIGNATURE 修改为  正式 包的签名！
static char *APP_SIGNATURE = "308202e4308201cc020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b30090603550406130255533020170d3234313033303135343530385a180f32303534313032333135343530385a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330820122300d06092a864886f70d01010105000382010f003082010a0282010100a5bfb2191c8fc174b11907f7c1fb2d7809a9543f43a17c2d84b0c4f0c7b71361c0a33f98c0a6e45d54d5eb4229a93dcfb78fd8e62057cbf67e84b09b33a06ef10ddd3576ec77c5207601f0d47a4fd889360dbb1bfb60d19adb35d78cc53c39e4134211f2d2262f9cbc5372c38fb9efe778ddace3642201bab0f2da2b0e4b5cee1c88039c5554f7281cda90f1780967e07cc623c339135fcc7f020b5cda9fbab2829b2863f7f94b694140fa744a05af5700946f81f5fa3a3a8cef125367640946d6c73ad957050eff33a63bc3fd82f1fbc8ba3087a0ad31043d266368897208f6dbe350a7aba822a0e2b0ff1f6e94ea94c187dcc8328980a4bcdae7e30301a60d0203010001300d06092a864886f70d010105050003820101001614fc5f47f814ae0814eff343441e13d56f31fb2ea553de94cf90d37e9da0fe3ed5870fe57afb2bbadec1368258955b84ddf39a445379544286ec74d9c3251ca9f5417eb0b9c750af9b32534f05816440a0bb5a9eded76c507032b39ebb628789e5bdcfd13363b7cdb94a4f4ed2869d8a3604c3617676e101161ffe3ea6aec934b01b093db45fc264a3a29de59753513d74d3d94395cbbc0328c36d5086f414a62ddff736634f0d173141d80a5417ed3a14fc9085b2db223280d354caf661b123ae569d1f284e0eff2cf6972f5bd8b2e53b81256ba55d83df9309fb0c1dc1a8bade54587dfcaaed535768d6cd64f7983f9666ed5502929a298d6b42b19be9cc";

extern "C"
JNIEXPORT jstring JNICALL
Java_com_xaye_myjni_jni_SignatureUtils_signatureParams(JNIEnv *env, jclass clazz, jstring params) {

    if (is_verify == 0) {
        return env->NewStringUTF("error_signature");
    }

    const char *paramsStr = env->GetStringUTFChars(params, nullptr);
    // MD5 签名规则，加点料
    // 1.字符串前面加点料
    string signature_str(paramsStr);
    signature_str.insert(0, EXTRA_SIGNATURE);
    // 2.后面去掉两位
    signature_str = signature_str.substr(0, signature_str.length() - 2);

    // 3.进行MD5签名, C++ 和 Java  是一样的，唯一不同的是C++需要自己回收内存
    MD5_CTX *ctx = new MD5_CTX();
    MD5Init(ctx);
    MD5Update(ctx, (unsigned char *) signature_str.c_str(), signature_str.length());
    unsigned char digest[16];
    MD5Final(digest, ctx);

    //  生产32位的字符串, 转换为十六进制字符串
    char md5_str[33]; // 32字符 + '\0' ,存放终止符 \0
    for (int i = 0; i < 16; i++) {
        // 不足的情况下补0， f:0f ,ab:ab

        // snprintf 是标准 C 库函数，用于安全地格式化字符串，相比 sprintf 多了一个缓冲区大小参数，防止缓冲区溢出。
        // str：目标缓冲区地址
        // size：缓冲区剩余可用大小,限制写入长度（3）
        // format：格式化字符串
        // ...：可变参数（要格式化的数据）

        // 每次 snprintf 会写入终止符，但下一次循环会覆盖它。最终通过 md5_str[32] = '\0' 显式添加终止符，确保字符串完整。
        snprintf(md5_str + i*2, 3, "%02x", digest[i]);
    }
    md5_str[32] = '\0';

    // 释放资源
    env->ReleaseStringUTFChars(params, paramsStr);

    return env->NewStringUTF(md5_str);
}

/**
PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);
Signature[] signatures = packageInfo.signatures;
return signatures[0].toCharsString();
*/
extern "C"
JNIEXPORT jstring JNICALL
Java_com_xaye_myjni_jni_SignatureUtils_signatureVerify(JNIEnv *env, jclass clazz, jobject context) {
    // C 调用 Java 代码，分为以下四步

    // 1. 获取包名
    jclass j_clz = env->GetObjectClass(context);
    jmethodID j_method_id = env->GetMethodID(j_clz, "getPackageName", "()Ljava/lang/String;");
    jstring j_package_name = (jstring)env->CallObjectMethod(context, j_method_id);

    // 2. 比对包名是否一样
    const char *c_package_name = env->GetStringUTFChars(j_package_name, nullptr);
    if (strcmp(PACKAGE_NAME, c_package_name) != 0) {
        return env->NewStringUTF("error_package_name");
    }
    LOGI("包名一致：%s", c_package_name);
    // 3. 获取签名

    // 3.1 获取 PackageManager , 先获取方法id ,用这个id 去对应的对象获取，都是按这个步骤。
    j_method_id = env->GetMethodID(j_clz, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject j_package_manager = env->CallObjectMethod(context, j_method_id);

    // 3.2 获取 PackageInfo
    j_clz = env->GetObjectClass(j_package_manager);
    j_method_id = env->GetMethodID(j_clz, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject j_package_info = env->CallObjectMethod(j_package_manager, j_method_id, j_package_name, 0x00000040);

    // 3.3 Signature[] 数组
    j_clz = env->GetObjectClass(j_package_info);
    jfieldID j_field_id = env->GetFieldID(j_clz, "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray j_signatures = (jobjectArray)env->GetObjectField(j_package_info, j_field_id);

    // 3.4 获取 signatures[0]
    jobject j_signature_first = env->GetObjectArrayElement(j_signatures, 0);

    // 3.5 获取 signatures[0].toCharsString()
    j_clz = env->GetObjectClass(j_signature_first); // 获取 Signature 类,因为 signatures[0] 是 Signature 类
    j_method_id = env->GetMethodID(j_clz, "toCharsString", "()Ljava/lang/String;");
    jstring j_signature_str = (jstring)env->CallObjectMethod(j_signature_first, j_method_id);
    const char *c_signature_str = env->GetStringUTFChars(j_signature_str, nullptr);
    if (strcmp(c_signature_str, APP_SIGNATURE) != 0) {
        return env->NewStringUTF("error_signature");
    }
    // 4. 签名比对
    is_verify = 1;
    LOGI("签名校验成功√");
    return env->NewStringUTF("success");
}