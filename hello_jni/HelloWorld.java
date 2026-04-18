public class HelloWorld {
    static {
        System.loadLibrary("hello"); // 加载本地库
    }
    public static void main(String[] args) throws Exception{
        System.out.println("PID: " + ProcessHandle.current().pid()); // 打出 PID 方便选进程
        System.in.read(); // 暂停，等你附加 GDB 后按回车继续
        final var helloWorld = new HelloWorld();
        helloWorld.sayHello(); // 调用本地方法
        System.out.println(helloWorld.getString()); // 调用本地方法并打印结果
    }
    // 声明本地方法
    public native void sayHello();
    public native String getString();
}

/*
public class HelloWorld {
    
    public static void main(String[] args) throws Exception {
        System.out.println("PID: " + ProcessHandle.current().pid()); // 打出 PID 方便选进程
        System.in.read(); // 暂停，等你附加 GDB 后按回车继续
        System.loadLibrary("hello"); // 加载本地库
        new HelloWorld().sayHello(); // 调用本地方法
        System.out.println(new HelloWorld().getString()); // 调用本地方法并打印结果
    }

    // 声明本地方法
    public native void sayHello();
    public native String getString();
}
*/
/*
java "--enable-native-access=ALL-UNNAMED" "-Djava.library.path=.\build" HelloWorld.java
*/