#include "caHgePort_System.h"
#include "caHgePort_ByteStream.h"
#include "caHgePort_Texture.h"
#include "caInterpolaters.h"

using cAni::Point2s;
using cAni::Point2f;
using cAni::StepInterpolater;
using cAni::LinearInterpolater;
using cAni::BsplineInterpolater;
HGE *hgeCurvedAniSystem::hge = 0;

iTexture *hgeCurvedAniSystem::allocTexture()
{
    return new hgeTexture;
}

void hgeCurvedAniSystem::release(iTexture *tex)
{
    delete tex;
}

void hgeCurvedAniSystem::release(iByteStream *bs)
{
    delete bs;
}
void hgeCurvedAniSystem::release(iCurveDataSet *cds)
{
    delete cds;
}
void hgeCurvedAniSystem::release(iCurveInterpolater *ci)
{
    // delete ci;
}
void hgeCurvedAniSystem::release(iStateMergerIf *sm)
{
    // delete sm;
}

iByteStream * hgeCurvedAniSystem::loadStream( const char *name )
{
    hgeByteStream *byteStream = new hgeByteStream;
    byteStream->loadByteStream(name);
    return byteStream;
}
//
//iCurveInterpolater *hgeCurvedAniSystem::createCurveInterpolater(const char *name)
//{
//    if (0 == strcmp("StepImage", name))
//    {
//        return new StepInterpolater<Image>;
//    }
//    else if (0 == strcmp("StepFloat", name))
//    {
//        return new StepInterpolater<float>;
//    }
//    else if (0 == strcmp("StepFloat2", name))
//    {
//        return new StepInterpolater<hgeVector>;
//    }
//    else if (0 == strcmp("StepFloat3", name))
//    {
//        return new StepInterpolater<Point3f>;
//    }
//    else if (0 == strcmp("StepRect", name))
//    {
//        return new StepInterpolater<Rect>;
//    }
//    /* NO linear image interpolaters currently
//    else if (0 == strcmp("LinearImage", name))
//    {
//    return new LinearInterpolater<Image>;
//    }
//    */
//    else if (0 == strcmp("LinearFloat", name))
//    {
//        return new LinearInterpolater<float>;
//    }
//    else if (0 == strcmp("LinearFloat2", name))
//    {
//        return new LinearInterpolater<hgeVector>;
//    }
//    else if (0 == strcmp("LinearFloat3", name))
//    {
//        return new LinearInterpolater<Point3f>;
//    }
//    else if (0 == strcmp("LinearRect", name))
//    {
//        return new LinearInterpolater<Rect>;
//    }
//    /* NO bspline image interpolaters currently
//    else if (0 == strcmp("BsplineImage", name))
//    {
//    return new BsplineInterpolater<Image>;
//    }
//    */
//    else if (0 == strcmp("BsplineFloat", name))
//    {
//        return new BsplineInterpolater<float>;
//    }
//    else if (0 == strcmp("BsplineFloat2", name))
//    {
//        return new BsplineInterpolater<hgeVector>;
//    }
//    else if (0 == strcmp("BsplineFloat3", name))
//    {
//        return new BsplineInterpolater<Point3f>;
//    }
//    else if (0 == strcmp("BsplineRect", name))
//    {
//        return new BsplineInterpolater<Rect>;
//    }
//    assert(0 && "unsupported curve interpolater.");
//    return 0;
//}
template<template <typename> class T >
inline iCurveInterpolater *createInterpolator_all(const char *name)
{
	if (0 == strcmp("Float", name))
    {
        static T<float> s_tf;
        return &s_tf;
    }
    else if (0 == strcmp("Float2", name))
    {
        static T<Point2f> s_t2f;
        return &s_t2f;
    }
    else if (0 == strcmp("Float3", name))
    {
        static T<Point3f> s_t3f;
        return &s_t3f;
    }
    else if (0 == strcmp("Rect", name))
    {
        static T<Rect> s_tr;
        return &s_tr;
    }
    /// and you could add any data types you want here
    //else if if (0 == strncmp("MyDataType", name, strlen("MyDataType")))
    //{
    //  return new T<MyDataType>;
    //}
	return 0;
}

template<template <typename> class T >
inline iCurveInterpolater *createInterpolator(const char *name)
{
	return createInterpolator_all<T>(name);
}

template<>
inline iCurveInterpolater *createInterpolator<StepInterpolater>(const char *name)
{
    if (0 == strcmp("Image", name)) // Image÷ª÷ß≥÷StepInterpolater
    {
        static StepInterpolater<Image> s_sii;
        return &s_sii;
    }
	return createInterpolator_all<StepInterpolater>(name);
}

iCurveInterpolater *hgeCurvedAniSystem::createCurveInterpolater(const char *name)
{
	if (0 == strncmp("Step", name, 4))
	{
		return createInterpolator<StepInterpolater>(name + 4);
	}
	else if (0 == strncmp("Linear", name, 6))
	{
		return createInterpolator<LinearInterpolater>(name + 6);
	}
	else if (0 == strncmp("Bspline", name, 7))
	{
		return createInterpolator<BsplineInterpolater>(name + 7);
	}
    /// and you could add any interpolaters you want here, such as sin/cos/circle...
    //else if if (0 == strncmp("sin", name, 3))
    //{
    //  return createInterpolator<SinInterpolater>(name + 3);
    //}
    assert(0 && "unsupported curve interpolater.");
	return 0;
}
