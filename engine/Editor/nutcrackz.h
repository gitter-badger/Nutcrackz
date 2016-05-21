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
#include "audiodlg.h"
#include "wx/glcanvas.h"

#include <vector>

//#include "musicplayer.h"

class MyFrame1;

class MyApp : public wxApp
{
	virtual bool OnInit();

	void onIdle(wxIdleEvent& evt);

	MyFrame1 *frame;
	AudioPlayerDlg *playerDlg;

	//MusicPlayer musicPlayer;
	
	bool render_loop_on;
	bool audioPlayerIsShown;

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
	void AudioPlayer(wxCommandEvent &WXUNUSED(event));

	void OnSetFocus(wxFocusEvent &event);
	void SetActiveTextCtrl(wxStyledTextCtrl *stc);
	void OnKeyDown(wxKeyEvent &event);
	void OnKeyUp(wxKeyEvent &event);
	void OnChangeSTC(wxStyledTextEvent &event);
	void OnPropertyGridChange(wxPropertyGridEvent& event);
	void OnClose(wxCloseEvent& event);

	//Getters
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

	bool hasBeenClosed;
	bool wantsToShowAP;

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

	void setPositionX() { objectPositionX->SetValue(0.0f); }
	void setPositionY() { objectPositionY->SetValue(0.0f); }
	void setPositionZ() { objectPositionZ->SetValue(0.0f); }
	float getPositionX() { varPosX = objectPositionX->GetValue(); wxString var = varPosX; return atof(var); }
	float getPositionY() { varPosY = objectPositionY->GetValue(); wxString var = varPosY; return atof(var); }
	float getPositionZ() { varPosZ = objectPositionZ->GetValue(); wxString var = varPosZ; return atof(var); }

	float getRotationX() { varRotX = objectRotationX->GetValue(); wxString var = varRotX; return atof(var); }
	float getRotationY() { varRotY = objectRotationY->GetValue(); wxString var = varRotY; return atof(var); }
	float getRotationZ() { varRotZ = objectRotationZ->GetValue(); wxString var = varRotZ; return atof(var); }

	float getScalingX() { varSclX = objectScalingX->GetValue(); wxString var = varSclX; return atof(var); }
	float getScalingY() { varSclY = objectScalingY->GetValue(); wxString var = varSclY; return atof(var); }
	float getScalingZ() { varSclZ = objectScalingZ->GetValue(); wxString var = varSclZ; return atof(var); }

	float getAmbientLight() { varAmbient = aLight->GetValue(); wxString var = varAmbient; return atof(var); }
	float getDiffuseLight() { varDiffuse = dLight->GetValue(); wxString var = varDiffuse; return atof(var); }

	virtual void OnNewDlg(wxCommandEvent& event);

private:
	std::vector<Viewport2D *> canvas2Ds;
	std::vector<Viewport3D *> canvas3Ds;

	bool keys[500];
	bool hasOpenedFile;
	bool hasSavedFile;
	bool hasChanged;
	
	int canvas2DCounter = 1;
	int canvas3DCounter = 1;

	wxPGProperty* objectPositionX;
	wxPGProperty* objectPositionY;
	wxPGProperty* objectPositionZ;

	wxPGProperty* objectRotationX;
	wxPGProperty* objectRotationY;
	wxPGProperty* objectRotationZ;

	wxPGProperty* objectScalingX;
	wxPGProperty* objectScalingY;
	wxPGProperty* objectScalingZ;

	wxPGProperty* aLight;
	wxPGProperty* dLight;
	
	wxVariant varPosX;
	wxVariant varPosY;
	wxVariant varPosZ;

	wxVariant varRotX;
	wxVariant varRotY;
	wxVariant varRotZ;

	wxVariant varSclX;
	wxVariant varSclY;
	wxVariant varSclZ;

	wxVariant varAmbient;
	wxVariant varDiffuse;
	
	GameObject3D go3D;
	GameObject3D::gameObject go;
	GameObject3D::gameObject tempGO;

	std::vector<GameObject3D *> gameObjects;
	std::vector<GameObject2D::gameObject *> gameObjects2D;
	std::vector<GameObject3D::gameObject *> gameObjects3D;

	wxWindow *curr_page;
	int curr_page_index;
	wxString curr_label;

	char *input;

	wxString dirPath;

	DECLARE_EVENT_TABLE()
};