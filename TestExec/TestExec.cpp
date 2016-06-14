/////////////////////////////////////////////////////////////////////
//  TestExec -	   To show all the project requirements are met    //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Remote Code Repository, Project 4               //
//  Author:		   Ronak Bhuptani, SUID#429019830, Syracuse        //
//                 University, rmbhupta@syr.edu                    //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a TestExec source code.  It is to show how and where all the project requiremtns are met.

Build Process:
==============
Required Packages
- Utiliies
Build commands (either one)
- devenv Remote_Code_Repository.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 03 May 2016
- first release

*/

#include "..//Utilities/Utilities.h"
#include <iostream>
#include <windows.h>
#include <string>
using SH = Utilities::StringHelper;

void showRequirements() {
	SH::title("Demonstrating Requirement 3 ");
	std::cout << std::endl;
	std::cout << std::endl << " To Test this requirement you can upload and download files using either Console Client or WPF CLient. I have already uploaded test data in Console Client. If you want to test your own data, you can change it in client's main function." 
		<< " You can also test it using GUI by checking Upload and Exctract Tab. You can also check in Server_Repository that metada is created for each file after Upload request from client."  <<std::endl;
	SH::title("Demonstrating Requirement 5 ");
	std::cout << std::endl;
	std::cout << std::endl << " You can test this requirement by sending an UPLOAD request from Client which will be processed on server's recevier side and a new folder with date stamp will be created in Server_Repository's respected file folder." << std::endl;
	SH::title("Demonstrating Requirement 6 ");
	std::cout << std::endl;
	std::cout << std::endl << " You can test this requirement by checking metadata of each file after uploading it through GUI with checking/unchecking Open? check box. based on the check box value, metadata file will be updated." << std::endl;
	SH::title("Demonstrating Requirement 7 (+ bonus) ");
	std::cout << std::endl;
	std::cout << std::endl << " You can test this requirement with WPF Client. To get all files with dependencies, check \"With Dependencies?\" check box, select file and then download. For test purpose, I have created XML file for \"Server_Repository\\Test\\Test.txt\" which has multi level dependencies. You can download that or test by creating your own data." 
		<<"\n You can check MetadataHandler::getAllDepFiles function on line #71 of MetadataHandler.cpp file to understand the code." <<std::endl;
	SH::title("Demonstrating Requirement 8 ");
	std::cout << std::endl;
	std::cout << std::endl << " You can see that We have three different processes (Server, Console Client, WPF Client) running currently which are communicating with each other. " << std::endl;
	SH::title("Demonstrating Requirement 9 ");
	std::cout << std::endl;
	std::cout << std::endl << " This project supports passing of HTTPMEssage using HTTPMessage class for asynchronous one-way messaging which can be verified in Sender's SendMessage function on line #17." << std::endl;
	SH::title("Demonstrating Requirement 10 ");
	std::cout << std::endl;
	std::cout << std::endl << " This project supports sending and receiving streams of bytes. You can check this functionality by checking Sender's Send File function on line #25 and Receiver's getFile function on line #62." << std::endl;
	SH::title("Demonstrating Requirement 11 ");
	std::cout << std::endl;
	std::cout << std::endl << " You cna test this requirement by seeing that all the process has started automatically without any kind of user interaction." << std::endl;
}
#ifdef TEST_EXEC

int main() {
	SetConsoleTitle("Test Executive");
	SH::title(" Starting Test Executive ");
	std::cout << std::endl;
	showRequirements();
	return 0;
}

#endif