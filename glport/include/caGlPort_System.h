#ifndef _caGlPort_System_H_
#define _caGlPort_System_H_

#include "caGlPort_define.h"
#include "caGlPort_Renderer.h"

class glCurvedAniSystem : public iSystem
{
public:
	glCurvedAniSystem();
	virtual ~glCurvedAniSystem()
	{
		dettach(&this->renderer);

		// MUST call this here
		iSystem::release();
		// gl->Release();
	}
	virtual iByteStream *loadStream(const char *name);
	virtual iCurveInterpolater *createCurveInterpolater(const char *name);
	virtual iStateMergerIf *createStateMerger(const char *name);
	virtual void release(iByteStream *bs);
	virtual void release(iCurveDataSet *cds);
	virtual void release(iCurveInterpolater *ci);
	virtual void release(iStateMergerIf *sm);
	virtual iRenderer *getRenderer()
	{
		return &renderer;
	}
	virtual float getTime();

	/// @fn getPortName
	/// @brief return the name of the port
	virtual const char *getPortName() const
	{
		return "GLport";
	}
	static iSystem* GetInstance()
	{
		return s_Instance;
	}
protected:
	virtual iTexture *allocTexture();
	virtual void release(cAni::iTexture *tex);
	glRenderer renderer;

	LARGE_INTEGER m_PerformenceFreq;
};

#endif//_caGlPort_System_H_
