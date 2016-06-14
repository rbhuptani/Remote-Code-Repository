#ifndef SERVER_H
#define SERVER_H
/////////////////////////////////////////////////////////////////////
//  Server.h -	   Responses to requests received from any client  //
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
This module defines a Server class.  It will start to listen on one socket
and sends message on another socket.

Public Interface:
=================
startSender		:	starts sender
startReceiver	:	starts receiver
connectSocket	:	connect to client with specified address and port
enQueue			:	enqueues message to receiver queue
processRequest	:	process message which came from receiver queue
deQueue			:	Dequeue messages from receiver queue

Build Process:
==============
Required Packages
- Client, XMLDocument, XMLParser, XMLElements, FileMgr, FileSystem
HTTPMessage, MetadataHandler
Build commands (either one)
- devenv Remote_Code_Repository.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 03 May 2016
- first release

*/
#include <io.h>
#include "..//Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "..//HTTPMessage/HTTPMessage.h"
#include "..//Sender/Sender.h"
#include "..//Receiver/Receiver.h"
#include "..//Sockets/Sockets.h"
#include "..//Utilities/Utilities.h"
#include "..//FileMgr/FileMgr.h"
#include "..//FileMgr/FileSystem.h"

// Server_Repository
class Server {
	//detailed description of all function in .cpp file
public:
	Server(std::string address = "localhost", int port = 8080) : _address(address), _port(port),  serverReceiver(rcvrQueue) {
		_rootDir = "./Server_Repository/";
		if (_access(_rootDir.c_str(), 0) != 0)
			_rootDir = "." + _rootDir;
		serverReceiver.setRecevierPath(_rootDir);
	}
	void processRequest(SocketConnecter& si, std::string requestMsg);
	void startSender();
	void startReceiver();
	bool ParseCommandLineArgs(int argc, char** args);
	bool connectSocket(SocketConnecter& si, std::string ipAddress, int port);
	void enQueue(std::string msg);
	std::string deQueue();
	std::string getAddress() { return _address; }
	int getPort() { return _port; }
private:
	BlockingQueue<std::string> rcvrQueue;
	std::string _address;
	int _port;
	std::string _rootDir;
	Receiver serverReceiver;
	Sender serverSender;
};

#endif
