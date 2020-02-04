#pragma once
#include "vertex2f.h"
#include "GLFW\glfw3.h"
class primitive
{
public:
	
	~primitive();

	primitive();

	primitive(vertex2f v1, vertex2f v2, vertex2f v3, vertex2f v4);
	void setTextureID(GLint texID);

	vertex2f getPosition();


	void setPosition(vertex2f pos);

	void draw();

public:
	vertex2f v1, v2, v3, v4, position;
	float width, height, halfWidth, halfHeight;
	GLuint TextureID;

};





