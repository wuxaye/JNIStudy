# JNIStudy

## 代码包含功能

- 传递int数据
- 传递String数据
- 传递Array数据
- ...

## 基础知识
### Java和Native之间基本数据类型转换
|     **Java类型** |     **JNI类型** |
|:---------------|:--------------|
|           byte |         jbyte |
|           char |         jchar |
|          short |        jshort |
|            int |          jint |
|           long |         jlong |
|          float |        jfloat |
|         double |       jdouble |
|        boolean |      jboolean |

### Java和Native之间引用数据类型转换
|          **Java类型** |       **JNI 类型** |
|:--------------------|:-----------------|
|         All objects |          jobject |
|    java.lang.String |          jstring |
|     java.lang.Class |           jclass |
| java.lang.Throwable |       jthrowable |
|           Object[ ] |  jobjectArray[ ] |
|          boolean[ ] | jbooleanArray[ ] |
|             byte[ ] |    jbyteArray[ ] |
|             char[ ] |    jcharArray[ ] |
|            short[ ] |   jshortArray[ ] |
|              int[ ] |     jintArray[ ] |
|             long[ ] |    jlongArray[ ] |
|            float[ ] |   jfloatArray[ ] |
|           double[ ] |  jdoubleArray[ ] |

### Native方法语句分析
```
extern "C" JNIEXPORT jstring JNICALL
Java_com_zqf_jnitest_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
    std::string hello = "Hello NDK";
    return env->NewStringUTF(hello.c_str());
}
```
### 解析：
- `extern “C”`：避免编绎器按照C++的方式去编绎C函数
1. C不支持函数的重载，编译之后函数名不变；
2. C++支持函数的重载（这点与Java一致），编译之后函数名会改变；
- `JNIEXPORT`：JNI重要标记关键字，不能少（VS结译能通过，运行会报错〕/(AS 运行不会报错)，规则(标记为该方法可以被外部调用)
3. 宏 JNIEXPORT 代表的就是右侧的表达式： **attribute** ((visibility ("default")))；
4. 或者也可以说： JNIEXPORT 是右侧表达式的别名；
5. 宏可表达的内容很多，如：一个具体的数值、一个规则、一段逻辑代码等；
- `JNICALL`：也是一个关键字，(可以少的)jni call〔约束函数入栈顺序，和堆栈内存清理的规则)
- `JNIEnv \*env` ：JNI桥梁核心，通过 JNIEnv 指针，可以在本地代码中操作与Java端相关的对象、方法和属性。
- `Java\_com\_zqf\_jnitest\_MainActivity\_stringFromJNI` ：Java\_包名com\_X×类名\_方法名
- `jstring` ：代表这个方法返回值为字符串，同理如果是void 则代表无返回值
- `jobject` ：MainActivity this 实例调用的
- `jclass`：MainActivity class 类调用的
- `env` 是一个指向 `JNIEnv` 的指针，必须使用 `->` 来调用 `NewStringUTF` 方法


### JNIEnv 主要功能：
**1.创建Java对象**：
```
// 示例：创建一个Java字符串对象
jstring str = env->NewStringUTF("Hello, JNI!");

```
**2.调用Java对象的方法**：
```
// 示例：调用Java对象的方法
jclass clazz = env->GetObjectClass(obj); // 获取对象的类
jmethodID methodId = env->GetMethodID(clazz, "methodName", "(methodSignature)methodReturnType"); // 获取方法ID
env->CallVoidMethod(obj, methodId, args); // 调用Java方法

```
**3.获取Java对象的属性**：
```
// 示例：获取Java对象的属性
jclass clazz = env->GetObjectClass(obj); // 获取对象的类
jfieldID fieldId = env->GetFieldID(clazz, "fieldName", "fieldSignature"); // 获取字段ID
jint value = env->GetIntField(obj, fieldId); // 获取字段值

```
### JNIEnv 使用注意事项：
- JNIEnv 是一个线程本地指针，它只在创建它的线程上下文中有效。不同线程的JNIEnv是不同的，不应该在一个线程中使用另一个线程的JNIEnv。
- JNI 的接口函数需要按照JNI规范来使用，并且需要小心管理本地引用，避免内存泄漏或内存溢出的问题。


### Native定义的函数名称作用于Java代表的意思
|               **函数名称** |                                      **作用** |
|:-----------------------|:--------------------------------------------|
|              NewObject |                                 创建Java类中的对象 |
|              NewString |                           创建Java类中的String对象 |
|         New<Type>Array |                              创建类型为Type的数组对象 |
|         Get<Type>Field |                                获得类型为Type的字段 |
|         Set<Type>Field |                                设置类型为Type的字段 |
|   GetStatic<Type>Field |                         获得类型为Type的static的字段 |
|   SetStatic<Type>Field |                         设置类型为Type的static的字段 |
|       Call<Type>Method |                       调用返回值类型为Type的static方法 |
| CallStatic<Type>Method |                       调用返回值类型为Type的static方法 |
|                        |                                             |

对比上面示例代码去记

ndk环境配置：[https://blog.csdn.net/cyf649669121/article/details/127646241](https://blog.csdn.net/cyf649669121/article/details/127646241)    