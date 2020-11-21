// TODO
// 1) Implement a stats info panel above the chatboxes. Could include inventory.
// 2) Implement a shop
// 3) Maybe use treasure trails as a way to get gp
// 4) Fix error messages to display in wxMessageBox
// 5) Build into standalone application (this is fucking hard. Failed so many times.)


// ***** NETWORKING INCLUDES AND DEFINITIONS *****
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


// ***** OTHER INCLUDES AND DEFINITIONS *****
#include "cMain.h"
#include <iostream>
#include <fstream>
#include "Commands.h"
#include "Info.h"
#include "Accounts.h"


// Event handling table
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_MENU(10001, cMain::OnMenuCreate)
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
	menuFile->Append(10001, "Create");
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
	m_txt1 = new wxTextCtrl(this, 20001, "", wxPoint(10, 665), wxSize(475, 25), wxTE_PROCESS_ENTER);
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 455), wxSize(475, 211));
	m_list2 = new wxListBox(this, wxID_ANY, wxPoint(700, 455), wxSize(475, 235));

	m_txt1->SetFocus();
}


// ************** BEGIN NETWORK INCORPORATION **************
std::string networkTest(std::string userInput)
{
	std::string serverOutput = "Error!";
	//std::string ipAddress = "127.0.0.1";				// Localhost IP
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


cMain::~cMain()
{
	// This is something about virtual shit idk
}

void cMain::OnEnterFocus(wxKeyEvent& evt)
{
	// Resets focus to the text entry field on pressing the Enter key

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


	// THIS IS WHERE YOU LEFT OFF ***********************************************
	// It loads the file but you just need the wins and losses
	// Could maybe just do this all in the Accounts.h file.
	if (userTextCtrl == wxString("!record"))
	{
		std::ifstream loadFile("SaveFile.txt");
		std::string loadString;
		std::getline(loadFile, loadString);
		loadFile.close();

		m_list1->AppendString(loadString);
	}

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

void cMain::OnMenuCreate(wxCommandEvent& evt)
{
	// Allows user to create a character by entering a name in a window
	// and saving a file with default data written to it.

	using namespace std;

	wxTextEntryDialog dlg(this, "Enter a name for your character.");
	if (dlg.ShowModal() == wxID_OK)
	{
		wxMessageBox("Your account is named: " + dlg.GetValue());

		string accountName = string(dlg.GetValue().mb_str());
		
		Account account;
		account.setAllInfo(accountName, 0, 0, 10000, 0);

		string saveFilePath = "SaveFile.txt";
		ofstream saveFile(saveFilePath, fstream::app);

		saveFile << account.a_name << ',' << account.a_wins << ',' << account.a_losses << ','
			<< account.a_gp << ',' << account.a_clues << '\n';

		saveFile.close();
	}

	evt.Skip();
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

void cMain::OnButtonSubmit(wxCommandEvent& evt)
{
	// Takes value from the text box and adds it to the list box when 
	// the submit button is clicked then clears the text box.

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
