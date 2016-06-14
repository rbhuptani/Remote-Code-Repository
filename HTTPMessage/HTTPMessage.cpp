/////////////////////////////////////////////////////////////////////
//  hTTPMessage.cpp - Creates HTTP style messages to communicate   //
//					between client and server					   //	
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Remote Code Repository, Project 4               //
//  Author:		   Ronak Bhuptani, SUID#429019830, Syracuse        //
//                 University, rmbhupta@syr.edu                    //
/////////////////////////////////////////////////////////////////////

#include "HTTPMessage.h"
#include <sstream>
#include<exception>
#include<iostream>
#include "..//Utilities/Utilities.h"
using SH = Utilities::StringHelper;

// this function will conver HTTpMEssage to string message
std::string HTTPMessage::createHTTPMessage(HTTPMessage msg)
{
	std::string smsg;
	smsg = COMMAND;
	smsg += SEPARATOR + msg.command;
	smsg.append(1, EOT);
	smsg += TO_ADDR;
	smsg += SEPARATOR + msg.toAddr;
	smsg.append(1, EOT);
	smsg += TO_PORT;
	smsg += SEPARATOR + std::to_string(msg.toPort);
	smsg.append(1, EOT);
	smsg += FROM_ADDR;
	smsg += SEPARATOR + msg.fromAddr;
	smsg.append(1, EOT);
	smsg += FROM_PORT;
	smsg += SEPARATOR + std::to_string(msg.fromPort);
	smsg.append(1, EOT);
	smsg += FILENAME;
	smsg += SEPARATOR + (msg.fileName == "" ? "NA" : msg.fileName);
	smsg.append(1, EOT);
	smsg += AUTHOR;
	smsg += SEPARATOR + (msg.author == "" ? "NA" : msg.author);
	smsg.append(1, EOT);
	smsg += CON_LEN;
	smsg += SEPARATOR + std::to_string(msg.contentLen);
	smsg.append(1, EOT);
	smsg += BODY;
	smsg += SEPARATOR + (msg.body == "" ? "NA" : msg.body);
	smsg.append(1, EOT);
	smsg += OPEN;
	smsg += SEPARATOR + (msg.open == "" ? "false" : msg.open);
	smsg.append(1, EOT);
	return smsg;
}

// this function create map of key-value pairs of HttpMessage attributes from string message
std::unordered_map<std::string, std::string> HTTPMessage::getAttributes(std::string msg)
{
	std::unordered_map<std::string, std::string> Attributes;
	std::vector<std::string> attributes = split(msg, EOT);
	try {
		for (auto attribute : attributes) {
			std::vector<std::string> KeyValue = split(attribute, SEPARATOR);
			Attributes[KeyValue[0]] = KeyValue[1];
		}
	}
	catch (std::exception ex) {
		std::cout << std::endl << msg << "  : " << ex.what() << std::endl;
	}
	return Attributes;
}

// split function to split string based on delimiter sep
std::vector<std::string> HTTPMessage::split(std::string str, char sep)
{
	std::vector<std::string> items;
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, sep))
	{
		items.push_back(item);
	}
	return items;
}

// this function will parse HTTPMessage from string message
HTTPMessage HTTPMessage::parseStringMessage(std::string msg)
{
	HTTPMessage hmsg;
	std::unordered_map<std::string, std::string> Attributes = getAttributes(msg);
	if (Attributes.size() > 0) {
		try {
			hmsg.command = Attributes[COMMAND];
			hmsg.toAddr = Attributes[TO_ADDR];
			hmsg.toPort = stoi(Attributes[TO_PORT]);
			hmsg.fromAddr = Attributes[FROM_ADDR];
			hmsg.fromPort = stoi(Attributes[FROM_PORT]);
			hmsg.fileName = Attributes[FILENAME];
			hmsg.author = Attributes[AUTHOR];
			hmsg.contentLen = stoi(Attributes[CON_LEN]);
			hmsg.body = Attributes[BODY];
			hmsg.open = Attributes[OPEN];
		}
		catch(std::exception ex){
			std::cout << "\n Some attribute not found";
		}
	}
	return hmsg;
}

// this function prints HTTP message
void HTTPMessage::toString()
{
	std::cout << std::endl;
	SH::title("HTTP Message is : ");
	std::cout << std::endl;
	std::cout << std::endl << COMMAND << " : " << command;
	std::cout << std::endl << TO_ADDR << " : " << toAddr;
	std::cout << std::endl << TO_PORT << " : " << toPort;
	std::cout << std::endl << FROM_ADDR << " : " << fromAddr;
	std::cout << std::endl << FROM_PORT << " : " << fromPort;
	std::cout << std::endl << FILENAME << " : " << fileName;
	std::cout << std::endl << AUTHOR << " : " << author;
	std::cout << std::endl << CON_LEN << " : " << contentLen;
	std::cout << std::endl << BODY << " : " << body;
	std::cout << std::endl;
}


#ifdef TEST_HTTPMESSAGE
int main()
{
	SH::title("Demonstrating HTTP_MESSAGE");
	std::cout << std::endl;
	HTTPMessage hmsg;
	hmsg.command = "Send File";
	hmsg.toAddr = "localhost";
	hmsg.toPort = 8080;
	hmsg.fromAddr = "localhost";
	hmsg.fromPort = 8081;
	hmsg.fileName = "TestFile.cpp";
	hmsg.author = "Ronak Bhuptani";
	hmsg.body = "This is test body";
	hmsg.contentLen = hmsg.body.size();
	std::string msg = HTTPMessage::createHTTPMessage(hmsg);
	SH::title("Showing HTTP Message:");
	std::cout << std::endl;
	std::cout << msg <<std::endl;
	SH::title("Parsing string http message to HTTP MEssage object");
	hmsg = HTTPMessage::parseStringMessage(msg);
	hmsg.toString();
	return 0;
}
#endif