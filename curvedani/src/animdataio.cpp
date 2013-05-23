#ifdef _MSC_VER
#pragma warning(disable: 4996)	// "name" 被声明为否决的
#pragma warning(disable:4786)
#endif
#include <cstdio>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "caAnimation.h"
#include "caAnimData.h"

#include "tinyxml/tinyxml.h"

using namespace std;

#define LOCUSANIM_ACCEPTED_VERSION 0x0101
typedef unsigned short WORD;
typedef unsigned long DWORD;
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

namespace cAni
{

struct LocusAni_Head
{
    WORD sig;  // always be 'LA'
    WORD version;
    DWORD anilength;
    char rendererName[64];
    char des[256];
    WORD numDataSets;
    WORD numAnimElements;
};


bool AnimData::saveToXml(const char *filename) const
{
    if (!this->renderer)
        return false;

    TiXmlDocument doc(filename);

    TiXmlElement baseElem("CurvedAni");
    int length = this->length;

    baseElem.SetAttribute("renderer", this->renderer->getName());
    baseElem.SetAttribute("length", length);
    size_t numDataSets = datasets.size();
    size_t numAnimElements = animElements.size();

    // save data sets
    for (size_t i = 0; i < numDataSets; i++)
    {
        TiXmlElement elem("DataSet");
        iCurveDataSet *cds = datasets[i];
        elem.SetAttribute("name", cds->getTypeName());
        if (!cds->saveToXml(&elem))
            return false;
        baseElem.InsertEndChild(elem);
    }

    // save animation elements
    for (size_t i = 0; i < numAnimElements; i++)
    {
        TiXmlElement elem("AnimElement");
        const AnimElement &animElement = animElements[i];

        unsigned char numLocus = (unsigned char)animElement.locus.size();
        for (size_t j = 0; j < numLocus; j++)
        {
            TiXmlElement locusElem("Locus");
            const Locus &lo = animElement.locus[j];
            const char *stateName = this->renderer->getStateName(lo.sid);
            if (!stateName)
                return false;
            locusElem.SetAttribute("StateId", stateName);
            const char *dsn = lo.dataset->getDataSetName();
            if (!dsn)
                return false;
            locusElem.SetAttribute("DataSet", dsn);
            locusElem.SetAttribute("InterpolaterId", renderer->getInterpolateStyleName(lo.interpolateStyle));
            locusElem.SetAttribute("MergeStyle", renderer->getMergeStyleName(lo.mergeStyle));
            elem.InsertEndChild(locusElem);
        }
        baseElem.InsertEndChild(elem);
    } 
    doc.InsertEndChild(baseElem);

    return doc.SaveFile();
}
class XmlAnimDataBuilder
{
public:
    XmlAnimDataBuilder(AnimData &rData) : m_rData(rData)
    {

    }

    bool ReadBaseInfo(TiXmlElement &elem)
    {
        int tmp;
        const char *rendererName;

        if ((rendererName = elem.Attribute("renderer")) == 0)
            return false;

        m_rData.renderer = iSystem::GetInstance()->getRenderer(rendererName);
        if (!m_rData.renderer)
            return false;

        if (!elem.Attribute("length", &tmp) || tmp < 0)
            return false;

        m_rData.length = tmp;
        return true;
    }
    bool ReadInlineDataSet(TiXmlElement &elem, Locus &lo)
    {
        const char *state = elem.Attribute("StateId");
        if (!state)
            return false;

        iCurveDataSet *cds = m_rData.renderer->createCurveDataSet(state);
        if (!cds || !cds->loadFromXml(&elem))
            return false;

        m_rData.datasets.push_back(cds);
        lo.dataset = cds;
        return true;
    }
    bool ReadDataSet(TiXmlElement &elem)
    {
        const char *name = elem.Attribute("name");
        if (!name)
            return false;
        const char *type = elem.Attribute("type");
        if (!type)
            return false;

        string cdsName = type;
		
        iCurveDataSet *cds = m_rData.renderer->createCurveDataSet(cdsName.c_str());
        if (!cds || !cds->loadFromXml(&elem))
            return false;

        map<string, size_t>::iterator ic = cdsMap.find(name);
        if (ic != cdsMap.end())
            return false;

        cdsMap.insert(ic, pair<string, size_t>(name, m_rData.datasets.size()));
        m_rData.datasets.push_back(cds);
        return true;
    }
    bool ReadLocus(TiXmlElement &elem, const char * /*parentName*/, AnimElement &animElement)
    {
        Locus lo;
        const char *stateId = elem.Attribute("StateId");
        if (!stateId)
            return false;
        lo.sid = m_rData.renderer->getStateId(stateId);
        if (lo.sid == InvalidStateId)
            return false;

        if (const char *dataSetName = elem.Attribute("DataSet"))
        {
            map<string, size_t>::iterator ic = cdsMap.find(dataSetName);
            if (ic == cdsMap.end())
                return false;

            assert(ic->second < m_rData.datasets.size());
            lo.dataset = m_rData.datasets[ic->second];
        }
        else
        {
            lo.dataset = 0;
            if (this->ReadInlineDataSet(elem, lo))
            {
            }
        }

        const char *interpolator = elem.Attribute("InterpolaterId");
        if (interpolator == 0)
            return false;

        string interpolatorName = interpolator;
        lo.interpolateStyle = m_rData.renderer->getInterpolateStyleId(interpolator);

        if (const char *mergeStyle = elem.Attribute("MergeStyle"))
        {
            lo.mergeStyle = m_rData.renderer->getMergeStyleId(mergeStyle);
        }
        else
        {
            lo.mergeStyle = m_rData.renderer->getDefaultMergeStyle(lo.sid);
        }

        animElement.locus.push_back(lo);
        return true;
    }
    bool ReadAnimElement(TiXmlElement &elem)
    {
        AnimElement animElement;
        const char *name = elem.Attribute("name");
        if (name)
        {
            map<string, size_t>::iterator ic = elementMap.find(name);
            if (ic != elementMap.end())
                return false;

            elementMap.insert(ic, pair<string, size_t>(name, m_rData.animElements.size()));
        }
        const char *parentName = elem.Attribute("parent");
        animElement.parentIndex = -1;
        if (parentName)
        {
            map<string, size_t>::iterator ic = elementMap.find(parentName);
            if (ic != elementMap.end())
            {
                animElement.parentIndex = (int)ic->second;
            }
            else
            {
                return false;
            }
        }

        //vector<Locus> &locus = animElement.locus;
        for (TiXmlElement *locusElement = (TiXmlElement *)elem.FirstChild("Locus"); locusElement;
            locusElement = (TiXmlElement *)locusElement->NextSibling("Locus"))
        {
            this->ReadLocus(*locusElement, parentName, animElement);
        }
        m_rData.animElements.push_back(animElement);
        return true;
    }
private:
	XmlAnimDataBuilder &operator = (const XmlAnimDataBuilder &);
    AnimData &m_rData;
    std::map<string, size_t> cdsMap;
    std::map<string, size_t> elementMap;
};

AnimData* AnimData::loadFromXml(const char *filename)
{

	TiXmlDocument doc;

	/// instead of loadfile with tinyxml, we open the file by user defined bytestream
	if (cAni::iByteStream *bs = iSystem::GetInstance()->loadStream(filename))
	{
		size_t length = bs->length();
		if (length == 0)
		{
			return 0;
		}
		std::string buf;
		buf.resize(length);
		bs->read(&buf[0], buf.length());
		doc.Parse(buf.c_str());
		iSystem::GetInstance()->release(bs);
	}
	else
	{
		return 0;
	}
    //if (!doc.LoadFile())
    //    return false;
	if (doc.Error())
		return 0;

    TiXmlHandle hNode(doc.FirstChild("CurvedAni"));
    TiXmlElement *elem = hNode.ToElement();
    if (!elem)
        return 0;

    AnimData data;
    XmlAnimDataBuilder builder(data);
    if (!builder.ReadBaseInfo(*elem))
    {
        return 0;
    }

    // load data sets
    for (elem = hNode.FirstChild("DataSet").ToElement(); elem;
        elem = (TiXmlElement *)elem->NextSibling("DataSet"))
    {
        if (!builder.ReadDataSet(*elem))
        {
            return 0;
        }
    }

    // load animation elements
    for (elem = hNode.FirstChild("AnimElement").ToElement(); elem;
        elem = (TiXmlElement *)elem->NextSibling("AnimElement"))
    {
        if (!builder.ReadAnimElement(*elem))
        {
            return 0;
        }
    }

    AnimData *p = new AnimData;
    data.passon(p);
    return p;
}

bool AnimData::saveToByteStream(iByteStream *bs) const
{
    LocusAni_Head head;
    head.sig = 'LA';
    head.version = LOCUSANIM_ACCEPTED_VERSION;
    head.numDataSets = (WORD)datasets.size();
    head.numAnimElements = (WORD)animElements.size();
    head.des[0] = 0;
    head.anilength = length;

    if (!bs->write(head))
        return false;

    int len;
    const char *name;

    // load data sets
    for (size_t i = 0; i < head.numDataSets; i++)
    {
        iCurveDataSet *cds = datasets[i];
        name = cds->getTypeName();
        len = (int)strlen(name);
        assert(len < MAX_PATH);
        if (!bs->write(len) || !bs->write(name, len))
            return false;

        if (!cds->saveToByteStream(bs))
            return false;
    }

    // load animation elements
    for (size_t i = 0; i < head.numAnimElements; i++)
    {
        const AnimElement &animElement = animElements[i];

        unsigned char numLocus = (unsigned char) animElement.locus.size();
        if (!bs->write(numLocus))
            return false;

        for (size_t j = 0; j < numLocus; j++)
        {
            const Locus &lo = animElement.locus[j];
            Locus::StreamData sd;
            sd.sid = (unsigned short)lo.sid;
            sd.dsid = (unsigned short)(find(datasets.begin(), datasets.end(), lo.dataset) - datasets.begin());
            sd.ipid = (unsigned short)lo.interpolateStyle;//find(interpolaters.begin(), interpolaters.end(), lo.interpolator) - interpolaters.begin();
            sd.mergeStyle = (unsigned short)lo.mergeStyle;
            assert(sd.dsid < datasets.size());// && sd.ipid < interpolaters.size());
            if (!bs->write(sd))
                return false;
        }
    } 
    return true;
}

AnimData* AnimData::loadFromByteStream(iByteStream *bs)
{
    assert(0 && "out-dated routine, need to modify");

    LocusAni_Head head;
    if (!bs->read(head))
        return 0;

    int len;
    char name[MAX_PATH];

    AnimData data;
    data.renderer = iSystem::GetInstance()->getRenderer(head.rendererName);
    if (!data.renderer)
    {
        return 0;
    }
    
    // load data sets
    for (size_t i = 0; i < head.numDataSets; i++)
    {
        if (!bs->read(len) || len >= MAX_PATH || !bs->read(name, len))
            return 0;
        name[len] = 0;
        iCurveDataSet *cds = data.renderer->createCurveDataSet(name);
        if (!cds)
            return 0;
        if (!cds->loadFromByteStream(bs))
            return 0;
        
        data.datasets.push_back(cds);
    }
    // load animation elements
    for (size_t i = 0; i < head.numAnimElements; i++)
    {
        AnimElement animElement;

        unsigned char numLocus;
        if (!bs->read(numLocus))
            return 0;

        vector<Locus> &locus = animElement.locus;
        for (size_t j = 0; j < numLocus; j++)
        {
            Locus lo;
            Locus::StreamData sd;
            if (!bs->read(sd) || sd.dsid >= data.datasets.size())// || sd.ipid >= interpolaters.size())
                return 0;
            lo.sid = sd.sid;
            lo.dataset = data.datasets[sd.dsid];
            lo.interpolateStyle = sd.ipid;//interpolator = interpolaters[sd.ipid];
            lo.mergeStyle = sd.mergeStyle;
            locus.push_back(lo);
        }

        data.animElements.push_back(animElement);
    } 

    data.length = head.anilength;
    
    AnimData *p = new AnimData;
    data.passon(p);
    return p;
}

}; // namespace cAni
