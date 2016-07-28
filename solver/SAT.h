#ifndef SOLVER_SAT_H
#define SOLVER_SAT_H
#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
#include <string>
#include <cmath>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>
#include "simp/SimpSolver.h"
#include "utils/System.h"
#include "utils/ParseUtils.h"
#include "utils/Options.h"
#include "core/Dimacs.h"

using namespace Minisat;

class SAT_solver
{
friend class print_solution;
public:
	SAT_solver() = default;
	SAT_solver(const std::string& path);

	void solve(std::vector<unsigned>& freeze_variable);
//	void solve();

	void print_solution(std::ostream& os);
private:
	SimpSolver minisat;
    std::string result;

	void freeze(const std::vector<unsigned>& freeze_variable);					
};


SAT_solver::SAT_solver(const std::string& path)
{
	result = "NULL";
	gzFile in = gzopen(path.c_str(), "rb");
	parse_DIMACS(in, minisat);
	gzclose(in);
}

void SAT_solver::print_solution(std::ostream& os)
{
	if(result == "NULL")
	{
		os << "SAT_solver: solver not yet started!!!" << std::endl;
		return;
	}
	else
	{
		os << result + "\n";
		for(int i = 0; i != minisat.nVars(); ++i)
		{
			if(minisat.model[i] != l_Undef)
			{
				os << " ";
				if(minisat.model[i] == l_True) os << "" + std::to_string(i + 1);
				else os << "-" + std::to_string(i + 1);
			}
		}
		os << std::endl;
	}
}


void SAT_solver::solve(std::vector<unsigned>& freeze_variable)
//void SAT_solver::solve()
{
	freeze(freeze_variable);
	minisat.eliminate(false);
	if(!minisat.okay())
	{
		result = "UNSAT";
		return;
	}

	vec<Lit> dummy;
	lbool ret = minisat.solveLimited(dummy);

	if(ret == l_True)
	{
		result = "SAT";
	}
	else if(ret == l_False) 
	{
		result = "UNSAT";
		return;
	}
	else 
	{
		result = "INDET";
		return;
	}
}

void SAT_solver::freeze(const std::vector<unsigned>& freeze_variable)
{
	static int already_frozen = 0;
	if(already_frozen == 0)
	{
		for(auto i: freeze_variable)
		{
			minisat.setFrozen(i - 1, true);
		}
		++already_frozen;
	}
}				

#endif