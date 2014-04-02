#include <boost/any.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>

using namespace std;


typedef map<string, map<string, map<string, boost::any> > > BenchmarkDict;


class BasicData
{
public:
    BasicData()
    {
        this->input_data_dir = "./data/";
        this->result_file_path = "./result/cpp/original_dict.serialization";
        this->seed_file_path = "./output/";

        string tool[3] = {"protobuf", "json", "thrift"};
        string data_key[7] = {"input_file_path", "input_data", "se_time",
            "seed_file_str", "seed_file_path", "seed_file_size", "dese_time"};
        string lang[3] = {"python", "java", "cpp"};
        string postfix[1] = {".serialized"};

        this->se_tool_list = list<string>(tool, tool + 3);
        this->se_data_key_list = list<string>(data_key, data_key + 7);
        this->lang_list = list<string>(lang, lang + 3);
        this->rm_postfix_list = list<string>(postfix, postfix + 1);
    }

    const string &get_input_data_dir() const
        { return this->input_data_dir; }
    const string &get_result_file_path() const
        { return this->result_file_path; }
    const string &get_seed_file_path() const
        { return this->seed_file_path; }

    const list<string> &get_tool_list() const
        { return this->se_tool_list; }
    const list<string> &get_key_list() const
        { return this->se_data_key_list; }
    const list<string> &get_lang_list() const
        { return this->lang_list; }
    const list<string> &get_postfix_list() const
        { return this->rm_postfix_list; }

private:
    string input_data_dir, result_file_path, seed_file_path;
    list<string> se_tool_list, se_data_key_list, lang_list, rm_postfix_list;
};


class Scenario
{
public:
private:
}
