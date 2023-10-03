# XML

Simple XML file reader

## How to use

Just add the XML.h and XML.cpp to your project and you are ready

## Usage

```c++
#include "XML.h"

int main()
{
  XML xml("pathToXMLfile.xml");
  if(!xml.isLoaded())
  {
    std::cout<<"xml file format not valid\n";
    return 0;
  }
  xml.goBack();                      //{
  xml.goTo(int);                     //  functions to navigate between tags
  xml.goTo(std::string);             //}

  xml.getValue();                    //get tag value
  xml.getName();                     //get tag name
  xml.getAtrValue(int);              //get attribute value with position 
  xml.getAtrValue(std::string);      //get attribute value with attribute name
	xml.getAtrLenght();                //get attribute's tag lenght
	xml.getAtrName(int);               //get attribute name given the position
	xml.getChildeName(int);            //get subtag name given the position
	xml.getChildeSize();               //get subtags number
}
```
