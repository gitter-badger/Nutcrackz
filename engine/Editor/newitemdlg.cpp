#include "newitemdlg.h"

NewItemDlg::NewItemDlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);

	m_listCtrl1 = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ALIGN_LEFT | wxLC_ICON | wxLC_SINGLE_SEL | wxLC_SMALL_ICON | wxLC_VRULES);
	m_mgr.AddPane(m_listCtrl1, wxAuiPaneInfo().Top().CaptionVisible(false).CloseButton(false).Movable(false).Dock().Fixed().Floatable(false).Row(0).Position(0).BestSize(wxSize(300, 70)).Layer(0));

	wxImageList *m_pImageList = new wxImageList(16, 16);
	wxIcon icon;
	icon.LoadFile(wxT("res/file_lua.ico"), wxBITMAP_TYPE_ICO);
	m_pImageList->Add(icon);

	wxIcon icon2;
	icon2.LoadFile(wxT("res/file_2ds.ico"), wxBITMAP_TYPE_ICO);
	m_pImageList->Add(icon2);

	wxIcon icon3;
	icon3.LoadFile(wxT("res/file_3ds.ico"), wxBITMAP_TYPE_ICO);
	m_pImageList->Add(icon3);
	/*wxIcon icon2;
	icon.LoadFile(wxT("res/user_offline.ico"), wxBITMAP_TYPE_ICO);
	m_pImageList->Add(icon2);
	wxIcon icon3;
	icon.LoadFile(wxT("res/user_offline.ico"), wxBITMAP_TYPE_ICO);
	m_pImageList->Add(icon3);*/
	m_listCtrl1->SetImageList(m_pImageList, wxIMAGE_LIST_SMALL);

	// Add first column       
	wxListItem col0;
	col0.SetId(0);
	col0.SetText(_("Foo"));
	col0.SetWidth(50);
	m_listCtrl1->InsertColumn(0, col0);

	//This should reflect your data
	m_listCtrl1->SetItemCount(3);

	long item0 = m_listCtrl1->InsertItem(0, "Lua Script"); //want this for col. 1
	m_listCtrl1->SetItemImage(item0, 0);

	long item1 = m_listCtrl1->InsertItem(1, "2D Scene"); //want this for col. 1
	m_listCtrl1->SetItemImage(item1, 1);

	long item2 = m_listCtrl1->InsertItem(2, "3D Scene"); //want this for col. 1
	m_listCtrl1->SetItemImage(item2, 2);

	m_button1 = new wxButton(this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxSize(-1, 30), 0);
	m_mgr.AddPane(m_button1, wxAuiPaneInfo().Right().CaptionVisible(false).CloseButton(false).PaneBorder(false).Movable(false).Dock().Fixed().Row(1).Position(1).BestSize(wxSize(-1, 30)));

	m_button11 = new wxButton(this, wxID_ANY, wxT("&Create"), wxDefaultPosition, wxSize(-1, 30), 0);
	m_mgr.AddPane(m_button11, wxAuiPaneInfo().Left().CaptionVisible(false).CloseButton(false).PaneBorder(false).Movable(false).Dock().Fixed().Position(1).BestSize(wxSize(-1, 30)));



	m_mgr.Update();
	this->Centre(wxBOTH);

	// Connect Events
	m_listCtrl1->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(NewItemDlg::SelectedItem), NULL, this);
	m_button1->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NewItemDlg::OnCancel), NULL, this);
	m_button11->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NewItemDlg::OnCreate), NULL, this);
}

NewItemDlg::~NewItemDlg()
{
	m_mgr.UnInit();

}

void NewItemDlg::SelectedItem(wxListEvent& event)
{
	long item = -1;
	for (;; )
	{
		item = m_listCtrl1->GetNextItem(item,
			wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED);
		if (item == -1)
			break;
		// this item is selected - do whatever is needed with it
		//wxLogMessage("Item %ld is selected.", item);
		textInfo = m_listCtrl1->GetItemText(item, 0);
	}

	event.Skip();
}

void NewItemDlg::OnCancel(wxCommandEvent& event)
{
	this->EndModal(wxID_CANCEL);

	event.Skip();
}

void NewItemDlg::OnCreate(wxCommandEvent& event)
{
	if (textInfo == "Lua Script")
	{
		is2DSceneSelected = false;
		is3DSceneSelected = false;
		isScriptSelected = true;
	}
	else if (textInfo == "2D Scene")
	{
		isScriptSelected = false;
		is3DSceneSelected = false;
		is2DSceneSelected = true;
	}
	else if (textInfo == "3D Scene")
	{
		isScriptSelected = false;
		is2DSceneSelected = false;
		is3DSceneSelected = true;
	}

	this->EndModal(wxID_OK);

	event.Skip();
}