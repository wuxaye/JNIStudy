#include <jni.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <android/log.h>

#define LOG_TAG "SerialPortJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static speed_t getBaudrate(jint baudrate) {
    // 定义波特率查找表
    static const struct {
        jint baudrate;
        speed_t speed;
    } baudrate_table[] = {
            {0,       B0},
            {50,      B50},
            {75,      B75},
            {110,     B110},
            {134,     B134},
            {150,     B150},
            {200,     B200},
            {300,     B300},
            {600,     B600},
            {1200,    B1200},
            {1800,    B1800},
            {2400,    B2400},
            {4800,    B4800},
            {9600,    B9600},
            {19200,   B19200},
            {38400,   B38400},
            {57600,   B57600},
            {115200,  B115200},
            {230400,  B230400},
            {460800,  B460800},
            {500000,  B500000},
            {576000,  B576000},
            {921600,  B921600},
            {1000000, B1000000},
            {1152000, B1152000},
            {1500000, B1500000},
            {2000000, B2000000},
            {2500000, B2500000},
            {3000000, B3000000},
            {3500000, B3500000},
            {4000000, B4000000},
    };

    // 查找波特率
    // 整个数组的大小 / 每个元素的大小 = 数组大小
    for (size_t i = 0; i < sizeof(baudrate_table) / sizeof(baudrate_table[0]); i++) {
        if (baudrate_table[i].baudrate == baudrate) {
            return baudrate_table[i].speed;
        }
    }

    // 默认返回值
    return -1;
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_xaye_myjni_jni_SerialPort_open(JNIEnv *env, jobject thiz, jstring path, jint baud_rate,
                                    jint data_bits, jint stop_bits, jchar parity) {
    const char *path_utf = env->GetStringUTFChars(path, NULL);

    // 打开串口设备，O_RDWR 允许读写，O_NOCTTY 表示不作为控制终端
    int descriptor = open(path_utf, O_RDWR | O_NOCTTY | O_NONBLOCK);

    env->ReleaseStringUTFChars(path, path_utf);

    if (descriptor == -1) {
        return JNI_FALSE;
    }

    struct termios cfg;
    if (tcgetattr(descriptor, &cfg) != 0) {
        close(descriptor);
        return JNI_FALSE; // 获取配置失败
    }

    // 设置波特率
    speed_t baudrate = getBaudrate(baud_rate);

    cfsetispeed(&cfg, baudrate);
    cfsetospeed(&cfg, baudrate);

    // 设置数据位
    cfg.c_cflag &= ~CSIZE;
    cfg.c_cflag |= (data_bits == 7 ? CS7 : CS8);

    // 设置校验位
    switch (parity) {
        case 'N': // 无校验
        case 'n':
            cfg.c_cflag &= ~PARENB;
            break;
        case 'O': // 奇校验
        case 'o':
            cfg.c_cflag |= PARENB;
            cfg.c_cflag |= PARODD;
            break;
        case 'E': // 偶校验
        case 'e':
            cfg.c_cflag |= PARENB;
            cfg.c_cflag &= ~PARODD;
            break;
    }

    // 设置停止位
    if (stop_bits == 2) {
        cfg.c_cflag |= CSTOPB;
    } else {
        cfg.c_cflag &= ~CSTOPB;
    }

    tcsetattr(descriptor, TCSANOW, &cfg); // 应用设置

    // 将打开的文件描述符保存在本地变量中
    jclass cls = env->GetObjectClass(thiz);
    jfieldID fid = env->GetFieldID(cls, "mNativeContext", "I");

    env->SetIntField(thiz, fid, descriptor);
    return JNI_TRUE;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_xaye_myjni_jni_SerialPort_close(JNIEnv *env, jobject thiz) {
    jclass cls = env->GetObjectClass(thiz);
    jfieldID fid = env->GetFieldID(cls, "mNativeContext", "I");
    int descriptor = env->GetIntField(thiz, fid);

    if (descriptor != -1) {
        close(descriptor); // 关闭串口
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_xaye_myjni_jni_SerialPort_write(JNIEnv *env, jobject thiz, jbyteArray data) {
    jclass cls = env->GetObjectClass(thiz);
    jfieldID fid = env->GetFieldID(cls, "mNativeContext", "I");
    int descriptor = env->GetIntField(thiz, fid);

    if (descriptor == -1) return; // 检查描述符有效性

    // 获取数据数组
    jbyte *buffer = env->GetByteArrayElements(data, NULL);
    jsize len = env->GetArrayLength(data);

    // 写入串口
    write(descriptor, buffer, len);

    // 释放数据数组
    env->ReleaseByteArrayElements(data, buffer, 0);
}
extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_xaye_myjni_jni_SerialPort_read(JNIEnv *env, jobject thiz) {
    jclass cls = env->GetObjectClass(thiz);
    jfieldID fid = env->GetFieldID(cls, "mNativeContext", "I");
    int descriptor = env->GetIntField(thiz, fid);

    if (descriptor == -1) return nullptr; // 检查描述符有效性

    // 缓冲区读取
    jbyte buffer[1024];
    int bytesRead = read(descriptor, buffer, sizeof(buffer));

    // 如果读取到数据
    if (bytesRead > 0) {
        jbyteArray result = env->NewByteArray(bytesRead);
        env->SetByteArrayRegion(result, 0, bytesRead, buffer);
        return result;
    } else {
        return nullptr; // 没有数据或读取失败
    }
}