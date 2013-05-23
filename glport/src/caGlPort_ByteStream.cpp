#include "caGlPort_ByteStream.h"


void glByteStream::loadByteStream(const char *name)
{
	if (data)
	{
		delete [] data;
		data = 0;
		size = 0;
	}
	FILE *fp = fopen(name, "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		data = new char[size];
		fread(data, size, 1, fp);
		fclose(fp);
	}
	cpos = 0;
}