#ifndef SENDER_H
#define SENDER_H

/////////////////////////////////////////////////////////////////////
//  Sender.h - Sends message through socket					       //
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
This module defines a Sender class.  It will be used by Server and Client to send messages

Public Interface:
=================
sendMessage		:	Sends string message
sendFile		:	Sends files
senderFactory	:	Creates a message based on operation type and send

Build Process:
==============
Required Packages
- FileMgr, FileSystem, HTTPMessage, Utilities
Build commands (either one)
- devenv Remote_Code_Repository.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 03 May 2016
- first release

*/

#include<iostream>
#include<fstream>
#include <string>
#include "..//HTTPMessage/HTTPMessage.h"
#include "..//Sockets/Sockets.h"
class Sender {
	//detailed description of all function in .cpp file
	using Operation = std::string;
public:
	void sendMessage(Socket &si, HTTPMessage msg);
	void sendFile(Socket &si, HTTPMessage msg);
	void senderFactory(Socket &si, HTTPMessage msg, Operation command);
private:

};

#endif