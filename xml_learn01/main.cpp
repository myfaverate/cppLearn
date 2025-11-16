#include "learn001.hpp"

int main(int, char**){
    learn01::getPersonsFromMysql();
}
/*
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release "
    -DMYSQL_INCLUDE_DIR="D:/SoftWare/MySQL8.4/include" "
    -DMYSQL_LIB_DIR="D:/SoftWare/MySQL8.4/lib"
*/