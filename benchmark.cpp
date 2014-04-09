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


using namespace std;


void init_data(const string &data_dir, Benchmark::BenchmarkDict &benchmark_dict)
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
            Benchmark::FileLevel &file_dict = benchmark_dict[*tool_iter];
            file_dict[filename_iter->substr(7)] = Benchmark::KeyLevel();

            for (list<string>::const_iterator key_iter
                    = bd.get_key_list().begin();
                key_iter != bd.get_key_list().end();
                ++key_iter)
            {
                Benchmark::KeyLevel &key_dict
                    = file_dict[filename_iter->substr(7)];

                if (*key_iter == "input_file_path")
                    key_dict[*key_iter]
                        = data_dir + filename_iter->substr(7);
                else if (*key_iter == "input_data")
                {
                    ifstream fin(filename_iter->c_str());
                    istreambuf_iterator<char> beg(fin), end;
                    string json_str(beg, end);
                    fin.close();

                    stringstream ss(json_str);
                    bp::ptree pt;
                    bp::read_json(ss, pt);

                    key_dict[*key_iter] = pt;
                }
                else
                    key_dict[*key_iter] = NULL;
            }
        }
    }
}


void test_go(Benchmark::BenchmarkDict &data_dict)
{
    for (Benchmark::BenchmarkDict::iterator tool_iter = data_dict.begin();
        tool_iter != data_dict.end();
        ++tool_iter)
    {
        cout << "  Tool: " << tool_iter->first << endl;

        Benchmark::FileLevel &file_level = data_dict[tool_iter->first];

        for (Benchmark::FileLevel::iterator file_iter = file_level.begin();
            file_iter != file_level.end();
            ++file_iter)
        {
            Benchmark::KeyLevel &data_info = file_level[file_iter->first];

            Benchmark::AddressBook ins(tool_iter->first, data_info);
            ins.serialization();

            string output_path = "./output/cpp/" + tool_iter->first + "/"
                + file_iter->first.substr(0, file_iter->first.length() - 5)
                + ".serialized";

            ofstream output_file(output_path.c_str());
            output_file << boost::any_cast<string>(data_info["seed_file_str"]);
            output_file.close();

            data_info["seed_file_path"] = output_path;
            data_info["seed_file_size"]
                = boost::any_cast<string>(data_info["seed_file_str"]).length();

            ins.deserialization();
        }
    }
}


void result_dict_output(const string &result_file_path,
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
                = any_cast<string>(file_iter->second["seed_file_str"]).length();

            se_file_level[file_iter->first] = se_key_level;
        }

        se_tool_level[tool_iter->first] = se_file_level;
    }


    ofstream serialized_file(result_file_path.c_str());
    archive::text_oarchive oa(serialized_file);

    oa << se_obj;

    serialized_file.close();
}


int main(int argc, char *argv[])
{
    Benchmark::BasicData bd;
    
    // Initializing information dictionary by data in path data_dir
    cout << "Init. Data\n";
    Benchmark::BenchmarkDict benchmark_dict;
    init_data(bd.get_input_data_dir(), benchmark_dict);
    
    // Running Benchmark
    cout << "Running Benchmark\n";
    test_go(benchmark_dict);
    
    // Processing the result and generating statistic results
    cout << "Saving Result to boost::serialization.\n";
    result_dict_output(bd.get_result_file_path(), benchmark_dict);

    exit(0);
}

