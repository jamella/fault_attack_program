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

private:
	std::vector<unsigned> get_CB_index(std::map<std::string, unsigned> CB_list);
	SAT_solver& Sat;
	const netlist_parser_ABC& parser;
	const constructor& circuit;
};

print_solution::print_solution(SAT_solver& Sat_info, const netlist_parser_ABC& parser_info, const constructor& circuit_info,ostream& os):Sat(Sat_info), parser(parser_info), circuit(circuit_info)
{
	auto CB_index = get_CB_index(parser.CB_name_to_index);
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
}


std::vector<unsigned> print_solution::get_CB_index(std::map<std::string, unsigned> CB_list)
{
	std::vector<unsigned> result;
	for(auto CB_name: CB_list)
	{
		result.push_back(parser.varIndexDict.at(CB_name.first));
	}
	return result;
}


#endif