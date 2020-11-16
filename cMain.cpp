// TODO
// 1) Make the interface look a little nicer
// 2) Add escape key to clear TxtCtrl


#include "cMain.h"
#include <iostream>
#include <fstream>
#include <ctime>


// Event handling table
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_MENU(10001, cMain::OnMenuOpen)
	EVT_MENU(10002, cMain::OnMenuSaveAs)
	EVT_MENU(10003, cMain::OnMenuExit)
	EVT_BUTTON(10004, cMain::OnButtonClicked)
	EVT_BUTTON(10005, cMain::OnButtonClear)
	EVT_TEXT_ENTER(20001, cMain::OnEnterPressed)
wxEND_EVENT_TABLE()


// Creates the menu bar, button, text box, and list box elements
cMain::cMain() : wxFrame(nullptr, 00001, "GUI Practice", wxPoint(30,30), wxSize(1200,760))
{
	m_MenuBar = new wxMenuBar();
	this->SetMenuBar(m_MenuBar);

	// Menu for file operations
	wxMenu* menuFile = new wxMenu();
	menuFile->Append(10001, "Open");
	menuFile->Append(10002, "Save As");
	menuFile->Append(10003, "Exit");

	// Add File Menu to the Menu Bar
	m_MenuBar->Append(menuFile, "File");

	m_btn1 = new wxButton(this, 10004, "Submit", wxPoint(490, 455), wxSize(100, 55));
	m_btn2 = new wxButton(this, 10005, "Clear", wxPoint(595, 455), wxSize(100, 55));
	m_btn3 = new wxButton(this, 10006, "TBD1", wxPoint(490, 515), wxSize(100, 55));
	m_btn4 = new wxButton(this, 10007, "TBD2", wxPoint(595, 515), wxSize(100, 55));
	m_btn5 = new wxButton(this, 10008, "TBD3", wxPoint(490, 575), wxSize(100, 55));
	m_btn6 = new wxButton(this, 10009, "TBD4", wxPoint(595, 575), wxSize(100, 55));
	m_btn7 = new wxButton(this, 10010, "TBD5", wxPoint(490, 635), wxSize(100, 55));
	m_btn8 = new wxButton(this, 10011, "TBD6", wxPoint(595, 635), wxSize(100, 55));
	// wxTE_PROCESS_ENTER used to accept the enter key with the field focused
	m_txt1 = new wxTextCtrl(this, 20001, "", wxPoint(10, 665), wxSize(475, 25), wxTE_PROCESS_ENTER);
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 455), wxSize(475, 211));
	m_list2 = new wxListBox(this, wxID_ANY, wxPoint(700, 455), wxSize(475, 235));

	m_txt1->SetFocus();
}

// This is something about virtual shit idk
cMain::~cMain()
{
}

void cMain::OnEnterPressed(wxCommandEvent& evt)
{	


	m_list1->AppendString(m_txt1->GetValue());
	m_txt1->Clear();

	// Set to false to stop windows bloop sound
	evt.Skip(false);

	// *** FIX THIS ***
	// The line "m_txt1->SetFocus()" doesn't work. Focus
	// is not being given to that box. Maybe an enter key
	// related issue. 
	//if (m_txt1->HasFocus())
	//{
	//	m_list1->AppendString(m_txt1->GetValue());
	//	m_txt1->Clear();

	//	// Set to false to stop windows bloop sound
	//	evt.Skip(false);
	//}

	//else
	//{
	//	m_txt1->SetFocus();
	//	/*evt.Skip(false);*/
	//}
}

void cMain::OnMenuOpen(wxCommandEvent& evt)
{
	using namespace std;

	// Opens a window asking the user for a text file to open
	wxFileDialog
		openFileDialog(this, _("Open save file"), "", "",
			"TXT files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	// User cancels
	if (openFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	
	// Checks for valid file then saves path as string
	wxFileInputStream input_stream(openFileDialog.GetPath());
	if (!input_stream.IsOk())
	{
		wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
		{
			return;
		}
	}
	string loadFilePath(openFileDialog.GetPath());
	
	// Clears list box
	m_list1->Clear();

	// Loads selected file and writes each line to the list box
	ifstream loadFile(loadFilePath);
	if (loadFile.is_open())
	{
		string line;
		while (getline(loadFile, line))
		{
			m_list1->AppendString(line.c_str());
		}
		loadFile.close();
	}
}

void cMain::OnMenuSaveAs(wxCommandEvent& evt)
{
	using namespace std;

	// Opens a window asking the user what to save file as
	wxFileDialog
		saveFileDialog(this, _("Save file as"), "", "",
			"TXT files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	// User cancels
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}

	// Checks that file can be saved
	wxFileOutputStream output_stream(saveFileDialog.GetPath());
	if (!output_stream.IsOk())
	{
		wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
		return;
	}

	// Gets user designated path and saves as a string and
	// opens up an output stream to save the data
	string saveFilePath(saveFileDialog.GetPath());
	ofstream saveFile(saveFilePath, fstream::app);

	// Stores strings from the list box as an array then saves
	// them one by one to the selected save file
	wxArrayString list = m_list1->GetStrings();
	size_t count = list.Count();
	for (size_t i = 0; i < count; ++i)
	{
		saveFile << list[i] << '\n';
	}

	saveFile.close();
}

void cMain::OnMenuExit(wxCommandEvent& evt)
{
	if (wxMessageBox(wxT("Are you sure you want to close the application? \nAll unsaved progress will be lost."),
		wxT("Confirm"),
		wxICON_QUESTION | wxYES_NO) == wxYES)
	{
		Close();
		evt.Skip();
	}	
}

// Takes value from the text box and adds it to the list box when 
// the "Click Me" box is clicked then clears the text box.
void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	m_list1->AppendString(m_txt1->GetValue());
	m_txt1->Clear();

	evt.Skip();
}

void cMain::OnButtonClear(wxCommandEvent& evt)
{
	m_list1->Clear();
	evt.Skip();
}
