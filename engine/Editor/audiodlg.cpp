#include "audiodlg.h"

#include <wx/log.h>

BEGIN_EVENT_TABLE(AudioPlayerDlg, wxDialog)

EVT_CLOSE(AudioPlayerDlg::OnClose)

END_EVENT_TABLE()

AudioPlayerDlg::AudioPlayerDlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);

	m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("Loaded:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText2->Wrap(-1);
	m_staticText2->SetMaxSize(wxSize(318, 28));

	m_mgr.AddPane(m_staticText2, wxAuiPaneInfo().Top().CaptionVisible(false).CloseButton(false).PaneBorder(false).Movable(false).Dock().Fixed().BottomDockable(false).TopDockable(false).LeftDockable(false).RightDockable(false).Floatable(false).BestSize(wxSize(303, 28)));

	m_button6 = new wxButton(this, wxID_ANY, wxT("Open Song"), wxDefaultPosition, wxDefaultSize, 0);
	m_button6->SetMaxSize(wxSize(95, 28));

	m_mgr.AddPane(m_button6, wxAuiPaneInfo().Top().CaptionVisible(false).CloseButton(false).PaneBorder(false).Movable(false).Dock().Fixed().Floatable(false).BestSize(wxSize(80, 28)).Layer(0));

	m_button7 = new wxButton(this, wxID_ANY, wxT("Play"), wxPoint(-1, 200), wxDefaultSize, 0);
	m_button7->SetMinSize(wxSize(95, 50));
	m_button7->SetMaxSize(wxSize(200, 50));

	m_mgr.AddPane(m_button7, wxAuiPaneInfo().Left().CaptionVisible(false).CloseButton(false).PaneBorder(false).Movable(false).Dock().Fixed().BottomDockable(false).TopDockable(false).LeftDockable(false).RightDockable(false).Floatable(false).BestSize(wxSize(115, 50)));

	m_button8 = new wxButton(this, wxID_ANY, wxT("Pause"), wxDefaultPosition, wxDefaultSize, 0);
	m_button8->SetMinSize(wxSize(95, 50));
	m_button8->SetMaxSize(wxSize(200, 50));

	m_mgr.AddPane(m_button8, wxAuiPaneInfo().Center().CaptionVisible(false).CloseButton(false).PaneBorder(false).Movable(false).Dock().Fixed().BottomDockable(false).TopDockable(false).LeftDockable(false).RightDockable(false).Floatable(false).BestSize(wxSize(115, 50)));

	m_button81 = new wxButton(this, wxID_ANY, wxT("Stop"), wxDefaultPosition, wxDefaultSize, 0);
	m_button81->SetMinSize(wxSize(95, 50));
	m_button81->SetMaxSize(wxSize(200, 50));

	m_mgr.AddPane(m_button81, wxAuiPaneInfo().Right().CaptionVisible(false).CloseButton(false).PaneBorder(false).Movable(false).Dock().Fixed().BottomDockable(false).TopDockable(false).LeftDockable(false).RightDockable(false).Floatable(false).BestSize(wxSize(115, 50)));
	
	m_mgr.Update();
	this->Centre(wxBOTH);
	// Connect Events
	m_button6->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioPlayerDlg::Import), NULL, this);
	m_button7->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioPlayerDlg::Play), NULL, this);
	m_button8->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioPlayerDlg::Pause), NULL, this);
	m_button81->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioPlayerDlg::Stop), NULL, this);
}

AudioPlayerDlg::~AudioPlayerDlg()
{
	// Disconnect Events
	m_button6->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioPlayerDlg::Import), NULL, this);
	m_button7->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioPlayerDlg::Play), NULL, this);
	m_button8->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioPlayerDlg::Pause), NULL, this);
	m_button81->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioPlayerDlg::Stop), NULL, this);

	m_mgr.UnInit();
}

void AudioPlayerDlg::OnClose(wxCloseEvent& event)
{
	isClosed = true;
	//this->Close();
	//Destroy();
}

void AudioPlayerDlg::Import(wxCommandEvent& evnt)
{
	wxFileDialog OpenDialog(this, _("Choose a song to play"), wxEmptyString, wxEmptyString, _("OGG files (*.ogg)|*.ogg"), wxFD_OPEN, wxDefaultPosition);

	if (OpenDialog.ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		if (!musicPlayer.hasInitialized)
		{
			musicPlayer.InitAudio();
		}

		if (OpenDialog.GetPath().EndsWith(".ogg"))
		{
			if (musicPlayer.hasPressedPlay() || musicPlayer.hasPressedPause() || musicPlayer.hasPressedResume() || !musicPlayer.hasPressedStop())
			{
				musicPlayer.StopMusic();

				musicPlayer.playIsPressed(false);
				musicPlayer.pauseIsPressed(false);
				musicPlayer.resumeIsPressed(false);
				musicPlayer.stopIsPressed(true);
			}

			musicPlayer.setFilePath(OpenDialog.GetPath());

			musicPlayer.LoadMusic();

			//wxLogMessage("File Path = %s", musicPlayer.getFilePath());

			musicPlayer.songIsLoaded(true);

			openedFileName = OpenDialog.GetFilename();
			m_staticText2->SetLabelText("Loaded: " + openedFileName);
		}
	}
}

void AudioPlayerDlg::Play(wxCommandEvent& evnt)
{
	if (!musicPlayer.hasPressedPlay() && !musicPlayer.hasPressedPause() && !musicPlayer.hasPressedResume())
	{
		musicPlayer.PlayMusic();

		m_staticText2->SetLabelText("Playing: " + openedFileName);

		musicPlayer.playIsPressed(true);
		musicPlayer.pauseIsPressed(false);
		musicPlayer.resumeIsPressed(false);
		musicPlayer.stopIsPressed(false);
	}
	else if (musicPlayer.hasPressedPause())
	{
		musicPlayer.ResumeMusic();

		m_staticText2->SetLabelText("Playing: " + openedFileName);

		musicPlayer.playIsPressed(false);
		musicPlayer.pauseIsPressed(false);
		musicPlayer.resumeIsPressed(true);
		musicPlayer.stopIsPressed(false);

		m_button8->SetLabelText("Pause");
	}
}

void AudioPlayerDlg::Pause(wxCommandEvent& evnt)
{
	if (musicPlayer.hasPressedPlay() || musicPlayer.hasPressedResume())
	{
		musicPlayer.PauseMusic();

		m_staticText2->SetLabelText("Loaded: " + openedFileName);

		musicPlayer.playIsPressed(false);
		musicPlayer.pauseIsPressed(true);
		musicPlayer.resumeIsPressed(false);
		musicPlayer.stopIsPressed(false);

		m_button8->SetLabelText("Resume");
	}
	else if (musicPlayer.hasPressedPause())
	{
		musicPlayer.ResumeMusic();

		m_staticText2->SetLabelText("Playing: " + openedFileName);

		musicPlayer.playIsPressed(false);
		musicPlayer.pauseIsPressed(false);
		musicPlayer.resumeIsPressed(true);
		musicPlayer.stopIsPressed(false);

		m_button8->SetLabelText("Pause");
	}
}

void AudioPlayerDlg::Stop(wxCommandEvent& evnt)
{
	if (!musicPlayer.hasPressedStop())
	{
		musicPlayer.StopMusic();

		m_staticText2->SetLabelText("Loaded: " + openedFileName);

		musicPlayer.playIsPressed(false);
		musicPlayer.pauseIsPressed(false);
		musicPlayer.resumeIsPressed(false);
		musicPlayer.stopIsPressed(true);

		m_button8->SetLabelText("Pause");
	}
}