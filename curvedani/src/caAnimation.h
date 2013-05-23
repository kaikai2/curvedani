#ifndef __ANIM_H_
#define __ANIM_H_

#include <cassert>
#include <vector>
using std::vector;

#include "curvedani.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4786)
#endif

/*
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
*/
namespace cAni
{
class AnimData;

/// Animation
/// @note playing instance of the AnimData
class Animation : public iAnimation
{
public:
    typedef size_t AnimId;
    enum
    {
        DefaultAnimId = ~0,
    };
    Animation(AnimId animIdCount = 1)
    {
        init(animIdCount);
    }

    virtual void setAnimData(const AnimData *pAnimData, AnimId aniId);
    virtual bool startAnim(int curtime, AnimId aniId = DefaultAnimId);
    virtual bool isPlaying() const
    {
        return m_play;
    }
    virtual bool isLoop() const
    {
        return m_loop;
    }
    virtual bool checkEnd(int curtime) const;
	virtual int getAnimLength() const;
    virtual void setAnimLoop(bool bLoop);
    virtual AnimId getCurAnim() const{return m_CurAnimId;}
	virtual int getAnimCount() const{return (int)m_AnimData.size();}
    // void setAnimTimer(int time);

    virtual void render(int frameTime, const iClipState *defaultClipState = 0) const;
protected:
    void init(AnimId animIdCount);
    const AnimData *getCurAnimData() const;

    AnimId m_CurAnimId;
    vector<const AnimData *> m_AnimData;

    int m_startTime;    // start time in game timing

    // int m_curTime;  // current time in animation timing
    bool m_play;
    bool m_loop;  // is loop ?
};

}; // namespace cAni


#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif __ANIM_H_