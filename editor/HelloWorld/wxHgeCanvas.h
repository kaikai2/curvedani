#ifndef _wx_HGE_Canvas_H_
#define _wx_HGE_Canvas_H_


#include "wx/window.h"

class HGE;

class wxHgeCanvas : public wxWindow
{
public:
	wxHgeCanvas();
	wxHgeCanvas(wxWindow *parent, wxWindowID id,
		const wxPoint& pos, const wxSize& size,
		long style, const wxString& name);
	virtual ~wxHgeCanvas();
	bool Create(wxWindow *parent, wxWindowID id,
		const wxPoint& pos, const wxSize& size,
		long style, const wxString& name);

protected:
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);

	HGE *m_pHge;
private:
	DECLARE_EVENT_TABLE()
	DECLARE_CLASS(wxHgeCanvas)
};

#endif//_wx_HGE_Canvas_H_
