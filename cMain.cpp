// TODO
// 0) Get networking working on a specified IP Address
// 1) Start creating a list of commands that can be interpreted by the server.
// 2) Implement shop with strong items for unlock
// 3) Figure out currency (gp, wins, etc.)
// 4) Maybe use treasure trails as a way to get gp
// 5) Fix error messages to display in wxMessageBox


// ***** NETWORKING INCLUDES AND DEFINITIONS *****
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


// ***** wxWidgets INCLUDES AND DEFINITIONS *****
#include "cMain.h"
#include <iostream>
#include <fstream>


// ***** OTHER INCLUDES AND DEFINITIONS *****
#include "Commands.h"


// Event handling table
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_MENU(10001, cMain::OnMenuOpen)
EVT_MENU(10002, cMain::OnMenuSaveAs)
EVT_MENU(10003, cMain::OnMenuExit)
EVT_BUTTON(10008, cMain::OnButtonCommands)
EVT_BUTTON(10010, cMain::OnButtonSubmit)
EVT_BUTTON(10011, cMain::OnButtonClear)
EVT_TEXT_ENTER(20001, cMain::OnEnterPressed)
EVT_KEY_DOWN(cMain::OnEnterFocus)
wxEND_EVENT_TABLE()


// Creates the menu bar, button, text box, and list box elements
cMain::cMain() : wxFrame(nullptr, 00001, "Duelist", wxPoint(30, 30), wxSize(1200, 760))
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

	m_btn1 = new wxButton(this, 10004, "Inventory", wxPoint(490, 455), wxSize(100, 55));
	m_btn2 = new wxButton(this, 10005, "Shop", wxPoint(595, 455), wxSize(100, 55));
	m_btn3 = new wxButton(this, 10006, "Make GP", wxPoint(490, 515), wxSize(100, 55));
	m_btn4 = new wxButton(this, 10007, "Resign", wxPoint(595, 515), wxSize(100, 55));
	m_btn5 = new wxButton(this, 10008, "Command List", wxPoint(490, 575), wxSize(100, 55));
	m_btn6 = new wxButton(this, 10009, "Settings", wxPoint(595, 575), wxSize(100, 55));
	m_btn7 = new wxButton(this, 10010, "Submit", wxPoint(490, 635), wxSize(100, 55));
	m_btn8 = new wxButton(this, 10011, "Clear", wxPoint(595, 635), wxSize(100, 55));
	// wxTE_PROCESS_ENTER used to accept the enter key with the field focused
	m_txt1 = new wxTextCtrl(this, 20001, "", wxPoint(10, 665), wxSize(475, 25), wxTE_PROCESS_ENTER);
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 455), wxSize(475, 211));
	m_list2 = new wxListBox(this, wxID_ANY, wxPoint(700, 455), wxSize(475, 235));

	m_txt1->SetFocus();
}


// ************** BEGIN NETWORK INCORPORATION **************
std::string networkTest(std::string userInput)
{
	std::string serverOutput = "Error!";
	std::string ipAddress = "127.0.0.1";			// IP Adress of the server
	int port = 54000;									// Listening port # on the server

	// Initialize winsock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start winsock, Err #" << wsResult << '\n';
		return serverOutput;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << '\n';
		WSACleanup();
		return serverOutput;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		//std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << '\n';
		wxMessageBox("Can't connect to server, Err #" + WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return serverOutput;
	}

	// Do-while loop to send and receive data
	char buf[4096];
	do
	{
		// Prompt the user for some text
		if (userInput.size() > 0)
		{
			// Send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					std::string response = std::string(buf, 0, bytesReceived);

					if (response == "NULL")
					{
						serverOutput = "NULL";
					}
					else
					{
						serverOutput = "<SERVER> " + response + '\n';
					}
				}
				else
				{
					break;
				}
			}
		}
		userInput = "test";
	} while (userInput != "test");

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();

	return serverOutput;
}
// ************** ENDOF NETWORK INCORPORATION **************


// This is something about virtual shit idk
cMain::~cMain()
{
}

// Resets focus to the text entry field on pressing the Enter key
void cMain::OnEnterFocus(wxKeyEvent& evt)
{
	if (evt.GetKeyCode() == WXK_RETURN)
	{
		m_txt1->SetFocus();

		evt.Skip(false);
	}
}

void cMain::OnEnterPressed(wxCommandEvent& evt)
{
	wxString userTextCtrl = m_txt1->GetValue();
	m_list1->AppendString(userTextCtrl);

	// Converts wxString from textctrl to a std::string so that the
	// networkTest() function can use it
	std::string serverInput = std::string(userTextCtrl.mb_str());

	std::string serverOutput = networkTest(serverInput);

	// Converts the output from the networkTest() function back
	// to a wxString
	wxString serverString(serverOutput);
	
	// Checks if server needs to write anything and does if it needs to
	if (serverString == "NULL")
	{
		m_txt1->Clear();
	}
	else 
	{
		m_list2->AppendString(serverString);
		m_txt1->Clear();
	}


	evt.Skip(false);
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

void cMain::OnButtonCommands(wxCommandEvent& evt)
{
	m_list1->AppendString("The available commands are: ");
	for (int i = 0; i < numCommands; ++i)
	{
		m_list1->AppendString("     !" +wxString(commands[i]) + '\n');
	}
	
	m_txt1->SetFocus();
	evt.Skip();
}

// Takes value from the text box and adds it to the list box when 
// the submit button is clicked then clears the text box.
void cMain::OnButtonSubmit(wxCommandEvent& evt)
{
	m_list1->AppendString(m_txt1->GetValue());
	m_txt1->Clear();
	m_txt1->SetFocus();

	evt.Skip();
}

void cMain::OnButtonClear(wxCommandEvent& evt)
{
	m_list1->Clear();
	m_txt1->SetFocus();

	evt.Skip();
}
