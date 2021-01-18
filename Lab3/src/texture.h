#pragma once

#include <glad/glad.h>

#include "stb_image.h"

class Texture2D
{
public:
    unsigned int ID;
    
    Texture2D() {};
    Texture2D(const char* textureFile, bool alpha);
    
    void Bind() const;

private:
    void loadTexture(const char* file, bool alpha);
};