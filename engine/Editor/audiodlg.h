#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/aui/aui.h>
#include <wx/filedlg.h>

#include "musicplayer.h"

class AudioPlayerDlg : public wxDialog
{
private:
	DECLARE_EVENT_TABLE()

protected:
	wxStaticText *m_staticText2;
	wxButton* m_button6;
	wxButton* m_button7;
	wxButton* m_button8;
	wxButton* m_button81;

public:
	AudioPlayerDlg(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Audio Player"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(398, 96), long style = wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP); wxAuiManager m_mgr;

	~AudioPlayerDlg();

	void OnClose(wxCloseEvent& event);

	// Virtual event handlers, overide them in your derived class
	virtual void Import(wxCommandEvent& event);
	virtual void Play(wxCommandEvent& event);
	virtual void Pause(wxCommandEvent& event);
	virtual void Stop(wxCommandEvent& event);

	bool isClosed;
	bool isWaveFormat;
	bool isOggFormat;
	bool isMp3Format;

	wxString openedFileName;

	MusicPlayer musicPlayer;
};