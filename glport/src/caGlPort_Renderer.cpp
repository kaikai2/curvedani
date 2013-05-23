#include "caGlPort_Renderer.h"
#include "caGlPort_CurvedDataSet.h"

#include <sstream>
#include "font.h"
#include "caGlPort_Texture.h"

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
	{glSI_Image,           "Image" },    // size_t
	{glSI_Position,        "Position" }, // vector2
	{glSI_Scale,           "Scale" },    // vector2
	{glSI_Angle,           "Angle" },    // float
	{glSI_Alpha,           "Alpha" },    // float
	{glSI_Color,           "Color" },    // vector3
	{glSI_AnchorOffPos,    "AnchorOffPos" }, // vector2
	{glSI_Text,            "Text" },     // string
	{glSI_Clip,             "Clip"},     // rect
};
static BindMapState2Name g_state2Name(s_StateInfos, s_StateInfos + sizeof(s_StateInfos) / sizeof(s_StateInfos[0]), cAni::StateId(cAni::InvalidStateId), "");

cAni::StateId glRenderer::getStateId(const char *stateName) const
{
	return g_state2Name.find(stateName);
}

const char *glRenderer::getStateName(cAni::StateId stateId) const
{
	return g_state2Name.find(stateId).c_str();
}
const char *glRenderer::_getStateTypeName(StateId statusId) const
{
	switch(statusId)
	{
	case glSI_Image:    // size_t
		return "Image";
	case glSI_Position: // vector2
		return "Float2";
	case glSI_Scale:    // vector2
		return "Float2";
	case glSI_Angle:    // float
		return "Float";
	case glSI_Alpha:    // float
		return "Float";
	case glSI_Color:    // vector3
		return "Float3";
	case glSI_AnchorOffPos: // vector2
		return "Float2";
	case glSI_Text:     // string
		return "String";
	case glSI_Clip:     // rect
		return "Rect";
	}
	return "";
}
typedef BindMap<cAni::InterpolateStyleId, std::string> BindMapInterpolateStyle2Name;
static BindMapInterpolateStyle2Name::Info s_InterpolateStyleInfos[] = 
{
	{glII_Step,        "Step" },
	{glII_Linear,      "Linear" },
	{glII_Bspline,     "Bspline" },
};
static BindMapInterpolateStyle2Name g_InterpolateStyle2Name(s_InterpolateStyleInfos, s_InterpolateStyleInfos + sizeof(s_InterpolateStyleInfos) / sizeof(s_InterpolateStyleInfos[0]), cAni::InterpolateStyleId(cAni::InvalidInterpolateStyleId), "");

/// @fn getInterpolateStyleId
/// @brief return id from name
InterpolateStyleId glRenderer::getInterpolateStyleId(const char *isName) const
{
	return g_InterpolateStyle2Name.find(isName);
}

/// @fn getInterpolateStyleName
/// @brief return name from id
const char *glRenderer::getInterpolateStyleName(InterpolateStyleId isId) const
{
	return g_InterpolateStyle2Name.find(isId).c_str();
}

typedef BindMap<cAni::MergeStyleId, std::string> BindMapMergeStyle2Name;
static BindMapMergeStyle2Name::Info s_MergeStyleInfos[] = 
{
	{glMI_Keep,         "Keep" },
	{glMI_Gain,         "Gain" },
	{glMI_Reduce,       "Reduce" },
    {glMI_Mult,         "Mult"},
	{glMI_SaturateColor, "SaturateColor" },
};
static BindMapMergeStyle2Name g_MergeStyle2Name(s_MergeStyleInfos, s_MergeStyleInfos + sizeof(s_MergeStyleInfos) / sizeof(s_MergeStyleInfos[0]), glMI_Keep, "None");
/// @fn getMergeStyleId
/// @brief return id from name
cAni::MergeStyleId glRenderer::getMergeStyleId(const char *msName) const
{
	return g_MergeStyle2Name.find(msName);
}

/// @fn getMergeStyleName
/// @brief return name from id
const char *glRenderer::getMergeStyleName(MergeStyleId msId) const
{
	return g_MergeStyle2Name.find(msId).c_str();
}

/// @fn getInterpolator
/// @brief get a working instance
iCurveInterpolater *glRenderer::getInterpolator(StateId stateId, InterpolateStyleId isId) const
{
	if (mapInterpolaters[std::make_pair(stateId, isId)] == 0)
	{
		std::stringstream ss;
		ss << this->getInterpolateStyleName(isId) << this->_getStateTypeName(stateId);
		mapInterpolaters[std::make_pair(stateId, isId)] = iSystem::GetInstance()->createCurveInterpolater(ss.str().c_str());
	}
	return mapInterpolaters[std::make_pair(stateId, isId)];
}



glRenderer::glRenderer()
{
	this->fontmap = new glport::FontMap(wglGetCurrentDC());
	fontmap->GetFont("Tahoma.ttf");
}

glRenderer::~glRenderer()
{
	delete this->fontmap;
	this->fontmap = 0;
}
void glRenderer::flush()
{

}
void glRenderer::merge(cAni::iClipState &rClipState,const cAni::iClipState &rParentClipState)
{
	//glClipState result;
	//result.reset(&rClipState);
	//for (int i = 0; i < NumOfGLSI; i++)
	//{
	//	result.merge((cAni::StateId)i, hsc, (const glClipState &)rClipState, (const glClipState &)rParentClipState);
	//}

	glStateMergerComposition merger(*this);
	glClipState result((glClipState &)rClipState);
	glClipState &child = (glClipState &)rClipState;
	const glClipState &parent = (const glClipState &)rParentClipState;

	if (child.mergeStyle[glSI_Angle] != 0 ||
		child.mergeStyle[glSI_Position] != 0 ||
		child.mergeStyle[glSI_Scale] != 0 ||
		child.mergeStyle[glSI_AnchorOffPos] != 0)
	{
		if (child.mergeStyle[glSI_Angle] != 0)
			child.angle += parent.angle;

		Wm4::Matrix3f m33;
		parent.getMatrix(m33,
			child.mergeStyle[glSI_AnchorOffPos],
			child.mergeStyle[glSI_Angle],
			child.mergeStyle[glSI_Scale],
			child.mergeStyle[glSI_Position]);
		m33 = m33.Transpose();
		Wm4::Vector3f wmPosition0 = m33 * Wm4::Vector3f(child.position.x + child.anchorOffPos.x, child.position.y + child.anchorOffPos.y, 1.f);
		child.position.x = wmPosition0.X();
		child.position.y = wmPosition0.Y();

		if (child.mergeStyle[glSI_Scale] != 0)
		{
			child.scale.x = child.scale.x * parent.scale.x;
			child.scale.y = child.scale.y * parent.scale.y;
		}
	}

	for (size_t i = 0; i < NumOfGLSI; i++)
	{
		child.merge(i, merger, result, parent);
	}
}
void glRenderer::render(const iClipState &rClipState)
{
	glClipState result;
	result.reset(&rClipState);

	float alpha = result.alpha;
	const Point3f &color = result.color;
	const Rect &clip = result.clip;

	if (clip.GetWidth() && clip.GetHeight())
	{
		glColorMask(0,0,0,0);								// Set Color Mask
		glEnable(GL_STENCIL_TEST);							// Enable Stencil Buffer For "marking" The Floor
		glStencilFunc(GL_ALWAYS, 1, 1);						// Always Passes, 1 Bit Plane, 1 As Mask
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);			// We Set The Stencil Buffer To 1 Where We Draw Any Polygon
		// Keep If Test Fails, Keep If Test Passes But Buffer Test Fails
		// Replace If Test Passes
		glDisable(GL_DEPTH_TEST);							// Disable Depth Testing
		//DrawFloor();										// Draw The Floor (Draws To The Stencil Buffer)
		glBegin(GL_QUADS);
		glVertex3f(result.position.x + clip.leftTop.x, result.position.y + clip.leftTop.y, 0);
		glVertex3f(result.position.x + clip.rightBottom.x, result.position.y + clip.leftTop.y, 0);
		glVertex3f(result.position.x + clip.rightBottom.x, result.position.y + clip.rightBottom.y, 0);
		glVertex3f(result.position.x + clip.leftTop.x, result.position.y + clip.rightBottom.y, 0);
		glEnd();
		// We Only Want To Mark It In The Stencil Buffer
		//glEnable(GL_DEPTH_TEST);							// Enable Depth Testing
		glColorMask(1,1,1,1);								// Set Color Mask to TRUE, TRUE, TRUE, TRUE
		glStencilFunc(GL_EQUAL, 1, 1);						// We Draw Only Where The Stencil Is 1
		// (I.E. Where The Floor Was Drawn)
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);				// Don't Change The Stencil Buffer
	}
	glColor4f(color.x, color.y, color.z, alpha);

	const Image &image = result.image;
	if (image.valid())
	{
		Wm4::Matrix3f m33;
		result.getMatrix(m33, 1, 1, 1, 1);
		//glClipState::updateMatrix(m33, (const glClipState&)rClipState, (const glClipState&)rParentClipState);
		m33 = m33.Transpose();
		glTexture &glTex = *(glTexture*)&image.getTexture();
		unsigned int tex = glTex.getGlTexture();
		glBindTexture(GL_TEXTURE_2D, tex);
		const Rect &texRect = image.getRect();
		const float w_2 = texRect.GetWidth() * 0.5f;
		const float h_2 = texRect.GetHeight() * 0.5f;
		Wm4::Vector3f wmPosition0 = m33 * Wm4::Vector3f(-w_2, -h_2, 1.f);
		Wm4::Vector3f wmPosition1 = m33 * Wm4::Vector3f(w_2, -h_2, 1.f);
		Wm4::Vector3f wmPosition2 = m33 * Wm4::Vector3f(w_2, h_2, 1.f);
		Wm4::Vector3f wmPosition3 = m33 * Wm4::Vector3f(-w_2, h_2, 1.f);

		float left = (float)texRect.leftTop.x / glTex.getWidth();
		float right = (float)texRect.rightBottom.x / glTex.getWidth();
		float top = (float)texRect.leftTop.y / glTex.getHeight();
		float bottom = (float)texRect.rightBottom.y / glTex.getHeight();

		glEnable(GL_TEXTURE_2D);    
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_QUADS);

		glTexCoord2f(left, top);
		glVertex3f(wmPosition0.X(), wmPosition0.Y(), 0.5f);// x-,y-
		glTexCoord2f(right, top);
		glVertex3f(wmPosition1.X(), wmPosition1.Y(), 0.5f);// x+,y-
		glTexCoord2f(right, bottom);
		glVertex3f(wmPosition2.X(), wmPosition2.Y(), 0.5f);// x+,y+
		glTexCoord2f(left, bottom);
		glVertex3f(wmPosition3.X(), wmPosition3.Y(), 0.5f);// x-,y+
		glEnd();
	}
	const std::string &text = result.text;
	if (text.length())
	{
		glport::Font *pFont = fontmap->GetFont("Impact");
		if (pFont)
		{
			glPushMatrix();
			glTranslatef(result.position.x, result.position.y, 0);
			glScalef(result.scale.x, -result.scale.y, 1); // reverse y direciton
			glTranslatef(-result.anchorOffPos.x, -result.anchorOffPos.y, 0);
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_LINE_SMOOTH);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
			glLineWidth(0.3f);
			pFont->textOut(text.c_str());
			glPopMatrix();
		}
	}

	glDisable(GL_STENCIL_TEST);
}

iCurveDataSet *glRenderer::createCurveDataSet(const char *name)
{
    StateId sid = getStateId(name);
    if (sid != cAni::InvalidStateId)
    {
        name = this->_getStateTypeName(sid);
    }

    if (0 == strcmp("Image", name))
    {
        return new glCurveDataSet<Image>;
    }
    else if (0 == strcmp("Float", name))
    {
        return new glCurveDataSet<float>;
    }
    else if (0 == strcmp("Float2", name))
    {
        return new glCurveDataSet<Point2f>;
    }
    else if (0 == strcmp("Float3", name))
    {
        return new glCurveDataSet<Point3f>;
    }
    else if (0 == strcmp("Rect", name))
    {
        return new glCurveDataSet<Rect>;
    }
    /*
    else if (0 == strcmp("CDS_gl_Int", name))
    {
    return new glCurveDataSet<int>;
    }
    */
    assert(0 && "unsupported curve data set.");
    return 0;
}

void glRenderer::releaseCurveDataSet(iCurveDataSet *cds)
{
	delete cds;
}

MergeStyleId glRenderer::getDefaultMergeStyle( StateId stateId ) const
{
    switch(stateId)
    {
    case glSI_Image:    // size_t
        return glMI_Keep;
    case glSI_Position: // vector2
        return glMI_Gain;
    //case glSI_Depth:	// float
    //    return glMI_None;
    case glSI_Scale:    // vector2
        return glMI_Mult;
    case glSI_Angle:    // float
        return glMI_Gain;
    case glSI_Alpha:    // float
        return glMI_Keep;

    case glSI_Color:    // vector3
        return glMI_Keep;

    //case glSI_ClipRect: // Rect(short4)
    //    return glMI_None;
    case glSI_AnchorOffPos: // vector2:
        return glMI_Keep;
    case glSI_Text:
        return glMI_Keep;
    case glSI_Clip:
        return glMI_Keep;

    default:
        assert(0);
        return glMI_Keep;
    }
}