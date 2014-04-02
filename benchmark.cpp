#include <cstdlib>
#include <iostream>
#include <fstream>

#include <string>
#include <map>
#include <list>

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/filesystem.hpp>

#include "lib/cpp/basic_lib.hpp"


using namespace std;


void init_data(const string &data_dir, BenchmarkDict &benchmark_dict)
{
    BasicData bd;
    namespace bf = boost::filesystem;

    list<string>::iterator tool_iter = bd.get_tool_list().begin();

    while (tool_iter != bd.get_tool_list().end())
    {
        bf::path input_data_path(data_dir.c_str());
        tool_iter++;
    }
}


void test_go()
{
}

void result_dict_output(const string &result_file_path,
        const BenchmarkDict &result_dict)
{
}


void clean_workspace()
{
}

void data_generating(const string &template_path,
        const string &output_path,
        const map<string, int> &config_dict)
{
    ;
}


const map<string, int> parse_config(istream &file_obj)
{
    map<string, int> result;

    while (!file_obj.eof())
    {
        char buf[64];
        file_obj.getline(buf, 63);
        string line(buf);
        vector<string> key_value;
        boost::split(key_value, line, boost::is_any_of("="));

        if (key_value.size() != 2)
            continue;

        int value = atoi(key_value[1].c_str())
        result.insert(map<string, int>::value_type(key_value[0], value));
    }

    return result;
}

int main(int argc, char *argv[])
{
    // CLeaning workspace
    cout << "CLeaning Workspace\n";
    
    // Testing Data Generating
    cout << "Generating Test Data\n";
    
    // Initializing information dictionary by data in path data_dir
    cout << "Init. Data\n";
    
    // Running Benchmark
    cout << "Running Benchmark\n";
    
    // Processing the result and generating statistic results
    cout << "Saving Result to C++ Serialization.\n";

    exit(0);
}
