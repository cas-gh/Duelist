#pragma once
#include "wx/wx.h"
#include <wx/wfstream.h>


class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

private:
	wxMenuBar* m_MenuBar = nullptr;

	void OnMenuOpen(wxCommandEvent& evt);
	void OnMenuSaveAs(wxCommandEvent& evt);
	void OnMenuExit(wxCommandEvent& evt);
	void OnButtonClear(wxCommandEvent& evt);

public:
	wxButton* m_btn1 = nullptr;
	wxButton* m_btn2 = nullptr;
	wxTextCtrl* m_txt1 = nullptr;
	wxListBox* m_list1 = nullptr;

	void OnButtonClicked(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};
