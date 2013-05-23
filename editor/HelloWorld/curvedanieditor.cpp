/////////////////////////////////////////////////////////////////////////////
// Name:        curvedanieditor.cpp
// Purpose:     
// Author:      kaikai
// Modified by: 
// Created:     03/01/2010 22:24:55
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "wxHgeCanvas.h"
////@end includes

#include "curvedanieditor.h"

////@begin XPM images
////@end XPM images


/*
 * CurvedAniEditor type definition
 */

IMPLEMENT_CLASS( CurvedAniEditor, wxFrame )


/*
 * CurvedAniEditor event table definition
 */

BEGIN_EVENT_TABLE( CurvedAniEditor, wxFrame )

////@begin CurvedAniEditor event table entries
////@end CurvedAniEditor event table entries

END_EVENT_TABLE()


/*
 * CurvedAniEditor constructors
 */

CurvedAniEditor::CurvedAniEditor()
{
    Init();
}

CurvedAniEditor::CurvedAniEditor( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*
 * CurvedAniEditor creator
 */

bool CurvedAniEditor::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin CurvedAniEditor creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end CurvedAniEditor creation
    return true;
}


/*
 * CurvedAniEditor destructor
 */

CurvedAniEditor::~CurvedAniEditor()
{
////@begin CurvedAniEditor destruction
////@end CurvedAniEditor destruction
}


/*
 * Member initialisation
 */

void CurvedAniEditor::Init()
{
////@begin CurvedAniEditor member initialisation
////@end CurvedAniEditor member initialisation
}


/*
 * Control creation for CurvedAniEditor
 */

void CurvedAniEditor::CreateControls()
{    
////@begin CurvedAniEditor content construction

    CurvedAniEditor* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(ID_MENUITEM_File_New, _("&New"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("&File"));
    wxMenu* itemMenu5 = new wxMenu;
    menuBar->Append(itemMenu5, _("E&dit"));
    itemFrame1->SetMenuBar(menuBar);

    wxToolBar* itemToolBar6 = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, ID_TOOLBAR );
    itemToolBar6->Realize();
    itemFrame1->SetToolBar(itemToolBar6);

    wxSplitterWindow* itemSplitterWindow7 = new wxSplitterWindow( itemFrame1, ID_SPLITTERWINDOW, wxDefaultPosition, wxDefaultSize/*wxSize(100, 100)*/, wxSP_3DBORDER|wxSP_3DSASH|wxNO_BORDER );
    itemSplitterWindow7->SetMinimumPaneSize(64);
    itemSplitterWindow7->SetSashGravity(.1);
	wxPanel* itemPanel8 = new AnimListPanel( itemSplitterWindow7, ID_PANEL, wxDefaultPosition, wxDefaultSize, /*wxSUNKEN_BORDER|*/wxTAB_TRAVERSAL );
	wxHgeCanvas *canvas = new wxHgeCanvas(itemSplitterWindow7, ID_HGECANVAS, wxDefaultPosition, wxDefaultSize, 0, "hgeCanvas");
    itemSplitterWindow7->SplitVertically(itemPanel8, canvas);

////@end CurvedAniEditor content construction
}

/*
 * Should we show tooltips?
 */

bool CurvedAniEditor::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap CurvedAniEditor::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin CurvedAniEditor bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end CurvedAniEditor bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon CurvedAniEditor::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin CurvedAniEditor icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end CurvedAniEditor icon retrieval
}



IMPLEMENT_CLASS( AnimListPanel, wxPanel )


/*
 * CurvedAniEditor event table definition
 */

BEGIN_EVENT_TABLE( AnimListPanel, wxPanel )

////@begin CurvedAniEditor event table entries
	//EVT_SIZE(AnimListPanel::OnSize)
////@end CurvedAniEditor event table entries

END_EVENT_TABLE()

void AnimListPanel::OnSize(wxSizeEvent& event)
{
	if (wxSizer *sizer = this->GetSizer())
	{
		//sizer->FitInside(this);
		//sizer->Layout()
		//this->GetSizer()->Layout();
	}
}
