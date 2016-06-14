#ifndef RECEIVER_H
#define RECEIVER_H

//////////////////////////////////////////////////////////////////////////
//  Receiver.h - Listens on socket and process the messages as received	//
//  ver 1.0																//
//  Language:      Visual C++ 2008, SP1									//
//  Platform:      Dell Precision T7400, Vista Ultimate SP1				//
//  Application:   Remote Code Repository, Project 4					//
//  Author:		   Ronak Bhuptani, SUID#429019830, Syracuse				//
//                 University, rmbhupta@syr.edu							//
//////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a receiver class.  It will be used by Server and Client
to listen on one socket and process request as they are recevied

Public Interface:
=================
operator()	:	this class is function and server and client will only use this function, 
				other function are helper function of this function and won't be used by other class

Build Process:
==============
Required Packages
- XMLDocument, XMLParser, XMLElements, FileMgr, FileSystem
HTTPMessage, MetadataHandler, Blocking Queue, Sockets
Build commands (either one)
- devenv Remote_Code_Repository.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 03 May 2016
- first release

*/

#include<string>
#include<iostream>
#include<fstream>
#include"..//Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include"..//Sockets/Sockets.h"
#include"..//HTTPMessage/HTTPMessage.h"
class Receiver {
public:
	//detailed description of all function in .cpp file
	Receiver(BlockingQueue<std::string> &rcvQueue, std::string rcvPath = "") : _rcvQueue(rcvQueue), _rcvPath(rcvPath) {}
	void setRecevierPath(std::string rcvPath);
	std::string getRecevierPath() { return _rcvPath; }
	void operator()(Socket &si);
	void swapAddress(HTTPMessage &msg);
	bool getFile(Socket &socket, HTTPMessage &msg);
	bool fileWriter(Socket& socket, HTTPMessage& rcvmessage, std::string filePath);
	bool buffReader(Socket& _socket, int size, Socket::byte* buffer);
	void getAllServerFiles(HTTPMessage &msg);
	void fileUploadHandler(Socket& _socket, HTTPMessage& rcvMessage);
	void getAllFilesHandler(HTTPMessage& rcvmessage);
	void fileDownloadHandler(Socket& _socket, HTTPMessage& rcvmessage);
private:
	BlockingQueue<std::string> & _rcvQueue;
	std::string _rcvPath;
};

#endif