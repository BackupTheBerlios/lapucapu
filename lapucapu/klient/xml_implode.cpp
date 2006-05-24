#include "xml_implode.h"

/**
 * Constructors/Destructors
 */
/**
 * Methods
 */
 
 
/**
 * 
 */
xml_implode::xml_implode(Glib::ustring stype) {
	
	type = stype;
	
	try
	{
		parser.set_validate();
		parser.set_substitute_entities();
		parser.parse_file(type + ".xml");
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
 * 
 */
Glib::ustring xml_implode::get_data ( ) {
	return xml->write_to_string();

}

/**
 * 
 */
bool xml_implode::set_message_from (Glib::ustring suid) {
	
	if(type == "message")
	{
		xmlpp::Node::NodeList::iterator iter = root_node->get_children("from").begin();
		iter = (*iter)->get_children("user").begin();
		const xmlpp::Element * element_user = dynamic_cast<const xmlpp::Element*>(*iter);
		xmlpp::Attribute * uid = element_user->get_attribute("uid");
		uid->set_value(suid);
		return true;	
	}
	else
		return false;
}

/**
 * ustawia uid u?ytkownika do ktorego ma trafic wiadomosc
 * na razie tylko jeden odbiorca
 */
bool xml_implode::set_message_to (Glib::ustring suid) {
	
	if(type == "message")
	{
		xmlpp::Node::NodeList::iterator iter = root_node->get_children("to").begin();
		xmlpp::Node::NodeList lista_to = (*iter)->get_children("user");
		uids_list lista;
		iter = lista_to.begin();
		const xmlpp::Element * element_user = dynamic_cast<const xmlpp::Element*>(*iter);
		xmlpp::Attribute * uid = element_user->get_attribute("uid");
		uid->set_value(suid);
		return true;	
	}
	else
		return false;	
}
	
/**
 * 
 */
bool xml_implode::set_message_body (Glib::ustring stext) {

	if(type == "message")
	{
		xmlpp::Node::NodeList::iterator iter = root_node->get_children("body").begin();
		xmlpp::Node::NodeList body_nodes = (*iter)->get_children();
	
		xmlpp::ContentNode* nodeText;
		Glib::ustring out;
		for( iter = body_nodes.begin(); iter != body_nodes.end(); ++iter)
		{
			nodeText = dynamic_cast<xmlpp::ContentNode*>(*iter);
			if(nodeText)
			{
				nodeText->set_content((const Glib::ustring&)stext);
//				stext = nodeText->get_content();
				return true;
			}
		}
		return false;	
	}
	else
		return false;	
}

/**
 * 
 */
bool xml_implode::set_login_data (Glib::ustring suid, Glib::ustring spasswd) {
	
	if(type == "request_login")
	{
		xmlpp::Node::NodeList::iterator iter = root_node->get_children("from").begin();
		iter = (*iter)->get_children("user").begin();
		const xmlpp::Element * element = dynamic_cast<const xmlpp::Element*>(*iter);
		xmlpp::Attribute * uid = element->get_attribute("uid");
		uid->set_value(suid);
		iter = (*iter)->get_children("passwd").begin();
		iter = (*iter)->get_children().begin();
		xmlpp::ContentNode* nodeText = dynamic_cast<xmlpp::ContentNode*>(*iter);
		nodeText->set_content((const Glib::ustring&)spasswd);
		return true;
	}
	else
		return false;
}

/**
 * 
 */
bool xml_implode::set_new_user_passwd (Glib::ustring spasswd) {
	
	if(type == "request_add_user")
	{
		xmlpp::Node::NodeList::iterator iter = root_node->get_children("from").begin();
		iter = (*iter)->get_children("user").begin();
		iter = (*iter)->get_children("passwd").begin();
		iter = (*iter)->get_children().begin();
		xmlpp::ContentNode* nodeText = dynamic_cast<xmlpp::ContentNode*>(*iter);
		nodeText->set_content((const Glib::ustring&)spasswd);
		return true;
	}
	else
		return false;
}

/**
 * 
 */
bool xml_implode::set_status (Glib::ustring suid, Glib::ustring sstatus, Glib::ustring sopis) {
	
	if(type == "request_change_status")
	{
		xmlpp::Node::NodeList::iterator iter = root_node->get_children("from").begin();
		iter = (*iter)->get_children("user").begin();
		const xmlpp::Element * element = dynamic_cast<const xmlpp::Element*>(*iter);
		xmlpp::Attribute * atrybut = element->get_attribute("uid");
		atrybut->set_value(suid);
		
		iter = (*iter)->get_children("status").begin();
		element = dynamic_cast<const xmlpp::Element*>(*iter);
		atrybut = element->get_attribute("state");
		atrybut->set_value(sstatus);
		
		iter = (*iter)->get_children().begin();
		xmlpp::ContentNode* nodeText = dynamic_cast<xmlpp::ContentNode*>(*iter);
		nodeText->set_content((const Glib::ustring&)sopis);
		return true;
	}
	else
		return false;
}


/**
 * request_users_list.xml
 */
bool xml_implode::set_uid_request_users_list (Glib::ustring suid) {
	
	if(type == "request_users_list")
	{
		xmlpp::Node::NodeList::iterator iter = root_node->get_children("from").begin();
		iter = (*iter)->get_children("user").begin();
		const xmlpp::Element * element = dynamic_cast<const xmlpp::Element*>(*iter);
		xmlpp::Attribute * atrybut = element->get_attribute("uid");
		atrybut->set_value(suid);
		return true;
	}
	else
		return false;
}


/**
 * request_users_states.xml
 */
bool xml_implode::set_uid_request_users_states (Glib::ustring suid) {
	
	if(type == "request_users_states")
	{
		xmlpp::Node::NodeList::iterator iter = root_node->get_children("from").begin();
		iter = (*iter)->get_children("user").begin();
		const xmlpp::Element * element = dynamic_cast<const xmlpp::Element*>(*iter);
		xmlpp::Attribute * atrybut = element->get_attribute("uid");
		atrybut->set_value(suid);
		return true;
	}
	else
		return false;
}
