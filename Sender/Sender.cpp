/////////////////////////////////////////////////////////////////////
//  Sender.h - Sends message through socket					       //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Remote Code Repository, Project 4               //
//  Author:		   Ronak Bhuptani, SUID#429019830, Syracuse        //
//                 University, rmbhupta@syr.edu                    //
/////////////////////////////////////////////////////////////////////

#include "Sender.h"
#include "..//Utilities/Utilities.h"

using SH = Utilities::StringHelper;

// sends stirng message thorugh Socket si
void Sender::sendMessage(Socket & si, HTTPMessage msg)
{
	std::string stringMsg = HTTPMessage::createHTTPMessage(msg);
	//std::cout << std::endl << "sendMessage : " << stringMsg << std::endl;
	si.sendString(stringMsg);
}

// sends file thorugh Socket si
void Sender::sendFile(Socket & si, HTTPMessage msg)
{
	std::ifstream infile;
	infile.open(msg.fileName, std::ios::binary | std::ios::ate | std::ios::in);
	if (!infile.is_open()) {
		msg.fileName = "." + msg.fileName;
		infile.open(msg.fileName, std::ios::binary | std::ios::ate | std::ios::in);
	}
	if (infile.is_open())
	{
		int size;
		char buffer[BUFF_SIZE];
		size = static_cast<int>(infile.tellg());
		infile.seekg(0, infile.beg);
		while (size > 0)
		{
			std::memset(buffer, 0, BUFF_SIZE);
			int bSize = BUFF_SIZE;
			if (size < bSize)
				bSize = size;
			size = size - bSize;
			std::string stringmessage = "";
			msg.contentLen = bSize;
			if (size == 0) {
				if (msg.command == START_UPLOAD || msg.command == UPLOAD)
					msg.command = END_UPLOAD;
				else
					msg.command = END_DOWNLOAD;
			}
			else {
				if (msg.command == START_UPLOAD || msg.command == UPLOAD)
					msg.command = START_UPLOAD;
				else
					msg.command = START_DOWNLOAD;
			}
			sendMessage(si, msg);
			infile.read(buffer, bSize);
			si.sendStream(bSize, buffer);
			if (infile.eof())
				break;
		}
		infile.close();
	}
	else
		std::cout <<std::endl << " Sender : Not able to open " << msg.fileName;
}

// creates message based on Operation type and sends string message / file through Socket
void Sender::senderFactory(Socket &si, HTTPMessage msg, Operation operation)
{
	if (operation == SEND_MESSAGE) {
		msg.command = SEND_HTTPMESSAGE;
		std::cout << std::endl << " Sender : Sending Message to localhost:" << msg.toPort << std::endl;
		sendMessage(si, msg);
	}
	else if (operation == UPLOAD_REQUEST) {
		msg.command = UPLOAD;
		std::cout << std::endl << " Sender : Sending Upload Response for File \"" << msg.fileName << "\" to localhost:" << msg.toPort << std::endl;
		sendFile(si, msg);
	}
	else if (operation == DOWNLOAD_REQUEST) {
		msg.command = REQUEST_DOWNLOAD;
		std::cout << std::endl << " Sender : Sending Download File \"" << msg.fileName << "\" request to localhost:" << msg.toPort << std::endl;
		sendMessage(si, msg);
	}
	else if (operation == UPLOAD_RESPONSE) {
		msg.command = ACK_UPLOAD;
		std::cout << std::endl << " Sending Upload Response for File \"" << msg.fileName << "\" to localhost:" << msg.toPort << std::endl;
		sendMessage(si, msg);
	}
	else if (operation == DOWNLOAD_RESPONSE) {
		msg.command = DOWNLOAD;
		std::cout << std::endl << " Sender : Sending Download Response for File \"" << msg.fileName << "\" to localhost:" << msg.toPort << std::endl;
		sendFile(si, msg);
	}
	else if (operation == GETALLFILES_REQUEST) {
		msg.command = REQUEST_ALL_FILES;
		std::cout << std::endl << " Sender : Sending Get All Files Request to localhost:" << msg.toPort << std::endl;
		sendMessage(si, msg);
	}
	else if (operation == GETALLFILES_RESPONSE) {
		msg.command = RESPONSE_ALL_FILES;
		std::cout << std::endl << " Sender : Sending Get All Files Response to localhost:" << msg.toPort << std::endl;
		sendMessage(si, msg);
	}
	else if (operation == END_CLIENT_RECEIVER) {
		msg.command = END_CLIENT_RECEIVER; 
		std::cout << std::endl << " Sender : Ending Client localhost:" << msg.fromPort << std::endl;
		sendMessage(si, msg);
	}
}

#ifdef TEST_SENDER

int main() {
	SH::title("Testing Sender");
	std::cout << std::endl;
	try {
		Sender sender;
		SocketSystem ss;
		SocketConnecter sc;
		HTTPMessage msg;
		while (!sc.connect("localhost", 8080))
		{
			std::cout<< "\n client waiting to connect";
			::Sleep(100);
		}
		//msg.command = "testFile";
		msg.fromAddr = "localhost";
		msg.fromPort = 8085;
		msg.toAddr = "localhost";
		msg.toPort = 8080;
		msg.body = "This is a test string";
		msg.author = "Ronak";
		msg.contentLen = msg.body.size();
		msg.fileName = "test_send.txt";
		sender.senderFactory(sc, msg, UPLOAD_RESPONSE);

		sc.shutDownSend();
	}
	catch(std::exception &ex){
		std::cout << std::endl << " Exception caught : " << ex.what() << std::endl;
	}
	return 0;
}

#endif