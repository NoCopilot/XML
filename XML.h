#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

class XML
{
	private:
		bool loadedCorrectly;
		struct tree
		{
			std::string name = "";
			std::string value = "";
			std::vector<std::string> atr_value;
			std::vector<std::string> atr_name;
			std::vector<tree*> childe;
			tree* father;
		} *head, *temp;	
	public:
		XML(std::string path);
		bool goTo(int index);
		bool goTo(std::string name);
		bool goBack();
		std::string getAtrValue(int index);
		std::string getAtrValue(std::string name);
		int getAtrLenght();
		std::string getAtrName(int index);
		std::string getValue();
		std::string getName();
		std::string getChildeName(int index);
		int getChildeSize();
		bool isLoaded();
};
