/////////////////////////////////////////////////////////////////////////////
// Name:        curvedanieditor.h
// Purpose:     
// Author:      kaikai
// Modified by: 
// Created:     03/01/2010 22:24:55
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CURVEDANIEDITOR_H_
#define _CURVEDANIEDITOR_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/splitter.h"
#include "wx/listctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_CURVEDANIEDITOR 10000
#define ID_MENUITEM_File_New 10002
#define ID_TOOLBAR 10003
#define ID_SPLITTERWINDOW 10004
#define ID_PANEL 10005
#define ID_TOOLBAR1 10008
#define ID_LISTBOX 10006
#define ID_PANEL2 10007
#define ID_HGECANVAS 10008
#define SYMBOL_CURVEDANIEDITOR_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_CURVEDANIEDITOR_TITLE _("CurvedAni Editor")
#define SYMBOL_CURVEDANIEDITOR_IDNAME ID_CURVEDANIEDITOR
#define SYMBOL_CURVEDANIEDITOR_SIZE wxSize(800, 600)
#define SYMBOL_CURVEDANIEDITOR_POSITION wxDefaultPosition
////@end control identifiers

class AnimListPanel: public wxPanel
{
	DECLARE_CLASS( AnimListPanel )
	DECLARE_EVENT_TABLE()
public:
	AnimListPanel();
	AnimListPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr)
	{
		Create(parent, winid, pos, size, style, name);

		wxArrayString itemListBox10Strings;
		// try some values here
		itemListBox10Strings.Add(wxString("run"));
		itemListBox10Strings.Add(wxString("walk"));
		itemListBox10Strings.Add(wxString("jump"));

		wxListBox *pListBox = new wxListBox(this,ID_LISTBOX, wxDefaultPosition, wxDefaultSize, itemListBox10Strings, wxLB_SINGLE );
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		sizer->Add(pListBox, 1, wxEXPAND);
		this->SetSizerAndFit(sizer);
		sizer->SetSizeHints(this);
	}

	void OnSize(wxSizeEvent& event);
private:
	//wxListBox *m_pListBox;
};

/*!
 * CurvedAniEditor class declaration
 */

class CurvedAniEditor: public wxFrame
{    
    DECLARE_CLASS( CurvedAniEditor )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    CurvedAniEditor();
    CurvedAniEditor( wxWindow* parent, wxWindowID id = SYMBOL_CURVEDANIEDITOR_IDNAME, const wxString& caption = SYMBOL_CURVEDANIEDITOR_TITLE, const wxPoint& pos = SYMBOL_CURVEDANIEDITOR_POSITION, const wxSize& size = SYMBOL_CURVEDANIEDITOR_SIZE, long style = SYMBOL_CURVEDANIEDITOR_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CURVEDANIEDITOR_IDNAME, const wxString& caption = SYMBOL_CURVEDANIEDITOR_TITLE, const wxPoint& pos = SYMBOL_CURVEDANIEDITOR_POSITION, const wxSize& size = SYMBOL_CURVEDANIEDITOR_SIZE, long style = SYMBOL_CURVEDANIEDITOR_STYLE );

    /// Destructor
    ~CurvedAniEditor();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin CurvedAniEditor event handler declarations

////@end CurvedAniEditor event handler declarations

////@begin CurvedAniEditor member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end CurvedAniEditor member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin CurvedAniEditor member variables
////@end CurvedAniEditor member variables

};

#endif
    // _CURVEDANIEDITOR_H_
