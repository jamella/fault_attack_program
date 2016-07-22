#ifndef ATTACT_CONSTRUCTOR_H
#define ATTACT_CONSTRUCTOR_H

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "Parser/parser.h"
#include "MUX/gate.h"
class constructor
{

friend class Attack_CNF_handler;
friend class print_solution;
friend class Parse_result;

public:
	constructor() = default;
	constructor(const Parser&);

	void show_all(ostream&);
	std::string show_input();
	std::string show_CB();
	std::string show_output();
	std::string show_netlist();
	std::string show_wire();

private:
	std::vector<std::string> PI_list;
	std::vector<std::string> internal_input_list;
	std::vector<std::string> internal_output_list;
	std::vector<std::string> PO_list;

	std::vector<Gate> Gate_list;

	std::vector<std::vector<std::string>> CB_list;
	std::vector<std::vector<std::vector<std::string>>> inports_CB_list;
	std::vector<std::vector<std::string>> wire_list;
	std::vector<std::vector<std::string>> netlist;
};


constructor::constructor(const Parser& info)
{
	PI_list = info.PI;
	PO_list = info.PO;
	for(unsigned i = 0; i != info.gate_counter; ++i)
	{
		internal_input_list.push_back(info.gateInfo.at(i).name + "_in");
	}
	for(unsigned i = 0; i != info.gate_counter; ++i)
	{
		internal_output_list.push_back(info.gateInfo.at(i).name + "_out");
	}

	for(auto g: info.gateInfo)
	{
		Gate gate(g.second.name, g.second.fanin, g.second.index);
		gate.build_gate(info);
		Gate_list.push_back(gate);
		inports_CB_list.push_back(gate.inport_CB_list);
		CB_list.push_back(gate.CB);
		wire_list.push_back(gate.wire);
		netlist.push_back(gate.Gate_model);
	}

}

void constructor::show_all(ostream& outfile)
{
	outfile << show_input();
	outfile << show_CB();
	outfile << show_output();
	outfile << show_wire();
	outfile << show_netlist();
}
std::string constructor::show_input()
{
	std::string out_str;
	out_str += "input ";
	for(auto item: PI_list)
	{
		out_str += item + ",";
	}
	out_str.back() = ';';
	out_str += "\n\n";

	out_str += "input_internal ";
	for(auto item: internal_input_list)
	{
		out_str += item + ",";
	}
	out_str.back() = ';';
	out_str += "\n\n";
	return out_str;
}
std::string constructor::show_CB()
{
	std::string out_str;
	for(auto vec: CB_list)
	{
		out_str += "input_CB ";
		for(auto item: vec)
		{
			out_str += item + ",";
		}
		out_str.back() = ';';
		out_str += "\n\n";
	}
	return out_str;
}

std::string constructor::show_wire()
{
	std::string out_str;
	out_str += "wire ";
	for(auto vec: wire_list)
	{
		for(auto i: vec)
		{
			out_str += i + ",";
		}
	}
	out_str.back() = ';';
	out_str += "\n\n";
	return out_str;
}

std::string constructor::show_output()
{
	std::string out_str;
	out_str += "output_internal ";
	for(auto item: internal_output_list)
	{
		out_str += item + ",";
	}
	out_str.back() = ';';	
	out_str += "\n\n";
	return out_str;
}
std::string constructor::show_netlist()
{
	std::string out_str;
	for(auto vec: netlist)
	{
		for(auto item: vec)
		{
			out_str += item + "\n";
		}
		out_str += "\n";
	}
	return out_str;	
}

#endif