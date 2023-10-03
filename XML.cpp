#include "XML.h"

XML::XML(std::string path)
{
	loadedCorrectly = true;
	
	std::fstream file;
	file.open(path, std::ios::in);
	if(!file){ loadedCorrectly = false; return;}
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content = buffer.str();
	
	std::vector<std::string> tags;
	std::vector<std::string> values;
	head = NULL;
	temp = NULL;
	
	for(int i=0; i<content.length(); i++)
	{
		if(content[i] != '<'){
			if(values.size() > 0)
			{
				if(content[i] != '\n') values[values.size()-1] += content[i];
			}
			continue;
		}
		int j = i, z;
		i++;
		if(i >= content.size()) goto end;
		
		if(content[i] == '?')
		{
			//checking for xml tag(<?xml?>)
			content = content.substr(0, j) + content.substr(content.find('>')+1);
			i = j;
			continue;
		}
		if(content[i] == '!')
		{
			//checking for comment(<!---->) or doctype(<!DOCTYPE)
			if(content.size() < i+2) goto end;
			if(content[i+1] == '-' && content[i+2] == '-')
			{
				z = i + 1;
				while(true)
				{
					z = content.find('>', z+1);
					if(z == -1) goto end;
					
					if(content[z-1] == content[z-2] && content[z-1] == '-')
					{
						content = content.substr(0, j) + content.substr(z+1);
						break;
					}
				}
			}
			else
			{
				z = content.find('>', i);
				if(z == -1)
				{
					end: {}
					loadedCorrectly = false;
					break;
				}
				content = content.substr(0, j) + content.substr(z+1);
			}
			i = j-1; continue;
		}
		if(content[i] == '/')
		{
			//closed tag
			std::string str = "";
			for(i++; i < content.size() && (content[i] >= 'a' && content[i] <= 'z' || content[i] >= 'A' && content[i] <= 'Z'); ++i) str += content[i];
			if(i == content.size()) goto end;
			
			if(str != tags[tags.size() - 1])goto end;
			else {
				tags.pop_back();
				temp->value = values[values.size() - 1];
				values.pop_back();
				temp = temp->father;
			}
			
			for(; (content[i] == ' ' || content[i] == '\t' || content[i] == '\n') && i < content.size(); ++i);
			if(i == content.size()) goto end;
			
			if(content[i] != '>') goto end;
			continue;
		}
		else
		{
			//tag opened
			j = i;
			std::string str = "";
			for(; i < content.size() && (content[i] >= 'a' && content[i] <= 'z' || content[i] >= 'A' && content[i] <= 'Z'); ++i) str += content[i];
			if(i == content.size() || j == i) goto end;
			tags.push_back(str);
			values.push_back("");
			if(head == NULL){
				head = new tree;
				temp = head;
				temp->father = NULL;
				temp->name = str;
			}
			else
			{
				tree* newtree = new tree;
				temp->childe.push_back(new tree);
				temp->childe[temp->childe.size()-1]->father = temp;
				temp = temp->childe[temp->childe.size()-1];
				temp->name = str;
			}
			//attributes
			bool check = false;
			
			while(true)
			{
				for(; (content[i] == ' ' || content[i] == '\t' || content[i] == '\n') && i < content.size(); ++i);
				if(i == content.size()) goto end;
				
				if(content[i] == '>'){check = true; break;}
				if(content[i] == '/')
				{
					if(content.size() == ++i) goto end;
					if(content[i] == '>'){tags.pop_back(); temp = temp->father; check = true; break;}
					else goto end;
				}
				//attribute name
				str = "";
				for(; i < content.size() && (content[i] >= 'a' && content[i] <= 'z' || content[i] >= 'A' && content[i] <= 'Z' || content[i] >= '0' && content[i] <= '9'
					  || content[i] == ':' || content[i] == '_' || content[i] == '-' || content[i] == '.'); ++i) str += content[i];
				if(i == content.size() || j == i) goto end;
				
				temp->atr_name.push_back(str);
				
				for(; (content[i] == ' ' || content[i] == '\t' || content[i] == '\n') && i < content.size(); ++i);
				if(i == content.size()) goto end;
				
				if(content[i] != '=') goto end;
				
				//attribute value
				z = content.find('\"', i);
				if(z == -1 || z+1 == content.size()) goto end;
				
				i = content.find('\"', z+1);
				if(z == -1) goto end;
				
				temp->atr_value.push_back(content.substr(z+1, i - z - 1));
				
				i++;
			}
			if(check) continue;
		}
	}
	if(loadedCorrectly) temp = head;
	else
	{
		temp = head;
		head = NULL;
	}
}

bool XML::goBack()
{
	if(temp->father == NULL) return false;
	temp = temp->father; return true;
}

bool XML::goTo(int index)
{
	if(index < 0 || index >= temp->childe.size()) return false;
	temp = temp->childe[index];
	return true;
}

bool XML::goTo(std::string name)
{
	for(int i=0; i<temp->childe.size(); ++i)
	{
		if(name == temp->childe[i]->name)
		{
			temp = temp->childe[i];
			return true;
		}
	}
	return false;
}

std::string XML::getAtrValue(int index)
{
	if(index < 0 || index >= temp->atr_name.size()) return "";
	return temp->atr_value[index];
}
std::string XML::getAtrValue(std::string name)
{
	for(int i=0; i<temp->atr_name.size(); ++i) if(name == temp->atr_name[i]) return temp->atr_value[i];
	return "";
}
int XML::getAtrLenght()
{
	return temp->atr_name.size();
}
std::string XML::getAtrName(int index)
{
	if(index < 0 || index >= temp->atr_name.size()) return "";
	return temp->atr_name[index];
}
std::string XML::getValue()
{
	return temp->value;
}
std::string XML::getName()
{
	return temp->name;
}
int XML::getChildeSize()
{
	return temp->childe.size();
}
bool XML::isLoaded()
{
	return loadedCorrectly;
}
std::string XML::getChildeName(int index)
{
	if (index < 0 || index >= temp->childe.size()) return "";
	return temp->childe[index]->name;
}
