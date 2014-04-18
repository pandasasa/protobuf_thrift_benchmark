#include <cstdlib>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <boost/archive/text_iarchive.hpp>

#include "../../lib/cpp/basic_lib.hpp"


class Analyser
{
public:
    Analyser(const Benchmark::SeToolLevel &se_dict,
            const std::string &config_file_path,
            const std::string &output_path)
        : se_dict(se_dict), output_path(output_path)
    {
        std::ifstream config_file(config_file_path.c_str());
        std::string line;

        while(config_file >> line)
        {
            if (line.substr(2, 4) == "loop")
            {
                int value = atoi(line.substr(7, line.length() - 7).c_str());
                if (line[0] == 'x')
                    this->x = value;
                else
                    this->y = value;
            }
        }

        config_file.close();

        this->key_list.push_back("se_time");
        this->key_list.push_back("dese_time");
        this->key_list.push_back("seed_file_size");
    }

    void default_write()
    {
        for (std::list<std::string>::iterator key = this->key_list.begin();
            key != this->key_list.end();
            ++key)
            this->write_data(*key);
    }

    void write_data(const std::string &key)
    {
        for (Benchmark::SeToolLevel::iterator tool = this->se_dict.begin();
            tool != this->se_dict.end();
            ++tool)
        {
            Benchmark::SeFileLevel &tool_dict = this->se_dict[tool->first];
            std::string out_path = this->output_path + "/" + tool->first + "."
                + key + ".result";

            std::ofstream write_file(out_path.c_str());
            
            for (int x = 0; x < this->x; x++)
            {
                for (int y = 0; y < this->y; y++)
                {
                    std::stringstream file_name_ss;
                    file_name_ss << x << '.' << y << ".json";
                    std::string file_name = file_name_ss.str();
                    Benchmark::SeKeyLevel &file_dict
                        = tool_dict[file_name.c_str()];
                    double value;

                    if (key == "seed_file_size")
                        write_file << file_dict[key.c_str()] << '\t';
                    else
                        write_file << file_dict[key.c_str()] / 1000.0
                            << '\t';
                }

                write_file << std::endl;
            }

            write_file.close();
        }
    }

private:
    int x, y;
    std::string output_path;
    Benchmark::SeToolLevel se_dict;
    
    std::list<std::string> key_list;
};

