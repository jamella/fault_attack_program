#ifndef SOLVER_ATTACK_CNF_HANDLER_H
#define SOLVER_ATTACK_CNF_HANDLER_H
#include <iostream>
#include <vector>
#include "solver/SAT.h"
#include "Parser/netlist_parser.h"
#include "attack/constructor.h"

class print_solution
{
public:
	print_solution() = default;
	print_solution(SAT_solver& Sat_info, const netlist_parser_ABC& parser_info, const constructor& circuit_info, ostream& os);


	std::vector<unsigned> get_CB_index(std::vector<std::string> CB_list);
	SAT_solver& Sat;
	int get_CB_index(std::string line);
	const netlist_parser_ABC& parser;
	const constructor& circuit;
};

print_solution::print_solution(SAT_solver& Sat_info, const netlist_parser_ABC& parser_info, const constructor& circuit_info,ostream& os):Sat(Sat_info), parser(parser_info), circuit(circuit_info)
{
	auto CB_index = get_CB_index(parser.CBs);
	Sat.solve(CB_index);
	if(Sat.result == "SAT")
	{
		os << "SAT" << std::endl;
		for(auto CB: CB_index)
		{
			os << parser.indexVarDict.at(CB) << " = ";
			if(Sat.minisat.model[CB - 1] == l_True) os << "1" << std::endl;
			else if(Sat.minisat.model[CB - 1] == l_False) os << "0" << std::endl;
			else os << "Undef" << std::endl;
		}
	}
	else os << Sat.result << std::endl;

	ofstream outfile("SAT.solu");	
	if(Sat.result == "SAT")
	{	
		for(auto ele: parser.POs)
		{
			if(Sat.minisat.model[parser.varIndexDict.at(ele) - 1] != l_Undef)
			{
				outfile << " ";
				if(Sat.minisat.model[parser.varIndexDict.at(ele) - 1] == l_True) outfile << ele << "  = 1\n";
				else outfile << ele << "  = 0\n";
			}
		}
		outfile << std::endl;
		for(auto ele: parser.PIs)
		{
			if(Sat.minisat.model[parser.varIndexDict.at(ele) - 1] != l_Undef)
			{
				outfile << " ";
				if(Sat.minisat.model[parser.varIndexDict.at(ele) - 1] == l_True) outfile << ele << "  = 1\n";
				else outfile << ele << "  = 0\n";
			}
		}
		outfile << std::endl;
	}
		else outfile << Sat.result << std::endl;	
}


std::vector<unsigned> print_solution::get_CB_index(std::vector<std::string> CB_list)
{
	std::vector<unsigned> result;
	for(auto CB_name: CB_list)
	{
		result.push_back(parser.varIndexDict.at(CB_name));
	}
	return result;
}

int print_solution::get_CB_index(std::string line)
{
 	regex pattern("gate[0-9]*_[0-9a-z]*_CB_([0-9]*)");
	smatch result;

	regex_search(line,result,pattern);
	return stoi(result[1].str());
}
#endif