#include <string>
#include "glCurvedani.h"
#include "caInterpolaters.h"
#include "caFileStream.h"

bool isXmlFile( const char * s ) 
{
    bool bXml = false;
    if (char *s2 = _strdup(s))
    {
        std::vector<const char *> pathTokens;
        for (const char *pathToken = strtok(s2, "/\\"); pathToken; pathToken = strtok(0, "/\\"))
        {
            pathTokens.push_back(pathToken);
        }
        const char *cdot = strrchr(pathTokens.back(), '.');
        if (cdot && 0 == strcmp(cdot, ".xml"))
            bXml = true;

        free(s2);
    }
    return bXml;
}

int convert( cAni::iAnimResManager * manager, const char * filename, std::string &out ) 
{
    const cAni::AnimData *pAnimData = manager->getAnimData(filename);
    if (!pAnimData)
    {
        fprintf(stderr, "failed to load from <in>.\n");
        return -1;
    }

    if (isXmlFile(filename))
    {
        cAni::FileStream stream;
        if (!stream.openFile(out.c_str(), "wb"))
        {
            fprintf(stderr, "failed to open output file for writing.\n");
            return -1;
        }
        if (!manager->saveToByteStream(pAnimData, &stream))
        {
            fprintf(stderr, "failed to save to output file.\n");
            return -1;
        }
    }
    else
    {
        if (!manager->saveToXml(pAnimData, out.c_str()))
        {
            fprintf(stderr, "failed to save to output file.\n");
            return -1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{	
    if (argc == 1)
    {
        printf("Usage:\n");
        printf("converter <in> [out]\n");
        printf("if <in> is .xml then converter will try to parse it and convert it to binary animation file <out>,\n");
        printf("otherwise, the converter will treat it as a binary animation file and convert it to a .xml file.\n");
        return 0;
    }

    std::string out;
    if (argc == 2)
    {
        out = argv[1];
        out += ".xml";
    }
    else
    {
        out = argv[2];
    }

    glCurvedAniSystem system;

    if (cAni::iAnimResManager *manager = iSystem::GetInstance()->createAnimResManager())
    {
        if (convert(manager, argv[1], out) == 0)
        {

        }

        iSystem::GetInstance()->release(manager);
    }
    return 0;
}
