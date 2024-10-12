package com.xaye.myjni

import org.junit.Test

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
class ExampleUnitTest {
    @Test
    fun addition_isCorrect() {
        //1.获取类的字节码

        //1.获取类的字节码
        val clazz = com.xaye.myjni.Test::class.java
        //2.通过字节码获取方法
        val method = clazz.getDeclaredMethod("print", String::class.java)
        //3.通过字节码创建对象
        val test = clazz.newInstance()
        //4.调用方法
        method.invoke(test, "hello world")
    }
}


internal class Test {
    fun print(str: String?) {
        println(str)
    }
}
