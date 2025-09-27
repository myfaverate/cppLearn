#include "learnc01.h"

// ========= Student =========
Student* StudentCreate(const char* name, const int age, const float score, const char *school){
    Student* self = malloc(sizeof(Student));
    if (self == NULL){
        printf("内存不足\n");
        return NULL;
    }
    self->parent.toString = StudentToString;
    self->parent.destroy = StudentDestroy;
    const int nameLength = strlen(name);
    self->parent.name = malloc(nameLength);
    strcpy(self->parent.name, name);
    self->parent.age = age;
    self->score = score;
    const int schoolLength = strlen(school);
    self->school = malloc(schoolLength);
    strcpy(self->school, school);
    printf("构造函数 -> Student(name=%s, age=%d, score=%f, school=%s)\n", self->parent.name, self->parent.age, self->score, self->school);
    return self;
}
static void StudentToString(Person *person){
    Student *student = (Student*) person;
    // student.
    printf("Student(name=%s, age=%d, score=%f, school=%s)\n", student->parent.name, student->parent.age, student->score, student->school);
}
static void StudentDestroy(Person *person){
    Student *student = (Student*) person;
    free(student->parent.name);
    free(student->school);
    free(student);
}

// ========= Teacher =========
Teacher* TeacherCreate(const char *name, const int age, const char *education, const char *school){
    Teacher *self = malloc(sizeof(Teacher));
    if(self == NULL){
        printf("Teacher 内存不足\n");
        return NULL;
    }

    self->parent.toString = TeacherToString;
    self->parent.destroy = TeacherDestroy;

    const int nameLength = strlen(name) + 1;
    self->parent.name = malloc(nameLength);
    strcpy(self->parent.name, name);
    self->parent.age = age;

    const int educationLength = strlen(education) + 1; // 带上 \0 空字符
    self->education = malloc(educationLength);
    strcpy(self->education, education);

    const int schoolLength = strlen(school) + 1;
    self->school = malloc(schoolLength);
    strcpy(self->school, school);

    printf("构造函数 Teacher(name=%s, age=%d, education=%s, school=%s, nameLength=%d)\n", self->parent.name, self->parent.age, self->education, self->school, nameLength);

    return self;
}
static void TeacherToString(Person *person){
    Teacher *teacher = (Teacher*) person;
    printf("Teacher(name=%s, age=%d, education=%s, school=%s)\n", teacher->parent.name, teacher->parent.age, teacher->education, teacher->school);
}
static void TeacherDestroy(Person *person){
    Teacher *teacher = (Teacher*) person;
    free(teacher->parent.name);
    free(teacher->school);
    free(teacher->education);
    free(teacher);
}

/**
 * 多态方法
 */
void printPersonInfo(Person *person){
    if(person == NULL){
        printf("person == NULL\n");
        return;
    }
    person->toString(person);
}

void hello(){
    Person *student = (Person*)StudentCreate("s_zsh", 18, 95.5, "tyut");
    Person *teacher = (Person*)TeacherCreate("t_zsh", 18, "本科", "tyut");
    printPersonInfo(teacher);
    printPersonInfo(student);
    student->destroy(student);
    teacher->destroy(teacher);
}