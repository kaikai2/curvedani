#ifndef _CaGlPort_Define_H_
#define _CaGlPort_Define_H_

#include <windows.h>
#include <gl/gl.h>
#include "curvedani.h"
#include "caImage.h"
#include "caPoint2d.h"
#include "caPoint3d.h"
#include "caRect.h"
#include "Wm4Matrix3.h"

/// gl版本且用一下stl,但在curvedani内当不该出现stl，以免非c++用户怪罪
#include <vector>
#include <map>
#include <string>

using cAni::StateId;
using cAni::MergeStyleId;
using cAni::InterpolateStyleId;
using cAni::iTexture;
using cAni::iCurveDataSet;
using cAni::iCurveInterpolater;
using cAni::iStateMergerIf;
using cAni::iClipState;
using cAni::iRenderer;
using cAni::iByteStream;
using cAni::iSystem;
using cAni::Image;
using cAni::Point2f;
using cAni::Point3f;
using cAni::Rect;
typedef unsigned long DWORD;

enum glCurvedAniStateId
{
	glSI_Image,    // size_t
	glSI_Position, // vector2
	glSI_Scale,    // vector2
	glSI_Angle,    // float
	glSI_Alpha,    // float
	glSI_Color,    // vector3
	glSI_AnchorOffPos, // vector2
	glSI_Text,     // string
	glSI_Clip,     // rect

	NumOfGLSI,
};

enum glCurvedAniMergeStyleId
{
	glMI_Keep,
	glMI_Gain,
	glMI_Reduce,
    glMI_Mult,
	glMI_SaturateColor, // saturate in [0,1]

	NumOfHGLMI,
};

enum glCurvedAniInterpolateStyleId
{
	glII_Step,
	glII_Linear,
	glII_Bspline,

	NumOfGlII,
};

#endif//_CaGlPort_Define_H_
