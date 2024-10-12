package com.xaye.myjni;

import java.lang.reflect.Method;

/**
 * Author xaye
 *
 * @date: 2024/10/10
 */
public class Demo {
    public static void main(String[] args) throws Exception {
        //1.获取类的字节码
        Class<Test> clazz = Test.class;
        //2.通过字节码获取方法
        //参数 "print"：要获取的方法名。
        //参数 String.class：方法参数的类型，这里表示 print 方法接受一个字符串参数。
        Method method = clazz.getDeclaredMethod("print", String.class);
        //3.通过字节码创建对象
        Test test = clazz.newInstance();
        //4.调用方法
        method.invoke(test, "hello world");
    }
}

class Test{
    public void print(String str) {
        System.out.println(str);
    }
}
