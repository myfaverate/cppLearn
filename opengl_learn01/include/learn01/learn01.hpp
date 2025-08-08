#ifndef OPENGL_LEARN01_HPP
#define OPENGL_LEARN01_HPP

extern "C" {
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <stb_image.h>
}

#include <string_view>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <random>

void helloWorld();
void helloWorld1();
void helloWorld2();
void message();
void loadImage(const std::string_view &imagePath, const std::string_view &vertPath, const std::string_view &shaderPath);

#endif // OPENGL_LEARN01_HPP