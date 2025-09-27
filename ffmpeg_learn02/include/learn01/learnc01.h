#ifndef LEARNC01_H
#define LEARNC01_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. 基类
typedef struct Person Person;
// 基类方法
typedef void (*ToStringFunc)(Person*);
typedef void (*DestroyFunc)(Person*);
// 基类结构体
struct Person {
    char *name;
    int age;
    ToStringFunc toString;
    DestroyFunc destroy;
};

// 2. 定义派生类类型别名
typedef struct Student Student;

// 定义结构体类型：struct Student
struct Student {
    Person parent;
    float score;    // 成绩（浮点数）
    char *school;
};

// 3. 定义派生类类型别名
typedef struct Teacher Teacher;

struct Teacher {
    Person parent;
    char *education;
    char *school;
};

// 定义方法
/**
 * 创建 Student 对象方法
 */
Student* StudentCreate(const char *name, const int age, const float score, const char *school);
static void StudentToString(Person *person);
static void StudentDestroy(Person *person);

/**
 * 创建 Teacher 对象方法
 */
Teacher* TeacherCreate(const char *name, const int age, const char *education, const char *school);
static void TeacherDestroy(Person *person);
static void TeacherToString(Person *person);

/**
 * 多态方法
 */
void printPersonInfo(Person *person);

void hello();

#endif // LEARNC01_H