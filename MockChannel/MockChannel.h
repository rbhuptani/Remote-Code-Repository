#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
/////////////////////////////////////////////////////////////////////
//  MockChannel.cpp : communication channel for project #4         //
//				 build as DLL then use this DLL to use native      //
//				 functions for                                     //
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
This module defines a Chanel Interface and classes.  It will be used by WPF application
to communicate with native code

Public Interface:
=================
getMessage	:	get message from client receving queue.
postMessage	:	post message to client's sending queue.
start()		:	start client.
stop()		:	stop client.

Build Process:
==============
Required Packages
- Client, MockChannel,HTTPMessage, Sender, Reciver
Build commands (either one)
- devenv Remote_Code_Repository.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 03 May 2016
- first release

*/

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include "..//HTTPMessage/HTTPMessage.h"

struct IChannel
{
public:
	virtual HTTPMessage getMessage() = 0;
	virtual void postMessage(const HTTPMessage& msg) = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
};

extern "C" {
	struct ObjectFactory
	{
		DLL_DECL IChannel* createChannel(std::string ip, int port);
	};
}


#endif


