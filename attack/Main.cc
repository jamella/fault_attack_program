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

	SAT_solver solver("model.cnf");

	ofstream out_result("result.txt");
	print_solution print_solu(solver, *atk_psr, cons, out_result);

	Parse_result result("result.txt", cons);
	result.print_solution(cout);
	return 0;
}
