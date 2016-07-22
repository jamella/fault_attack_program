#ifndef PARSER_ATTACK_PARSER_H
#define PARSER_ATTACK_PARSER_H

#include "Parser/netlist_parser.h"
class Attack_parser: public netlist_parser_ABC
{
public:
	Attack_parser() = default;
	Attack_parser(const std::string path):netlist_parser_ABC(path){net_counter = 1;};

	virtual ~Attack_parser() = default;
	virtual void parse_circuit();


private:
	virtual void parse_PI(const std::string&);
	virtual void parse_CB(const std::string&);
	virtual void parse_internal_PI(const std::string&);
	virtual void parse_internal_PO(const std::string&);
	virtual void parse_wire(const std::string&);
//	virtual void parse_gate(std::string&);

	std::map<std::string, unsigned> PI_internal_name_to_index;
	std::map<std::string, unsigned> PO_internal_name_to_index;

	std::map<unsigned, std::string> PI_internal_index_to_name;
	std::map<unsigned, std::string> PO_internal_index_to_name;
	unsigned net_counter;	
};

void Attack_parser::parse_circuit()
{
	for(auto line: Vline)
	{
		strip_all(line, "\r");
		strip_all(line, "\n");
		if(line.find("input_internal") != std::string::npos) parse_internal_PI(line);
		else if(line.find("input_CB") != std::string::npos) parse_CB(line);
		else if(line.find("output_internal") != std::string::npos) parse_internal_PO(line);
		else if(line.find("input") != std::string::npos) parse_PI(line);
		else if(line.find("wire") != std::string::npos) parse_wire(line);
		else if(line != "") parse_gate(line);
	}
}

void Attack_parser::parse_PI(const std::string& line)
{
	auto net_list = split_wire_info(line, "input");
	for(auto net: net_list)
	{
		varIndexDict.insert(std::pair<std::string, unsigned>(net, net_counter));
		indexVarDict.insert(std::pair<unsigned, std::string>(net_counter, net));
		PI_name_to_index.insert(std::pair<std::string, unsigned>(net, net_counter));
		PI_index_to_name.insert(std::pair<unsigned, std::string>(net_counter, net));
		++net_counter;
	}
}

void Attack_parser::parse_internal_PI(const std::string& line)
{
	auto net_list = split_wire_info(line, "input_internal");
	for(auto net: net_list)
	{
		varIndexDict.insert(std::pair<std::string, unsigned>(net, net_counter));
		indexVarDict.insert(std::pair<unsigned, std::string>(net_counter, net));
		PI_internal_name_to_index.insert(std::pair<std::string, unsigned>(net, net_counter));
		PI_internal_index_to_name.insert(std::pair<unsigned, std::string>(net_counter, net));
		++net_counter;
	}		
}

void Attack_parser::parse_CB(const std::string& line)
{
	auto net_list = split_wire_info(line, "input_CB");
	for(auto net: net_list)
	{
		varIndexDict.insert(std::pair<std::string, unsigned>(net, net_counter));
		indexVarDict.insert(std::pair<unsigned, std::string>(net_counter, net));
		CB_name_to_index.insert(std::pair<std::string, unsigned>(net, net_counter));
		CB_index_to_name.insert(std::pair<unsigned, std::string>(net_counter, net));
		++net_counter;
	}
	
}

void Attack_parser::parse_internal_PO(const std::string& line)
{
	auto net_list = split_wire_info(line, "output_internal");
	for(auto net: net_list)
	{
		varIndexDict.insert(std::pair<std::string, unsigned>(net, net_counter));
		indexVarDict.insert(std::pair<unsigned, std::string>(net_counter, net));
		PO_internal_name_to_index.insert(std::pair<std::string, unsigned>(net, net_counter));
		PO_internal_index_to_name.insert(std::pair<unsigned, std::string>(net_counter, net));
		++net_counter;
	}
}
void Attack_parser::parse_wire(const std::string& line)
{
	auto net_list = split_wire_info(line, "wire");
	for(auto net: net_list)
	{
		varIndexDict.insert(std::pair<std::string, unsigned>(net, net_counter));
		indexVarDict.insert(std::pair<unsigned, std::string>(net_counter, net));
		wire_name_to_index.insert(std::pair<std::string, unsigned>(net, net_counter));
		wire_index_to_name.insert(std::pair<unsigned, std::string>(net_counter, net));
		++net_counter;
	}
}

#endif