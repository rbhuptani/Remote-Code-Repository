#ifndef WINDOW_H
#define WINDOW_H

/////////////////////////////////////////////////////////////////////
// Window.h - C++\CLI implementation of WPF Application            //\
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Remote Code Repository, Project 4               //
//  Author:		   Ronak Bhuptani, SUID#429019830, Syracuse        //
//                 University, rmbhupta@syr.edu                    //
//	Source:		   Jim Fawcett, CSE687 - Object Oriented Design,   //
//				   Spring 2015									   //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
The WFP application can post a message to the Channel's client's sending queue. It hosts
a receive thread that reads the receive queue and dispatches any message
to update GUI of WPF application.

Public Interface:
=================
WPFCppCliDemo() - Class constructor, to initialize GUI

Build Process:
==============
Required Packages
- Client, BlockingQueue, Utilities, HTTPMessage
Build commands (either one)
- devenv Remote_Code_Repository.sln /rebuild debug

Maintenance History:
====================
ver 4.0 : 03 May 2016
- final version
ver 3.0 : 22 Apr 2016
- added support for multi selection of Listbox items.  Implemented by
	Saurabh Patel.  Thanks Saurabh.
ver 2.0 : 15 Apr 2015
- completed message passing demo with moch channel
- added FileBrowserDialog to show files in a selected directory
ver 1.0 : 13 Apr 2015
- incomplete demo with GUI but not connected to mock channel

*/


using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#include <iostream>
namespace CppCliWindows
{
	ref class WPFCppCliDemo : Window
	{
		// MockChannel references
		IChannel* pChann_;
		// Controls for Window
		DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();
		TabItem^ hUploadTab = gcnew TabItem();  //for upload
		TabItem^ hExtractTab = gcnew TabItem(); // for extract
		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();
		//controls for upload tab
		Grid^ hUploadGrid = gcnew Grid();
		TextBox^ uploadFile = gcnew TextBox(); // to store the path of upload file
		Button^ hBrowseButton = gcnew Button();
		Button^ hGetAllFilesButton = gcnew Button();
		Button^ hUploadButton = gcnew Button();
		ListBox^ hallFilesList = gcnew ListBox();
		Grid^ hGrid2 = gcnew Grid();
		Label^ checkInLabel = gcnew Label();
		CheckBox^ checkinStatus = gcnew CheckBox();
		//controls for extraction tab
		Grid^ hExtractGrid = gcnew Grid();
		Button^ hGetAllFilesButton_E = gcnew Button();
		Button^ hDownoadButton = gcnew Button();
		ListBox^ hallFilesList_E = gcnew ListBox();
		CheckBox^ withDep = gcnew CheckBox();
		Label^ downloadLabel = gcnew Label();
		Label^ withDepLabel = gcnew Label();
		// receive thread
		Thread^ recvThread;
		// server and client related details.
		String^ clientdetails;
		String^ serverdetails;
		String^ serverAddress;
		String^ serverPort;
		String^ clientAddress;
		String^ clientPort;
	public:
		WPFCppCliDemo(array<System::String^>^ args);
		~WPFCppCliDemo();
		//initialize GUI
		void setUpStatusBar();
		void setUpTabControl();
		void setUpUploadView();
		void setUpDownloadView();
		void getMessage();
		void updateListBox(array<String^>^ args);
		void updateStatusbar(String^ args);
		void clear(Object^ sender, RoutedEventArgs^ args);
		void setUpUploadView1();
		void setUpUploadView2();
		void setUpUploadView3();
		void setUpDownloadView1();
		void setUpDownloadView2();
		void setUpDownloadView3();
		void setUpDownloadView4();
		void OnLoaded(Object^ sender, RoutedEventArgs^ args);
		void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
		//click methods
		void onUploadButtonClick(Object^ sender, RoutedEventArgs^ args);
		void onSelectFileButtonClick(Object^ sender, RoutedEventArgs^ args);
		void onGetAllFilesButtonClick(Object^ sender, RoutedEventArgs^ args);
		void onDownloadButtonClick(Object^ sender, RoutedEventArgs^ args);
	private:
		std::string toStdString(String^ pStr);
		String^ toSystemString(std::string& str);
		bool ParseCommandLineArgs(array<System::String^>^ args);
	};
}


#endif
