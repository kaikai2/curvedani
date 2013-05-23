#ifndef _caGlPort_ClipState_H_
#define _caGlPort_ClipState_H_

#include "caGlPort_define.h"
#include "caGlPort_Merger.h"

/// 参数
class glClipState : public iClipState
{
public:
	using iClipState::getState;
	glClipState() : image(), position(0, 0), scale(1, 1), angle(0), alpha(1), color(1, 1, 1), anchorOffPos(0, 0)
	{
		memset(mergeStyle, 0, sizeof(mergeStyle));
	}

	glClipState(const glClipState &rhs) : image(), position(0, 0), scale(1, 1), angle(0), alpha(1), color(1, 1, 1), anchorOffPos(0, 0)
	{
		resetBy(&rhs);
	}
	virtual void reset(const iClipState *defaultClipState = 0)
	{
		resetBy((const glClipState *)defaultClipState);
	}
    virtual void resetToDefault(const iRenderer *renderer)
    {
        resetBy(0);
        for (size_t j = 0; j < NumOfGLSI; j++)
        {
            mergeStyle[j] = renderer->getDefaultMergeStyle(j);
        }
    }
	void resetBy(const glClipState *defaultClipState = 0)
	{
		if (defaultClipState)
		{
			const glClipState &rOther = *defaultClipState;
			rOther.getState(glSI_Image,    image);
			rOther.getState(glSI_Position, position);
			rOther.getState(glSI_Scale,    scale);
			rOther.getState(glSI_Angle,    angle);
			rOther.getState(glSI_Alpha,    alpha);
			rOther.getState(glSI_Color,    color);
			rOther.getState(glSI_AnchorOffPos, anchorOffPos);
			rOther.getState(glSI_Text,     text);
			rOther.getState(glSI_Clip,     clip);
			for (int i = 0; i < NumOfGLSI; i++)
			{
				setStateMergeStyle((StateId)i, rOther._getStateMergeStyle((StateId)i));
			}
		}
		else
		{
			image = Image();
			position = Point2f(0, 0);
			scale = Point2f(1, 1);
			angle = 0;
			alpha = 1;
			color = Point3f(1, 1, 1);
			anchorOffPos = Point2f(0, 0);
			text = "";
			clip = Rect();

			memset(mergeStyle, 0, sizeof(mergeStyle));
		}
	}

	virtual const void *getState(StateId sid) const
	{
		switch(sid)
		{
		case glSI_Image:    // size_t
			return &image;
		case glSI_Position: // vector2
			return &position;
		case glSI_Scale:    // vector2
			return &scale;
		case glSI_Angle:    // float
			return &angle;
		case glSI_Alpha:    // float
			return &alpha;
		case glSI_Color:    // vector3
			return &color;
		case glSI_AnchorOffPos: // vector2
			return &anchorOffPos;
		case glSI_Text: // string
			return &text;
		case glSI_Clip:// rect
			return &clip;
		default:
			assert(0);
			break;
		}
		return 0;
	}
	// merge style: 父子结点融合方式.
private:
	MergeStyleId _getStateMergeStyle(StateId sid) const
	{
		if (sid < NumOfGLSI)
		{
			return mergeStyle[sid];
		}
		return 0;
	}
public:
	virtual void setStateMergeStyle(StateId sid, MergeStyleId mergeStyle)
	{
		if (sid < NumOfGLSI)
		{
			this->mergeStyle[sid] = mergeStyle;
		}
	}

	virtual iClipState *clone() const
	{
		return new glClipState(*this);
	}
	virtual void release()
	{
		delete this;
	}

	virtual bool makeSample(StateId sid, iCurveInterpolater &rInterpolater, iCurveDataSet &rDataSet, int time)
	{
		void *pData = _getState(sid);
		return pData != 0 && rInterpolater.getSample(sid, rDataSet, time, pData);
	}

	void merge(StateId sid, glStateMergerComposition &rStateMergerComposition, const glClipState &rChild, const glClipState &rParent)
	{
		static bool bNeedMerge[] = 
		{
			false, //glSI_Image,    // size_t
			false, //glSI_Position, // vector2
			false, //glSI_Scale,    // vector2
			false, //glSI_Angle,    // float
			true, //glSI_Alpha,    // float
			true, //glSI_Color,    // vector3
			false, //glSI_AnchorOffPos, // vector2
			false, //glSI_Text,     // string
			false, //glSI_Clip,     // rect
			//NumOfGLSI,
		};
		if (sid < NumOfGLSI && bNeedMerge[sid])
		{
			rStateMergerComposition.setMergerStyle(sid, rChild._getStateMergeStyle(sid));
			rStateMergerComposition.merge(sid, rChild.getState(sid), rParent.getState(sid), this->_getState(sid));
		}
	}

	void getMatrix(Wm4::Matrix3f &mResult, MergeStyleId msAnchorOffPos, MergeStyleId msAngle, MergeStyleId msScale, MergeStyleId msPosition) const
	{
		mResult.MakeIdentity();

		if (msAnchorOffPos != 0)
		{
			Wm4::Matrix3f m33(
				1, 0, 0,
				0, 1, 0,
				-anchorOffPos.x, -anchorOffPos.y, 1);
			mResult = mResult * m33;
		}
		if (msAngle != 0)
		{
			Wm4::Matrix3f m33_rot;
			m33_rot.FromAxisAngle(Wm4::Vector3f(0, 0, 1), angle);
			mResult = mResult * m33_rot;
		}
		if (msScale != 0)
		{
			Wm4::Matrix3f mScale;
			mScale.MakeDiagonal(scale.x, scale.y, 1);
			mResult = mResult * mScale;
		}
		if (msPosition != 0)
		{
			//if (msAnchorOffPos != 0)
			//{
			//    Wm4::Matrix3f mPos(
			//        1, 0, 0,
			//        0, 1, 0,
			//        position.x + anchorOffPos.x, position.y + anchorOffPos.y, 1);
			//    mResult = mResult * mPos;
			//}
			//else
			{
				Wm4::Matrix3f mPos(
					1, 0, 0,
					0, 1, 0,
					position.x, position.y, 1);
				mResult = mResult * mPos;
			}
		}
		else
		{
			//if (msAnchorOffPos != 0)
			//{
			//    Wm4::Matrix3f m33(
			//        1, 0, 0,
			//        0, 1, 0,
			//        anchorOffPos.x, anchorOffPos.y, 1);
			//    mResult = mResult * m33;
			//}
		}
	}
	static void updateMatrix(Wm4::Matrix3f &mResult, const glClipState &rChild, const glClipState &rParent)
	{
		Wm4::Matrix3f mChild;
		rChild.getMatrix(mChild, 1, 1, 1, 1);

		Wm4::Matrix3f mParent;
		rParent.getMatrix(mParent,
			rChild._getStateMergeStyle(glSI_AnchorOffPos),
			rChild._getStateMergeStyle(glSI_Angle),
			rChild._getStateMergeStyle(glSI_Scale),
			rChild._getStateMergeStyle(glSI_Position));

		mResult = mChild * mParent;
	}
	Image image;
	Point2f position;
	Point2f scale;
	float angle;
	float alpha;
	Point3f color;
	Point2f anchorOffPos;
	std::string text;
	Rect clip;
	MergeStyleId mergeStyle[NumOfGLSI];
};

#endif//_caGlPort_ClipState_H_
