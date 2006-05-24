#include "xml_explode.h"

/**
 * Constructors/Destructors
 */
 
xml_explode::xml_explode(Glib::ustring sdata) 
{
	try
	{
		parser.set_validate();
		parser.set_substitute_entities();
		parser.parse_memory(sdata);
		if(parser)
		{
			xml = parser.get_document();
			root_node = xml->get_root_node();
			if(root_node->get_name() != "xml")
			{
				this->set_last_error("Wrong root node name!");
			}
		}
	}
	catch(const std::exception& ex)
	{
		this->set_last_error((std::string)"Exception caught: " + (std::string)ex.what());
	}
}

/**
 * Methods
 */

/**
 * 
 * Metoda sprawdzaj?ca typ przekazanego dokumentu xml
 * <message type="xml_explode::check_type ()"/>
 * 
 */
Glib::ustring xml_explode::check_type () 
{
	xmlpp::Node::NodeList lista = root_node->get_children("message");
	
	if(lista.size() == 1)
	{
		xmlpp::Node::NodeList::iterator iter = lista.begin();
		xmlpp::Node * node_message = *iter;
		const xmlpp::Element * element_value = dynamic_cast<const xmlpp::Element*>(node_message);
		xmlpp::Attribute * atrybut = element_value->get_attribute("type");
		if(atrybut) // to sprawdzenie jest dokonywane przez dtd - niepotrzebne
			return atrybut->get_value();
		else
		{
			this->set_last_error("node message nie posiada wartosci type");
			return 0;
		}
	}
	else
	{
		this->set_last_error("node message nie wystepuje dokladnie raz: lista.size() != 1 ");
		return 0;
	}
}

/**
 * 
 */
bool xml_explode::logged () {
	xmlpp::Node::NodeList lista = root_node->get_children("anwser");
	if(lista.size() == 1) // to sprawdzenie jest realizowane przez dtd
	{
		xmlpp::Node::NodeList::iterator iter = lista.begin();
		xmlpp::Node * node_anwser = *iter;
		const xmlpp::Element * element_anwser = dynamic_cast<const xmlpp::Element*>(node_anwser);
		xmlpp::Attribute * atrybut = element_anwser->get_attribute("state");
		if(atrybut) // to sprawdzenie jest realizowane przez dtd
		{
			if(atrybut->get_value() == "logged_in")
				return true;
			else if(atrybut->get_value() == "acces_deny") 
				return false;
			else // to sprawdzenie jest realizowane przez dtd
			{
				this->set_last_error("niepoprawna wartosc atrybutu state elementu anwser:");
			}
		}
		else
		{
			this->set_last_error("element anwser nie posiada wartosci state");
			return 0;
		}
	}
	else // to sprawdzenie jest realizowane przez dtd
	{
		this->set_last_error("element state nie wystepuje dokladnie raz: lista.size() != 1 ");
		return 0;
	}
	return true;
}
/**
 *  Pobranie opisu zalogowania
 */
Glib::ustring xml_explode::get_login_desc() {
	xmlpp::Node::NodeList::iterator iter = root_node->get_children("anwser").begin();
	xmlpp::Node::NodeList lista = (*iter)->get_children();
	
	const xmlpp::TextNode* nodeText;
	Glib::ustring out;
	for( iter = lista.begin(); iter != lista.end(); ++iter)
	{
		nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
		if(nodeText)
			out += nodeText->get_content();
	}
	return out;
}

/**
 * 
 */
Glib::ustring xml_explode::get_message_body () {
	xmlpp::Node::NodeList::iterator iter = root_node->get_children("body").begin();
	xmlpp::Node::NodeList body_nodes = (*iter)->get_children();
	
	const xmlpp::TextNode* nodeText;
	Glib::ustring out;
	for( iter = body_nodes.begin(); iter != body_nodes.end(); ++iter)
	{
		nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
		if(nodeText)
			out += nodeText->get_content();
	}
	return out;
}

/**
 * metoda pobiera wartosci uid node user zagnie?dzonych w node from
 */
Glib::ustring xml_explode::get_uid_from () {
	xmlpp::Node::NodeList::iterator iter = root_node->get_children("from").begin();
	iter = (*iter)->get_children("user").begin();
	const xmlpp::Element * element_user = dynamic_cast<const xmlpp::Element*>(*iter);
	xmlpp::Attribute * uid = element_user->get_attribute("uid");
	return uid->get_value();
}

/**
 * 
 */
uids_list xml_explode::get_message_uids_to () {
	xmlpp::Node::NodeList::iterator iter = root_node->get_children("to").begin();
	xmlpp::Node::NodeList lista_to = (*iter)->get_children("user");
	const xmlpp::Element * element_user;
	uids_list lista;
	for(iter = lista_to.begin(); iter != lista_to.end(); ++iter)
	{
		element_user = dynamic_cast<const xmlpp::Element*>(*iter);
		xmlpp::Attribute * uid = element_user->get_attribute("uid");
		lista.push_front(uid->get_value());
	}
	return lista;
}


std::vector<int> xml_explode::get_message_uids_to_int () {
	xmlpp::Node::NodeList::iterator iter = root_node->get_children("to").begin();
	xmlpp::Node::NodeList lista_to = (*iter)->get_children("user");
	const xmlpp::Element * element_user;
	std::vector<int> adresaci;
	for(iter = lista_to.begin(); iter != lista_to.end(); ++iter)
	{
		element_user = dynamic_cast<const xmlpp::Element*>(*iter);
		xmlpp::Attribute * uid = element_user->get_attribute("uid");
		adresaci.push_back( atoi( uid->get_value().c_str() ) );
	}
	return adresaci;
}

/**
 * Sprawdzenie czy dodano u?ytkownika
 */
bool xml_explode::respose_add_user()
{
	xmlpp::Node::NodeList::iterator iter = root_node->get_children("anwser").begin();
	iter = (*iter)->get_children("user").begin();
	const xmlpp::Element * element = dynamic_cast<const xmlpp::Element*>(*iter);
	xmlpp::Attribute * state = element->get_attribute("state");
	if(state->get_value() == "add_ok")
		return true;
	else
		return false; // state->get_value() == "add_fail"
}

/**
 * Pobierz opis procesu dodawania u?ytkownika
 */
Glib::ustring xml_explode::get_add_user_desc()
{
	xmlpp::Node::NodeList::iterator iter = root_node->get_children("anwser").begin();
	xmlpp::Node::NodeList nodes = (*iter)->get_children();
	
	const xmlpp::TextNode* nodeText;
	Glib::ustring out;
	for( iter = nodes.begin(); iter != nodes.end(); ++iter)
	{
		nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter);
		if(nodeText)
			out += nodeText->get_content();
	}
	return out;
}


/**
 * Pobiera liste u?ytkownik?w (indeksowan? po uid-ie liste obiektow user)
 */
users_list xml_explode::get_user_states()
{
	xmlpp::Node::NodeList::iterator iter = root_node->get_children("users").begin();
	xmlpp::Node::NodeList nodes = (*iter)->get_children();	
	
	users_list ulist;
	user current_person;
	Glib::ustring temp;
	
	xmlpp::Node::NodeList::iterator iter_status;
		
	for(iter = nodes.begin(); iter != nodes.end(); ++iter)
	{
		if((*iter)->get_name() == "user")
		{
			current_person.set_uid(get_value_from_iter(iter, "uid"));
			
			iter_status = (*iter)->get_children("status").begin(); // tu dostaje dostep do noda status
			
			current_person.set_state(get_value_from_iter(iter_status, "state"));
				
			current_person.set_opis(get_all_text_from_iter(iter_status));
				
			ulist[current_person.get_uid()] = current_person;
						
		} // omin smieci
	}
	
	return ulist;
}
	

/**
 * prywatna metoda pobiera text z noda podanego jako argument
 */
Glib::ustring xml_explode::get_all_text_from_iter(xmlpp::Node::NodeList::iterator iter)
{
	xmlpp::Node::NodeList nodes = (*iter)->get_children();
	const xmlpp::TextNode* nodeText;
	Glib::ustring out;
	for(xmlpp::Node::NodeList::iterator iter2 = nodes.begin(); iter2 != nodes.end(); ++iter2)
	{
		nodeText = dynamic_cast<const xmlpp::TextNode*>(*iter2);
		if(nodeText)
			out += nodeText->get_content();
	}
	return out;
}

/**
 * prywatna metoda pobiera wartosc name z noda node
 * lub zero przy niepowodzeniu
 */
Glib::ustring xml_explode::get_value_from_iter(xmlpp::Node::NodeList::iterator iter, Glib::ustring name)
{
	const xmlpp::Element * my_element = dynamic_cast<const xmlpp::Element*>(*iter);
	xmlpp::Attribute * my_attribute = my_element->get_attribute(name);
	return my_attribute->get_value();
}

///**
// * nie wiem dlaczego tego jeszcze nie wywalilem - chyba po pijaku to napisa?em :P 
// */
//Glib::ustring xml_explode::get_login_uid() {
//	
//	xmlpp::Node::NodeList::iterator iter = root_node->get_children("from").begin();
//	iter = (*iter)->get_children("user").begin();
//	iter = (*iter)->get_children("nick").begin();
//	return get_all_text_from_iter(iter);
//}

/**
 * 
 */
Glib::ustring xml_explode::get_login_passwd() {
	xmlpp::Node::NodeList::iterator iter = root_node->get_children("from").begin();
	iter = (*iter)->get_children("user").begin();
	iter = (*iter)->get_children("passwd").begin();
	return get_all_text_from_iter(iter);	
}

