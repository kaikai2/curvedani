#include "caHgePort_Renderer.h"
#include "caHgePort_Merger.h"
#include "caHgePort_Texture.h"
#include "caHgePort_CurvedDataSet.h"

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>

HGE *hgeRenderer::hge = 0;

template<typename T>
inline T clamp(const T &a, const T &lo, const T &hi)
{
    assert(lo <= hi);
    if (a >= hi)
        return hi;
    else if (a <= lo)
        return lo;
    return a;
}

template<typename key, typename value>
class BindMap
{
public:
    struct Info
    {
        key k;
        value v;
    };
    BindMap(Info *begin, Info *end, const key &ik, const value &iv)
        : invalidKey(ik), invalidValue(iv)
    {
        for (Info *i = begin; i != end; i++)
        {
            a2b[i->k] = i->v;
            b2a[i->v] = i->k;
        }
    }
    const value &find(const key &k) const
    {
        typename std::map<key, value>::const_iterator i = a2b.find(k);
        if (i == a2b.end())
        {
            return invalidValue;
        }
        return i->second;
    }
    const key &find(const value &v) const
    {
        typename std::map<value, key>::const_iterator i = b2a.find(v);
        if (i == b2a.end())
        {
            return invalidKey;
        }
        return i->second;
    }
private:
    BindMap &operator = (const BindMap &rhs)
    {
        rhs;
        return *this;
    }
    const key invalidKey;
    const value invalidValue;
    std::map<key, value> a2b;
    std::map<value, key> b2a;
};

// no way to use this
template<typename key>
class BindMap<key, key>
{
private:
    BindMap(){}
    ~BindMap(){}
};

typedef BindMap<cAni::StateId, std::string> BindMapState2Name;
static BindMapState2Name::Info s_StateInfos[] = 
{
    {hgeSI_Image,           "Image" },    // size_t
    {hgeSI_Position,        "Position" }, // vector2
	{hgeSI_Depth,			"Depth" },	  // float
    {hgeSI_Scale,           "Scale" },    // vector2
    {hgeSI_Angle,           "Angle" },    // float
    {hgeSI_Alpha,           "Alpha" },    // float
    {hgeSI_Color,           "Color" },    // vector3
    {hgeSI_ClipRect,        "ClipRect" }, // Rect(short4)
    {hgeSI_AnchorOffPos,    "AnchorOffPos" }, // vector2
};
static BindMapState2Name g_state2Name(s_StateInfos, s_StateInfos + sizeof(s_StateInfos) / sizeof(s_StateInfos[0]), cAni::StateId(cAni::InvalidStateId), "");

cAni::StateId hgeRenderer::getStateId(const char *stateName) const
{
    return g_state2Name.find(stateName);
}

const char *hgeRenderer::getStateName(cAni::StateId stateId) const
{
    return g_state2Name.find(stateId).c_str();
}
const char *_getStateTypeName(StateId statusId)
{
    switch(statusId)
    {
    case hgeSI_Image:
        return "Image";
    case hgeSI_Position:
        return "Float2";
	case hgeSI_Depth:
		return "Float";
    case hgeSI_Scale:
        return "Float2";
    case hgeSI_Angle:
        return "Float";
    case hgeSI_Alpha:
        return "Float";
    case hgeSI_Color:
        return "Float3";
    case hgeSI_ClipRect:
        return "Rect";
    case hgeSI_AnchorOffPos:
        return "Float2";
	}
	assert(0);
    return "";
}
typedef BindMap<cAni::InterpolateStyleId, std::string> BindMapInterpolateStyle2Name;
static BindMapInterpolateStyle2Name::Info s_InterpolateStyleInfos[] = 
{
    {hgeII_Step,        "Step" },
    {hgeII_Linear,      "Linear" },
    {hgeII_Bspline,     "Bspline" },
};
static BindMapInterpolateStyle2Name g_InterpolateStyle2Name(s_InterpolateStyleInfos, s_InterpolateStyleInfos + sizeof(s_InterpolateStyleInfos) / sizeof(s_InterpolateStyleInfos[0]), cAni::InterpolateStyleId(cAni::InvalidInterpolateStyleId), "");

/// @fn getInterpolateStyleId
/// @brief return id from name
InterpolateStyleId hgeRenderer::getInterpolateStyleId(const char *isName) const
{
    return g_InterpolateStyle2Name.find(isName);
}

/// @fn getInterpolateStyleName
/// @brief return name from id
const char *hgeRenderer::getInterpolateStyleName(InterpolateStyleId isId) const
{
    return g_InterpolateStyle2Name.find(isId).c_str();
}


typedef BindMap<cAni::MergeStyleId, std::string> BindMapMergeStyle2Name;
static BindMapMergeStyle2Name::Info s_MergeStyleInfos[] = 
{
    {hgeMI_Keep,         "Keep" },
    {hgeMI_Gain,         "Gain" },
    {hgeMI_Reduce,       "Reduce" },
    {hgeMI_Mult,         "Mult" },
    {hgeMI_SaturateColor, "SaturateColor" },
};
static BindMapMergeStyle2Name g_MergeStyle2Name(s_MergeStyleInfos, s_MergeStyleInfos + sizeof(s_MergeStyleInfos) / sizeof(s_MergeStyleInfos[0]), hgeMI_Keep, "None");
/// @fn getMergeStyleId
/// @brief return id from name
cAni::MergeStyleId hgeRenderer::getMergeStyleId(const char *msName) const
{
    return g_MergeStyle2Name.find(msName);
}

/// @fn getMergeStyleName
/// @brief return name from id
const char *hgeRenderer::getMergeStyleName(MergeStyleId msId) const
{
    return g_MergeStyle2Name.find(msId).c_str();
}
/// @fn getInterpolator
/// @brief get a working instance
iCurveInterpolater *hgeRenderer::getInterpolator(StateId stateId, InterpolateStyleId isId) const
{
	if (stateId < 0 || stateId >= NumOfHgeSI ||
		isId < 0 || isId >= NumOfHgeII)
	{
		return 0;
	}

    if (mapInterpolaters[stateId][isId] == 0)
    {
        std::stringstream ss;
        ss << this->getInterpolateStyleName(isId) << _getStateTypeName(stateId);
        mapInterpolaters[stateId][isId] = iSystem::GetInstance()->createCurveInterpolater(ss.str().c_str());
    }
    return mapInterpolaters[stateId][isId];
}

hgeRenderer::hgeRenderer()
{
	memset(mapInterpolaters, 0, sizeof(mapInterpolaters));
	assert(!hge);
	hge = hgeCreate(HGE_VERSION);
	//mapMergers[hgeMI_None] = 0;
	//mapMergers[hgeMI_Gain] = iSystem::GetInstance()->createStateMerger("GainFloat");
	//
	//,
 //   ,
 //   hgeMI_Reduce,
	//hgeMI_SaturateColor, // saturate in [0,1]
}

hgeRenderer::~hgeRenderer()
{
	for (int i = 0; i < sizeof(mapInterpolaters)/sizeof(mapInterpolaters[0][0]); i++)
    {
        iSystem::GetInstance()->release(mapInterpolaters[0][i]);
    }
	hge->Release();
}

void hgeRenderer::merge(iClipState &rClipState, const iClipState &rParentClipState)
{
    hgeStateMergerComposition merger(*this);
    hgeClipState result((hgeClipState &)rClipState);
    hgeClipState &child = (hgeClipState &)rClipState;
    const hgeClipState &parent = (const hgeClipState &)rParentClipState;

	if (child.mergeStyle[hgeSI_Angle] != 0 ||
		child.mergeStyle[hgeSI_Position] != 0 ||
		child.mergeStyle[hgeSI_Scale] != 0 ||
		child.mergeStyle[hgeSI_AnchorOffPos] != 0)
	{
		if (child.mergeStyle[hgeSI_Angle] == 1)
			child.angle += parent.angle;

		Wm4::Matrix3f m33;
		parent.getMatrix(m33,
			child.mergeStyle[hgeSI_AnchorOffPos],
			child.mergeStyle[hgeSI_Angle],
			child.mergeStyle[hgeSI_Scale],
			child.mergeStyle[hgeSI_Position]);
		m33 = m33.Transpose();
		Wm4::Vector3f wmPosition0 = m33 * Wm4::Vector3f(child.position.x + child.anchorOffPos.x, child.position.y + child.anchorOffPos.y, 1.f);
		child.position.x = wmPosition0.X();
		child.position.y = wmPosition0.Y();

		if (child.mergeStyle[hgeSI_Scale] != 0)
		{
			child.scale.x = child.scale.x * parent.scale.x;
			child.scale.y = child.scale.y * parent.scale.y;
		}
	}

    for (size_t i = 0; i < NumOfHgeSI; i++)
    {
        child.merge(i, merger, result, parent);
    }
}
void hgeRenderer::render(const iClipState &rClipState)
{
	const hgeClipState &result = (const hgeClipState &)rClipState;
	m_vRenderList.push_back(result);
}
void hgeRenderer::flush()
{
	struct OrderByDepth : public std::binary_function<hgeClipState, hgeClipState, bool>
	{
		bool operator ()(const hgeClipState &a, const hgeClipState &b)
		{
			return a.depth > b.depth;
		}
	};
	std::sort(m_vRenderList.begin(), m_vRenderList.end(), OrderByDepth());
	for (size_t i = 0; i < m_vRenderList.size(); i++)
	{
		_render(m_vRenderList[i]);
	}
	m_vRenderList.clear();
}
void hgeRenderer::_render(const hgeClipState &result)
{
	const Image &image = result.image;
//	float angle = result.angle;

    Wm4::Matrix3f m33;
    result.getMatrix(m33, 1,  1, 1, 1);
	m33 = m33.Transpose();
	float alpha = result.alpha;
	const Point3f &color = result.color;
	const Rect &clipRect = result.clipRect;
    hgeQuad q;
    hgeTexture &hgeTex = *(hgeTexture*)&image.getTexture();
    q.tex = hgeTex.getHgeTexture();
    {
        int a = clamp(int(alpha * 255), 0, 255);
        int r = clamp(int(color.x * 255), 0, 255);
        int g = clamp(int(color.y * 255), 0, 255);
        int b = clamp(int(color.z * 255), 0, 255);

        q.v[3].col = q.v[2].col = q.v[1].col = q.v[0].col = ARGB(a, r, g, b);
    }
    const Rect &texRect = image.getRect();
	{
		const float w_2 = texRect.GetWidth() * 0.5f;
		const float h_2 = texRect.GetHeight() * 0.5f;
		Wm4::Vector3f wmPosition0 = m33 * Wm4::Vector3f(-w_2, -h_2, 1.f);
		Wm4::Vector3f wmPosition1 = m33 * Wm4::Vector3f(w_2, -h_2, 1.f);
		Wm4::Vector3f wmPosition2 = m33 * Wm4::Vector3f(w_2, h_2, 1.f);
		Wm4::Vector3f wmPosition3 = m33 * Wm4::Vector3f(-w_2, h_2, 1.f);
		q.v[0].x = wmPosition0.X();
		q.v[0].y = wmPosition0.Y();
		q.v[1].x = wmPosition1.X();
		q.v[1].y = wmPosition1.Y();
		q.v[2].x = wmPosition2.X();
		q.v[2].y = wmPosition2.Y();
		q.v[3].x = wmPosition3.X();
		q.v[3].y = wmPosition3.Y();
	}

    q.v[3].z = q.v[2].z = q.v[1].z = q.v[0].z = result.depth;

	float texwidth = (float)hgeTex.getWidth();
	float texheight = (float)hgeTex.getHeight();
    q.v[3].tx = q.v[0].tx = (float)texRect.leftTop.x / texwidth;
    q.v[2].tx = q.v[1].tx = (float)texRect.rightBottom.x / texwidth;
    q.v[1].ty = q.v[0].ty = (float)texRect.leftTop.y / texheight;
    q.v[3].ty = q.v[2].ty = (float)texRect.rightBottom.y / texheight;

    q.blend = BLEND_DEFAULT;
    if (clipRect.Visible())
    {
        hge->Gfx_SetClipping(clipRect.leftTop.x, clipRect.leftTop.y, clipRect.rightBottom.x, clipRect.rightBottom.y);
        hge->Gfx_RenderQuad(&q);
        hge->Gfx_SetClipping();
    }
    else
    {
        hge->Gfx_RenderQuad(&q);
    }
}

/// @fn createCurveDataSet
/// @brief create a curved data set of this renderer
iCurveDataSet *hgeRenderer::createCurveDataSet(const char *name)
{
    cAni::StateId sid = getStateId(name);
    if (sid != cAni::InvalidStateId)
    {
        name = _getStateTypeName(sid);
    }

    if (0 == strcmp("Image", name))
    {
        return new hgeCurveDataSet<Image>;
    }
    else if (0 == strcmp("Float", name))
    {
        return new hgeCurveDataSet<float>;
    }
    else if (0 == strcmp("Float2", name))
    {
        return new hgeCurveDataSet<hgeVector>;
    }
    else if (0 == strcmp("Float3", name))
    {
        return new hgeCurveDataSet<Point3f>;
    }
    else if (0 == strcmp("Rect", name))
    {
        return new hgeCurveDataSet<Rect>;
    }
    /*
    else if (0 == strcmp("Int", name))
    {
    return new hgeCurveDataSet<int>;
    }
    */
    assert(0 && "unsupported curve data set.");
    return 0;
}

/// @fn releaseCurveDataSet
/// @brief release a curved data set
void hgeRenderer::releaseCurveDataSet(iCurveDataSet *cds)
{
	delete cds;
}

MergeStyleId hgeRenderer::getDefaultMergeStyle( StateId stateId ) const
{
    switch(stateId)
    {
    case hgeSI_Image:    // size_t
        return hgeMI_Keep;
    case hgeSI_Position: // vector2
        return hgeMI_Gain;
    case hgeSI_Depth:	// float
        return hgeMI_Keep;
    case hgeSI_Scale:    // vector2
        return hgeMI_Mult;
    case hgeSI_Angle:    // float
        return hgeMI_Gain;
    case hgeSI_Alpha:    // float
        return hgeMI_Keep;

    case hgeSI_Color:    // vector3
        return hgeMI_Keep;

    case hgeSI_ClipRect: // Rect(short4)
        return hgeMI_Keep;
    case hgeSI_AnchorOffPos: // vector2:
        return hgeMI_Keep;
    default:
        assert(0);
        return hgeMI_Keep;
    }
}