///////////////////////////////////////////////////////////////////////
// Utilities.cpp - small, generally usefule, helper classes          //
// ver 1.1                                                           //
// Language:    C++, Visual Studio 2015                              //
//  Application:   Code Parser, Project 2                            //
//  Author:		   Ronak Bhuptani, SUID#429019830, Syracuse          //
//                 University, rmbhupta@syr.edu                      //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University       //
//                 (315) 443-3948, jfawcett@twcny.rr.com             //
///////////////////////////////////////////////////////////////////////

#include <cctype>
#include <iostream>
#include "Utilities.h"
#include <ctime>

using namespace Utilities;

std::function<void(const std::string&)> Title =
  [](auto src) { StringHelper::Title(src, '='); };

std::function<void(const std::string&)> title =
  [](auto src) { StringHelper::Title(src, '-'); };

void StringHelper::title(const std::string& src)
{
  std::cout << "\n  " << src;
  std::cout << "\n " << std::string(src.size() + 2, '-');
}

std::string Utilities::StringHelper::getTab(const size_t n)
{
	std::string tabs = "";
	for (size_t i = 0; i < n; i++)
		tabs += "\t";
	return tabs;
}

void StringHelper::Title(const std::string& src, char underline)
{
  std::cout << "\n  " << src;
  std::cout << "\n " << std::string(src.size() + 2, underline);
}

std::string Utilities::StringHelper::changeExtenssion(std::string input, std::string newExt) {
	size_t index = input.find_last_of('.');
	return input.replace(index, 1, "_") + newExt;
}

std::string Utilities::StringHelper::changeExtenssiontoCpp(std::string input, std::string newExt) {
	size_t index = input.find_last_of('.');
	return input.replace(index, 2, newExt) ;
}

std::string Utilities::StringHelper::getFileName(std::string input) {
	size_t s = input.size();
	size_t index = input.find_last_of('.');
	return input.replace(index, s-index, "");
}

std::string Utilities::StringHelper::addTimeStamp(std::string input)
{
	size_t s = input.size();
	std::time_t result = std::time(nullptr);
	std::string currTime = std::to_string(result);
	size_t index = input.find_last_of('.');
	std::string ext = input.substr(index, s - index);
	input = getFileName(input) + "_" + currTime + ext;
	return input;
}

std::string Utilities::StringHelper::getFolderName(std::string path)
{
	path = path.substr(0, path.size() - 1);
	size_t index = path.find_last_of('/');
	if(index == std::string::npos)
		index = path.find_last_of('\\');
	if (index == std::string::npos)
		return "";
	std::string result = path.substr(++index, path.size() - index);
	return result;
}

std::string Utilities::StringHelper::trim(std::string str)
{
	size_t endpos = str.find_last_not_of(" \t");
	if (std::string::npos != endpos)
		str = str.substr(0, endpos + 1);
	size_t startpos = str.find_first_not_of(" \t");
	if (std::string::npos != startpos)
		str = str.substr(startpos);
	endpos = str.find_last_not_of("\n");
	if (std::string::npos != endpos)
		str = str.substr(0, endpos + 1);
	startpos = str.find_first_not_of("\n");
	if (std::string::npos != startpos)
		str = str.substr(startpos);
	return str;
}

void Utilities::StringHelper::getDirPattern(int argc, char *argv[], std::string& dir, std::string& pattern, std::string& stopFlag)
{
	dir = "./";
	pattern = "*.cpp";
	stopFlag = "false";
	if (argc > 2) {
		for (int i = 0; i < argc; i++) {
			std::string d(argv[i]);
			if (d == "/d") 
				if (i + 1 < argc)
					dir = argv[i + 1];
			std::string p(argv[i]);
			if (p == "/p")
				if (i + 1 < argc)
					pattern = argv[i + 1];
			std::string s(argv[i]);
			if (s == "/s")
				if (i + 1 < argc)
					stopFlag = argv[i + 1];
		}
	}
}

void Utilities::StringHelper::getDirPatternN(int argc, char *argv[], std::string& dir, std::string& pattern, std::string& nThreads)
{
	dir = "./";
	pattern = "*.cpp,*.h";
	nThreads = "2";
	if (argc > 2) {
		for (int i = 0; i < argc; i++) {
			std::string d(argv[i]);
			if (d == "/d")
				if (i + 1 < argc)
					dir = argv[i + 1];
			std::string p(argv[i]);
			if (p == "/p")
				if (i + 1 < argc)
					pattern = argv[i + 1];
			std::string s(argv[i]);
			if (s == "/n")
				if (i + 1 < argc)
					nThreads = argv[i + 1];
		}
	}
}

void Utilities::StringHelper::PrintThreadSafe(std::string msg)
{
	/*static std::mutex _mtx;
	{
		std::lock_guard<std::mutex> l(_mtx);
		std::cout << std::endl << msg << std::endl;
	}*/
}

std::vector<std::string> StringHelper::split(const std::string& src)
{
  std::vector<std::string> accum;
  std::string temp;
  size_t index = 0;
  do
  {
    while ((isspace(src[index]) || src[index] == ',') && src[index] != '\n')
    {
      ++index;
      if (temp.size() > 0)
      {
        accum.push_back(temp);
        temp.clear();
      }
    }
    if(src[index] != '\0')
      temp += src[index];
  } while (index++ < src.size());
  if(temp.size() > 0)
    accum.push_back(temp);
  return accum;
}

void Utilities::putline()
{
  std::cout << "\n";
}

std::vector<std::string> StringHelper::split(const std::string& src, char ch) {
	std::vector<std::string> resultVec;
	std::stringstream ss(src);
	std::string item;
	while (getline(ss, item, ch))
		resultVec.push_back(item);
	return resultVec;
}


#ifdef TEST_UTILITIES

int main()
{
  Title("Testing Utilities Package");
  putline();

  title("test StringHelper::split(std::string)");

  std::string test = "a, \n, bc, de, efg, i, j k lm nopq rst";
  std::cout << "\n  test string = " << test;
  
  std::vector<std::string> result = StringHelper::split(test);
  
  std::cout << "\n";
  for (auto item : result)
  {
    if (item == "\n")
      std::cout << "\n  " << "newline";
    else
      std::cout << "\n  " << item;
  }
  std::cout << "\n";

  title("test std::string Converter<T>::toString(T)");

  std::string conv1 = Converter<double>::toString(3.1415927);
  std::string conv2 = Converter<int>::toString(73);
  std::string conv3 = Converter<std::string>::toString("a_test_string plus more");

  std::cout << "\n  Converting from values to strings: ";
  std::cout << conv1 << ", " << conv2 << ", " << conv3;
  putline();

  title("test T Converter<T>::toValue(std::string)");

  std::cout << "\n  Converting from strings to values: ";
  std::cout << Converter<double>::toValue(conv1) << ", ";
  std::cout << Converter<int>::toValue(conv2) << ", ";
  std::cout << Converter<std::string>::toValue(conv3);

  std::cout << "\n\n";
  return 0;
}
#endif
