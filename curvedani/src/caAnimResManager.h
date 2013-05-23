#ifndef __CurvedAni_AnimResManager_H_
#define __CurvedAni_AnimResManager_H_

#include "curvedani.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

namespace cAni
{
//#pragma warning(push)
//#pragma warning(disable:4786)


    /// AnimResManager
    /// @note   manage AnimData as resources
    class AnimResManager : public iAnimResManager
    {
    public:
        AnimResManager()
        {
        }
        virtual ~AnimResManager();

        /// getAnimData
        /// @param aniFileName  [in] main file name of ani file, no suffix needed
        virtual const AnimData* getAnimData(const char *aniFileName);
        virtual bool release(const char *aniFileName);
        virtual bool saveToByteStream(const AnimData *pAnimData, iByteStream *bs) const;
        virtual bool saveToXml(const AnimData *pAnimData, const char *filename) const;
        virtual void reload();
    private:
        AnimResManager(const AnimResManager &)
        {
        }
        AnimResManager &operator = (const AnimResManager &)
        {
            return *this;
        }
        AnimData* findAnimData(const char *aniFileName) const;
        AnimData* loadAnimData(const char *aniFileName);

        struct Pair
        {
            Pair(AnimData *_pAnimData, const char *_anifilename)
                :pAnimData(_pAnimData),anifilename(_anifilename)
            {
            }
            AnimData* pAnimData;
            string anifilename;
            bool operator == (const char*name) const
            {
                return anifilename == name;
            }
        };
        vector< Pair > m_AnimData;
    };

} // namespace cAni

#endif __CurvedAni_AnimResManager_H_