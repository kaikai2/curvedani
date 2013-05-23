#ifndef _caHgePort_Renderer_H_
#define _caHgePort_Renderer_H_

#include "caHgePort_define.h"
#include "caHgePort_ClipState.h"
#include "caHgePort_Merger.h"

class hgeRenderer : public iRenderer
{
public:
    hgeRenderer();
    virtual ~hgeRenderer();

    /// @fn getName
    /// @brief get renderer's unique name
    const char *getName() const
    {
        return "HGE";
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
	virtual iCurveDataSet *createCurveDataSet(const char *name);

	/// @fn releaseCurveDataSet
	/// @brief release a curved data set
	virtual void releaseCurveDataSet(iCurveDataSet *cds);

    virtual void merge(iClipState &rClipState, const iClipState &rParentClipState);
protected:
	void _render(const hgeClipState &rClipState);

    hgeClipState cs;
    static HGE *hge;
    mutable iCurveInterpolater *mapInterpolaters[NumOfHgeSI][NumOfHgeII];
private:
	std::vector<hgeClipState> m_vRenderList;
	//std::map<MergeStyleId, iStateMergerIf*> mapMergers;
};
#endif//_caHgePort_Renderer_H_