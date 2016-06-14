#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H

/////////////////////////////////////////////////////////////////////
//  hTTPMessage.h - Creates HTTP style messages to communicate     //
//					between client and server					   //	
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
This module defines a HTTPMessage class.  It will be used by sender receiver to 
parse Http syle messages

Public Interface:
=================
createHTTPMessage	:	creates a string message from HTTPMessage class
parseStringMessage	:	creates HTTPMessage from String message class

Build Process:
==============
Required Packages
- HTTPMessage, Utilities
Build commands (either one)
- devenv Remote_Code_Repository.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 03 May 2016
- first release

*/




#include<string>
#include<vector>
#include<unordered_map>

//commands
#define UPLOAD "00"
#define START_UPLOAD "01"
#define END_UPLOAD "02"
#define REQUEST_UPLOAD "04"
#define ACK_UPLOAD "05"

#define DOWNLOAD "11"
#define START_DOWNLOAD "12"
#define END_DOWNLOAD "13"
#define REQUEST_DOWNLOAD "14"
#define ACK_DOWNLOAD "15"

#define SEND_HTTPMESSAGE "21"
#define ACK_HTTPMESSAGE "22"

#define REQUEST_ALL_FILES "31"
#define RESPONSE_ALL_FILES "32"

#define END_CLIENT_RECEIVER "41"
#define END_SERVER "42"

//functions
#define SEND_MESSAGE "sendMessage"
#define UPLOAD_REQUEST "uploadRequest"
#define DOWNLOAD_REQUEST "downloadRequest"
#define UPLOAD_RESPONSE "uploadResponse"
#define DOWNLOAD_RESPONSE "downloadResponse"
#define GETALLFILES_REQUEST "reqgetallfiles"
#define GETALLFILES_RESPONSE "resgetallfiles"


#define BUFF_SIZE 1024
#define COMMAND "command"
#define TO_ADDR "to_Addr"
#define TO_PORT "to_Port"
#define FROM_ADDR "from_Addr"
#define FROM_PORT "from_Port"
#define FILENAME "fileName"
#define AUTHOR "author"
#define CON_LEN "contentLen"
#define BODY "body"
#define OPEN "open"
#define SEPARATOR '|'
#define FILES_SEP '^'
#define EOT 0x03
class HTTPMessage {
public:
	//detailed description of all function in .cpp file
	static std::string createHTTPMessage(HTTPMessage msg);
	static std::unordered_map<std::string, std::string> getAttributes(std::string msg);
	static std::vector<std::string> split(std::string str, char sep);
	static HTTPMessage parseStringMessage(std::string msg);
	void toString();
	std::string command;
	std::string toAddr;
	int toPort;
	std::string fromAddr;
	int fromPort;
	std::string fileName;
	std::string author;
	std::string open;
	int contentLen= 0;
	std::string body = "";
};
#endif
