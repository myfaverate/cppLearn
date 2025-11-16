#ifndef LEARN001_HPP
#define LEARN001_HPP

#include <iostream>
#include <string>
#include <format>
#include <vector>
#include <fstream>
#include <iterator> 

#include <pugixml.hpp>
#include <rapidxml.hpp>
#include <tinyxml2.h> // 内部兼容 C++

// mysql
#include<mysql/jdbc.h>

namespace learn01 {
    void hello();
    void parseXmlToPerson();
    void parseXmlToPerson1();
    void parseXmlToPerson2();
    void getPersonsFromMysql();
}

#endif // LEARN001_HPP