#include "hge.h"
#include "hgefont.h"
#include "hgecurvedani.h"
#include "caInterpolaters.h"

HGE *hge = 0;

cAni::iAnimation *anim = 0;
cAni::iAnimResManager *armanager = 0;
hgeFont *font = 0;
int time;
const char *aniFile = "arm.xml";
void reload()
{
    if (anim)
    {
        anim->setAnimData(0, 0);
        if (armanager)
        {
            armanager->release(aniFile);
            anim->setAnimData(armanager->getAnimData(aniFile), 0);
            anim->startAnim(0);
        }
    }
}

bool FrameFunc()
{
    hgeInputEvent event;
    if (hge->Input_GetEvent(&event))
    {
        switch(event.type)
        {
        case INPUT_KEYDOWN:
            switch(event.key)
            {
            case HGEK_R:
                reload();
                break;
            }
        }
    }
    return false;
}

class BoolScope
{
public:
	BoolScope(bool &bFlag) : rbFlag(bFlag)
	{
		rbFlag = true;
	}
	~BoolScope()
	{
		rbFlag = false;
	}
	bool InScope() const
	{
		return rbFlag;
	}
private:
    BoolScope &operator = (const BoolScope &rhs)
    {
        rhs;
        return *this;
    }
	bool &rbFlag;
};
bool RenderFunc()
{
	static bool bRenderFlag = false;
	
	if  (bRenderFlag)
	{
		_asm int 3;
		return true;
	}
	BoolScope scope(bRenderFlag);

    assert(anim);
    hge->Gfx_BeginScene();
    float t1 = hge->Timer_GetTime();
	//float x = 0.f, y = 0.f;
	//hge->Input_GetMousePos(&x, &y);
    int time = int(t1*100); // anim 100fps
 //   if (time < 0)
 //       time = 0;
 //   if (time >= 800)
 //       time = 799;
    
	hge->Gfx_Clear(ARGB(0, 0, 0, 0));
	hge->Gfx_RenderLine(20, 300, 820, 300, ARGB(128,255,255,255));
	hge->Gfx_RenderLine(420, 0, 420, 600, ARGB(128,255,255,255));

	hge->Gfx_SetTransform(0, 0, 420, 300, 0, 1, 1);
	int c = 0;
	for (int i = 25; i < 800-24; i+=50)
	{
		for (int j = 25; j < 600-24; j+=50)
		{
		//hge->Gfx_SetTransform(0, 0, i, j, 0, 1, 1);
			hgeClipState cs;
            cs.position = hgeVector(i - 400, j - 300);
            cs.scale = hgeVector(0.1f, 0.1f);
            anim->render(time + c, &cs);
            c += 10;
		}
    }
    anim->render(time, 0);
	iSystem::GetInstance()->getRenderer("HGE")->flush();
    hge->Gfx_SetTransform();
    font->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d (constant) %d %d\nPress 'R' to reload %s", hge->Timer_GetDelta(), hge->Timer_GetFPS(), time, c, aniFile);
    hge->Gfx_EndScene();

	return false;
}

int APIENTRY WinMain(HINSTANCE,HINSTANCE,LPSTR cmdline,int)
{	
	char path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);
    
    FILE *fp = ::fopen(cmdline, "rt");
    if (fp)
    {
        aniFile = cmdline;
        fclose(fp);
    }

    //ÉèÖÃHGEÊôÐÔ		
    hge = hgeCreate(HGE_VERSION);
    hge->System_SetState(HGE_LOGFILE, "SysInfo.log");
    hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
    hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
    hge->System_SetState(HGE_TITLE, "hge curved ani demo");
    hge->System_SetState(HGE_WINDOWED, true);
    hge->System_SetState(HGE_SCREENWIDTH, 840);
    hge->System_SetState(HGE_SCREENHEIGHT, 600);
    hge->System_SetState(HGE_SCREENBPP, 32);
    hge->System_SetState(HGE_FPS, 60);
    hge->System_SetState(HGE_HIDEMOUSE, false);
//#ifdef _DEBUG
#if HGE_VERSION >= 0x170
    hge->System_SetState(HGE_SHOWSPLASH, false);
#endif
//#endif

    if (hge->System_Initiate())
    {
        hgeCurvedAniSystem system; // NOTE: this system will release before hge's shutdown
        armanager = iSystem::GetInstance()->createAnimResManager();

        anim = system.createAnimation();
        anim->setAnimData(armanager->getAnimData(aniFile), 0); 
        anim->startAnim(0);

        // Load a font
        font=new hgeFont("font1.fnt");

        hge->System_Start();

        iSystem::GetInstance()->release(anim);
        anim = 0;
        iSystem::GetInstance()->release(armanager);
        armanager = 0;
        delete font;
    }

    hge->System_Shutdown();
    hge->Release();
    return 0;
}
