#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/listctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/aui/aui.h>

class NewItemDlg : public wxDialog
{
public:

	NewItemDlg(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Create new item"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(303, 142), long style = wxCAPTION | wxDEFAULT_DIALOG_STYLE); wxAuiManager m_mgr;

	~NewItemDlg();

	//Getters
	bool getScriptSelected() { return isScriptSelected; }
	bool get2DSceneSelected() { return is2DSceneSelected; }
	bool get3DSceneSelected() { return is3DSceneSelected; }

	// Virtual event handlers, overide them in your derived class
	virtual void SelectedItem(wxListEvent& event);
	virtual void OnCancel(wxCommandEvent& event);
	virtual void OnCreate(wxCommandEvent& event);

protected:
	wxListCtrl* m_listCtrl1;
	wxButton* m_button1;
	wxButton* m_button11;

private:
	bool isScriptSelected;
	bool is2DSceneSelected;
	bool is3DSceneSelected;

	long item;

	wxString textInfo;

};