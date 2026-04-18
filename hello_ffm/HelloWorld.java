import java.lang.foreign.*;
import java.lang.invoke.*;
import java.util.Optional;

public class HelloWorld {
    static {
        System.loadLibrary("hello_ffm");
    }
    @SuppressWarnings("UnnecessaryModifier")
    public static void main(@SuppressWarnings("unused") final String[] args) throws Throwable {

        System.out.println("PID: " + ProcessHandle.current().pid()); // 打出 PID 方便选进程
        System.in.read(); // 暂停，等你附加 GDB 后按回车继续

        final Linker linker = Linker.nativeLinker();
        final SymbolLookup lookup = SymbolLookup.loaderLookup();

        // 调用 void sayHello()
        final MethodHandle sayHello = linker.downcallHandle(
            lookup.find("sayHello").orElseThrow(),
            FunctionDescriptor.ofVoid()
        );

        sayHello.invoke();

        // 调用 const char* getString() → 返回指针
        final MethodHandle getString = linker.downcallHandle(
            lookup.find("getString").orElseThrow(),
            FunctionDescriptor.of(ValueLayout.ADDRESS)
        );

        Optional.ofNullable(getString.invoke()).filter(MemorySegment.class::isInstance)
                .map(MemorySegment.class::cast)
                .ifPresent((MemorySegment result) -> {
                    // 从指针读取字符串
                    final String str = result.reinterpret(Long.MAX_VALUE).getString(0);
                    System.out.println("str: " + str);
                });

        // final MemorySegment result = (MemorySegment) getString.invokeExact();
        // // 从指针读取字符串
        // final String str = result.reinterpret(Long.MAX_VALUE).getString(0);
        // System.out.println("str: " + str);
    }
}

//  java "--enable-native-access=ALL-UNNAMED" "-Djava.library.path=.\build" HelloWorld.java