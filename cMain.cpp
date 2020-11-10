// TODO
// Implement a save file deletion tool. Maybe done
// through another menu bar option that deletes the
// currently loaded file.
//
// Also implement either some keyword functionality or
// another button to clear out the currently stored data
// in the list box.
//
//

#include "cMain.h"
#include <iostream>
#include <fstream>
#include <ctime>


// Event handling table
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_MENU(10001, cMain::OnMenuOpen)
	EVT_MENU(10002, cMain::OnMenuSaveAs)
	EVT_MENU(10003, cMain::OnMenuExit)
	EVT_BUTTON(10004, OnButtonClicked)
wxEND_EVENT_TABLE()


// Creates the menu bar, button, text box, and list box elements
cMain::cMain() : wxFrame(nullptr, wxID_ANY, "GUI Practice", wxPoint(30,30), wxSize(600,500))
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

	m_btn1 = new wxButton(this, 10004, "Submit", wxPoint(10, 10), wxSize(100, 35));
	m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 70), wxSize(300, 25));
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 110), wxSize(300, 300));
}

// This is something about virtual shit idk
cMain::~cMain()
{
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
// Also appends submitted values to a save file.
void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	using namespace std;

	m_list1->AppendString(m_txt1->GetValue());

	// This adds a timestamp to the save file. Re-enable this
	// later when ready to add that functionality
	//time_t timestamp = time(nullptr);
	//saveFile << ctime(&timestamp) << " " << (m_txt1->GetValue()) << '\n';

	m_txt1->Clear();
	evt.Skip();
}
