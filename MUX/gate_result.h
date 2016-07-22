#ifndef MUX_GATE_RESULT_H
#define MUX_GATE_RESULT_H
#include "MUX/gate_result.h"
#include "attack/constructor.h"
#include <string>
class Gate_result : public virtual Gate
{
public:
	Gate_result() = default;
	Gate_result(const std::string& gate_name, const unsigned& fanin_num, const unsigned& Gate_index):Gate(gate_name, fanin_num, Gate_index){}	

private:
	std::vector<std::vector<int>> inport_CB_value;
	std::vector<int> body_CB_value;
	std::vector<>

}



#endif