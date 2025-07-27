#include <format>
#include <iostream>
#include <string>

using namespace std;

#include "src/include/add.hpp"

int main() {
  // 默认初始化
  int a;
  // 拷贝初始化
  int b = 0;
  // 直接初始化
  int c(0);
  // 括号初始化
  int d{0};
  // 拷贝括号初始化
  int e = {0};
  int x{};  // 0
  // 拷贝直接初始化
  int y = int();  // 0, 不能写成 int y();
  cout << format("x: {}, y: {}, add: {}", x, y, add(1, 2));
  return 0;
}
