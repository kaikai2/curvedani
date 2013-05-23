#include "caGlPort_CurvedDataSet.h"

template<>
bool glCurveDataSet<Image>::saveToByteStream(iByteStream *bs) const
{
	assert(sizeof(Image) == getDataSize());

	unsigned short size = (unsigned short)getDataCount();
	if (!bs->write(size))
		return false;


	for (size_t i = 0; i < size; i++)
	{
		int time = getTime(i);
		const Image &image = *(const Image *)getData(i);
		const char *filename = image.getTexture().getFilename();
		int len = (int)strlen(filename);
		if (len >= MAX_PATH || !bs->write(time) || !bs->write(len) || !bs->write(filename, len) || !bs->write(image.getRect()))
			return false;
	}
	return true;
}


template<>
const char *glCurveDataSet<Image>::getTypeName() const
{
	return "Image";
}
template<>
void glCurveDataSet<Image>::setDataString(size_t index, const char *valueStr)
{
	if (char *buf = _strdup(valueStr))
	{
		char *pos = strchr(buf, ';');
		assert(pos && *pos == ';');
        if (pos)
        {
            *pos = 0;
            Image image(iSystem::GetInstance()->getTexture(buf));
            int temp[4];
            if (sscanf(pos + 1, "%d,%d,%d,%d", temp, temp + 1, temp + 2, temp + 3) != 4)
            {
                assert(0);
            }
            Rect &rect = image.getRect();
            rect.leftTop = cAni::Point2s((short)temp[0], (short)temp[1]);
            rect.rightBottom = cAni::Point2s((short)temp[2], (short)temp[3]);
            this->setData(index, &image);
        }
		free(buf);
	}
}
template<>
const char *glCurveDataSet<Image>::getDataString(size_t index) const
{
	static char buf[MAX_PATH + 10 * 4 + 5];
	assert(index < data.size());
	const Image &image = *(const Image *)getData(index);
	const char *texFilename = image.getTexture().getFilename();
	if (!texFilename)
		return 0;
	const Rect &rect = image.getRect();
	sprintf(buf, "%s;%d,%d,%d,%d", texFilename, rect.leftTop.x, rect.leftTop.y, rect.rightBottom.x, rect.rightBottom.y);
	return buf;
}

template<>
const char *glCurveDataSet<float>::getTypeName() const
{
	return "Float";
}
template<>
void glCurveDataSet<float>::setDataString(size_t index, const char *valueStr)
{
    float f;
    if (1 == sscanf(valueStr, "%f", &f))
    {
        this->setData(index, &f);
    }
}
template<>
const char *glCurveDataSet<float>::getDataString(size_t index) const
{
	static char buf[10];
	float f = *(const float *)getData(index);
	sprintf(buf, "%f", f);
	return buf;
}

template<>
const char *glCurveDataSet<Point2f>::getTypeName() const
{
	return "Float2";
}
template<>
void glCurveDataSet<Point2f>::setDataString(size_t index, const char *valueStr)
{
    Point2f v;
    if (2 == sscanf(valueStr, "%f,%f", &v.x, &v.y))
    {
        this->setData(index, &v);
    }
}
template<>
const char *glCurveDataSet<Point2f>::getDataString(size_t index) const
{
	static char buf[10];
	const Point2f &v = *(const Point2f *)getData(index);
	sprintf(buf, "%f,%f", v.x, v.y);
	return buf;
}

template<>
const char *glCurveDataSet<Point3f>::getTypeName() const
{
	return "Float3";
}

template<>
void glCurveDataSet<Point3f>::setDataString(size_t index, const char *valueStr)
{
    Point3f v;
    if (3 == sscanf(valueStr, "%f,%f,%f", &v.x, &v.y, &v.z))
    {
        this->setData(index, &v);
    }
}
template<>
const char *glCurveDataSet<Point3f>::getDataString(size_t index) const
{
	static char buf[10];
	const Point3f &v = *(const Point3f *)getData(index);
	sprintf(buf, "%f,%f,%f", v.x, v.y, v.z);
	return buf;
}

template<>
const char *glCurveDataSet<Rect>::getTypeName() const
{
	return "Rect";
}
template<>
void glCurveDataSet<Rect>::setDataString(size_t index, const char *valueStr)
{
    Rect v;
    if (4 == sscanf(valueStr, "%d,%d,%d,%d", &v.leftTop.x, &v.leftTop.y, &v.rightBottom.x, &v.rightBottom.y))
    {
        this->setData(index, &v);
    }
}
template<>
const char *glCurveDataSet<Rect>::getDataString(size_t index) const
{
	static char buf[10];
	const Rect &v = *(const Rect *)getData(index);
	sprintf(buf, "%d,%d,%d,%d", v.leftTop.x, v.leftTop.y, v.rightBottom.x, v.rightBottom.y);
	return buf;
}

// 对image的读写进行特化
template<> bool glCurveDataSet<Image>::loadFromByteStream(iByteStream *bs)
{
	assert(sizeof(Image) == getDataSize());

	unsigned short size;
	if (!bs->read(size))
		return false;
	resize(size);

	int len;
	char name[MAX_PATH];
	Rect rect;

	for (size_t i = 0; i < size; i++)
	{
		int time;
		if (!bs->read(time) || !bs->read(len) || len >= MAX_PATH || !bs->read(name, len) || !bs->read(rect))
		{
			resize(0);
			return false;
		}

		name[len] = 0;
		Image image(iSystem::GetInstance()->getTexture(name), rect);
		setData(i, &image, time);
	}
	return true;
}