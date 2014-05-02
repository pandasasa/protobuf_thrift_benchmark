#include <cstdlib>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>

#include "lib/cpp/address_book.hpp"
#include "result/cpp/analyse.hpp"


using namespace std;


void test_go(const string &data_dir, Benchmark::BenchmarkDict &benchmark_dict)
{
    namespace bf = boost::filesystem;
    namespace bp = boost::property_tree;

    Benchmark::BasicData bd;
    vector<string> data_file_list;

    bf::path input_data_path(data_dir.c_str());
    bf::directory_iterator end_iter;
    for (bf::directory_iterator iter(input_data_path);
        iter != end_iter;
        ++iter)
    {
        if (bf::is_regular_file(iter->path()))
        {
            string file_path(iter->path().string());
            string ext = file_path.substr(file_path.length() - 5, 5);

            if (ext == ".json")
                data_file_list.push_back(iter->path().string());
        }
    }

    for (list<string>::const_iterator tool_iter = bd.get_tool_list().begin();
        tool_iter != bd.get_tool_list().end();
        ++tool_iter)
    {
        benchmark_dict[*tool_iter] = Benchmark::FileLevel();

        for (vector<string>::iterator filename_iter = data_file_list.begin();
            filename_iter != data_file_list.end();
            ++filename_iter)
        {
            // Initializating the data
            Benchmark::FileLevel &file_dict = benchmark_dict[*tool_iter];
            file_dict[filename_iter->substr(7)] = Benchmark::KeyLevel();
            Benchmark::KeyLevel &data_info
                = file_dict[filename_iter->substr(7)];

            ifstream data_file(filename_iter->c_str());
            istreambuf_iterator<char> begin(data_file), end;
            string json_str(begin, end);
            data_file.close();

            stringstream ss(json_str);
            bp::ptree pt;
            bp::read_json(ss, pt);

            data_info["input_data"] = pt;

            // Running Benchmark
            Benchmark::AddressBook ins(*tool_iter, data_info);
            ins.serialization();

            string output_path = "./output/cpp/" + *tool_iter + "/"
                + filename_iter->substr(0, filename_iter->length() - 5)
                + ".serialized";

            ofstream output_file(output_path.c_str());
            output_file << boost::any_cast<string>(data_info["seed_file_str"]);
            output_file.close();

            int size 
                = boost::any_cast<string>(data_info["seed_file_str"]).length();
            data_info["seed_file_size"] = size;

            ins.deserialization();

            // Clear the original string and serialized string
            // for reducing the size of boost::serialization file
            data_info.erase("input_data");
            data_info.erase("seed_file_str");
        }
    }
}


const Benchmark::SeToolLevel result_dict_output(const string &result_file_path,
        Benchmark::BenchmarkDict &result_dict)
{
    using namespace Benchmark;
    using namespace boost;

    Serializable se_obj;
    SeToolLevel &se_tool_level = se_obj.se_obj;
    
    for (BenchmarkDict::iterator tool_iter = result_dict.begin();
        tool_iter != result_dict.end();
        ++tool_iter)
    {
        SeFileLevel se_file_level;

        for (FileLevel::iterator file_iter = tool_iter->second.begin();
            file_iter != tool_iter->second.end();
            ++file_iter)
        {
            SeKeyLevel se_key_level;

            se_key_level["se_time"]
                = any_cast<long>(file_iter->second["se_time"]);
            se_key_level["dese_time"]
                = any_cast<long>(file_iter->second["dese_time"]);
            se_key_level["seed_file_size"]
                = any_cast<int>(file_iter->second["seed_file_size"]);

            se_file_level[file_iter->first] = se_key_level;
        }

        se_tool_level[tool_iter->first] = se_file_level;
    }


    ofstream serialized_file(result_file_path.c_str());
    archive::text_oarchive oa(serialized_file);

    oa << se_obj;

    serialized_file.close();

    return se_tool_level;
}


int main(int argc, char *argv[])
{
    Benchmark::BasicData bd;
    
    // Initializing data and running benchmark
    cout << "  Init. Data\n";
    Benchmark::BenchmarkDict benchmark_dict;
    test_go(bd.get_input_data_dir(), benchmark_dict);
    
    // Processing the result and generating statistic results
    cout << "  Saving Result to boost::serialization.\n";
    Benchmark::SeToolLevel se_dict
        = result_dict_output(bd.get_result_file_path(), benchmark_dict);

    // Outputing result to txt files
    cout << "  Outputing result to txt files.\n";

    string config_file_path("./gen.config");
    string output_path("./result/cpp/");

    Analyser analyser(se_dict, config_file_path, output_path);
    analyser.default_write();

    exit(0);
}

