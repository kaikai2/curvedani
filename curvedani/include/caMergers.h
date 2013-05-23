/*
�����ں���
Ҳ�����Լ������µ��ں���

*/

#ifndef CANI_MERGERS_H
#define CANI_MERGERS_H

#include "curvedani.h"
#include "caImage.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4554)
#endif

namespace cAni
{
    // �ں���
	/// �����ں���
	template<class T>
	class KeepMerger : public iStateMerger
	{
	public:
		virtual const char *getMergerName() const
		{
			return "Keep";
		}
		virtual void merge(StateId sid, const void *child, const void *parent, void *result) const
		{
			sid, parent;

			*(T*)result = *(const T*)child;
		}
	};

    /// �ӷ��ں���
    template<class T>
    class GainMerger : public iStateMerger
    {
    public:
        virtual const char *getMergerName() const
        {
            return "Gain";
        }
        virtual void merge(StateId sid, const void *child, const void *parent, void *result) const
        {
            sid;

            *(T*)result = *(const T*)child + *(const T*)parent;
        }
    };
    /// �����ں���
    template<class T>
    class ReduceMerger : public iStateMerger
    {
    public:
        virtual const char *getMergerName() const
        {
            return "Reduce";
        }
        virtual void merge(StateId sid, const void *child, const void *parent, void *result) const
        {
            sid;

            *(T*)result = *(const T*)child - *(const T*)parent;
        }
    };

    /// �˷��ں���
    template<class T>
    class MultMerger : public iStateMerger
    {
    public:
        virtual const char *getMergerName() const
        {
            return "Mult";
        }
        virtual void merge(StateId sid, const void *child, const void *parent, void *result) const
        {
            sid;

            *(T*)result = *(const T*)child * *(const T*)parent;
        }
    };
    /// �����ں���
    template<class T, template<typename>class Parser>
    class SaturateMerger : public iStateMerger
    {
    public:
        virtual const char *getMergerName() const
        {
            return "Saturate";
        }
        /// @fn parseParam
        /// @brief ��ýű��ڲ���
        virtual void parseParam(const char *t)
        {
            Parser<T> parser;
            minValue = parser.parseToken(t, ",");
            maxValue = parser.parseNext();
        }
        virtual void merge(StateId sid, const void *child, const void *parent, void *result) const
        {
            sid;

            T &rRes = *(T*)result;
            rRes = *(const T*)child + *(const T*)parent;
            if (rRes > maxValue)
            {
                rRes = maxValue;
            }
            else if (rRes < minValue)
            {
                rRes = minValue;
            }
        }
    private:
        T minValue, maxValue;
    };
    /// ���ģʽ�������ṩ��Ľӿڣ���Ҫ���������ṩ
    template<int nMaxStateId, int nMaxMergerId>
    class cStateMergerComposition : public iStateMergerIf
    {
    public:
        cStateMergerComposition(iRenderer &_renderer) : renderer(_renderer)
        {
            memset(mergers, 0, sizeof(mergers));
            memset(curMergers, 0, sizeof(curMergers));
        }
        virtual ~cStateMergerComposition()
        {
            for (size_t i = 0; i < nMaxStateId; i++)
            {
                for (size_t j = 0; j < nMaxMergerId; j++)
                {
                    iSystem::GetInstance()->release(mergers[i][j]);
                }
            }
            memset(mergers, 0, sizeof(mergers));
            memset(curMergers, 0, sizeof(curMergers));
        }
        virtual void merge(StateId sid, const void *child, const void *parent, void *result) const
        {
            if (iStateMergerIf *sm = getMerger(sid))
            {
                sm->merge(sid, child, parent, result);
            }
        }
        // �����������ںϷ�ʽ
        void setMergerStyle(StateId sid, MergeStyleId mid)
        {
            iStateMergerIf *const cInvalidPointer = (iStateMergerIf *)1;
            if (sid < 0 || sid >= nMaxStateId ||
                mid < 0 || mid >= nMaxMergerId)
            {
                return;
            }
            if (mergers[sid][mid])
            {
                curMergers[sid] = (mergers[sid][mid] == cInvalidPointer) ? 0 : mergers[sid][mid];
                return;
            }

            std::string typeName = renderer.getStateName(sid);
            std::string mergeStyleName = renderer.getMergeStyleName(mid);
            if (mergeStyleName.length() && typeName.length())
            {
                std::string stateMergerName = mergeStyleName + typeName;
                if (iStateMergerIf *sm = iSystem::GetInstance()->createStateMerger(stateMergerName.c_str()))
                {
                    curMergers[sid] = mergers[sid][mid] = sm;
                }
                else
                {
                    mergers[sid][mid] = cInvalidPointer;
                    curMergers[sid] = 0;
                }
            }
        }
    protected:
        // ������Ԫ�ط��ʽӿ�
        iStateMergerIf *getMerger(StateId sid) const
        {
            return curMergers[sid];
        }
        cAni::iRenderer &renderer;
        iStateMergerIf *mergers[nMaxStateId][nMaxMergerId];
        iStateMergerIf *curMergers[nMaxStateId];
    };
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif//CANI_MERGERS_H