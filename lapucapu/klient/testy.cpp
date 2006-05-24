#include <fstream>
#include <iostream>
#include <glibmm.h>
#include <string>
#include <assert.h>
#include <libxml++.h>
#include <istream>
#include "mytypes.h"
#include "messages.h"

using namespace std;
xmlpp::Node* xml;
int func1();
int func2();

int mmmain(int argc, char* argv[])
{
  std::string filepath;
  if(argc > 1 )
    filepath = argv[1]; //Allow the user to specify a different XML file to parse.
  else
    filepath = "example.xml";

  cout << "Wczytuje plik: " << filepath << endl << endl;
  
  std::ifstream  in (filepath.c_str(), std::ios_base::binary);
  
  assert(in);



try
	{
		xmlpp::DomParser parser;
		parser.set_validate();
		parser.set_substitute_entities();
		parser.parse_stream((std::istream&)in);
		if(parser)
		{
			xml = parser.get_document()->get_root_node();
			if(xml->get_name() != "xml")
			{
				unreconized_xml("Wrong root node name!");
			}
			else
			{
				SHOW("ok : nazwa root_node to 'xml'");
				func2();
			}
		}
	}
	catch(const std::exception& ex)
	{
		unreconized_xml((std::string)"Exception caught: " + (std::string)ex.what());
	}
}


// read a file into memory
//#include <iostream>
//#include <fstream>
//using namespace std;
//
//int nmain () {
//  int length;
//  char * buffer;
//
//  ifstream is;
//  is.open ("test.txt", ios::binary );
//
//  // get length of file:
//  is.seekg (0, ios::end);
//  length = is.tellg();
//  is.seekg (0, ios::beg);
//
//  // allocate memory:
//  buffer = new char [length];
//
//  // read data as a block:
//  is.read (buffer,length);
//
//  is.close();
//
//  cout.write (buffer,length);
//
//  return 0;
//}

int func1()
{
	xmlpp::Node::NodeList lista = xml->get_children("message");
	if(lista.size() == 1)
	{
		xmlpp::Node::NodeList::iterator iter = lista.begin();
		xmlpp::Attribute * atrybut = dynamic_cast<const xmlpp::Element*>(*iter)->get_attribute("type");
		if(atrybut)
		{
			cout << atrybut->get_value().data() << endl;
		}
		else
		{
			unreconized_xml("node message nie posiada wartosci value");
			return 0;
		}
	}
	else
	{
		unreconized_xml("node message nie wystepuje dokladnie raz");
		return 0;
	}
	
	return 0;
}

int func2()
{
	SHOWU(xml->get_name());
	// chyba jestem w z?ym node ... bo nie wy?wietla jego nazwy!!!
	SHOW("Glib::ustring xml_explode::check_type ()");
	xmlpp::Node::NodeList lista = xml->get_children();
	xmlpp::Node::NodeList::iterator iter;

	SHOW("Ilosc elementow xml->get_children():");
	SHOW(lista.size());
	
	for(iter = lista.begin(); iter != lista.end(); iter++)
		SHOWU((*iter)->get_name());
	
	lista = xml->get_children("message");	
	SHOW(lista.size());
	
	if(lista.size() == 1)
	{
		xmlpp::Node::NodeList::iterator iter = lista.begin();
		xmlpp::Node * node_message = *iter;
		
		SHOW("node_message->get_name():");
		SHOWU(node_message->get_name());
		
		if(const xmlpp::Element * element_value = dynamic_cast<const xmlpp::Element*>(node_message))
		{
			const xmlpp::Element::AttributeList& attributes = element_value->get_attributes();
		    for(xmlpp::Element::AttributeList::const_iterator iter2 = attributes.begin(); iter2 != attributes.end(); ++iter2)
		    {
				const xmlpp::Attribute* attribute = *iter2;
				SHOW("  Attribute: ");
				SHOWU(attribute->get_name());
				SHOW(" = "); SHOWU(attribute->get_value());
		    }
		}
//		xmlpp::Attribute * atrybut = element_value->get_attribute("value");
//		
//		if(atrybut)
//		{
//			SHOWU(atrybut->get_value());
//			return 0;
//		}
//		else
//		{
//			unreconized_xml("node message nie posiada wartosci value");
//			return 0;
//		}
	}
	else
	{
		unreconized_xml("node message nie wystepuje dokladnie raz");
		return 0;
	}
}



//
//
//	xmlpp::Node::NodeList lista = xml->get_children();
//	xmlpp::Node::NodeList::iterator iter;
//
//	SHOW("Ilosc elementow xml->get_children():");
//	SHOW(lista.size());
//	
//	for(iter = lista.begin(); iter != lista.end(); iter++)
//		SHOWU((*iter)->get_name());
//	
//	