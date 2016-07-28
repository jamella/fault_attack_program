
#ifndef MUX_MUX_H
#define MUX_MUX_H
#include <vector>
#include <string>
#include "utils/tools.h"

class Standard_Mux
{

friend class Gate;

public:
    Standard_Mux() = default;
    Standard_Mux(const std::vector<std::string> &input_info, const std::vector<std::string> &CB_info, const std::string &output_info, const unsigned& i);
    std::vector<std::string> to_vector(const unsigned& gate_index);
private:
    std::vector<std::string> two_input_MUX(const std::string &in_info1,const std::string &in_info2 , std::string &out_info, const std::string &CB_info, const unsigned& unit_index, const unsigned& level_index, const bool flag);
    std::vector<std::string> model;
    std::vector<std::string> input;
    std::vector<std::string> wire;
    std::vector<std::string> CB;
    std::string output;

    unsigned gate_index;
};

Standard_Mux::Standard_Mux(const std::vector<std::string> &input_info, const std::vector<std::string> &CB_info,
                           const std::string &output_info, const unsigned& i)
{
    input = input_info;
    CB = CB_info;
    output = output_info;
    gate_index = i;
    unsigned input_size = input_info.size();

    unsigned num_of_mux_this_line = input_size/2;
    unsigned level_counter = 0;
    // to initiate the generation, make input for this line to be PI, size for this line to be half of input, make level counter = 0
    std::vector<std::string> input_for_this_line = input_info;
    std::vector<std::string> output_for_this_line;
    std::string CB_for_this_line;

    while(num_of_mux_this_line != 1)            // for each line of tree, is this line is not the root
    {
        CB_for_this_line = CB_info[level_counter];

        //============================================
        /*
        std::cerr << "line num = " << level_counter << std::endl;
        std::cerr << std::endl;
        std::cerr << "Num of mux for this line: " << num_of_mux_this_line << std::endl;
        std::cerr << std::endl;
        std::cerr << "Input for this line:" << std::endl;
        for (auto i: input_for_this_line) {
            std::cerr << i << std::endl;
        }
        std::cerr << std::endl;
        std::cerr << "CB for this line:" << std::endl;
        std::cerr << CB_for_this_line << std::endl;
        std::cerr << std::endl;
         */
        //============================================

        for (unsigned num_of_unit = 0;
             num_of_unit != num_of_mux_this_line; ++num_of_unit)   // for each mux in this line
        {
            std::string out_info;
            model += two_input_MUX(input_for_this_line[2*num_of_unit], input_for_this_line[2*num_of_unit + 1] ,out_info, CB_for_this_line, num_of_unit, level_counter, true);  // use 2 input, 1 CB and its unique index to generate a model and out_info
            output_for_this_line.push_back(out_info);
            wire.push_back(out_info);
        }

        ++level_counter;
        input_for_this_line = output_for_this_line;
        num_of_mux_this_line = num_of_mux_this_line / 2;

        //============================================
        /*
        std::cerr << "output for this line:" << std::endl;
        for (auto i: input_for_this_line) {
            std::cerr << i << std::endl;
        }
        std::cerr << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
        */
        //============================================
        output_for_this_line.clear();
    }
    CB_for_this_line = CB_info[level_counter];
    model += two_input_MUX(input_for_this_line[0], input_for_this_line[1] ,output, CB_for_this_line, 0, level_counter, false);

    //============================================
    /*
    std::cerr << "line num = " << level_counter << std::endl;
    std::cerr << std::endl;
    std::cerr << "Num of mux for this line: " << num_of_mux_this_line << std::endl;
    std::cerr << std::endl;
    std::cerr << "Input for this line:" << std::endl;
    for (auto i: input_for_this_line) {
        std::cerr << i << std::endl;
    }
    std::cerr << std::endl;
    std::cerr << "CB for this line:" << std::endl;
    std::cerr << CB_for_this_line << std::endl;
    std::cerr << std::endl;
    */
    //============================================

    
}

std::vector<std::string> Standard_Mux::two_input_MUX(const std::string &in_info1,const std::string &in_info2 , std::string &out_info, const std::string &CB_info, const unsigned& unit_index, const unsigned& level_index, const bool flag)
{

    std::vector<std::string> result;
    std::string inv_out = "gate" + std::to_string(gate_index) + "_body_" + std::to_string(level_index) + "_" + std::to_string(unit_index) + "_inv";
    std::string and1_out = "gate" + std::to_string(gate_index) + "_body_" + std::to_string(level_index) + "_" + std::to_string(unit_index) + "_and0";
    std::string and2_out = "gate" + std::to_string(gate_index) + "_body_" + std::to_string(level_index) + "_" + std::to_string(unit_index) + "_and1";
    std::string or_out = "gate" + std::to_string(gate_index) +"_body_" + std::to_string(level_index) + "_" + std::to_string(unit_index) + "_or";
    result.push_back("inv1 gate( .a(" + CB_info +  "), .O(" + inv_out +") );");
    result.push_back("and2 gate( .a(" + in_info1 +"), .b(" + inv_out + "), .O(" + and1_out + ") );");
    result.push_back("and2 gate( .a(" + in_info2 +"), .b(" + CB_info + "), .O(" + and2_out + ") );");
    if(flag)
    {
        result.push_back("or2 gate( .a(" + and1_out + "), .b(" + and2_out + "), .O(" + or_out +") );");
        out_info = or_out;
    }
    else
    {
        result.push_back("or2 gate( .a(" + and1_out + "), .b(" + and2_out + "), .O(" + out_info +") );");
    }
    wire.push_back(inv_out);
    wire.push_back(and1_out);
    wire.push_back(and2_out);
    return result;
}

std::vector<std::string> Standard_Mux::to_vector(const unsigned &gate_index)
{
    auto result = model;
    for(auto &i: result)
    {
        while(i.find("INDEX") != std::string::npos)
        i.replace(i.find("INDEX"), 5, std::to_string(gate_index));
    }
    return result;
}


class Cool_Mux
{
friend class Gate;    
public:
    Cool_Mux() = default;
    Cool_Mux(const std::vector<std::string> &input_info, const std::vector<std::string> &CB_info, const std::string &output_info, const unsigned &i, const unsigned &f);
    std::vector<std::string> to_vector(const unsigned& gate_index, const unsigned& inport_index);
private:
    std::string create_AND(const std::string &input1, const std::string &input2, const std::string &output);
    std::string create_OR(const std::vector<std::string> inputs, const std::string output_info);
    std::vector<std::string> model;
    std::vector<std::string> input;
    std::vector<std::string> CB;
    std::vector<std::string> wire;
    std::string output;

    unsigned gate_index;
    unsigned fanin_index;
    unsigned Mux_size;
};

Cool_Mux::Cool_Mux(const std::vector<std::string> &input_info, const std::vector<std::string> &CB_info, const std::string &output_info, const unsigned &i, const unsigned& f)
{
    input = input_info;
    CB = CB_info;
    gate_index = i;
    fanin_index = f;
    output = output_info;
    Mux_size = input_info.size();
    std::vector<std::string> AND_output;

    for(unsigned index = 0; index != Mux_size; ++index)
    {
        std::string this_and_output_wire = "gate" + std::to_string(gate_index) + "_fanin" + std::to_string(fanin_index) + "_and" + std::to_string(index);    // "1_fanin1_and1"
        model.push_back(create_AND(input_info[index], CB_info[index], this_and_output_wire));
        AND_output.push_back(this_and_output_wire);
    }
    model.push_back(create_OR(AND_output, output_info));

}

std::string Cool_Mux::create_AND(const std::string &input1, const std::string &input2, const std::string &output)
{
    wire.push_back(output);
    return "and2 gate( .a(" + input1 + "), .b(" + input2 + "), .O(" + output + ") );";
}
std::string Cool_Mux::create_OR(const std::vector<std::string> inputs, const std::string output_info)
{
    std::string result = "or" + std::to_string(Mux_size) + " gate(";
    int index = 1;
    for(auto i: inputs)
    {
        result += " .";
        result += std::to_string(index++);
        result.push_back('(');
        result += i + "),";
    }
    result += " .O(" + output_info + ") );";
    return result;
}

std::vector<std::string> Cool_Mux::to_vector(const unsigned& gate_index, const unsigned& inport_index)
{
    auto result = model;
    for(auto &i: result)
    {
        while(i.find("INDEX") != std::string::npos)
            i.replace(i.find("INDEX"), 5, std::to_string(gate_index));
        while(i.find("NUM") != std::string::npos)
            i.replace(i.find("NUM"), 3, std::to_string(inport_index));
    }
    return result;
}

#endif //MUX_MUX_H




