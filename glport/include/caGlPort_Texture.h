#ifndef _CaGlPort_Texture_H_
#define _CaGlPort_Texture_H_

#include "caGlPort_define.h"

/// 实现下列接口
/// 纹理提供者
class glTexture : public iTexture
{
public:
	glTexture() : tex(0)
	{
	}
	virtual ~glTexture();
	virtual size_t getWidth() const
	{
		assert(tex);
		return size_t(width);
	}
	virtual size_t getHeight() const
	{
		assert(tex);
		return size_t(height);
	}
	unsigned int getGlTexture()
	{
		if (!tex)
		{
			loadTexture(this->getFilename());
		}
		return tex;
	}
	void loadTexture(const char *filename);
protected:
	unsigned int tex;
	int width;
	int height;
};

#endif//_CaGlPort_Texture_H_
