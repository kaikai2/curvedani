#include "wxHgeCanvas.h"
#include "wx/dcclient.h"

//void wxHgeCanvas::OnDraw(wxDC *dc)
//{
//	//hge->Gfx_BeginScene();
//
//
//	//hge->Gfx_EndScene();
//}
//

IMPLEMENT_CLASS(wxHgeCanvas, wxWindow)

BEGIN_EVENT_TABLE(wxHgeCanvas, wxWindow)
	EVT_SIZE(wxHgeCanvas::OnSize)
	EVT_PAINT(wxHgeCanvas::OnPaint)
	//EVT_ERASE_BACKGROUND(wxHgeCanvas::OnEraseBackground)
	//EVT_MOUSE_EVENTS(wxHgeCanvas::OnMouse)
END_EVENT_TABLE()

#include "hge.h"

#pragma comment(lib, "hge")
//bool RenderFunc()
//{
//	return false;
//}

wxHgeCanvas::wxHgeCanvas() : m_pHge(0)
{

}

wxHgeCanvas::wxHgeCanvas(wxWindow *parent, wxWindowID id,
						 const wxPoint& pos, const wxSize& size,
						 long style, const wxString& name) : m_pHge(0)
{
	Create(parent, id, pos, size, style, name);
}

wxHgeCanvas::~wxHgeCanvas()
{
	if (m_pHge)
	{
		m_pHge->System_Shutdown();
		m_pHge->Release();
		m_pHge = 0;
	}
}

bool wxHgeCanvas::Create(wxWindow *parent, wxWindowID id,
	const wxPoint& pos, const wxSize& size,
	long style, const wxString& name)
{
	if (!wxWindow::Create(parent, id, pos, size, style, name))
		return false;

	m_pHge = hgeCreate(HGE_VERSION);
	m_pHge->System_SetState(HGE_HWNDPARENT, (HWND)this->GetHWND());
	//m_pHge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	m_pHge->System_Initiate();
	m_pHge->System_Start();
	return true;
}

void wxHgeCanvas::OnSize(wxSizeEvent& event)
{
	if(m_pHge)
	{
		//m_pHge->System_Shutdown();
		m_pHge->System_SetState(HGE_SCREENWIDTH, event.GetSize().GetWidth());
		m_pHge->System_SetState(HGE_SCREENHEIGHT, event.GetSize().GetHeight());
		//m_pHge->System_Initiate();
	}
}

void wxHgeCanvas::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	
	if(m_pHge)
	{
		//HWND wnd = hge->System_GetState(HGE_HWND);
		//::SetWindowPos(wnd, 0, 0, 0, )
//		HTARGET target = m_pHge->Target_Create(dc.GetSize().GetWidth(), dc.GetSize().GetHeight(), false);
		m_pHge->Gfx_BeginScene();
		static char r, g, b;
		r++; g++; b++;
		m_pHge->Gfx_Clear(ARGB(255, r,g,b));
		m_pHge->Gfx_EndScene();
		//HTEXTURE tex = m_pHge->Target_GetTexture(target);
		//DWORD *data = m_pHge->Texture_Lock(tex);
		//int depth = m_pHge->System_GetState(HGE_SCREENBPP) / 8;
		//wxBitmap *bitmap = new wxBitmap(data, dc.GetSize().GetWidth(), dc.GetSize().GetHeight(), depth);
		//dc.DrawBitmap(*bitmap, wxPoint());
		//
		//m_pHge->Texture_Unlock(tex);
		//m_pHge->Target_Free(target);
	}
}
