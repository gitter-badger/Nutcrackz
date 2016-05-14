#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/toolbar.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/treectrl.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/stc/stc.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/aui/auibook.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/fontdlg.h>

#include "viewport2d.h"
#include "viewport3d.h"
#include "wx/glcanvas.h"

#include <vector>

class MyFrame1;

class MyApp : public wxApp
{
	virtual bool OnInit();

	void onIdle(wxIdleEvent& evt);

	MyFrame1 *frame;
	bool render_loop_on;

public:
	void activateRenderLoop(bool on);
};

class MyFrame1 : public wxFrame
{
public:

	MyFrame1(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(841, 720), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL | wxWANTS_CHARS);
	wxAuiManager m_mgr;
	
	~MyFrame1();

	void OnChange(wxStyledTextEvent& event);
	void OpenFile(wxCommandEvent& WXUNUSED(event));
	void SaveFile(wxCommandEvent& WXUNUSED(event));
	void SaveFileAs(wxCommandEvent& WXUNUSED(event));
	void Import(wxCommandEvent& WXUNUSED(event));
	void Exit(wxCommandEvent& WXUNUSED(event));
	void Debug(wxCommandEvent &WXUNUSED(event));

	void OnSetFocus(wxFocusEvent &event);
	void SetActiveTextCtrl(wxStyledTextCtrl *stc);
	void OnKeyDown(wxKeyEvent &event);
	void OnKeyUp(wxKeyEvent &event);
	void OnChangeSTC(wxStyledTextEvent &event);
	void OnPropertyGridChange(wxPropertyGridEvent& event);

	//Events
	//void OnKeyDown(wxKeyEvent &event);
	//void OnKeyUp(wxKeyEvent &event);

	//Getters
	//InputManager getInput() { return m_input; }
	wxPropertyGrid * getPropertyGrid() { return m_propertyGrid1; }
	Viewport2D *getCanvas2D() { return canvas2D; }
	Viewport3D *getCanvas3D() { return canvas3D; }
	
	//Setters
	void setCanvas3DsDeltaTime(float value)
	{
		for (std::vector<Viewport3D *>::iterator iter = canvas3Ds.begin(); iter != canvas3Ds.end(); ++iter)
		{
			(*iter)->setDeltaTime(value);
		}
	}

protected:
	wxTreeCtrl* m_treeCtrl1;
	wxPropertyGrid* m_propertyGrid1;
	wxAuiNotebook* m_auinotebook1;
	wxStyledTextCtrl* m_scintilla1;
	wxStyledTextCtrl *currControl;
	wxMenuBar* m_menubar1;
	wxMenu* m_menu1;
	wxMenu* m_menu11;
	wxMenu* m_menu2;
	wxMenu* m_menu6;
	wxMenu* m_menu21;
	wxMenu* m_menu31;
	wxMenu* m_menu41;
	wxMenu* m_menu5;
	wxMenu* m_menu4;
	wxMenu* m_menu3;
	wxString CurrentDocPath;
	wxString DocumentPath;

	wxFileDialog *OpenDialog;
	wxFileDialog *SaveDialog;

	Viewport2D *canvas2D;
	Viewport3D *canvas3D;

	float getAmbientLight() { varAmbient = aLight->GetValue(); wxString var = varAmbient; return atof(var); }
	float getDiffuseLight() { varDiffuse = dLight->GetValue(); wxString var = varDiffuse; return atof(var); }

	// Virtual event handlers, overide them in your derived class
	//virtual void onCreate2DScene(wxCommandEvent& event);
	//virtual void onCreate3DSCene(wxCommandEvent& event);
	//virtual void onCreateScript(wxCommandEvent& event);
	virtual void OnNewDlg(wxCommandEvent& event);

private:
	//std::vector<wxStyledTextCtrl *> stcs;
	std::vector<Viewport2D *> canvas2Ds;
	std::vector<Viewport3D *> canvas3Ds;
	bool keys[500];
	bool hasOpenedFile;

	int canvas2DCounter = 1;
	int canvas3DCounter = 1;

	wxPGProperty* aLight;
	wxPGProperty* dLight;
	wxVariant varAmbient;
	wxVariant varDiffuse;

	char *input;

	//InputManager m_input;

	wxString dirPath;

	DECLARE_EVENT_TABLE()
};