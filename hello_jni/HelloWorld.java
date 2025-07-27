public class HelloWorld {
    
    public static void main(String[] args) {
        System.loadLibrary("hello"); // 加载本地库
        new HelloWorld().sayHello(); // 调用本地方法
        System.out.println(new HelloWorld().getString()); // 调用本地方法并打印结果
    }

    // 声明本地方法
    public native void sayHello();
    public native String getString();
}