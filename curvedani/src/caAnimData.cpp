#include "caAnimData.h"
#include "caAnimElement.h"
#include "caLocus.h"

#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <cassert>

class CDepnedenceManager
{
    typedef std::multimap<size_t, size_t> mmn2n;
    typedef std::pair<mmn2n::iterator, mmn2n::iterator> mmn2npii;
public:
    void DefineNode(size_t nIndex)
    {
        m_setItem.insert(nIndex);
    }
    void Dependon(size_t nRelier, size_t nBeDepended)
    {
        DefineNode(nRelier);
        DefineNode(nBeDepended);
        m_mapDepend.insert(std::make_pair(nRelier, nBeDepended));
    }

    void VisitFromRelier(std::vector<size_t> &vecResult)
    {
        std::vector<size_t> vecAllItem(m_setItem.begin(), m_setItem.end());
        std::map<size_t, std::set<size_t> > mapReliers; // all in mapReliers[a] are depend on a
        for (mmn2n::iterator it = m_mapDepend.begin(); it != m_mapDepend.end(); ++it)
        {
            mapReliers[it->second].insert(it->first);
        }

        // O(n^2 * m), might be O(nlogn*m)
        while(!vecAllItem.empty())
        {
            for (size_t i = 0; i < vecAllItem.size();)
            {
                size_t id = vecAllItem[i];
                if (mapReliers[id].empty())
                {
                    std::swap(vecAllItem[i], vecAllItem.back());
                    vecAllItem.pop_back();

                    vecResult.push_back(id);
                    mmn2npii p = m_mapDepend.equal_range(id);
                    for (mmn2n::iterator it = p.first; it != p.second; ++it)
                    {
                        mapReliers[it->second].erase(id);
                    }
                }
                else
                {
                    ++i;
                }
            }
        }
    }
private:
    std::set<size_t> m_setItem;
    mmn2n m_mapDepend; // a depend on map[a]

};
namespace cAni
{
    void AnimData::release()
    {
        this->length = 0;

        animElements.clear();
        for (vector<iCurveDataSet*>::iterator ds = datasets.begin(); ds != datasets.end(); ++ds)
        {
            iSystem::GetInstance()->release(*ds);
        }
        datasets.clear();
        //     for (vector<iCurveInterpolater*>::iterator ip = interpolaters.begin(); ip != interpolaters.end(); ++ip)
        //     {
        //         iSystem::GetInstance()->release(*ip);
        //     }
        //    interpolaters.clear();
    }
    void AnimData::passon(AnimData *successor)
    {
        assert(nRefCount == 0);
        successor->nRefCount = nRefCount;
        successor->datasets.swap(datasets);
        successor->renderer = renderer;
        successor->length = length;
        CDepnedenceManager dm;
        for (size_t i = 0; i < animElements.size(); i++)
        {
            dm.DefineNode(i);
            if (animElements[i].parentIndex != -1)
            {
                dm.Dependon(i, animElements[i].parentIndex);
            } 
        }
        std::vector<size_t> vecOrder;
        dm.VisitFromRelier(vecOrder);
        assert(vecOrder.size() == animElements.size());
        std::vector<AnimElement> vecAnimElement(animElements.size());
        std::vector<int> vecNewPos(vecOrder.size(), -1);
        for (size_t i = 0; i < vecOrder.size(); i++)
        {
            size_t index = vecOrder.size() - i - 1;
            size_t oldPos = vecOrder[index];
            AnimElement &ae = animElements[oldPos];
            vecAnimElement[i].locus.swap(ae.locus);
            vecNewPos[oldPos] = i;
            if (ae.parentIndex != -1)
            {
                /*
                parent b-ebd
                index  abcde
                order  13420
                bufVAE bdeca    //vecAnimElement
                */
                vecAnimElement[i].parentIndex = vecNewPos[ae.parentIndex];
            }
            else
            {
                vecAnimElement[i].parentIndex = -1;
            }
        }
        successor->animElements.swap(vecAnimElement);
    }

    void AnimData::render(int frameTime, const iClipState *defaultClipState/* = 0*/) const
    {
        const iClipState *pClipState = defaultClipState;
        if (pClipState == 0)
        {
            iClipState *workingClipState = renderer->getClipState();
            workingClipState->reset(0);
            pClipState = workingClipState;
        }

        const size_t nSize = animElements.size();

        /// sample all elements' clipstates
        vector<iClipState *> allClipStates(nSize, 0); // need a object buffer
        vector<bool> vecBeReady(nSize, false);
        for(size_t i = 0; i < nSize; i++)
        {
            allClipStates[i] = pClipState->clone(); // need a object buffer
            allClipStates[i]->resetToDefault(renderer);
            animElements[i].sample(frameTime, *allClipStates[i], *this->renderer);
        }

		/// calculate the hierarchy relations
		for (size_t i = 0; i < nSize; i++)
		{
			if (animElements[i].parentIndex == -1)
            {
                this->renderer->merge(*allClipStates[i], *pClipState);
				continue;
			}
			size_t nParent = animElements[i].parentIndex;
			if (nParent < i)
			{
				this->renderer->merge(*allClipStates[i], *allClipStates[nParent]);
			}
			else
			{
				assert(0);
			}
		}

        /// render all by the original element sequence
        for (size_t i = 0; i < nSize; i++)
        {
            this->renderer->render(*allClipStates[i]);
        }

        for(size_t i = 0; i < nSize; i++)
        {
            allClipStates[i]->release();
            // allClipStates[i] = 0; // this is not needed.
        }
    }

} // namespace cAni
