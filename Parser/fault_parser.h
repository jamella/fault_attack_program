#ifndef PARSER_FAULT_PARSER_H
#define PARSER_FAULT_PARSER_H
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
class Trial
{
public:
	std::vector<bool> input_vector;
	std::vector<bool> output_vector_FF;
	std::vector<bool> output_vector_S1;
	std::vector<bool> output_vector_S0;
	std::string fault_site;

	unsigned fault_case = 0;
};

class Fault_parser
{
	friend class Attack_CNF_handler;
public:
	Fault_parser();
	Fault_parser(std::string path_info);

private:
	void parse_FF_line(const std::string& line, Trial& round);
	void parse_S1_line(const std::string& line, Trial& round);
	void parse_S0_line(const std::string& line, Trial& round);


	std::vector<std::string> parse_line(const std::string&);
	std::vector<bool> stob(const std::string &);
	const std::string path;
	std::ifstream infile;

	std::vector<std::string> list;
	std::vector<Trial> trial_list;
};

Fault_parser::Fault_parser(std::string path_info):path(path_info)
{
	infile.open(path);
	{
		if(!infile.is_open())
		{
			std::cerr << "Fault_parser: file is not open correctly!" << std::endl;
		}
		else
		{
//			parse_head_line(line);
			std::string line;
			while(getline(infile, line))
			{
				Trial round;
				list.push_back(line);
				parse_FF_line(line, round);
				getline(infile, line);
				parse_S1_line(line, round);
				getline(infile, line);
				parse_S0_line(line, round);
				trial_list.push_back(round);
			}
		}
	}
}

void Fault_parser::parse_FF_line(const std::string& line, Trial& round)
{
	auto result = parse_line(line);
	if(result.size() == 4)
	{
		round.input_vector = stob(result.at(0));
		round.output_vector_FF = stob(result.at(3));
		round.fault_site = result.at(1);		
	}
	else
	{
		std::cerr << "trial information is not completed!" << std::endl;
		std::cerr << line << std::endl;
	}

}

void Fault_parser::parse_S1_line(const std::string& line, Trial& round)
{
	auto result = parse_line(line);
	if(result.size() == 4)
	{
		auto result = parse_line(line);
		auto input_vector = stob(result.at(0));
		auto fault_site = result.at(1);
		if((input_vector != round.input_vector) ||(fault_site != round.fault_site))
		{
			std::cerr << "fault information is not matched!!! ->" << std::endl;
			std::cerr << line << std::endl;
			exit(0);
		}
		round.output_vector_S1 = stob(result.at(3));
	}
	else
	{
		std::cerr << "trial information is not completed!" << std::endl;
		std::cerr << line << std::endl;
	}
}

void Fault_parser::parse_S0_line(const std::string& line, Trial& round)
{
	auto result = parse_line(line);	
	if(result.size() == 4)
	{
		auto result = parse_line(line);
		auto input_vector = stob(result.at(0));
		auto fault_site = result.at(1);
		if((input_vector != round.input_vector) ||(fault_site != round.fault_site))
		{
			std::cerr << "fault information is not matched!!! ->" << std::endl;
			std::cerr << line << std::endl;
			exit(0);
		}
		round.output_vector_S0 = stob(result.at(3));
	}
	else
	{
		std::cerr << "trial information is not completed!" << std::endl;
		std::cerr << line << std::endl;
	}
}

std::vector<std::string> Fault_parser::parse_line(const std::string& line)
{
    std::vector<std::string> res;
    if((line.find("#") == std::string::npos) && (line != ""))
    {
        std::regex pattern("([0|1]*);([^;]*);([0|1|-]);([0|1]*)");
        std::smatch result;
        std::regex_search(line, result, pattern);
        res.push_back(result[1].str());
        res.push_back(result[2].str());
        res.push_back(result[3].str());
        res.push_back(result[4].str());

        return res;
    }
    else return res;
}


std::vector<bool> Fault_parser::stob(const std::string &line)
{
    std::vector<bool> result;
    for(auto i: line)
    {
        if(i == '0') result.push_back(false);
        if(i == '1') result.push_back(true);
    }
    return  result;	
}

#endif