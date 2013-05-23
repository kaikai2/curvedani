#include <cassert>

#include "caAnimElement.h"
#include "caLocus.h"

namespace cAni
{
    /// sample has 2 functions
    bool AnimElement::sample(int frameTime, iClipState &rCS, iRenderer &render) const
    {
        if (locus.empty())
            return true; // no locus to do, also treat as finish

        for (size_t i = 0; i < locus.size(); i++)
        {
            const Locus &rL = locus[i];

            /// 1st is to set merge style of every locus
            rCS.setStateMergeStyle(rL.sid, rL.mergeStyle);
            if (rL.dataset)
            {
                if (iCurveInterpolater *pInterpolator = render.getInterpolator(rL.sid, rL.interpolateStyle))
                {
                    /// 2nd is to calculate the data value by frame time
                    if (!rCS.makeSample(rL.sid, *pInterpolator, *rL.dataset, frameTime))
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }
} // namespace cAni
