#ifndef _caGlPort_ByteStream_H_
#define _caGlPort_ByteStream_H_

#include "caGlPort_define.h"

class glByteStream : public iByteStream
{
public:
	glByteStream() : data(0), size(0)
	{
	}
	virtual ~glByteStream()
	{
		if (data)
		{
			//            gl->Resource_Free(data);
			size = 0;
		}
		//        gl->Release();
	}
	virtual bool write(const void *, size_t )
	{
		assert(0 && "not allowed to write");
		return false;
	}
	virtual bool read(void *buf, size_t len)
	{
		if (len > size - cpos)
			return false;
		if (len > 0)
		{
			memcpy(buf, data + cpos, len);
			cpos += (DWORD)len;
		}
		return true;
	}
	/// seek to position
	virtual void seek(size_t pos)
	{
		cpos = min(DWORD(pos), size);
	}
	/// seek with an offset
	virtual void seekOffset(int offset)
	{
		seek(cpos + offset);
	}
	virtual size_t length() const
	{
		return size;
	}
	virtual size_t tell() const
	{
		return cpos;
	}
	void loadByteStream(const char *name);

protected:
	char *data;
	DWORD size;
	DWORD cpos;
};

#endif//_caGlPort_ByteStream_H_
