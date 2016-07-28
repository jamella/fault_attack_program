#ifndef PARSER_PARSE_RESULT_H
#define PARSER_PARSE_RESULT_H
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include "attack/constructor.h"
#include "MUX/gate.h"
#include "utils/tools.h"
class Parse_result
{
public:
	Parse_result() = default;
	Parse_result(std::string p, constructor cons);
	void print_solution(ostream&);

private:
	std::vector<std::string> derive_info(const std::string&);
	void assign_gate(int, std::string, int, int);
	int find_fan_index(std::string);	

	std::string path;
	constructor cons;
	std::vector<Gate> gate_list;
};

Parse_result::Parse_result(std::string p, constructor c):path(p), cons(c)
{
	gate_list = cons.Gate_list;
	ifstream infile(path);
	if(!infile.is_open()) std::cerr << "file: " << path << " is not open!!" << std::endl;
	else
	{
		std::string line;
		while(getline(infile, line))
		{
//			std::cerr << "line is: " << line << std::endl;
			if((line != "") && (line.find("SAT") == std::string::npos))
			{
				auto info = derive_info(line);
				int gate_index = std::stoi(info.at(1));
				std::string type = info.at(2);
				int index = std::stoi(info.at(3));
				int value = std::stoi(info.at(4));
				assign_gate(gate_index, type, index, value);
			}
		}
	}
}

std::vector<std::string> Parse_result::derive_info(const std::string& info)
{
	std::vector<std::string> res;
	std::smatch result;
	std::regex pattern("gate([0-9]*)_(.*)_CB_([0-9]*)\\s=\\s([0-1]*)");
	regex_search(info, result, pattern);
	for(auto ele: result)
	{
		res.push_back(ele.str());
//		std::cerr << "\t" << ele << std::endl;
	}
	return res;
}

int Parse_result::find_fan_index(std::string fan_type)
{
	std::smatch result;
	std::regex pattern("fanin([0-9]*)");
	regex_search(fan_type, result, pattern);
	auto res = result[1].str();
	return std::stoi(res);
}	

void Parse_result::assign_gate(int gate_index, std::string type, int index, int value)
{
	auto& gate = gate_list.at(gate_index);
	if(type == "body")
	{
		gate.body_CB_value.push_back(value);

	}
	else if(type.find("fanin") != std::string::npos)
	{
		auto fan_index = find_fan_index(type);	
		gate.inport_CB_value.at(fan_index).push_back(value);
	}

}

void Parse_result::print_solution(ostream& s)
{
	s << "gate_index\tgate_output\tgate_body_value\t\tgate_input_net" << std::endl;
	for(unsigned index = 0; index != gate_list.size(); ++index)
	{
		gate_list.at(index).decide_gate();
		s << index << "\t\t" << gate_list.at(index).name << "\t\t";
		for(auto value: gate_list.at(index).body_CB_value)
		{
			s << value;
		}
		s << "\t\t";
		for(auto in: gate_list.at(index).real_input_name)
		{
			s << in << " ";
		}
		s << std::endl;
	}
}

#endif