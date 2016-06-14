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

#define IN_DLL
#include "MockChannel.h"
#include "Cpp11-BlockingQueue.h"
#include "..//Client/Client.h"
#include <string>
#include <thread>
#include <iostream>

//Channel class to send and reads messages from Client.
class Channel : public IChannel
{
public:
	Channel(std::string ip, int port);
	void start();
	void stop();
	void postMessage(const HTTPMessage& msg) { client_.postMessage(msg); }
	HTTPMessage getMessage() { return client_.getMessage(); }
private:
	std::thread thread_;
	Client client_;
	bool stop_ = false;

};

// pass ip and port to client 
Channel::Channel(std::string ip, int port) : client_(ip, port) {
	std::string _rootDir = "./WPF_Client_Repository/";
	if (_access(_rootDir.c_str(), 0) != 0)
		_rootDir = "." + _rootDir;
	client_.setReceiverPath(_rootDir);	
}

void Channel::start()
{
	std::cout << "\n  Communication Channel starting up";
	std::thread receiverThread(&Client::startReceiver, &client_);
	std::thread senderThread(&Client::startSender, &client_);
	receiverThread.detach();
	senderThread.detach();
}
// signal server thread to stop

void Channel::stop()
{
	
}

// create communication channel 
IChannel* ObjectFactory::createChannel(std::string ip, int port)
{
	return new Channel(ip, port);
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
	ObjectFactory objFact;
	IChannel* pMockChannel = objFact.createChannel("localhost", 8090);
	pMockChannel->start();
	HTTPMessage msg;
	pMockChannel->postMessage(msg);
	std::cin.get();
}
#endif
