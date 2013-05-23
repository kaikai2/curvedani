#include "caAnimResManager.h"
#include "caAnimData.h"

#include <algorithm>
using std::find;

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

namespace cAni
{

    AnimResManager::~AnimResManager()
    {
        vector<Pair>::iterator animdata = m_AnimData.begin();
        while(animdata != m_AnimData.end())
        {
            delete animdata->pAnimData;
            ++animdata;
        }
        m_AnimData.clear();
    }
    bool AnimResManager::saveToByteStream(const AnimData *pAnimData, iByteStream *bs) const
    {
        return pAnimData->saveToByteStream(bs);
    }
    bool AnimResManager::saveToXml(const AnimData *pAnimData, const char *filename) const
    {
        return pAnimData->saveToXml(filename);
    }
    bool AnimResManager::release(const char *aniFileName)
    {
        const AnimData* pAnimData = findAnimData(aniFileName);
        if (pAnimData == 0)
        {
            return false;
        }

        if (pAnimData->nRefCount > 0)
        {
            return false;
        }

        vector< Pair >::iterator node = find(m_AnimData.begin(), m_AnimData.end(), aniFileName);
        if (node != m_AnimData.end())
        {
            m_AnimData.erase(node);
            delete pAnimData;
        }
        else
        {
            assert(0);
            return false;
        }
        return true;
    }

    AnimData* AnimResManager::findAnimData(const char *aniFileName) const
    {
        assert(aniFileName != NULL && *aniFileName != 0);
        vector< Pair >::const_iterator node = find(m_AnimData.begin(), m_AnimData.end(), aniFileName);
        if (node != m_AnimData.end())
        {
            return node->pAnimData;
        }
        return 0;
    }

    AnimData* AnimResManager::loadAnimData(const char *aniFileName)
    {
        bool bXmlFile = false;
        if (char *s2 = strdup(aniFileName))
        {
            vector<const char *> pathTokens;
            for (const char *pathToken = strtok(s2, "/\\"); pathToken; pathToken = strtok(0, "/\\"))
            {
                pathTokens.push_back(pathToken);
            }
            const char *cdot = strrchr(pathTokens.back(), '.');
            if (cdot && 0 == strcmp(cdot, ".xml"))
                bXmlFile = true;
            free(s2);
        }
        else
        {
            return 0;
        }
        if (bXmlFile)
        {
            return AnimData::loadFromXml(aniFileName);
        }
        else
        {
            if (iByteStream *byteStream = iSystem::GetInstance()->loadStream(aniFileName))
            {
                AnimData *pAnimData = AnimData::loadFromByteStream(byteStream);
                iSystem::GetInstance()->release(byteStream);
                return pAnimData;
            }
        }
        return 0;
    }

    const AnimData* AnimResManager::getAnimData(const char *aniFileName)
    {
        if (AnimData *pAnimData = findAnimData(aniFileName))
        {
            return pAnimData;
        }
        if (AnimData *pAnimData = loadAnimData(aniFileName))
        {
            m_AnimData.push_back(Pair(pAnimData, aniFileName));
            return pAnimData;
        }
        return 0;
    }

    void AnimResManager::reload()
    {
        for (size_t i = 0; i < m_AnimData.size(); ++i)
        {
            Pair &rPair = m_AnimData[i];
            rPair.pAnimData->release();
            delete rPair.pAnimData;
            rPair.pAnimData = AnimData::loadFromXml(rPair.anifilename.c_str());
        }
    }
} // namespace cAni
