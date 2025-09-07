// #include "learn01.hpp"
// class ThreadPool {
// public:
//     explicit ThreadPool(size_t numThreads) {
//         start(numThreads);
//         std::cout << "ThreadPool Constructor called with " 
//                   << numThreads << " threads." << std::endl;
//     }

//     ~ThreadPool() {
//         std::cout << "ThreadPool Destructor called" << std::endl;
//         stop();
//     }

//     // 提交任务，返回 future
//     template<class T>
//     auto enqueue(T task) -> std::future<typename std::result_of<T()>::type> {
//         using ReturnType = typename std::result_of<T()>::type;

//         auto wrapper = std::make_shared<std::packaged_task<ReturnType()>>(std::move(task));

//         {
//             std::unique_lock<std::mutex> lock(mEventMutex);
//             mTasks.emplace([=] {
//                 (*wrapper)();
//             });
//         }

//         mEventVar.notify_one();
//         return wrapper->get_future();
//     }

// private:
//     std::vector<std::thread> mThreads;
//     std::condition_variable mEventVar;

//     std::mutex mEventMutex;
//     bool mStopping = false;

//     std::queue<std::function<void()>> mTasks;

//     void start(size_t numThreads) {
//         for (size_t i = 0; i < numThreads; ++i) {
//             mThreads.emplace_back([this] {
//                 while (true) {
//                     std::function<void()> task;

//                     {   // 临界区
//                         std::unique_lock<std::mutex> lock(mEventMutex);

//                         mEventVar.wait(lock, [this]{ return mStopping || !mTasks.empty(); });

//                         if (mStopping && mTasks.empty())
//                             break;

//                         task = std::move(mTasks.front());
//                         mTasks.pop();
//                     }

//                     task(); // 执行任务
//                 }
//             });
//         }
//     }

//     void stop() {
//         {   // 通知所有线程退出
//             std::unique_lock<std::mutex> lock(mEventMutex);
//             mStopping = true;
//         }

//         mEventVar.notify_all();

//         for (auto &thread : mThreads)
//             thread.join();
//     }
// };

// class Person {
// private:
//     std::string name;
//     int age;
//     std::string gender;
// public:
//     Person() : name(""), age(0), gender(""){
//         std::cout << "Person Default Constructor" << "\n";
//     }
//     Person(std::string name, int age, std::string gender) : name(name), age(age), gender(gender){
//         std::cout << std::format("Person Constructor name: {}, age: {}, gender: {}", name, age, gender) << "\n";
//     }
//     Person(const Person& other) : name(other.name), age(other.age), gender(other.gender){
//         std::cout << std::format("Person Copy Constructor name: {}, age: {}, gender: {}", name, age, gender) << "\n";
//     }
//     Person(Person&& other) noexcept : name(std::move(other.name)), age(std::exchange(other.age, 0)), gender(std::move(other.gender)){
//         std::cout << std::format("Person Move Constructor name: {}, age: {}, gender: {}", name, age, gender) << "\n";
//     }
//     ~Person(){
//         std::cout << std::format("Person Destructor name: {}, age: {}, gender: {}", name, age, gender) << "\n";
//     }
//     Person& operator=(const Person& other){
//         if(this != &other){
//             name = other.name;
//             age = other.age;
//             gender = other.gender;
//         }
//         std::cout << std::format("Person Copy Assignment Operator name: {}, age: {}, gender: {}", name, age, gender) << "\n";
//         return *this;
//     }
//     Person& operator=(Person&& other) noexcept {
//         if(this != &other){
//             name = std::move(other.name);
//             age = std::exchange(other.age, 0);
//             gender = std::move(other.gender);
//         }
//         std::cout << std::format("Person Move Assignment Operator name: {}, age: {}, gender: {}", name, age, gender) << "\n";
//         return *this;   
//     }
//     friend std::ostream& operator<<(std::ostream& os, const Person& person) {
//         os << std::format("name: {}, age: {}, gender: {}", person.name, person.age, person.gender);
//         return os;
//     }
//     std::string getName() const { 
//         return name;
//     }
//     int getAge() const { 
//         return age;
//     }
//     std::string getGender() const { 
//         return gender; 
//     }
//     void setName(const std::string name) { 
//         this->name = name; 
//     }
//     void setAge(const int age) { 
//         this->age = age; 
//     }
//     void setGender(const std::string gender){
//         this->gender = gender;
//     }
// };
// /**
//  * @author zsh
//  * 所有类对象的成员方法均是inline方法
//  * 标准写法是
//  * hpp: 声明类
//  * cpp: 实现类
//  */
// class Animal {
// private:
//     std::string species;
//     int age;
// public:
//     // 默认构造函数
//     Animal() : species("单细胞"), age(0){
//         std::cout << "Animal Default Constructor" << "\n";
//     }
//     // 带参构造函数
//     Animal(std::string species, int age) : species(species), age(age){
//         std::cout << std::format("Animal Constructor species: {}, age: {}", species, age) << "\n";
//     }
//     // 拷贝构造函数
//     Animal(const Animal& other) : species(other.species), age(other.age){
//         std::cout << std::format("Animal Copy Constructor species: {}, age: {}", species, age) << "\n";
//     }
//     // 移动构造函数
//     Animal(Animal&& other) noexcept : species(std::move(other.species)), age(std::exchange(other.age, 0)){
//         std::cout << std::format("Animal Move Constructor species: {}, age: {}", species, age) << "\n";
//     }
//     // 析构函数
//     virtual ~Animal(){
//         std::cout << std::format("Animal Destructor species: {}, age: {}", species, age) << "\n";
//     }
//     // 重写赋值运算符
//     Animal& operator=(const Animal& other){
//         if(this != &other){
//             species = other.species;
//             age = other.age;
//         }
//         std::cout << std::format("Animal Copy Assignment Operator species: {}, age: {}", species, age) << "\n";
//         return *this;
//     }
//     // 移动赋值运算符
//     Animal& operator=(Animal&& other) noexcept {
//         if(this != &other){
//             species = std::move(other.species);
//             age = std::exchange(other.age, 0);
//         }
//         std::cout << std::format("Animal Move Assignment Operator species: {}, age: {}", species, age) << "\n";
//         return *this;   
//     }
//     // 重新<<运算符, 友元：访问成员私有变量
//     friend std::ostream& operator<<(std::ostream& os, const Animal& animal) {
//         os << std::format("species: {}, age: {}", animal.species, animal.age);
//         return os;
//     }
//     // 定义为全局方法也是可以的
//     // static std::ostream& operator<<(std::ostream& os, const Animal& animal) {
//     //     os << std::format("species: {}, age: {}", animal.getSpecies(), animal.getAge());
//     //     return os;
//     // }

//     /**
//      * 第一个 const 代表返回不可变的值，一般和引用搭配
//      * 第二个 const 代表这个函数无法修改this对象的任何属性
//      */
//     const std::string& getSpecies() const { 
//         return species;
//     }
//     int getAge() const { 
//         return age; 
//     }
//     void setSpecies(const std::string species) { 
//         this->species = species; 
//     }
//     void setAge(const int age) { 
//         this->age = age; 
//     }
//     // 虚函数，有虚函数，此对象仍然是一个正常可实例化的对象
//     virtual void makeSound() const {
//         std::cout << "Some generic animal sound" << std::endl;
//     };
//     virtual void eat(){
//         std::cout << "Animal is eating" << std::endl;
//     }
//     virtual void sleep(){
//         std::cout << "Animal is sleeping" << std::endl;
//     }
// };

// class Dog : virtual public Animal {
// public:
//     // 默认构造函数
//     Dog() : Animal("狗", 0){
//         std::cout << std::format("Dog Default Constructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 带参构造函数
//     Dog(int age) : Animal("狗", age){
//         std::cout << std::format("Dog Constructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 拷贝构造
//     Dog(const Dog& other) : Animal(other.getSpecies(), other.getAge()){
//         std::cout << std::format("Dog Copy Constructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 移动构造
//     Dog(Dog&& other) noexcept : Animal(std::move(other)){
//         std::cout << std::format("Dog Move Constructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 析构函数
//     ~Dog() override {
//         std::cout << std::format("Dog Destructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 拷贝赋值运算符
//     Dog& operator=(const Dog& other){
//         Animal::operator=(other);
//         std::cout << std::format("Dog Copy Assignment Operator Species: {}, age: {}\n", getSpecies(), getAge());
//         return *this;
//     }
//     // 移动赋值运算符
//     Dog& operator=(Dog&& other) noexcept {
//         Animal::operator=(std::move(other));  // 将other转为Animal&&，匹配父类移动赋值
//         std::cout << std::format("Dog Move Assignment Operator species: {}, age: {}", getSpecies(), getAge()) << "\n";
//         return *this;   
//     }
//     // 狗叫
//     void makeSound() const override  {
//         std::cout << "Woof! Woof!" << std::endl;
//     }
// };


// class Cat : virtual public Animal {
// public:
//     // 默认构造函数
//     Cat() : Animal("猫", 0){
//         std::cout << std::format("Cat Default Constructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 带参构造函数
//     Cat(int age) : Animal("猫", age){
//         std::cout << std::format("Cat Constructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 拷贝构造
//     Cat(const Cat& other) : Animal(other.getSpecies(), other.getAge()){
//         std::cout << std::format("Cat Copy Constructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 移动构造
//     Cat(Cat&& other) noexcept : Animal(std::move(other)){
//         std::cout << std::format("Cat Move Constructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 析构函数
//     ~Cat() override {
//         std::cout << std::format("Cat Destructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 拷贝赋值运算符
//     Cat& operator=(const Cat& other){
//         Animal::operator=(other);
//         std::cout << std::format("Cat Copy Assignment Operator Species: {}, age: {}\n", getSpecies(), getAge());
//         return *this;
//     }
//     // 移动赋值运算符
//     Cat& operator=(Cat&& other) noexcept {
//         Animal::operator=(std::move(other));  // 将other转为Animal&&，匹配父类移动赋值
//         std::cout << std::format("Cat Move Assignment Operator species: {}, age: {}", getSpecies(), getAge()) << "\n";
//         return *this;   
//     }
//     // 猫叫
//     void makeSound() const override  {
//         std::cout << "Mia O! Mia O!" << std::endl;
//     }
// };

// class CatDog : public Dog, public Cat {
// public:
//     // 默认构造函数
//     CatDog() : Animal("猫狗", 0){
//         std::cout << std::format("CatDog Default Constructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 带参构造函数
//     CatDog(int age) : Animal("猫狗", age){
//         std::cout << std::format("CatDog Constructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 拷贝构造
//     CatDog(const CatDog& other) : Animal(other.getSpecies(), other.getAge()){
//         std::cout << std::format("CatDog Copy Constructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 移动构造
//     CatDog(CatDog&& other) noexcept : Animal(std::move(other)){
//         std::cout << std::format("CatDog Move Constructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 析构函数
//     ~CatDog() override {
//         std::cout << std::format("CatDog Destructor species: {}, age: {}\n", getSpecies(), getAge());
//     }
//     // 拷贝赋值运算符
//     CatDog& operator=(const CatDog& other){
//         Animal::operator=(other);
//         std::cout << std::format("CatDog Copy Assignment Operator Species: {}, age: {}\n", getSpecies(), getAge());
//         return *this;
//     }
//     // 移动赋值运算符
//     CatDog& operator=(CatDog&& other) noexcept {
//         Animal::operator=(std::move(other));  // 将other转为Animal&&，匹配父类移动赋值
//         std::cout << std::format("CatDog Move Assignment Operator species: {}, age: {}", getSpecies(), getAge()) << "\n";
//         return *this;   
//     }
//     // 猫狗叫
//     void makeSound() const override  {
//         std::cout << "Mia Woof! Mia Woof!" << std::endl;
//     }
// };

// namespace learn01{
//     void learn01(){
//         ThreadPool pool(4);
//         uint32_t count = 0;
//         constexpr uint32_t COUNT = 100000000;
//         auto f1 = pool.enqueue([&] {
//             for(uint32_t i = 0; i < COUNT; ++ i) count ++ ;
//             return 1;
//         });

//         auto f2 = pool.enqueue([&] {
//             for(uint32_t i = 0; i < COUNT; ++ i) count ++ ;
//             return 2;
//         });
// #ifdef NDEBUG
//     std::cout << "Release build (optimizations likely enabled)\n";
// #else
//     std::cout << "Debug build\n";
// #endif
//         std::cout << "Result of Task 1: " << f1.get() << std::endl;
//         std::cout << "Result of Task 2: " << f2.get() << std::endl;
//         std::cout << "count: " << count << std::endl;
//     }
//     void learn03(){
//         CatDog c(1);
//         std::cout << c << "\n";
//     }
// }