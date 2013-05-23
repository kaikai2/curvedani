#include "caHgePort_CurvedDataSet.h"
//#include "caPoint2d.h"

using cAni::Point2s;

// 对image的读写进行特化
bool hgeCurveDataSet<Image>::loadFromByteStream(iByteStream *bs)
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

bool hgeCurveDataSet<Image>::saveToByteStream(iByteStream *bs) const
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


const char *hgeCurveDataSet<Image>::getTypeName() const
{
    return "Image";
}
void hgeCurveDataSet<Image>::setDataString(size_t index, const char *valueStr)
{
    char *buf = _strdup(valueStr);
    char *pos = strchr(buf, ';');
    assert(pos && *pos == ';');
    *pos = 0;
    
    Image image(iSystem::GetInstance()->getTexture(buf));
    int temp[4];
    if (sscanf(pos + 1, "%d,%d,%d,%d", temp, temp + 1, temp + 2, temp + 3) != 4)
    {
        assert(0);
    }
    Rect &rect = image.getRect();
    rect.leftTop = Point2s((short)temp[0], (short)temp[1]), rect.rightBottom = Point2s((short)temp[2], (short)temp[3]);
    free(buf);
    this->setData(index, &image);
}
const char *hgeCurveDataSet<Image>::getDataString(size_t index) const
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

const char *hgeCurveDataSet<float>::getTypeName() const
{
    return "Float";
}
void hgeCurveDataSet<float>::setDataString(size_t index, const char *valueStr)
{
    float f;
    sscanf(valueStr, "%f", &f);
    this->setData(index, &f);
}
const char *hgeCurveDataSet<float>::getDataString(size_t index) const
{
    static char buf[10];
    float f = *(const float *)getData(index);
    sprintf(buf, "%f", f);
    return buf;
}

const char *hgeCurveDataSet<hgeVector>::getTypeName() const
{
    return "Float2";
}
void hgeCurveDataSet<hgeVector>::setDataString(size_t index, const char *valueStr)
{
    hgeVector v;
    sscanf(valueStr, "%f,%f", &v.x, &v.y);
    this->setData(index, &v);
}
const char *hgeCurveDataSet<hgeVector>::getDataString(size_t index) const
{
    static char buf[10];
    const hgeVector &v = *(const hgeVector *)getData(index);
    sprintf(buf, "%f,%f", v.x, v.y);
    return buf;
}

const char *hgeCurveDataSet<Point3f>::getTypeName() const
{
    return "Float3";
}
void hgeCurveDataSet<Point3f>::setDataString(size_t index, const char *valueStr)
{
    Point3f v;
    sscanf(valueStr, "%f,%f,%f", &v.x, &v.y, &v.z);
    this->setData(index, &v);
}
const char *hgeCurveDataSet<Point3f>::getDataString(size_t index) const
{
    static char buf[10];
    const Point3f &v = *(const Point3f *)getData(index);
    sprintf(buf, "%f,%f,%f", v.x, v.y, v.z);
    return buf;
}

const char *hgeCurveDataSet<Rect>::getTypeName() const
{
    return "Rect";
}
void hgeCurveDataSet<Rect>::setDataString(size_t index, const char *valueStr)
{
    Rect r;
    sscanf(valueStr, "%d,%d,%d,%d", &r.leftTop.x, &r.leftTop.y, &r.rightBottom.x, &r.rightBottom.y);
    this->setData(index, &r);
}
const char *hgeCurveDataSet<Rect>::getDataString(size_t index) const
{
    static char buf[10];
    const Rect &r = *(const Rect *)getData(index);
    sprintf(buf, "%d,%d,%d,%d", r.leftTop.x, r.leftTop.y, r.rightBottom.x, r.rightBottom.y);
    return buf;
}
