#pragma once

#include <glad/glad.h>

class Texture2D
{
public:
	unsigned int ID;

	Texture2D(const char* file);
	void Bind() const;
};