/////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application          //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Remote Code Repository, Project 4               //
//  Author:		   Ronak Bhuptani, SUID#429019830, Syracuse        //
//                 University, rmbhupta@syr.edu                    //
//	Source:		   Jim Fawcett, CSE687 - Object Oriented Design,   //
//				   Spring 2015									   //
/////////////////////////////////////////////////////////////////////
#include "Window.h"
#include <string>
#include <io.h>
#include "..//Utilities/Utilities.h"
using SH = Utilities::StringHelper;
using namespace CppCliWindows;

//WPF Application constructor initalization 
WPFCppCliDemo::WPFCppCliDemo(array<System::String^>^ args)
{
	if (!ParseCommandLineArgs(args))
		return;
	// set up channel
	ObjectFactory* pObjFact = new ObjectFactory;
	pChann_ = pObjFact->createChannel(toStdString(clientAddress), stoi(toStdString(clientPort)));
	pChann_->start();
	delete pObjFact;
	// client's receive thread
	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();
	// set event handlers
	this->Loaded +=
		gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing +=
		gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hUploadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::onUploadButtonClick);
	hBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::onSelectFileButtonClick);
	hGetAllFilesButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::onGetAllFilesButtonClick);
	hGetAllFilesButton_E->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::onGetAllFilesButtonClick);
	hDownoadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::onDownloadButtonClick);
	// set Window properties
	this->Title = "Remote Code Repository";
	this->WindowState = System::Windows::WindowState::Maximized;
	this->WindowStyle = System::Windows::WindowStyle::ThreeDBorderWindow;
	// attach dock panel to Window
	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);
	// setup Window controls and views
	setUpTabControl();
	setUpStatusBar();
	setUpUploadView();
	setUpDownloadView();
}
// WPF Application dsctr
WPFCppCliDemo::~WPFCppCliDemo()
{
	delete pChann_;
}

//set up status bar in GUI
void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "very important messages will appear here";
	//status->FontWeight = FontWeights::Bold;
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

//set up tab control in GUI
void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hUploadTab->Header = "Upload Files";
	hExtractTab->Header = "Extract Files";
	hTabControl->Items->Add(hUploadTab);
	hTabControl->Items->Add(hExtractTab);
}

// Parse command line arguments to ser server and clients address and port
bool CppCliWindows::WPFCppCliDemo::ParseCommandLineArgs(array<System::String^>^ args)
{
	Console::WriteLine(L"\n {0}\n", args->Length);
	if (args->Length == 2)
	{
		clientdetails = args[0];
		serverdetails = args[1];

		array<System::String^>^ server = serverdetails->Split(':');
		array<System::String^>^ client = clientdetails->Split(':');
		if (server->Length == 2)
		{
			serverAddress = server[0];
			serverPort = server[1];
		}
		if (client->Length == 2)
		{
			clientAddress = client[0];
			clientPort = client[1];
		}
		return true;
	}
	else
	{
		Console::WriteLine(L"\n Please give proper command line arguments. For more details check README file.s\n");
		return false;
	}
}

//Conver String^ to std::string
std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}

//Conver std::string to  String^ 
String^ WPFCppCliDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}

// receives message from receive queue and process them
void WPFCppCliDemo::getMessage()
{
	while (true)
	{
		HTTPMessage msg = pChann_->getMessage();
		if (msg.command == RESPONSE_ALL_FILES) {
			std::string allFiles = msg.body;
			std::vector<std::string> allFilesList = SH::split(allFiles, '^');
			if (allFilesList.size() != 0 && allFilesList[0] == "NA") {
				String^ statusMsg = "No *.h or *.cpp found on Server";
				array<Object^>^ parms = gcnew array<Object^> {statusMsg };
				Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
				Dispatcher->Invoke(act1, parms);
				continue;
			}
			array<String^>^ args = gcnew array<String^>(allFilesList.size());
			for (size_t i = 0; i < allFilesList.size(); i++)
				args[i] = toSystemString(allFilesList[i]);
			array<Object^>^ parms = gcnew array<Object^> {args };
			Action< array<String^>^>^ act = gcnew Action<array<String^>^>(this, &WPFCppCliDemo::updateListBox);
			Dispatcher->Invoke(act, parms);
		}
		if (msg.command == ACK_UPLOAD) {
			String^ statusMsg = "File Uploaded Successfully.";
			array<Object^>^ parms = gcnew array<Object^> {statusMsg };
			Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
			Dispatcher->Invoke(act1, parms);
		}
		if (msg.command == ACK_DOWNLOAD) {
			String^ statusMsg = "File Downloaded Successfully to ." + toSystemString(msg.fileName) ;
			array<Object^>^ parms = gcnew array<Object^> {statusMsg };
			Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
			Dispatcher->Invoke(act1, parms);
		}
	}
}

//updates list box on GUI
void WPFCppCliDemo::updateListBox(array<String^>^ args)
{
	hallFilesList->Items->Clear();
	hallFilesList_E->Items->Clear();
	for each(String^ arg in args) {
		hallFilesList_E->Items->Add(arg);
		hallFilesList->Items->Add(arg);
	}
	hStatus->Text = "File list updated.";
}

// update status bar on GUi
void CppCliWindows::WPFCppCliDemo::updateStatusbar(String ^ args)
{
	hStatus->Text = args;
}


// clear status bar on GUI
void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";

}

// set up download view on GUI
void CppCliWindows::WPFCppCliDemo::setUpDownloadView1() {
	hExtractGrid->Margin = Thickness(20);
	hExtractGrid->Width = 750;
	hExtractTab->Content = hExtractGrid;
	RowDefinition^ r1 = gcnew RowDefinition();
	r1->Height = GridLength(75);
	hExtractGrid->RowDefinitions->Add(r1);
	Border^ hBorder2 = gcnew Border();
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	downloadLabel->Content = "Get All files list by clicking on Get All Files Button and Check below checkbox to download files with dependencies. \nYou can select multiple files by clicking multiple Items.";
	downloadLabel->Content += "\nNote :- To check Download with dependencies, try to download \"\\Server_Repository\\Test\" file as I have already \nset two level dependencies for this file. You can also create your own test data and check.";
	downloadLabel->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	downloadLabel->FontWeight = FontWeights::Bold;
	downloadLabel->FontSize = 12;
	downloadLabel->Height = 70;
	downloadLabel->Width = 800;
	hBorder2->Child = downloadLabel;
	hExtractGrid->SetRow(hBorder2, 0);
	hExtractGrid->Children->Add(hBorder2);
}
// set up download view on GUI
void CppCliWindows::WPFCppCliDemo::setUpDownloadView2() {
	RowDefinition^ r2 = gcnew RowDefinition();
	r2->Height = GridLength(75);
	hExtractGrid->RowDefinitions->Add(r2);
	hGetAllFilesButton_E->Content = "Get All Files List";
	hGetAllFilesButton_E->Height = 30;
	hGetAllFilesButton_E->Width = 120;
	hGetAllFilesButton_E->BorderThickness = Thickness(2);
	hGetAllFilesButton_E->BorderBrush = Brushes::Black;
	hExtractGrid->SetRow(hGetAllFilesButton_E, 1);
	hExtractGrid->Children->Add(hGetAllFilesButton_E);
}
// set up download view on GUI
void CppCliWindows::WPFCppCliDemo::setUpDownloadView3() {
	RowDefinition^ r3 = gcnew RowDefinition();
	r3->Height = GridLength(350);
	hExtractGrid->RowDefinitions->Add(r3);
	Border^ hBorder3 = gcnew Border();
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hallFilesList_E->SelectionMode = SelectionMode::Multiple;
	hBorder3->Child = hallFilesList_E;
	hExtractGrid->SetRow(hBorder3, 2);
	hExtractGrid->Children->Add(hBorder3);
}
// set up download view on GUI
void CppCliWindows::WPFCppCliDemo::setUpDownloadView4() {
	RowDefinition^ r4 = gcnew RowDefinition();
	r4->Height = GridLength(75);
	hExtractGrid->RowDefinitions->Add(r4);
	Grid^ innerGrid = gcnew Grid();
	hExtractGrid->SetRow(innerGrid, 3);
	RowDefinition^ ri1 = gcnew RowDefinition();
	ColumnDefinition^ c1 = gcnew ColumnDefinition();
	ColumnDefinition^ c2 = gcnew ColumnDefinition();
	ColumnDefinition^ c3 = gcnew ColumnDefinition();
	ri1->Height = GridLength(75);
	innerGrid->RowDefinitions->Add(ri1);
	c1->Width = GridLength(50);
	c2->Width = GridLength(300);
	c2->Width = GridLength(400);
	innerGrid->ColumnDefinitions->Add(c1);
	innerGrid->ColumnDefinitions->Add(c2);
	innerGrid->ColumnDefinitions->Add(c3);
	withDepLabel->Content = "With Dependencies?";
	withDepLabel->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	withDepLabel->FontWeight = FontWeights::Bold;
	withDepLabel->FontSize = 14;
	withDepLabel->Height = 40;
	withDepLabel->Width = 290;
	withDepLabel->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	withDep->Width = 30;
	withDep->Height = 30;
	withDep->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	hDownoadButton->Content = "Download";
	hDownoadButton->Height = 30;
	hDownoadButton->Width = 120;
	hDownoadButton->BorderThickness = Thickness(2);
	hDownoadButton->BorderBrush = Brushes::Black;
	innerGrid->SetRow(withDep, 0);
	innerGrid->SetRow(withDepLabel, 0);
	innerGrid->SetRow(hDownoadButton, 0);
	innerGrid->SetColumn(withDep, 0);
	innerGrid->SetColumn(withDepLabel, 1);
	innerGrid->SetColumn(hDownoadButton, 2);
	innerGrid->Children->Add(withDep);
	innerGrid->Children->Add(withDepLabel);
	innerGrid->Children->Add(hDownoadButton);
	hExtractGrid->Children->Add(innerGrid);
}


// set up download view on GUI
void CppCliWindows::WPFCppCliDemo::setUpDownloadView()
{
	Console::Write("\n  setting up Extract view");
	setUpDownloadView1();
	setUpDownloadView2();
	setUpDownloadView3();
	setUpDownloadView4();
}

//set up upload view on GUI
void CppCliWindows::WPFCppCliDemo::setUpUploadView()
{
	Console::Write("\n  setting up Upload view");
	setUpUploadView1();
	setUpUploadView2();
	setUpUploadView3();
}

//set up upload view on GUI
void WPFCppCliDemo::setUpUploadView1() {
	hUploadGrid->Margin = Thickness(20);
	hUploadGrid->Width = 750;
	hUploadTab->Content = hUploadGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hUploadGrid->RowDefinitions->Add(hRow1Def);
	hRow1Def->Height = GridLength(75);
	ColumnDefinition^ c1 = gcnew ColumnDefinition();
	ColumnDefinition^ c2 = gcnew ColumnDefinition();
	ColumnDefinition^ c3 = gcnew ColumnDefinition();
	hUploadGrid->ColumnDefinitions->Add(c1);
	hUploadGrid->ColumnDefinitions->Add(c2);
	hUploadGrid->ColumnDefinitions->Add(c3);
	uploadFile->Text = "Please Select File";
	uploadFile->Height = 20;
	uploadFile->Width = 480;
	uploadFile->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	uploadFile->FontSize = 12;
	hBrowseButton->Content = "Select File";
	hBrowseButton->Height = 30;
	hBrowseButton->Width = 120;
	hBrowseButton->BorderThickness = Thickness(2);
	hBrowseButton->BorderBrush = Brushes::Black;
	hGetAllFilesButton->Content = "Get All Files List";
	hGetAllFilesButton->Height = 30;
	hGetAllFilesButton->Width = 120;
	hGetAllFilesButton->BorderThickness = Thickness(2);
	hGetAllFilesButton->BorderBrush = Brushes::Black;
	hUploadGrid->SetRow(uploadFile, 0);
	hUploadGrid->SetRow(hBrowseButton, 0);
	hUploadGrid->SetRow(hGetAllFilesButton, 0);
}
//set up upload view on GUI
void WPFCppCliDemo::setUpUploadView2() {
	ColumnDefinition^ c1 = gcnew ColumnDefinition();
	ColumnDefinition^ c2 = gcnew ColumnDefinition();
	ColumnDefinition^ c3 = gcnew ColumnDefinition();
	c1->Width = GridLength(2 * hUploadGrid->Width / 3);
	c2->Width = GridLength(hUploadGrid->Width / 6);
	c3->Width = GridLength(hUploadGrid->Width / 6);
	hUploadGrid->ColumnDefinitions->RemoveAt(2);
	hUploadGrid->ColumnDefinitions->RemoveAt(1);
	hUploadGrid->ColumnDefinitions->RemoveAt(0);
	hUploadGrid->ColumnDefinitions->Add(c1);
	hUploadGrid->ColumnDefinitions->Add(c2);
	hUploadGrid->ColumnDefinitions->Add(c3);
	hUploadGrid->SetColumn(uploadFile, 0);
	hUploadGrid->SetColumn(hBrowseButton, 1);
	hUploadGrid->SetColumn(hGetAllFilesButton, 2);
	hUploadGrid->Children->Add(uploadFile);
	hUploadGrid->Children->Add(hBrowseButton);
	hUploadGrid->Children->Add(hGetAllFilesButton);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(300);
	hUploadGrid->RowDefinitions->Add(hRow2Def);
	Border^ hBorder2 = gcnew Border();
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hallFilesList->SelectionMode = SelectionMode::Multiple;
	hBorder2->Child = hallFilesList;
	hUploadGrid->SetRow(hBorder2, 1);
	c1->Width = GridLength(hUploadGrid->Width / 2);
	hUploadGrid->SetColumnSpan(hBorder2, 2);
	hUploadGrid->SetColumn(hBorder2, 0);
	hUploadGrid->Children->Add(hBorder2);
}
//set up upload view on GUI
void WPFCppCliDemo::setUpUploadView3() {
	ColumnDefinition^ c1 = gcnew ColumnDefinition();
	ColumnDefinition^ c2 = gcnew ColumnDefinition();
	ColumnDefinition^ c3 = gcnew ColumnDefinition();
	c1->Width = GridLength(2 * hUploadGrid->Width / 3);
	c2->Width = GridLength(hUploadGrid->Width / 6);
	c3->Width = GridLength(hUploadGrid->Width / 6);
	hUploadGrid->ColumnDefinitions->RemoveAt(2);
	hUploadGrid->ColumnDefinitions->RemoveAt(1);
	hUploadGrid->ColumnDefinitions->RemoveAt(0);
	hUploadGrid->ColumnDefinitions->Add(c1);
	hUploadGrid->ColumnDefinitions->Add(c2);
	hUploadGrid->ColumnDefinitions->Add(c3);
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(75);
	hUploadGrid->RowDefinitions->Add(hRow3Def);
	checkInLabel->Content = "Open ?";
	checkInLabel->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	checkInLabel->FontWeight = FontWeights::Bold;
	checkInLabel->FontSize = 16;
	checkInLabel->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	checkInLabel->Height = 40;
	checkInLabel->Width = 100;
	checkinStatus->Width = 30;
	checkinStatus->Height = 30;
	checkinStatus->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hUploadButton->Content = "Upload";
	hUploadButton->Height = 30;
	hUploadButton->Width = 120;
	hUploadButton->BorderThickness = Thickness(2);
	hUploadButton->BorderBrush = Brushes::Black;
	hUploadGrid->SetRow(checkinStatus, 2);
	hUploadGrid->SetRow(checkInLabel, 2);
	hUploadGrid->SetRow(hUploadButton, 2);
	hUploadGrid->SetColumn(checkinStatus, 2);
	hUploadGrid->SetColumn(checkInLabel, 1);
	hUploadGrid->SetColumn(hUploadButton, 0);
	hUploadGrid->Children->Add(checkinStatus);
	hUploadGrid->Children->Add(checkInLabel);
	hUploadGrid->Children->Add(hUploadButton);
}


//onloaded method
void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}

//onloading method
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}

//this function is called when upload is clicked and it sends upload message to server
void CppCliWindows::WPFCppCliDemo::onUploadButtonClick(Object ^ sender, RoutedEventArgs ^ args)
{
	std::string fileName = toStdString(uploadFile->Text);
	std::string depFiles = "NA";
	int count = hallFilesList->SelectedItems->Count;
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		depFiles = "";
		for each (String^ item in hallFilesList->SelectedItems)
			depFiles += toStdString(item) + "^";
	}
	depFiles.pop_back();
	HTTPMessage msg;
	msg.open = "false";
	if (checkinStatus->IsChecked)
		msg.open = "true";
	msg.toAddr = toStdString(serverAddress);
	msg.toPort = stoi(toStdString(serverPort));
	msg.body = depFiles;
	msg.author = "Ronak";
	msg.contentLen = msg.body.size();
	msg.fileName = toStdString(uploadFile->Text);
	msg.command = UPLOAD_REQUEST;
	pChann_->postMessage(msg);
	String^ statusMsg = "Upload \"" + toSystemString(msg.fileName) + "\" Request has been sent.";
	array<Object^>^ parms = gcnew array<Object^> {statusMsg };
	Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
	Dispatcher->Invoke(act1, parms);
}
//this function is called when browse is clicked and opens file dialogue box
void CppCliWindows::WPFCppCliDemo::onSelectFileButtonClick(Object ^ sender, RoutedEventArgs ^ args)
{
	Forms::OpenFileDialog^ openFileDialog1 = gcnew Forms::OpenFileDialog();

	openFileDialog1->Filter = "Header Files (*.h)|*.h|Source Files (*.cpp)|*.cpp";
	openFileDialog1->FilterIndex = 2;
	openFileDialog1->RestoreDirectory = true;

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		uploadFile->Text = openFileDialog1->FileName;
		Console::Write("\n  Selected File : {0}", openFileDialog1->FileName);
	}
}

// this function is called when GetAllFiles button is clicked and it sends GETALLFILES_REQUEST to server
void CppCliWindows::WPFCppCliDemo::onGetAllFilesButtonClick(Object ^ sender, RoutedEventArgs ^ args)
{
	HTTPMessage msg;
	msg.toAddr = toStdString(serverAddress);
	msg.toPort = stoi(toStdString(serverPort));
	msg.body = "Get Files"; // update
	msg.author = "Ronak";
	msg.contentLen = msg.body.size();
	msg.fileName = "NA";
	msg.command = GETALLFILES_REQUEST;
	pChann_->postMessage(msg);
	String^ statusMsg = "Get All Files List Request has been sent.";
	array<Object^>^ parms = gcnew array<Object^> {statusMsg };
	Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
	Dispatcher->Invoke(act1, parms);
}

// this function is called when download button is clicked and it sends Download Request to server
void CppCliWindows::WPFCppCliDemo::onDownloadButtonClick(Object ^ sender, RoutedEventArgs ^ args)
{
	int count = hallFilesList_E->SelectedItems->Count;
	std::string withDepstr = "false";
	if (withDep->IsChecked)
		withDepstr = "true";

	HTTPMessage msg;
	msg.toAddr = toStdString(serverAddress);
	msg.toPort = stoi(toStdString(serverPort));
	msg.body = withDepstr;
	msg.author = "Ronak";
	msg.contentLen = msg.body.size();
	if (count > 0) {
		for each (String^ item in hallFilesList_E->SelectedItems) {
			msg.fileName = toStdString(item);
			msg.command = DOWNLOAD_REQUEST;
			pChann_->postMessage(msg);
		}
		String^ statusMsg = "Download Request has been sent.";
		array<Object^>^ parms = gcnew array<Object^> {statusMsg };
		Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
		Dispatcher->Invoke(act1, parms);
	}
	else {
		String^ statusMsg = "No file selected to download.";
		array<Object^>^ parms = gcnew array<Object^> {statusMsg };
		Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
		Dispatcher->Invoke(act1, parms);
	}
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::Title = "WPF Client Console";
	Console::WriteLine(L"\n Starting WPFCppCliDemo");

	Application^ app = gcnew Application();
	app->Run(gcnew WPFCppCliDemo(args));
	Console::WriteLine(L"\n\n");
}