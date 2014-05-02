#ifndef BASIC_LIB_HPP
#define BASIC_LIB_HPP

#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <jsoncpp/json/json.h>

namespace Benchmark {

    typedef std::map<std::string, Json::Value> KeyLevel;
    typedef std::map<std::string, KeyLevel> FileLevel;
    typedef std::map<std::string, FileLevel> ToolLevel;
    typedef ToolLevel BenchmarkDict;
    
    typedef std::map<std::string, int> SeKeyLevel;
    typedef std::map<std::string, SeKeyLevel> SeFileLevel;
    typedef std::map<std::string, SeFileLevel> SeToolLevel;


    class Serializable
    {
    public:
        SeToolLevel se_obj;

        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            ar & se_obj;
        }
    };


    class BasicData
    {
    public:
        BasicData()
        {
            this->input_data_dir = "./data/";
            this->result_file_path = "./result/cpp/original_dict.serialization";
            this->seed_file_path = "./output/";

            std::string tool[] = {"protobuf", "json", "thrift_compact",
                "thrift_binary"};
            std::string data_key[] = {"input_data", "se_time", "seed_file_str",
                "seed_file_size", "dese_time"};

            this->se_tool_list
                = std::list<std::string>(tool, tool + 4);
            this->se_data_key_list
                = std::list<std::string>(data_key, data_key + 5);
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


    class Scenario
    {
    public:
        Scenario(const std::string &tool, KeyLevel &data_info_dict)
            : tool(tool), data_dic(data_info_dict) {}

        void serialization() 
        {
            long final_time = -1;
            long se_time = 0;

            for (int i = 0; i < this->REPEAT_TIME; i++)
            {
                if (this->tool == "protobuf")
                    se_time = this->se_protobuf();
                else if (this->tool == "thrift_compact")
                    se_time = this->se_thrift_compact();
                else if (this->tool == "thrift_binary")
                    se_time = this->se_thrift_binary();
                else
                    se_time = this->se_json();
                
                if ((final_time == -1 && se_time > 0)
                        || (final_time > se_time && se_time > 0))
                    final_time = se_time;
                else
                    continue;
            }

            this->data_dic["se_time"] = Json::Value();
            this->data_dic["se_time"]["value"] = static_cast<int>(final_time);
        }

        void deserialization()
        {
            long final_time = -1;
            long de_time = 0;

            for (int i = 0; i < this->REPEAT_TIME; i++)
            {
                if (this->tool == "protobuf")
                    de_time = this->de_protobuf();
                else if (this->tool == "thrift_compact")
                    de_time = this->de_thrift_compact();
                else if (this->tool == "thrift_binary")
                    de_time = this->de_thrift_binary();
                else
                    de_time = this->de_json();

                if ((final_time == -1 && de_time > 0)
                        || (final_time > de_time && de_time > 0))
                    final_time = de_time;
                else
                    continue;
            }

            this->data_dic["dese_time"] = Json::Value();
            this->data_dic["dese_time"]["value"]
                = static_cast<int>(final_time);
        }

        virtual long se_protobuf() = 0;
        virtual long se_thrift_compact() = 0;
        virtual long se_thrift_binary() = 0;
        virtual long de_protobuf() = 0;
        virtual long de_thrift_compact() = 0;
        virtual long de_thrift_binary() = 0;
        virtual long se_json() = 0;
        virtual long de_json() = 0;

    private:
        std::string tool;
        KeyLevel &data_dic;
        const static int REPEAT_TIME = 100;
    };
}

#endif

