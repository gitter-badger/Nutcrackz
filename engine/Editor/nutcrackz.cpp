#include "nutcrackz.h"
#include "newitemdlg.h"

#include <wx/textfile.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/clipbrd.h>
#include <wx/utils.h>
#include <wx/propgrid/propgrid.h>

#include <stdio.h>
#include <cmath>
#include <cstring>
#include <shellapi.h>
#include <string>

BEGIN_EVENT_TABLE(MyFrame1, wxFrame)

EVT_KEY_DOWN(MyFrame1::OnKeyDown)
EVT_KEY_UP(MyFrame1::OnKeyUp)
EVT_STC_CHANGE(wxID_ANY, MyFrame1::OnChangeSTC)
EVT_PG_CHANGED(wxID_ANY, MyFrame1::OnPropertyGridChange)
EVT_CLOSE(MyFrame1::OnClose)

END_EVENT_TABLE()

MyFrame1::MyFrame1(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);

	int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };
	
	m_treeCtrl1 = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE);
	m_mgr.AddPane(m_treeCtrl1, wxAuiPaneInfo().Left().Caption(wxT("Project Explorer")).CloseButton(false).Movable(false).Dock().Resizable().FloatingSize(wxSize(200, 98)).BottomDockable(false).TopDockable(false).RightDockable(false).Floatable(false).Position(0).BestSize(wxSize(200, -1)).MinSize(wxSize(200, -1)));

	m_propertyGrid1 = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE | wxPG_SPLITTER_AUTO_CENTER);
	m_mgr.AddPane(m_propertyGrid1, wxAuiPaneInfo().Left().Caption(wxT("Properties")).CloseButton(false).Movable(false).Dock().Resizable().FloatingSize(wxDefaultSize).BottomDockable(false).TopDockable(false).RightDockable(false).Floatable(false).Position(1));

	m_auinotebook1 = new wxAuiNotebook(this, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1), wxAUI_NB_DEFAULT_STYLE);
	m_mgr.AddPane(m_auinotebook1, wxAuiPaneInfo().Center().CaptionVisible(false).CloseButton(false).Movable(false).Dock().Resizable().FloatingSize(wxSize(-1, -1)).BottomDockable(false).TopDockable(false).LeftDockable(false).RightDockable(false).Floatable(false).Row(1).Position(0).CentrePane());

	m_scintilla1 = new wxStyledTextCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 200), 0, wxEmptyString);
	m_scintilla1->SetUseTabs(true);
	m_scintilla1->SetTabWidth(4);
	m_scintilla1->SetIndent(4);
	m_scintilla1->SetTabIndents(true);
	m_scintilla1->SetBackSpaceUnIndents(true);
	m_scintilla1->SetViewEOL(false);
	m_scintilla1->SetViewWhiteSpace(false);
	m_scintilla1->SetMarginWidth(2, 0);
	m_scintilla1->SetIndentationGuides(true);
	m_scintilla1->SetMarginWidth(1, 0);
	m_scintilla1->SetMarginWidth(0, 0);

	m_scintilla1->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS);
	m_scintilla1->MarkerSetBackground(wxSTC_MARKNUM_FOLDER, wxColour(wxT("BLACK")));
	m_scintilla1->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, wxColour(wxT("WHITE")));
	m_scintilla1->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS);
	m_scintilla1->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, wxColour(wxT("BLACK")));
	m_scintilla1->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, wxColour(wxT("WHITE")));
	m_scintilla1->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY);
	m_scintilla1->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS);
	m_scintilla1->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, wxColour(wxT("BLACK")));
	m_scintilla1->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, wxColour(wxT("WHITE")));
	m_scintilla1->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS);
	m_scintilla1->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, wxColour(wxT("BLACK")));
	m_scintilla1->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, wxColour(wxT("WHITE")));
	m_scintilla1->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY);
	m_scintilla1->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY);
	m_scintilla1->SetSelBackground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
	m_scintilla1->SetSelForeground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));

	m_scintilla1->AppendText("---------------------------------------\nWelcome to Dystopia Game Engine.\n---------------------------------------\n");
	m_scintilla1->SetReadOnly(true);
	
	m_scintilla1->SetCaretForeground(wxColour(0, 0, 0));

	/*switch between:
	
	m_scintilla1->SetReadOnly(false);
	and insert text between these two lines above and below, -
	m_scintilla1->SetReadOnly(true);
	
	- everytime you want to log new stuff!*/

	m_scintilla1->SetReadOnly(true);

	m_mgr.AddPane(m_scintilla1, wxAuiPaneInfo().Bottom().Caption(wxT("Output")).CloseButton(false).Movable(false).Dock().Resizable().FloatingSize(wxSize(-1, -1)).BottomDockable(true).TopDockable(false).LeftDockable(false).RightDockable(false).Floatable(false).Row(0).Position(2));

	m_menubar1 = new wxMenuBar(0);
	m_menu1 = new wxMenu();
	
	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem(m_menu11, wxID_ANY, wxString(wxT("New...")), wxEmptyString, wxITEM_NORMAL);
	m_menu1->Append(m_menuItem2);
	
	m_menu1->AppendSeparator();

	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem(m_menu1, wxID_ANY, wxString(wxT("Open File")) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL);
	m_menu1->Append(m_menuItem5);

	wxMenuItem* m_menuItem6;
	m_menuItem6 = new wxMenuItem(m_menu1, wxID_ANY, wxString(wxT("Save File")) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL);
	m_menu1->Append(m_menuItem6);

	wxMenuItem* m_menuItem7;
	m_menuItem7 = new wxMenuItem(m_menu1, wxID_ANY, wxString(wxT("Save As")) + wxT('\t') + wxT("Ctrl+Shift+S"), wxEmptyString, wxITEM_NORMAL);
	m_menu1->Append(m_menuItem7);

	m_menu1->AppendSeparator();

	wxMenuItem* m_menuItem21;
	m_menuItem21 = new wxMenuItem(m_menu1, wxID_ANY, wxString(wxT("Import...")), wxEmptyString, wxITEM_NORMAL);
	m_menu1->Append(m_menuItem21);

	m_menu1->AppendSeparator();

	wxMenuItem* m_menuItem8;
	m_menuItem8 = new wxMenuItem(m_menu1, wxID_ANY, wxString(wxT("Exit")) + wxT('\t') + wxT("Alt+F4"), wxEmptyString, wxITEM_NORMAL);
	m_menu1->Append(m_menuItem8);

	m_menubar1->Append(m_menu1, wxT("File"));

	m_menu2 = new wxMenu();
	wxMenuItem* m_menuItem9;
	m_menuItem9 = new wxMenuItem(m_menu2, wxID_ANY, wxString(wxT("Undo")), wxEmptyString, wxITEM_NORMAL);
	m_menu2->Append(m_menuItem9);

	wxMenuItem* m_menuItem10;
	m_menuItem10 = new wxMenuItem(m_menu2, wxID_ANY, wxString(wxT("Redo")), wxEmptyString, wxITEM_NORMAL);
	m_menu2->Append(m_menuItem10);

	m_menu2->AppendSeparator();

	wxMenuItem* m_menuItem11;
	m_menuItem11 = new wxMenuItem(m_menu2, wxID_ANY, wxString(wxT("Cut")), wxEmptyString, wxITEM_NORMAL);
	m_menu2->Append(m_menuItem11);

	wxMenuItem* m_menuItem12;
	m_menuItem12 = new wxMenuItem(m_menu2, wxID_ANY, wxString(wxT("Copy")), wxEmptyString, wxITEM_NORMAL);
	m_menu2->Append(m_menuItem12);

	wxMenuItem* m_menuItem13;
	m_menuItem13 = new wxMenuItem(m_menu2, wxID_ANY, wxString(wxT("Paste")), wxEmptyString, wxITEM_NORMAL);
	m_menu2->Append(m_menuItem13);

	m_menu2->AppendSeparator();

	wxMenuItem* m_menuItem14;
	m_menuItem14 = new wxMenuItem(m_menu2, wxID_ANY, wxString(wxT("Select All")), wxEmptyString, wxITEM_NORMAL);
	m_menu2->Append(m_menuItem14);

	m_menubar1->Append(m_menu2, wxT("Edit"));

	m_menu6 = new wxMenu();
	m_menu21 = new wxMenu();
	wxMenuItem* m_menu21Item = new wxMenuItem(m_menu6, wxID_ANY, wxT("2D Primitives"), wxEmptyString, wxITEM_NORMAL, m_menu21);
	wxMenuItem* m_menuItem16;
	m_menuItem16 = new wxMenuItem(m_menu21, wxID_ANY, wxString(wxT("Box")), wxEmptyString, wxITEM_NORMAL);
	m_menu21->Append(m_menuItem16);

	wxMenuItem* m_menuItem24;
	m_menuItem24 = new wxMenuItem(m_menu21, wxID_ANY, wxString(wxT("Capsule")), wxEmptyString, wxITEM_NORMAL);
	m_menu21->Append(m_menuItem24);

	m_menu6->Append(m_menu21Item);

	m_menu31 = new wxMenu();
	wxMenuItem* m_menu31Item = new wxMenuItem(m_menu6, wxID_ANY, wxT("3D Primitives"), wxEmptyString, wxITEM_NORMAL, m_menu31);
	wxMenuItem* m_menuItem17;
	m_menuItem17 = new wxMenuItem(m_menu31, wxID_ANY, wxString(wxT("Box")), wxEmptyString, wxITEM_NORMAL);
	m_menu31->Append(m_menuItem17);

	m_menu6->Append(m_menu31Item);

	m_menu6->AppendSeparator();

	m_menu41 = new wxMenu();
	wxMenuItem* m_menu41Item = new wxMenuItem(m_menu6, wxID_ANY, wxT("2D Physics"), wxEmptyString, wxITEM_NORMAL, m_menu41);
	wxMenuItem* m_menuItem22;
	m_menuItem22 = new wxMenuItem(m_menu41, wxID_ANY, wxString(wxT("Box")), wxEmptyString, wxITEM_NORMAL);
	m_menu41->Append(m_menuItem22);

	m_menu6->Append(m_menu41Item);

	m_menu6->AppendSeparator();

	m_menu5 = new wxMenu();
	wxMenuItem* m_menu5Item = new wxMenuItem(m_menu6, wxID_ANY, wxT("2D Lights"), wxEmptyString, wxITEM_NORMAL, m_menu5);
	wxMenuItem* m_menuItem23;
	m_menuItem23 = new wxMenuItem(m_menu5, wxID_ANY, wxString(wxT("Point Light")), wxEmptyString, wxITEM_NORMAL);
	m_menu5->Append(m_menuItem23);

	m_menu6->Append(m_menu5Item);

	m_menubar1->Append(m_menu6, wxT("Geometry"));

	m_menu4 = new wxMenu();
	wxMenuItem* m_menuItem151;
	m_menuItem151 = new wxMenuItem(m_menu4, wxID_ANY, wxString(wxT("Audio Player")) + wxT('\t') + wxT("Shift+A"), wxEmptyString, wxITEM_NORMAL);
	m_menu4->Append(m_menuItem151);

	m_menubar1->Append(m_menu4, wxT("Tools"));

	m_menu3 = new wxMenu();
	wxMenuItem* m_menuItem15;
	m_menuItem15 = new wxMenuItem(m_menu3, wxID_ANY, wxString(wxT("About")), wxEmptyString, wxITEM_NORMAL);
	m_menu3->Append(m_menuItem15);

	m_menubar1->Append(m_menu3, wxT("Help"));

	this->SetMenuBar(m_menubar1);

	m_mgr.Update();
	this->Centre(wxBOTH);

	//Menu Connect Events
	this->Connect(m_menuItem2->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame1::OnNewDlg));
	this->Connect(m_menuItem5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame1::OpenFile));
	this->Connect(m_menuItem6->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame1::SaveFile));
	this->Connect(m_menuItem7->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame1::SaveFileAs));
	this->Connect(m_menuItem21->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame1::Import));
	this->Connect(m_menuItem8->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame1::Exit));
	this->Connect(m_menuItem151->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame1::AudioPlayer));
}

MyFrame1::~MyFrame1()
{
	m_mgr.UnInit();
}

void MyFrame1::OnClose(wxCloseEvent& event)
{
	/*if (event.CanVeto() && m_bFileNotSaved)
	{
		if (wxMessageBox("The file has not been saved... continue closing?",
			"Please confirm",
			wxICON_QUESTION | wxYES_NO) != wxYES)
		{
			event.Veto();
			return;
		}
	}*/
	hasBeenClosed = true;

	Destroy();  // you may also do:  event.Skip();
				// since the default event handler does call Destroy(), too
}


void MyFrame1::OnNewDlg(wxCommandEvent& event)
{
	NewItemDlg itemDlg(NULL, wxID_ANY);

	if (itemDlg.ShowModal() == wxID_OK)
	{
		if (itemDlg.getScriptSelected() == true)
		{
			wxPanel *tmpPanel;

			tmpPanel = new wxPanel(m_auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

			wxBoxSizer *tmpBS;

			tmpBS = new wxBoxSizer(wxHORIZONTAL);

			wxStyledTextCtrl *tmpScintilla;

			tmpScintilla = new wxStyledTextCtrl(tmpPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString);

			// Set the lexer to the C++ lexer
			tmpScintilla->SetLexer(wxSTC_LEX_LUA);

			// Set the color to use for various elements
			tmpScintilla->StyleSetForeground(wxSTC_LUA_COMMENTLINE, wxColor(60, 162, 2));
			tmpScintilla->StyleSetForeground(wxSTC_LUA_COMMENT, wxColor(60, 162, 2));
			tmpScintilla->StyleSetForeground(wxSTC_LUA_PREPROCESSOR, wxColor(100, 100, 100));
			tmpScintilla->StyleSetForeground(wxSTC_LUA_STRING, wxColor(200, 60, 10));
			tmpScintilla->StyleSetForeground(wxSTC_LUA_WORD, wxColor(0, 0, 255));

			// Give a list of keywords. They will be given the style specified for
			// wxSTC_C_WORD items.
			tmpScintilla->SetKeyWords(0, wxT("return int char"));

			tmpScintilla->Bind(wxEVT_STC_CHANGE, &MyFrame1::OnChangeSTC, this);
			tmpScintilla->SetUseTabs(true);
			tmpScintilla->SetTabWidth(4);
			tmpScintilla->SetIndent(4);
			tmpScintilla->SetTabIndents(true);
			tmpScintilla->SetBackSpaceUnIndents(true);
			tmpScintilla->SetViewEOL(false);
			tmpScintilla->SetViewWhiteSpace(false);
			tmpScintilla->SetMarginWidth(2, 0);
			tmpScintilla->SetIndentationGuides(true);
			tmpScintilla->SetMarginType(1, wxSTC_MARGIN_SYMBOL);
			tmpScintilla->SetMarginMask(1, wxSTC_MASK_FOLDERS);
			tmpScintilla->SetMarginWidth(1, 16);
			
			tmpScintilla->SetMarginSensitive(1, true);
			tmpScintilla->SetProperty(wxT("fold"), wxT("1"));
			tmpScintilla->SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);
			tmpScintilla->SetMarginType(0, wxSTC_MARGIN_NUMBER);
			tmpScintilla->SetMarginWidth(0, tmpScintilla->TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_99999")));

			{
				wxFont font = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas");
				tmpScintilla->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
			}

			tmpScintilla->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS);
			tmpScintilla->MarkerSetBackground(wxSTC_MARKNUM_FOLDER, wxColour(wxT("BLACK")));
			tmpScintilla->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, wxColour(wxT("WHITE")));
			tmpScintilla->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS);
			tmpScintilla->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, wxColour(wxT("BLACK")));
			tmpScintilla->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, wxColour(wxT("WHITE")));
			tmpScintilla->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY);
			tmpScintilla->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS);
			tmpScintilla->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, wxColour(wxT("BLACK")));
			tmpScintilla->MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, wxColour(wxT("WHITE")));
			tmpScintilla->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS);
			tmpScintilla->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, wxColour(wxT("BLACK")));
			tmpScintilla->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, wxColour(wxT("WHITE")));
			tmpScintilla->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY);
			tmpScintilla->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY);
			tmpScintilla->SetSelBackground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
			tmpScintilla->SetSelForeground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));

			tmpScintilla->SetCaretForeground(wxColour(0, 0, 0));

			tmpScintilla->SetFocus();

			tmpBS->Add(tmpScintilla, 1, wxEXPAND | wxALL, 5);

			tmpPanel->SetSizer(tmpBS);
			tmpPanel->Layout();
			tmpBS->Fit(tmpPanel);

			tmpScintilla->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(MyFrame1::OnSetFocus), NULL, this);

			m_auinotebook1->AddPage(tmpPanel, wxT("Lua Script"), true, wxNullBitmap);
		}
		else if (itemDlg.get2DSceneSelected() == true)
		{
			int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };

			wxPanel *tmpPanel;

			tmpPanel = new wxPanel(m_auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

			wxBoxSizer *tmpBS;

			tmpBS = new wxBoxSizer(wxVERTICAL);

			canvas2D = new Viewport2D((wxPanel*)tmpPanel, args);

			tmpBS->Add(canvas2D, 1, wxBOTTOM | wxEXPAND | wxRIGHT | wxTOP, 5);

			tmpPanel->SetSizer(tmpBS);
			tmpPanel->Layout();
			tmpBS->Fit(tmpPanel);
			m_auinotebook1->AddPage(tmpPanel, wxT("2D scene"), false, wxNullBitmap);

			canvas2D->setOutput(*m_scintilla1);

			canvas2D->SetLabel("2D Scene " + std::to_string(canvas2DCounter));
			canvas2Ds.push_back(canvas2D);

			canvas2D->SetFocus();

			wxWindow *current_page = m_auinotebook1->GetCurrentPage();
			int current_page_index = m_auinotebook1->GetPageIndex(current_page);
			m_auinotebook1->SetPageText(current_page_index, canvas2D->GetLabel());

			canvas2DCounter++;
		}
		else if (itemDlg.get3DSceneSelected() == true)
		{
			int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };

			wxPanel *tmpPanel;

			tmpPanel = new wxPanel(m_auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

			wxBoxSizer *tmpBS;

			tmpBS = new wxBoxSizer(wxVERTICAL);

			Viewport3D *canvas3D = new Viewport3D((wxPanel*)tmpPanel, args);
			tmpBS->Add(canvas3D, 1, wxBOTTOM | wxEXPAND | wxRIGHT | wxTOP, 5);

			tmpPanel->SetSizer(tmpBS);
			tmpPanel->Layout();
			tmpBS->Fit(tmpPanel);

			m_auinotebook1->AddPage(tmpPanel, wxT("3D scene"), false, wxNullBitmap);

			canvas3D->setOutput(*m_scintilla1);

			canvas3D->SetLabel("3D Scene " + std::to_string(canvas3DCounter));
			canvas3Ds.push_back(canvas3D);

			canvas3D->SetFocus();

			wxWindow *current_page = m_auinotebook1->GetCurrentPage();
			int current_page_index = m_auinotebook1->GetPageIndex(current_page);
			m_auinotebook1->SetPageText(current_page_index, canvas3D->GetLabel());

			if (canvas3Ds.size() == 1)
			{
				wxPGProperty* positionProp = m_propertyGrid1->Append(new wxPropertyCategory("Transform"));

				wxPGProperty* x = m_propertyGrid1->Insert(positionProp, new wxFloatProperty("X: ",
					wxPG_LABEL, atof(canvas3D->getPositionX())));
				wxPGProperty* y = m_propertyGrid1->Insert(positionProp, new wxFloatProperty("Y: ",
					wxPG_LABEL, atof(canvas3D->getPositionY())));
				wxPGProperty* z = m_propertyGrid1->Insert(positionProp, new wxFloatProperty("Z: ",
					wxPG_LABEL, atof(canvas3D->getPositionZ())));

				wxPGProperty* lightProp = m_propertyGrid1->Append(new wxPropertyCategory("Directional Light"));

				aLight = m_propertyGrid1->Insert(lightProp, new wxFloatProperty("Ambient: ",
					wxPG_LABEL, canvas3D->getAmbientIntensity()));

				dLight = m_propertyGrid1->Insert(lightProp, new wxFloatProperty("Diffuse: ",
					wxPG_LABEL, canvas3D->getDiffuseIntensity()));
			}
			canvas3DCounter++;
		}
	}

	event.Skip();
}

void MyFrame1::OnPropertyGridChange(wxPropertyGridEvent& event)
{
	for (std::vector<Viewport3D *>::iterator iter = canvas3Ds.begin(); iter != canvas3Ds.end(); ++iter)
	{
		(*iter)->setAmbientIntensity(getAmbientLight());
		(*iter)->setDiffuseIntensity(getDiffuseLight());
	}
}

void MyFrame1::OpenFile(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog OpenDialog(
		this, _("Choose a file to open"), wxEmptyString, wxEmptyString, _("LUA files (*.lua)|*.lua|2D Scenes (*.2ds) | *.2ds|3D Scenes (*.3ds)|*.3ds"), wxFD_OPEN, wxDefaultPosition);

	// Creates a "open file" dialog with 4 file types
	if (OpenDialog.ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		if (OpenDialog.GetPath().EndsWith(".lua"))
		{
			wxPanel *tmpPanel;

			tmpPanel = new wxPanel(m_auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

			wxBoxSizer *tmpBS;

			tmpBS = new wxBoxSizer(wxHORIZONTAL);

			wxStyledTextCtrl *tmpScintilla;

			tmpScintilla = new wxStyledTextCtrl(tmpPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString);

			// Set the lexer to the C++ lexer
			tmpScintilla->SetLexer(wxSTC_LEX_LUA);

			// Set the color to use for various elements
			tmpScintilla->StyleSetForeground(wxSTC_LUA_COMMENTLINE, wxColor(60, 162, 2));
			tmpScintilla->StyleSetForeground(wxSTC_LUA_COMMENT, wxColor(60, 162, 2));
			tmpScintilla->StyleSetForeground(wxSTC_LUA_PREPROCESSOR, wxColor(100, 100, 100));
			tmpScintilla->StyleSetForeground(wxSTC_LUA_STRING, wxColor(200, 60, 10));
			tmpScintilla->StyleSetForeground(wxSTC_LUA_WORD, wxColor(0, 0, 255));

			// Give a list of keywords. They will be given the style specified for
			// wxSTC_C_WORD items.
			tmpScintilla->SetKeyWords(0, wxT("return int char"));

			tmpScintilla->Bind(wxEVT_STC_CHANGE, &MyFrame1::OnChangeSTC, this);
			tmpScintilla->SetUseTabs(true);
			tmpScintilla->SetTabWidth(4);
			tmpScintilla->SetIndent(4);
			tmpScintilla->SetTabIndents(true);
			tmpScintilla->SetBackSpaceUnIndents(true);
			tmpScintilla->SetViewEOL(false);
			tmpScintilla->SetViewWhiteSpace(false);
			tmpScintilla->SetMarginWidth(2, 0);
			tmpScintilla->SetIndentationGuides(true);
			tmpScintilla->SetMarginType(1, wxSTC_MARGIN_SYMBOL);
			tmpScintilla->SetMarginMask(1, wxSTC_MASK_FOLDERS);
			tmpScintilla->SetMarginWidth(1, 16);
			tmpScintilla->SetMarginSensitive(1, true);
			tmpScintilla->SetProperty(wxT("fold"), wxT("1"));
			tmpScintilla->SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);
			tmpScintilla->SetMarginType(0, wxSTC_MARGIN_NUMBER);
			tmpScintilla->SetMarginWidth(0, tmpScintilla->TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_99999")));

			{
				wxFont font = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas");
				tmpScintilla->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
			}

			tmpScintilla->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS);
			tmpScintilla->MarkerSetBackground(wxSTC_MARKNUM_FOLDER, wxColour(wxT("BLACK")));
			tmpScintilla->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, wxColour(wxT("WHITE")));
			tmpScintilla->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS);
			tmpScintilla->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, wxColour(wxT("BLACK")));
			tmpScintilla->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, wxColour(wxT("WHITE")));
			tmpScintilla->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY);
			tmpScintilla->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS);
			tmpScintilla->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, wxColour(wxT("BLACK")));
			tmpScintilla->MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, wxColour(wxT("WHITE")));
			tmpScintilla->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS);
			tmpScintilla->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, wxColour(wxT("BLACK")));
			tmpScintilla->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, wxColour(wxT("WHITE")));
			tmpScintilla->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY);
			tmpScintilla->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY);
			tmpScintilla->SetSelBackground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
			tmpScintilla->SetSelForeground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));

			tmpBS->Add(tmpScintilla, 1, wxEXPAND | wxALL, 5);

			tmpPanel->SetSizer(tmpBS);
			tmpPanel->Layout();
			tmpBS->Fit(tmpPanel);

			tmpScintilla->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(MyFrame1::OnSetFocus), NULL, this);

			m_auinotebook1->AddPage(tmpPanel, OpenDialog.GetFilename(), true, wxNullBitmap);
			
			//tmpScintilla->SetCaretLineVisibleAlways(true);

			CurrentDocPath = OpenDialog.GetPath();

			// Sets our current document to the file the user selected
			tmpScintilla->LoadFile(CurrentDocPath); //Opens that file
			dirPath = OpenDialog.GetDirectory();
			
			hasOpenedFile = true;
		}
		else if (OpenDialog.GetPath().EndsWith(".3ds"))
		{
			int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };

			wxPanel *tmpPanel;

			tmpPanel = new wxPanel(m_auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

			wxBoxSizer *tmpBS;

			tmpBS = new wxBoxSizer(wxVERTICAL);

			canvas3D = new Viewport3D((wxPanel*)tmpPanel, args);
			tmpBS->Add(canvas3D, 1, wxBOTTOM | wxEXPAND | wxRIGHT | wxTOP, 5);

			tmpPanel->SetSizer(tmpBS);
			tmpPanel->Layout();
			tmpBS->Fit(tmpPanel);

			m_auinotebook1->AddPage(tmpPanel, OpenDialog.GetFilename(), false, wxNullBitmap);

			float posX, posY, posZ;
			float rotX, rotY, rotZ;
			float sclX, sclY, sclZ;

			int numGos;

			std::string m_file;
			std::string m_objName;

			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;
			
			int j = -1;

			canvas3D->SetLabel(OpenDialog.GetFilename());
			canvas3Ds.push_back(canvas3D);

			canvas3D->SetFocus();

			std::string path = OpenDialog.GetPath();

			GameObject3D::gameObject test;

			fstream file;

			int objCount = 0;

			file.open(path.c_str(), ios::binary | ios::in);

			if (!file.is_open())
			{
				cout << "Could not open file!" << endl;
			}

			file.seekg(0, ios::beg);

			while (file.good())
			{
				file.read((char *)&test, sizeof(GameObject3D::gameObject));

				GameObject3D *go3D = new GameObject3D(test.m_filepath);
				GameObject3D::gameObject *gameObj = new GameObject3D::gameObject();

				gameObj->setPath(test.m_filepath);
				gameObj->setName(test.m_name);
				gameObj->setPosition(test.m_positionXYZ);
				gameObj->setRotation(test.m_rotationXYZ);
				gameObj->setScale(test.m_scaleXYZ);

				go3D->setName(test.m_name);
				go3D->setPosition(test.m_positionXYZ);
				go3D->setRotation(test.m_rotationXYZ);
				go3D->setScale(test.m_scaleXYZ);

				gameObjects.push_back(gameObj);

				canvas3D->addGameObject(go3D);
				objCount++;
			}
			file.close();

			gameObjects.resize(objCount - 1);
			canvas3D->resizeGameObjects(objCount - 1);

			//ostringstream oss;
			//oss << gameObjects.size();//canvas3D->getMyGameObjects().size();

			//m_scintilla1->SetReadOnly(false);
			//m_scintilla1->AppendText("GameObjects in scene: " + oss.str());

			canvas3D->setOutput(*m_scintilla1);

			/*if (canvas3Ds.size() == 1)
			{
				wxPGProperty* positionProp = m_propertyGrid1->Append(new wxPropertyCategory("Transform"));

				wxPGProperty* x = m_propertyGrid1->Insert(positionProp, new wxFloatProperty("X: ",
					wxPG_LABEL, atof(canvas3D->getPositionX())));
				wxPGProperty* y = m_propertyGrid1->Insert(positionProp, new wxFloatProperty("Y: ",
					wxPG_LABEL, atof(canvas3D->getPositionY())));
				wxPGProperty* z = m_propertyGrid1->Insert(positionProp, new wxFloatProperty("Z: ",
					wxPG_LABEL, atof(canvas3D->getPositionZ())));

				wxPGProperty* lightProp = m_propertyGrid1->Append(new wxPropertyCategory("Directional Light"));

				aLight = m_propertyGrid1->Insert(lightProp, new wxFloatProperty("Ambient: ",
					wxPG_LABEL, canvas3D->getAmbientIntensity()));

				dLight = m_propertyGrid1->Insert(lightProp, new wxFloatProperty("Diffuse: ",
					wxPG_LABEL, canvas3D->getDiffuseIntensity()));

				//if (canvas3D->hasOutput())
				//m_scintilla1->AppendText("3D viewport just got this output!");
			}*/
		}
		else if (OpenDialog.GetPath().EndsWith(".2ds"))
		{
			int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };

			wxPanel *tmpPanel;

			tmpPanel = new wxPanel(m_auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

			wxBoxSizer *tmpBS;

			tmpBS = new wxBoxSizer(wxVERTICAL);

			canvas2D = new Viewport2D((wxPanel*)tmpPanel, args);

			tmpBS->Add(canvas2D, 1, wxBOTTOM | wxEXPAND | wxRIGHT | wxTOP, 5);


			tmpPanel->SetSizer(tmpBS);
			tmpPanel->Layout();
			tmpBS->Fit(tmpPanel);
			m_auinotebook1->AddPage(tmpPanel, OpenDialog.GetFilename(), false, wxNullBitmap);

			float posX;
			float posY;

			glm::vec2 position;

			wxFileInputStream input(OpenDialog.GetPath());
			wxTextInputStream text(input, wxT("\x09"), wxConvUTF8);
			while (input.IsOk() && !input.Eof())
			{
				wxString line = text.ReadLine();
				
				// do something with the string
				if (line.StartsWith("Position X:"))
				{
					std::string x = line.Remove(0, 12);

					posX = ::atof(x.c_str());
				}

				if (line.StartsWith("Position Y:"))
				{
					std::string y = line.Remove(0, 12);

					posY = ::atof(y.c_str());
				}

				position = glm::vec2(posX, posY);

				canvas2D->setPositionXY(position);

				if (line.StartsWith("Rotation:"))
				{
					std::string myRot = line.Remove(0, 10);

					float valueRot = ::atof(myRot.c_str());

					canvas2D->setRotation(valueRot);
				}
			}

			canvas2D->SetLabel(OpenDialog.GetFilename());			
			canvas2Ds.push_back(canvas2D);

			canvas2D->SetFocus();
			
			dirPath = OpenDialog.GetDirectory();
		}
	}
}

void MyFrame1::SaveFile(wxCommandEvent& WXUNUSED(event))
{
	wxWindow *current_page = m_auinotebook1->GetCurrentPage();
	int current_page_index = m_auinotebook1->GetPageIndex(current_page);
	wxString current_label = m_auinotebook1->GetPageText(current_page_index);

	if (current_label.EndsWith(" *"))
	{
		if (dirPath != "" && m_auinotebook1->GetPageText(current_page_index) != "Lua Script" && m_auinotebook1->GetPageText(current_page_index) != "Lua Script *" && m_auinotebook1->GetPageText(current_page_index).Contains(".lua"))
		{
			wxString newLabel = current_label.Remove(current_label.length() - 2, 2);

			currControl->SaveFile(dirPath + "\\" + newLabel);

			m_auinotebook1->SetPageText(current_page_index, newLabel);
		}
		else
		{
			wxFileDialog SaveDialog(
				this, _("Choose a filetype to save to"), wxEmptyString, wxEmptyString, _("LUA Scripts (*.lua)|*.lua|2D Scenes (*.2ds) | *.2ds|3D Scenes (*.3ds)|*.3ds"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

			if (SaveDialog.ShowModal() == wxID_OK)
			{
				if (SaveDialog.GetPath().EndsWith(".lua"))
				{
					if (currControl != nullptr)
					{
						currControl->SaveFile(SaveDialog.GetPath());

						m_auinotebook1->SetPageText(current_page_index, SaveDialog.GetFilename());

						hasSavedFile = true;
					}

					dirPath = SaveDialog.GetDirectory();
					CurrentDocPath = SaveDialog.GetPath();
				}
				else if (SaveDialog.GetPath().EndsWith(".3ds"))
				{
					wxString newLabel = current_label;//current_label.Remove(current_label.length() - 2, 2);

					if (m_auinotebook1->GetPageText(current_page_index) == newLabel)
					{
						wxTextFile file(dirPath + "\\" + current_label);

						int lineCount = file.GetLineCount();

						for (std::vector<Viewport3D *>::iterator iter = canvas3Ds.begin(); iter != canvas3Ds.end(); ++iter)
						{
							if (current_label == (*iter)->GetLabel())
							{
								std::string path = SaveDialog.GetPath();

								fstream file(path, ios::binary | ios::out | ios::trunc);

								if (!file.is_open())
								{
									cout << "error opening file!" << endl;
								}
								else
								{
									for (std::vector<GameObject3D::gameObject *>::iterator goIter = gameObjects.begin(); goIter != gameObjects.end(); goIter++)
									{
										GameObject3D::gameObject *gos;
										gos = (*goIter);

										file.write((char *)gos, sizeof(GameObject3D::gameObject));
									}
								}

								file.close();

								(*iter)->SetLabel(current_label);
							}
						}
						m_auinotebook1->SetPageText(current_page_index, current_label);
					}
				}
				else if (SaveDialog.GetPath().EndsWith(".2ds"))
				{
				}
			}
		}
	}
	else
	{
		wxFileDialog SaveDialog(
			this, _("Choose a filetype to save to"), wxEmptyString, wxEmptyString, _("LUA files (*.lua)|*.lua|2D Scenes (*.2ds) | *.2ds|3D Scenes (*.3ds)|*.3ds"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

		if (dirPath != "" /*&& m_auinotebook1->GetPageText(current_page_index) != "2D Scene *"*/)
		{
			if (m_auinotebook1->GetPageText(current_page_index) != "2D Scene")
			{
				if (m_auinotebook1->GetPageText(current_page_index) != "3D Scene" || !m_auinotebook1->GetPageText(current_page_index).EndsWith(".3ds"))
				{
					wxTextFile file(dirPath + "\\" + current_label);

					int lineCount = file.GetLineCount();

					for (std::vector<Viewport2D *>::iterator iter = canvas2Ds.begin(); iter != canvas2Ds.end(); ++iter)
					{
						if (current_label == (*iter)->GetLabel())
						{
							for (int i = 0; i < lineCount; i++)
							{
								file.RemoveLine(i);
							}

							file.AddLine((*iter)->getPositionX());
							file.AddLine((*iter)->getPositionY());
							file.AddLine((*iter)->getRotation());

							file.Write();

							file.Close();
						}
					}

					m_auinotebook1->SetPageText(current_page_index, current_label);
				}
			}
			
			if (m_auinotebook1->GetPageText(current_page_index) != "3D Scene")
			{
				if (m_auinotebook1->GetPageText(current_page_index) != "2D Scene" || !m_auinotebook1->GetPageText(current_page_index).EndsWith(".2ds"))
				{
					wxString newLabel = current_label;//current_label.Remove(current_label.length() - 2, 2);

					if (m_auinotebook1->GetPageText(current_page_index) == newLabel)
					{
						wxTextFile file(dirPath + "\\" + current_label);

						int lineCount = file.GetLineCount();

						for (std::vector<Viewport3D *>::iterator iter = canvas3Ds.begin(); iter != canvas3Ds.end(); ++iter)
						{
							if (current_label == (*iter)->GetLabel())
							{
								std::string path = SaveDialog.GetPath();

								fstream file(path, ios::binary | ios::out | ios::trunc);

								if (!file.is_open())
								{
									cout << "error opening file!" << endl;
								}
								else
								{
									for (std::vector<GameObject3D::gameObject *>::iterator goIter = gameObjects.begin(); goIter != gameObjects.end(); goIter++)
									{
										GameObject3D::gameObject *gos;
										gos = (*goIter);

										file.write((char *)gos, sizeof(GameObject3D::gameObject));
									}
								}

								file.close();

								(*iter)->SetLabel(current_label);
							}
						}
						m_auinotebook1->SetPageText(current_page_index, current_label);
					}
				}
			}
		}
		else
		{	
			if (SaveDialog.ShowModal() == wxID_OK)
			{
				if (SaveDialog.GetPath().EndsWith(".2ds"))
				{
					wxTextFile file(SaveDialog.GetPath());

					for (std::vector<Viewport2D *>::iterator iter = canvas2Ds.begin(); iter != canvas2Ds.end(); ++iter)
					{
						if (current_label == (*iter)->GetLabel())
						{
							file.AddLine((*iter)->getPositionX());
							file.AddLine((*iter)->getPositionY());
							file.AddLine((*iter)->getRotation());

							file.Write();

							file.Close();
						}
					}

					m_auinotebook1->SetPageText(current_page_index, SaveDialog.GetFilename());

					dirPath = SaveDialog.GetDirectory();
					CurrentDocPath = SaveDialog.GetPath();
				}

				if (SaveDialog.GetPath().EndsWith(".3ds"))
				{
					wxTextFile file(SaveDialog.GetPath());

					for (std::vector<Viewport3D *>::iterator iter = canvas3Ds.begin(); iter != canvas3Ds.end(); ++iter)
					{
						if (current_label == (*iter)->GetLabel())
						{
							std::string path = SaveDialog.GetPath();

							fstream file(path, ios::binary | ios::out | ios::trunc);

							if (!file.is_open())
							{
								cout << "error opening file!" << endl;
							}
							else
							{
								for (std::vector<GameObject3D::gameObject *>::iterator goIter = gameObjects.begin(); goIter != gameObjects.end(); goIter++)
								{
									GameObject3D::gameObject *gos;
									gos = (*goIter);

									file.write((char *)gos, sizeof(GameObject3D::gameObject));
								}
							}

							file.close();

							(*iter)->SetLabel(current_label);
						}
					}
					m_auinotebook1->SetPageText(current_page_index, SaveDialog.GetFilename());

					dirPath = SaveDialog.GetDirectory();
					CurrentDocPath = SaveDialog.GetPath();
				}
			}
		}
	}
}

void MyFrame1::SaveFileAs(wxCommandEvent& WXUNUSED(event))
{
	wxWindow *current_page = m_auinotebook1->GetCurrentPage();
	int current_page_index = m_auinotebook1->GetPageIndex(current_page);
	wxString current_label = m_auinotebook1->GetPageText(current_page_index);

	if (current_label.EndsWith(" *"))
	{
		wxFileDialog SaveDialog(
			this, _("Choose a filetype to save to"), wxEmptyString, wxEmptyString, _("LUA Scripts (*.lua)|*.lua|2D Scenes (*.2ds) | *.2ds|3D Scenes (*.3ds)|*.3ds"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

		if (SaveDialog.ShowModal() == wxID_OK)
		{
			if (SaveDialog.GetPath().EndsWith(".lua"))
			{
				if (currControl != nullptr)
				{
					currControl->SaveFile(SaveDialog.GetPath());

					m_auinotebook1->SetPageText(current_page_index, SaveDialog.GetFilename());
					
					hasSavedFile = true;
				}
			}
			else if (SaveDialog.GetPath().EndsWith(".3ds"))
			{
				wxString newLabel = current_label;//current_label.Remove(current_label.length() - 2, 2);

				if (m_auinotebook1->GetPageText(current_page_index) == newLabel)
				{
					for (std::vector<Viewport3D *>::iterator iter = canvas3Ds.begin(); iter != canvas3Ds.end(); ++iter)
					{
						if (current_label == (*iter)->GetLabel())
						{
							std::string path = SaveDialog.GetPath();

							fstream file(path, ios::binary | ios::out | ios::trunc);

							if (!file.is_open())
							{
								cout << "error opening file!" << endl;
							}
							else
							{
								for (std::vector<GameObject3D::gameObject *>::iterator goIter = gameObjects.begin(); goIter != gameObjects.end(); goIter++)
								{
									GameObject3D::gameObject *gos;
									gos = (*goIter);

									file.write((char *)gos, sizeof(GameObject3D::gameObject));
								}
							}

							file.close();
						
							(*iter)->SetLabel(current_label);
						}
					}
					m_auinotebook1->SetPageText(current_page_index, current_label);
				}
			}
			else if (SaveDialog.GetPath().EndsWith(".2ds"))
			{
			}

			dirPath = SaveDialog.GetDirectory();
			CurrentDocPath = SaveDialog.GetPath();
		}
	}
	if (!current_label.EndsWith(" *"))
	{
		wxFileDialog SaveDialog(
			this, _("Choose a file to save"), wxEmptyString, wxEmptyString, _("LUA files (*.lua)|*.lua|2D Scenes (*.2ds) |*.2ds|3D Scenes (*.3ds)|*.3ds"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

		if (SaveDialog.ShowModal() == wxID_OK)
		{
			if (SaveDialog.GetPath().EndsWith(".lua"))
			{
				std::string path = SaveDialog.GetPath();
			}

			if (SaveDialog.GetPath().EndsWith(".2ds"))
			{			
				wxTextFile file(SaveDialog.GetPath());

				for (std::vector<Viewport2D *>::iterator iter = canvas2Ds.begin(); iter != canvas2Ds.end(); ++iter)
				{
					if (current_label == (*iter)->GetLabel())
					{
						file.AddLine((*iter)->getPositionX());
						file.AddLine((*iter)->getPositionY());
						file.AddLine((*iter)->getRotation());

						file.Write();

						file.Close();
					}
				}
			}

			if (SaveDialog.GetPath().EndsWith(".3ds"))
			{
				wxTextFile file(SaveDialog.GetPath());

				for (std::vector<Viewport3D *>::iterator iter = canvas3Ds.begin(); iter != canvas3Ds.end(); ++iter)
				{
					if (current_label == (*iter)->GetLabel())
					{
						std::string path = SaveDialog.GetPath();

						fstream file(path, ios::binary | ios::out | ios::trunc);

						if (!file.is_open())
						{
							cout << "error opening file!" << endl;
						}
						else
						{
							for (std::vector<GameObject3D::gameObject *>::iterator goIter = gameObjects.begin(); goIter != gameObjects.end(); goIter++)
							{
								GameObject3D::gameObject *gos;
								gos = (*goIter);

								file.write((char *)gos, sizeof(GameObject3D::gameObject));
							}
						}

						file.close();
						
						(*iter)->SetLabel(SaveDialog.GetFilename());
					}
				}
			}

			m_auinotebook1->SetPageText(current_page_index, SaveDialog.GetFilename());

			dirPath = SaveDialog.GetDirectory();
			CurrentDocPath = SaveDialog.GetPath();
		}
	}
}

void MyFrame1::Import(wxCommandEvent& WXUNUSED(event))
{
	wxWindow *current_page = m_auinotebook1->GetCurrentPage();
	int current_page_index = m_auinotebook1->GetPageIndex(current_page);
	wxString current_label = m_auinotebook1->GetPageText(current_page_index);

	wxFileDialog OpenDialog(
		this, _("Choose a file to open"), wxEmptyString, wxEmptyString, _("Obj files (*.obj)|*.obj|Collada files (*.dae)|*.dae|Modo files (*.lxo)|*.lxo|Fbx files (*.fbx)|*.fbx"), wxFD_OPEN, wxDefaultPosition);

	// Creates a "open file" dialog with 4 file types
	if (OpenDialog.ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		if (OpenDialog.GetPath().EndsWith(".obj"))
		{
			std::string path = OpenDialog.GetPath();
			std::string name = OpenDialog.GetFilename();
			
			for (std::vector<Viewport3D *>::iterator iter = canvas3Ds.begin(); iter != canvas3Ds.end(); ++iter)
			{
				if (current_label == (*iter)->GetLabel())
				{
					GameObject3D *model = new GameObject3D(path);
					GameObject3D::gameObject *gameObj = new GameObject3D::gameObject();
					
					name.resize(name.length() - 4);

					gameObj->setPath((char*)path.c_str());
					gameObj->setName((char*)name.c_str());
					gameObj->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
					gameObj->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
					gameObj->setScale(glm::vec3(1.0f, 1.0f, 1.0f));

					model->setName(name);
					model->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
					model->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
					model->setScale(glm::vec3(1.0f, 1.0f, 1.0f));

					gameObjects.push_back(gameObj);

					(*iter)->addGameObject(model);
				}
			}
		}
		else if (OpenDialog.GetPath().EndsWith(".dae"))
		{
			std::string path = OpenDialog.GetPath();
			std::string name = OpenDialog.GetFilename();

			GameObject3D *model = new GameObject3D(path);

			for (std::vector<Viewport3D *>::iterator iter = canvas3Ds.begin(); iter != canvas3Ds.end(); ++iter)
			{
				if (current_label == (*iter)->GetLabel())
				{
					name.resize(name.length() - 4);

					go.setName((char*)name.c_str());
					go3D.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
					go3D.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
					go3D.setScale(glm::vec3(1.0f, 1.0f, 1.0f));

					(*iter)->addGameObject(model);
				}
			}
		}
		else if (OpenDialog.GetPath().EndsWith(".lxo"))
		{
			std::string path = OpenDialog.GetPath();
			std::string name = OpenDialog.GetFilename();

			GameObject3D *model = new GameObject3D(path);

			for (std::vector<Viewport3D *>::iterator iter = canvas3Ds.begin(); iter != canvas3Ds.end(); ++iter)
			{
				if (current_label == (*iter)->GetLabel())
				{
					name.resize(name.length() - 4);

					go.setName((char*)name.c_str());
					go3D.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
					go3D.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
					go3D.setScale(glm::vec3(1.0f, 1.0f, 1.0f));

					(*iter)->addGameObject(model);
				}
			}
		}
		else if (OpenDialog.GetPath().EndsWith(".fbx"))
		{
			std::string path = OpenDialog.GetPath();
			std::string name = OpenDialog.GetFilename();

			GameObject3D *model = new GameObject3D(path);

			for (std::vector<Viewport3D *>::iterator iter = canvas3Ds.begin(); iter != canvas3Ds.end(); ++iter)
			{
				if (current_label == (*iter)->GetLabel())
				{
					name.resize(name.length() - 4);

					go.setName((char*)name.c_str());
					go3D.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
					go3D.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
					go3D.setScale(glm::vec3(1.0f, 1.0f, 1.0f));

					(*iter)->addGameObject(model);
				}
			}
		}
		OpenDialog.Close();
	}
}

void MyFrame1::Exit(wxCommandEvent& WXUNUSED(event))
{
	this->Close(true);
}

void MyFrame1::AudioPlayer(wxCommandEvent &WXUNUSED(event))
{	
	if (!wantsToShowAP)
	{
		wantsToShowAP = true;
	}
	else if (wantsToShowAP)
	{
		wantsToShowAP = false;
	}
}

void MyFrame1::Debug(wxCommandEvent &WXUNUSED(event))
{
	wxWindow *current_page = m_auinotebook1->GetCurrentPage();
	int current_page_index = m_auinotebook1->GetPageIndex(current_page);
	wxString current_label = m_auinotebook1->GetPageText(current_page_index);

	if (current_page != nullptr)
	{
		if (!current_label.StartsWith("Lua Script"))
		{
			if (current_label.EndsWith(" *"))
			{
				wxString newLabel = current_label.Remove(current_label.length() - 2, 2);

				std::string fullPath = dirPath + "\\" + newLabel;

				wxExecute(wxT("debugger/sq.exe " + fullPath));

			}
			else if (current_label.EndsWith(".lua"))
			{
				std::string fullPath = dirPath + "\\" + current_label;

				//wxExecute(wxT("debugger/sq.exe " + fullPath));
			}
		}
		else
		{
			wxLogError("Sorry, but the debugger does not accept non-existing files!");
		}
	}
}

void MyFrame1::OnSetFocus(wxFocusEvent &event)
{
	wxStyledTextCtrl *stc = wxDynamicCast(event.GetEventObject(), wxStyledTextCtrl);
	if (stc != NULL)
	{
		stc->SetSTCFocus(true);
		SetActiveTextCtrl(stc);
	}
}

void MyFrame1::SetActiveTextCtrl(wxStyledTextCtrl *stc)
{
	currControl = stc;
}

void MyFrame1::OnChangeSTC(wxStyledTextEvent &event)
{
	wxWindow *current_page = m_auinotebook1->GetCurrentPage();
	int current_page_index = m_auinotebook1->GetPageIndex(current_page);
	wxString current_label = m_auinotebook1->GetPageText(current_page_index);

	if (currControl != NULL)
	{
		int currentPos = currControl->GetCurrentPos();
		int wordStartPos = currControl->WordStartPosition(currentPos, true);

		// Display the autocompletion list
		int lenEntered = currentPos - wordStartPos;
		if (lenEntered > 0)
		{
			if (!current_label.EndsWith(" *"))
			{
				if (hasSavedFile == true)
				{
					hasSavedFile = false;
				}

				if (hasOpenedFile == true)
				{
					hasOpenedFile = false;
				}

				m_auinotebook1->SetPageText(current_page_index, current_label + " *");
			}
		}
		else if (lenEntered <= 0 || hasSavedFile == true || hasOpenedFile == true)
		{
			if (current_label.EndsWith(" *"))
			{
				wxString newLabel = current_label.Remove(current_label.length() - 2, 2);

				m_auinotebook1->SetPageText(current_page_index, newLabel);
			}
		}
	}
}

void MyFrame1::OnChange(wxStyledTextEvent& event)
{
	//Autocompletion! disabled for the moment

	/*wxStyledTextCtrl* stc = (wxStyledTextCtrl*)event.GetEventObject();

	// Find the word start
	int currentPos = stc->GetCurrentPos();
	int wordStartPos = stc->WordStartPosition(currentPos, true);

	// Display the autocompletion list
	int lenEntered = currentPos - wordStartPos;
	if (lenEntered > 0)
	{
		stc->AutoCompShow(lenEntered, "and break do else elseif end false for function if in local nil not or repeat return then true until while");
		stc->StartStyling(lenEntered, 0);
		stc->SetStyling(5, 1);
		stc->SetStyling(2, 2);
	}*/

	//stc->SetCaretLineVisibleAlways(true);
}

void MyFrame1::OnKeyDown(wxKeyEvent &evnt)
{
	long key = evnt.GetKeyCode();

	keys[key] = true;

	evnt.Skip();
}

void MyFrame1::OnKeyUp(wxKeyEvent &evnt)
{
	long key = evnt.GetKeyCode();

	keys[key] = false;

	evnt.Skip();
}

bool MyApp::OnInit()
{
	render_loop_on = false;

	frame = new MyFrame1(NULL, -1, _T("Nutcrackz Game Engine"), wxDefaultPosition, wxSize(1280, 720));
	
	frame->Show();

	playerDlg = new AudioPlayerDlg(NULL, -2);

	playerDlg->Hide();

	activateRenderLoop(true);
	return true;
}

void MyApp::activateRenderLoop(bool on)
{
	if (on && !render_loop_on)
	{
		Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle));
		render_loop_on = true;
	}
	else if (!on && render_loop_on)
	{
		Disconnect(wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle));
		render_loop_on = false;
	}
}

void MyApp::onIdle(wxIdleEvent& evt)
{
	const int MAX_PHYSICS_STEPS = 6;
	const float MS_PER_SECOND = 1000.0f;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / 60.0f;
	const float MAX_DELTA_TIME = 1.0f;

	float previousTicks = float(SDL_GetTicks());

	if (render_loop_on)
	{
		float newTicks = float(SDL_GetTicks());
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;

		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		int i = 0;

		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

			frame->setCanvas3DsDeltaTime(deltaTime);

			if (frame->getCanvas3D() != nullptr)
			{
				if (frame->getCanvas3D()->get3DModelSelected() == true)
				{
					frame->getPropertyGrid()->Append(new wxStringProperty("Name", "Name", "<Insert Name>"));

					wxPGProperty* transformProp = frame->getPropertyGrid()->Append(new wxStringProperty("Transform",
						wxPG_LABEL,
						"<composed>"));

					frame->getPropertyGrid()->AppendIn(transformProp, new wxFloatProperty("X:", wxPG_LABEL, 0.0f));

					frame->getPropertyGrid()->AppendIn(transformProp, new wxFloatProperty("Y:", wxPG_LABEL, 0.0f));

					frame->getPropertyGrid()->AppendIn(transformProp, new wxFloatProperty("Z:", wxPG_LABEL, 0.0f));

					transformProp->SetExpanded(false);

					wxPGProperty* rotationProp = frame->getPropertyGrid()->Append(new wxStringProperty("Rotation",
						wxPG_LABEL,
						"<composed>"));

					frame->getPropertyGrid()->AppendIn(rotationProp, new wxFloatProperty("X:", wxPG_LABEL, 0.0f));

					frame->getPropertyGrid()->AppendIn(rotationProp, new wxFloatProperty("Y:", wxPG_LABEL, 0.0f));

					frame->getPropertyGrid()->AppendIn(rotationProp, new wxFloatProperty("Z:", wxPG_LABEL, 0.0f));

					rotationProp->SetExpanded(false);

					wxPGProperty* scaleProp = frame->getPropertyGrid()->Append(new wxStringProperty("Scale",
						wxPG_LABEL,
						"<composed>"));

					frame->getPropertyGrid()->AppendIn(scaleProp, new wxFloatProperty("X:", wxPG_LABEL, 0.0f));

					frame->getPropertyGrid()->AppendIn(scaleProp, new wxFloatProperty("Y:", wxPG_LABEL, 0.0f));

					frame->getPropertyGrid()->AppendIn(scaleProp, new wxFloatProperty("Z:", wxPG_LABEL, 0.0f));

					scaleProp->SetExpanded(false);

					// Add int property
					frame->getPropertyGrid()->Append(new wxIntProperty("IntProperty", wxPG_LABEL, 12345678));
					// Add float property (value type is actually double)
					frame->getPropertyGrid()->Append(new wxFloatProperty("FloatProperty", wxPG_LABEL, 12345.678));
					// Add a bool property
					frame->getPropertyGrid()->Append(new wxBoolProperty("BoolProperty", wxPG_LABEL, false));
					// A string property that can be edited in a separate editor dialog.
					frame->getPropertyGrid()->Append(new wxLongStringProperty("LongStringProperty",
						wxPG_LABEL,
						"This is much longer string than the "
						"first one. Edit it by clicking the button."));
					// String editor with dir selector button.
					frame->getPropertyGrid()->Append(new wxDirProperty("DirProperty", wxPG_LABEL, ::wxGetUserHome()));
					// wxArrayStringProperty embeds a wxArrayString.
					frame->getPropertyGrid()->Append(new wxArrayStringProperty("Label of ArrayStringProperty",
						"NameOfArrayStringProp"));
					// A file selector property.
					frame->getPropertyGrid()->Append(new wxFileProperty("FileProperty", wxPG_LABEL, wxEmptyString));
					// Extra: set wild card for file property (format same as in wxFileDialog).
					frame->getPropertyGrid()->SetPropertyAttribute("FileProperty",
						wxPG_FILE_WILDCARD,
						"All files (*.*)|*.*");
				}
			}

			totalDeltaTime -= deltaTime;

			i++;
		}

		if (frame->wantsToShowAP && !audioPlayerIsShown)
		{
			playerDlg->Show();

			audioPlayerIsShown = true;
		}
		
		if (playerDlg->isClosed && audioPlayerIsShown)
		{
			frame->wantsToShowAP = false;

			playerDlg->Hide();

			audioPlayerIsShown = false;
			playerDlg->isClosed = false;
		}

		if (frame->hasBeenClosed)
		{
			playerDlg->Destroy();
		}

		evt.RequestMore(); // render continuously, not only once on idle
	}
}

IMPLEMENT_APP(MyApp)