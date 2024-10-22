#include <jni.h>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <android/log.h>

// 定义用于日志的TAG
#define LOG_TAG "ProducerConsumer"
// 定义日志宏
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// 生产者消费者问题的类实现
class ProducerConsumer {
public:
    // 构造函数
    ProducerConsumer() : stop(false) {}

    // 启动生产者和消费者线程
    void start() {
        LOGI("start");
        // 启动生产者线程，传入当前对象的指针
        producer_thread = std::thread(&ProducerConsumer::producer, this);
        // 启动消费者线程，传入当前对象的指针
        consumer_thread = std::thread(&ProducerConsumer::consumer, this);
    }

    // 停止线程的函数
    void stopThreads() {
        {
            // 锁住互斥量，以安全地修改共享变量
            std::lock_guard<std::mutex> lock(mtx);
            stop = true; // 设置停止标志，表示不再生产或消费
        }
        // 通知所有在条件变量上等待的线程
        cond_var.notify_all();
        // 等待生产者线程结束
        producer_thread.join();
        // 等待消费者线程结束
        consumer_thread.join();

        LOGI("stopThreads");
    }

private:
    std::queue<int> buffer; // 共享缓冲区，用于存放生产的数据
    const unsigned int max_buffer_size = 5; // 最大缓冲区大小
    std::mutex mtx; // 用于保护共享数据的互斥锁
    std::condition_variable cond_var; // 用于线程间同步的条件变量
    std::thread producer_thread; // 生产者线程对象
    std::thread consumer_thread; // 消费者线程对象
    bool stop; // 停止标志，控制消费者线程的退出

    // 生产者线程的主函数
    void producer() {
        for (int i = 0; i < 20; ++i) {
            // 模拟生产过程，休眠1秒
            std::this_thread::sleep_for(std::chrono::milliseconds (1000));
            // 锁住互斥量，以确保对共享缓冲区的安全访问
            std::unique_lock<std::mutex> lock(mtx);

            if (stop) return;

            // 如果缓冲区已满，则等待
            while (buffer.size() >= max_buffer_size) {
                cond_var.wait(lock); // 等待条件变量通知
            }
            // 将新生产的数据放入缓冲区
            buffer.push(i);
            // 日志记录生产的数据
            LOGI("Produced: %d", i);
            // 通知消费者线程
            cond_var.notify_one();
        }
    }

    // 消费者线程的主函数
    void consumer() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            // 如果缓冲区为空且未收到停止信号，则等待
            while (buffer.empty() && !stop) {
                cond_var.wait(lock); // 等待条件变量通知
            }
            // 如果设置了停止标志且缓冲区为空，退出消费者线程
            if (stop && buffer.empty()) return;
            // 如果缓冲区非空，则消费数据
            if (!buffer.empty()) {
                int item = buffer.front(); // 获取缓冲区前端的数据
                buffer.pop(); // 从缓冲区移除该数据
                // 日志记录消费的数据
                LOGI("Consumed: %d", item);
                // 通知生产者线程
                cond_var.notify_one();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // 模拟消费时间
        }
    }
};




/*
 * 知识点：
 * reinterpret_cast 可以将一个类型的指针转换为另一种不相关类型的指针。例如，可以将 int* 转换为 void*，或者将 char* 转换为 float*。
 * */

extern "C"
JNIEXPORT jlong JNICALL
Java_com_xaye_myjni_JNI3_startProducerConsumer(JNIEnv *env, jobject thiz) {

    auto *pc = new ProducerConsumer();

    pc->start();

    // 返回指向 ProducerConsumer 对象的指针，将其转换为 jlong 类型以供 Java 使用
    return reinterpret_cast<jlong>(pc); // 返回指针
}

/*
 * 注意：
 *  这个方法 JNI 调用在主线程中执行，且涉及到的 C++ 线程停止操作没有及时返回控制权给 Java 虚拟机 (JVM)，主线程会卡一下
 *
 *  解决方法就是 std::thread 在子线程中执行，不要影响主线程操作
 * */
extern "C"
JNIEXPORT void JNICALL
Java_com_xaye_myjni_JNI3_stopProducerConsumer(JNIEnv *env, jobject thiz, jlong ptr) {

    // 将传入的 jlong 指针转换回 ProducerConsumer 类型的指针
    auto *pc = reinterpret_cast<ProducerConsumer *>(ptr);

    std::thread([pc]() {
        pc->stopThreads();

        delete pc; // 释放内存
    }).detach();
}