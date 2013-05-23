/////////////////////////////////////////////////////////////////////////////
// Name:        curvedaniapp.cpp
// Purpose:     
// Author:      kaikai
// Modified by: 
// Created:     03/01/2010 22:24:11
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
#include "curvedanieditor.h"
////@end includes

#include "curvedaniapp.h"

////@begin XPM images

////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( CurvedAniApp )
////@end implement app


/*
 * CurvedAniApp type definition
 */

IMPLEMENT_CLASS( CurvedAniApp, wxApp )


/*
 * CurvedAniApp event table definition
 */

BEGIN_EVENT_TABLE( CurvedAniApp, wxApp )

////@begin CurvedAniApp event table entries
////@end CurvedAniApp event table entries

END_EVENT_TABLE()


/*
 * Constructor for CurvedAniApp
 */

CurvedAniApp::CurvedAniApp()
{
    Init();
}


/*
 * Member initialisation
 */

void CurvedAniApp::Init()
{
////@begin CurvedAniApp member initialisation
////@end CurvedAniApp member initialisation
}
/*
 * Initialisation for CurvedAniApp
 */
bool CurvedAniApp::OnInit()
{    
////@begin CurvedAniApp initialisation
    // Remove the comment markers above and below this block
    // to make permanent changes to the code.

#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif
////@end CurvedAniApp initialisation

	CurvedAniEditor *editor = new CurvedAniEditor(NULL);
	editor->Show(true);
	SetTopWindow(editor);
    return true;
}


/*
 * Cleanup for CurvedAniApp
 */

int CurvedAniApp::OnExit()
{  
////@begin CurvedAniApp cleanup
    return wxApp::OnExit();
////@end CurvedAniApp cleanup
}

