#include <boost/any.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/std::string.hpp>


namespace Benchmark {

    typedef std::map<std::string, boost::any> KeyLevel;
    typedef std::map<std::string, KeyLevel> FileLevel;
    typedef std::map<std::string, FileLevel> ToolLevel;
    typedef ToolLevel BenchmarkDict;


    class BasicData
    {
    public:
        BasicData()
        {
            this->input_data_dir = "./data/";
            this->result_file_path = "./result/cpp/original_dict.serialization";
            this->seed_file_path = "./output/";

            std::string tool[3] = {"protobuf", "json", "thrift"};
            std::string data_key[7] = {"input_file_path", "input_data",
                "se_time", "seed_file_str", "seed_file_path", "seed_file_size",
                "dese_time"};
            std::string lang[3] = {"python", "java", "cpp"};

            this->se_tool_list
                = std::list<std::string>(tool, tool + 3);
            this->se_data_key_list
                = std::list<std::string>(data_key, data_key + 7);
            this->lang_list
                = std::list<std::string>(lang, lang + 3);
            this->rm_postfix_list
                = std::list<std::string>(postfix, postfix + 1);
        }

        const std::string &get_input_data_dir() const
            { return this->input_data_dir; }
        const std::string &get_result_file_path() const
            { return this->result_file_path; }
        const std::string &get_seed_file_path() const
            { return this->seed_file_path; }

        const std::list<std::string> &get_tool_list() const
            { return this->se_tool_list; }
        const std::list<std::string> &get_key_list() const
            { return this->se_data_key_list; }
        const std::list<std::string> &get_lang_list() const
            { return this->lang_list; }

    private:
        std::string input_data_dir, result_file_path, seed_file_path;
        std::list<std::string> se_tool_list, se_data_key_list,
            lang_list, rm_postfix_list;
    };

    class Serialized
    {
        friend class boost::serialization::access;
    public:
        Serialized(const BenchmarkDict &dict)
            { this->dict = dict; }

        const BenchmarkDict &get_dict() const
            { return dict; }

    private:
        BenchmarkDict dict;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version)
            { ar & dict; }
    };

    class Scenario
    {
    public:
    private:
    };

}