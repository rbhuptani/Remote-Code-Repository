#ifndef CLIENT_H
#define CLIENT_H

/////////////////////////////////////////////////////////////////////
//  Client.h - Sends message to Server and Receives messages       //
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
This module defines a Client class.  It will be connected to server
to send and receive files from server repository.

Public Interface:
=================
startSender		:	starts sender 
startReceiver	:	starts receiver
connectSocket	:	connect to server with specified address and port
postMessage		:	enqueues message to sender queue
processRequest	:	process message which came from sender queue
getMessage		:	Dequeue messages from receiver queue

Build Process:
==============
Required Packages
- Client, XMLDocument, XMLParser, XMLElements, FileMgr, FileSystem
  HTTPMessage, MetadataHandler, Sender ,Receiver
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


class Client {
public:
	//detailed description of all function in .cpp file
	Client(std::string address = "localhost", int port = 8085) : _address(address), _port(port), _server_address("localhost"), _server_port(8080),  clientReceiver(rcvrQueue) 
	{
		std::string _rootDir = "./Client_Repository/";
		if (_access(_rootDir.c_str(), 0) != 0)
			_rootDir = "." + _rootDir;
		clientReceiver.setRecevierPath(_rootDir);
	}
	void setReceiverPath(std::string path) { clientReceiver.setRecevierPath(path); }
	std::string getReceiverPath() { return clientReceiver.getRecevierPath(); }
	void startSender();
	void startReceiver();
	bool connectSocket(SocketConnecter& si, std::string ipAddress, int port);
	void postMessage(const HTTPMessage& msg);
	void processRequest(SocketConnecter& si, HTTPMessage request);
	HTTPMessage getMessage();
	bool ParseCommandLineArgs(int argc, char ** argv);
	//getter for address and port
	std::string getServerAddress() { return _server_address; }
	std::string getClientAddress() { return _address; }
	int getServerPort() { return _server_port; }
	int getClientPort() { return _port; }
private:
	BlockingQueue<HTTPMessage> sndrQueue;
	BlockingQueue<std::string> rcvrQueue;
	std::string _address;
	int _port;
	std::string _server_address;
	int _server_port;
	Receiver clientReceiver;
	Sender clientSender;

};

#endif
