#include "caGlPort_System.h"
#include "caGlPort_Texture.h"
#include "caGlPort_ByteStream.h"
#include "caInterpolaters.h"
#include "caMergers.h"


using cAni::StepInterpolater;
using cAni::LinearInterpolater;
using cAni::BsplineInterpolater;
using cAni::KeepMerger;
using cAni::GainMerger;
using cAni::ReduceMerger;
using cAni::SaturateMerger;
using cAni::MultMerger;

iTexture *glCurvedAniSystem::allocTexture()
{
	return new glTexture;
}

void glCurvedAniSystem::release(iTexture *tex)
{
	delete tex;
}

glCurvedAniSystem::glCurvedAniSystem()
{
	if (!QueryPerformanceFrequency(&m_PerformenceFreq))
	{
		assert(0);
	}

	attach(&this->renderer);
}
float glCurvedAniSystem::getTime()
{
	LARGE_INTEGER counter;
	if (QueryPerformanceCounter(&counter))
	{
		return float((double)counter.QuadPart / (double)m_PerformenceFreq.QuadPart);
	}
	return GetTickCount() * 0.001f;
}



template<template <typename> class T >
inline iCurveInterpolater *createInterpolator_all(const char *name)
{
	if (0 == strcmp("Float", name))
	{
		static T<float> s_i;
		return &s_i;
	}
	else if (0 == strcmp("Float2", name))
	{
		static T<Point2f> s_i;
		return &s_i;
	}
	else if (0 == strcmp("Float3", name))
	{
		static T<Point3f> s_i;
		return &s_i;
	}
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
		static StepInterpolater<Image> s_i;
		return &s_i;
	}
	return createInterpolator_all<StepInterpolater>(name);
}

iCurveInterpolater *glCurvedAniSystem::createCurveInterpolater(const char *name)
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
	assert(0 && "unsupported curve interpolater.");
	return 0;
}



template<template <typename> class T >
class MergerFactory
{
public:
	static iStateMergerIf *createMerger(const char *name)
	{
		if (0 == strcmp("Float", name))
		{
			static T<float> s_m;
			return &s_m;
		}
		else if (0 == strcmp("Float2", name))
		{
			static T<Point2f> s_m;
			return &s_m;
		}
		else if (0 == strcmp("Float3", name))
		{
			static T<Point3f> s_m;
			return &s_m;
		}
		else if (0 == strcmp("Matrix3", name))
		{
			static T<Wm4::Matrix3f> s_m;
			return &s_m;
		}
		/// and you could add any data types you want here
		//else if if (0 == strncmp("MyDataType", name, strlen("MyDataType")))
		//{
		//  return new T<MyDataType>;
		//}
		return 0;
	}
};

template<typename T>
class ParserNull
{
public:
	T parseToken(const char * /*str*/, const char * /*delim*/)
	{
		return T();
	}
	T parseNext()
	{
		return T();
	}
	const char *next;
};

static iStateMergerIf *createSaturateMerger(const char *name)
{
	if (0 == strcmp("Float", name))
	{
		static SaturateMerger<float, ParserNull> s_m;
		return &s_m;
	}
	/// and you could add any data types you want here
	//else if if (0 == strncmp("MyDataType", name, strlen("MyDataType")))
	//{
	//  stati T<MyDataType> s_m;
	//  return &s_m;
	//}
	return 0;
}

iStateMergerIf *glCurvedAniSystem::createStateMerger(const char *name)
{
	if (0 == strncmp("Keep", name, 4))
	{
		return MergerFactory<KeepMerger>::createMerger(name + 4);
	}
	else if (0 == strncmp("Gain", name, 4))
	{
		return MergerFactory<GainMerger>::createMerger(name + 4);
	}
	else if (0 == strncmp("Reduce", name, 6))
	{
		return MergerFactory<ReduceMerger>::createMerger(name + 6);
	}
	else if (0 == strncmp("Saturate", name, 8))
	{
		return createSaturateMerger(name + 8);
	}
    else if (0 == strcmp("MultFloat", name))
    {
        static MultMerger<float> s_mf;
        return &s_mf;
    }

	/// and you could add any interpolaters you want here
	//else if if (0 == strncmp("Modulate", name, 3))
	//{
	//  return createMerger<ModulateMerger>(name + 3);
	//}
	assert(0 && "unsupported state merger.");
	return 0;
}
void glCurvedAniSystem::release(iByteStream *bs)
{
	delete bs;
}
void glCurvedAniSystem::release(iCurveDataSet *cds)
{
	delete cds;
}
void glCurvedAniSystem::release(iCurveInterpolater *ci)
{
	// do nothing
}
void glCurvedAniSystem::release(iStateMergerIf *)
{
	// do nothing
}

iByteStream * glCurvedAniSystem::loadStream(const char *name)
{
	glByteStream *ByteStream = new glByteStream;
	ByteStream->loadByteStream(name);
	return ByteStream;
}