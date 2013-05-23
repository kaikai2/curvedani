#ifndef _caGlPort_Renderer_H_
#define _caGlPort_Renderer_H_

#include "caGlPort_define.h"
#include "caGlPort_ClipState.h"
#include "caGlPort_Merger.h"

namespace glport
{
	class FontMap;
}

class glRenderer : public iRenderer
{
public:
	glRenderer();
	virtual ~glRenderer();

	/// @fn getName
	/// @brief get renderer's unique name
	const char *getName() const
	{
		return "gl";
	}

	/// @fn getStateId
	/// @brief return id from name
	StateId getStateId(const char *stateName) const;
	/// @fn getStateName
	/// @brief return name from id
	const char *getStateName(StateId statusId) const;

	/// @fn getInterpolateStyleId
	/// @brief return id from name
	virtual InterpolateStyleId getInterpolateStyleId(const char *isName) const;

	/// @fn getInterpolateStyleName
	/// @brief return name from id
	virtual const char *getInterpolateStyleName(InterpolateStyleId isId) const;

	/// @fn getMergeStyleId
	/// @brief return id from name
	virtual MergeStyleId getMergeStyleId(const char *isName) const;

    /// @fn getDefaultMergeStyle
    /// @brief return id from stateid
    virtual MergeStyleId getDefaultMergeStyle(StateId stateId) const;

	/// @fn getMergeStyleName
	/// @brief return name from id
	virtual const char *getMergeStyleName(MergeStyleId msId) const;

	/// @fn getClipState
	/// @brief get a working instance
	virtual iClipState *getClipState()
	{
		return &cs;
	}
	/// @fn getInterpolator
	/// @brief get a working instance
	virtual iCurveInterpolater *getInterpolator(StateId stateId, InterpolateStyleId isId) const;

	virtual void render(const iClipState &rClipState);
	virtual void flush();

	/// @fn createCurveDataSet
	/// @brief create a curved data set of this renderer
    /// @param name could be type of dataset, or state name
	virtual iCurveDataSet *createCurveDataSet(const char *name);
	/// @fn releaseCurveDataSet
    /// @brief release a curved data set
	virtual void releaseCurveDataSet(iCurveDataSet *cds);
	virtual void merge(cAni::iClipState &rClipState,const cAni::iClipState &rParentClipState);
protected:
	const char *_getStateTypeName(StateId statusId) const;
	glClipState cs;
	glport::FontMap *fontmap;

	mutable std::map<std::pair<StateId, InterpolateStyleId>, iCurveInterpolater *> mapInterpolaters;
};

#endif//_caGlPort_Renderer_H_
