#include "primitive.h"
#include "vertex2f.h"
#include <GLFW\glfw3.h>



primitive::~primitive()
{
}

	primitive::primitive() {
		width = 0;
		height = 0;
		halfWidth = 0;
		halfHeight = 0;
	}

	primitive::primitive(vertex2f v1, vertex2f v2, vertex2f v3, vertex2f v4) {
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
		this->v4 = v4;


		width = v4.x - v1.x;
		height = v2.y - v1.y;
	}

	void primitive::setTextureID(GLint texID)
	{
		TextureID = texID;
	}

	vertex2f primitive::getPosition() {
		return position;
	}


	void primitive::setPosition(vertex2f pos) {
		position = pos;

		v1.x = position.x;  //-1
		v1.y = position.y; //-0.4

		v2.x = v1.x;
		v2.y = position.y + height; //-0.2

		v3.x = position.x + width; //-0.8
		v3.y = v2.y;  //-0.2

		v4.x = v3.x;
		v4.y = v1.y; //-0.4

	}

	void primitive::draw() {
		if (TextureID != 0) {

			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND | GL_ALPHA_TEST);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, TextureID);

		}

		if (v1.y == v2.y) {
			v2.y += 0.2;
			v3.y += 0.2;
			v3.x += 0.2;
			v4.x += 0.2;
		}

		glBegin(GL_QUADS);
		
		glTexCoord2f(0.0, 1.0);
		glVertex2f(v1.x, v1.y);
				
		glTexCoord2f(0.0, 0.0);
		glVertex2f(v2.x, v2.y);
				
		glTexCoord2f(1.0, 0.0);
		glVertex2f(v3.x, v3.y);
				
		glTexCoord2f(1.0, 1.0);
		glVertex2f(v4.x, v4.y);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);

	}

