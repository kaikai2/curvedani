#include "caHgePort_Merger.h"
#include "caInterpolaters.h"
#include "caMergers.h"
#include "caHgePort_System.h"

using cAni::Point2s;
using cAni::Point2f;
using cAni::KeepMerger;
using cAni::GainMerger;
using cAni::ReduceMerger;
using cAni::SaturateMerger;
using cAni::MultMerger;

template<template <typename> class T >
class MergerFactory
{
public:
    static iStateMergerIf *createMerger(const char *name)
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
		else if (0 == strcmp("Matrix3", name))
		{
            static T<Wm4::Matrix3f> s_tm;
            return &s_tm;
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
    T parseToken(const char *str, const char *delim)
    {
		str, delim;
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
        static SaturateMerger<float, ParserNull> s_sm;
        return &s_sm;
    }
    /// and you could add any data types you want here
    //else if if (0 == strncmp("MyDataType", name, strlen("MyDataType")))
    //{
    //  return new T<MyDataType>;
    //}
    return 0;
}


iStateMergerIf *hgeCurvedAniSystem::createStateMerger(const char *name)
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
        MultMerger<float> s_mf;
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