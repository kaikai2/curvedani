/////////////////////////////////////////////////////////////////////////////
// Name:        curvedaniapp.h
// Purpose:     
// Author:      kaikai
// Modified by: 
// Created:     03/01/2010 22:24:11
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CURVEDANIAPP_H_
#define _CURVEDANIAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
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
////@end control identifiers

/*!
 * CurvedAniApp class declaration
 */

class CurvedAniApp: public wxApp
{    
    DECLARE_CLASS( CurvedAniApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    CurvedAniApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin CurvedAniApp event handler declarations
////@end CurvedAniApp event handler declarations

////@begin CurvedAniApp member function declarations
////@end CurvedAniApp member function declarations

////@begin CurvedAniApp member variables
////@end CurvedAniApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(CurvedAniApp)
////@end declare app

#endif
    // _CURVEDANIAPP_H_
