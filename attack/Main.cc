#include <iostream>
#include <fstream>
#include "string"
#include "simp/SimpSolver.h"
#include "Parser/attack_parser.h"
#include "Parser/parser.h"
#include "Parser/fault_parser.h"
#include "attack/constructor.h"
#include "CNF/Attack_CNF_handler.h"
#include "solver/SAT.h"
#include "solver/print_solution.h"
#include "Parser/Parse_result.h"

using namespace std;

int find_index(string line)
{
 	regex pattern("-?([0-9]*)\\s0");
	smatch result;

	regex_search(line,result,pattern);
	return stoi(result[1].str());
}

int main(int argc, char const *argv[])
{
	cout << "Project started" << endl;
	string gate_info(argv[1]);
	string fault_info(argv[2]);

	cout << "READing: " << gate_info << endl;
	cout << "READing: " << fault_info << endl;
	Parser par(gate_info);
	constructor cons(par);
	ofstream outfile("model.v");
	cons.show_all(outfile);
	cerr << "Finish generating netlist" << endl;

	Attack_parser *atk_psr = new Attack_parser("model.v");
	atk_psr->parse_circuit();
	cerr << "finish parse circuit" << endl;
	Fault_parser flt_psr(fault_info);
	cerr << "Finish analyze fault info" << endl;
	Attack_CNF_handler att_CNF(atk_psr, cons);
	cerr << "Finish att cnf" << endl;
	att_CNF.start_attack(flt_psr);
	cerr << "Finish building final CNF" <<endl;

	ofstream out_cnf("model.cnf");
	att_CNF.print_file(out_cnf);


	ifstream add_cnf("add_cnf.cnf");
	string line;
	while(getline(add_cnf, line))
	{
//		std::cerr << "assigning: " << find_index(line) << std::endl;
//		cerr << "add :" << atk_psr->indexVarDict.at(find_index(line)) << " " << line << endl;
		ofstream out_add_cnf("model.cnf", std::fstream::app);
		out_add_cnf << line << endl;	


//		string CMD;
//		cin >> CMD;
//		if(CMD != "g") break;
	}
		SAT_solver solver("model.cnf");

		ofstream out_result("result.txt");
		print_solution print_solu(solver, *atk_psr, cons, out_result);

		Parse_result result("result.txt", cons);
		result.print_solution(cout);

		cout << "Num of CB = " << atk_psr->CBs.size() << endl;
		cout << "Num of Var = " << atk_psr->varIndexDict.size() << endl;
		string word;
		while(1)
		{
			cout << "enter var to check : ";
			cin >> word;
			if(word == "q") break;
			else
			{
				cout << word << " = " << atk_psr->varIndexDict.at(word) << endl;
			}			
		}
		while(1)
		{
			cout << "enter index to check : ";
			cin >> word;
			if(word == "q") break;
			else
			{
				cout << word << " = " << atk_psr->indexVarDict.at(stoi(word)) << endl;
			}			
		}


/*
	SAT_solver solver("model.cnf");

	ofstream out_result("result.txt");
	print_solution print_solu(solver, *atk_psr, cons, out_result);

	Parse_result result("result.txt", cons);
	result.print_solution(cout);	
*/
	return 0;
}
