#include <string>
#include <sstream>
#include <set>

#include "../../lib/cpp/basic_lib.hpp"


class Analyser
{
public:
    Analyser(const Benchmark::SeToolLevel &se_dict,
            const std::string &config_file_path,
            const std::string &output_path)
        : se_dict(se_dict), output_path(output_path)
    {
        this->key_list.push_back("se_time");
        this->key_list.push_back("dese_time");
        this->key_list.push_back("seed_file_size");

        // Getting the x, y sequence value in file name
        Benchmark::SeToolLevel::const_iterator iter = se_dict.begin();
        const Benchmark::SeFileLevel &tool_dict = iter->second;
        for (Benchmark::SeFileLevel::const_iterator file_iter
                = tool_dict.begin();
            file_iter != tool_dict.end();
            ++file_iter)
        {
            std::string full_name = file_iter->first;
            int point_index = full_name.find(".");
            int x = atoi(full_name.substr(0, point_index).c_str());

            std::string sub_name = full_name.substr(point_index + 1);
            point_index = sub_name.find(".");
            int y = atoi(sub_name.substr(0, point_index).c_str());

            this->x_set.insert(x);
            this->y_set.insert(y);
        }
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
        for (Benchmark::SeToolLevel::const_iterator tool = this->se_dict.begin();
            tool != this->se_dict.end();
            ++tool)
        {
            Benchmark::SeFileLevel &tool_dict = this->se_dict[tool->first];

            std::string out_path = this->output_path + "/" + tool->first + "."
                + key + ".result";

            std::ofstream write_file(out_path.c_str());
            
            for (std::set<int>::iterator x_iter = this->x_set.begin();
                x_iter != this->x_set.end();
                ++x_iter)
            {
                for (std::set<int>::iterator y_iter = this->y_set.begin();
                    y_iter != this->y_set.end();
                    ++y_iter)
                {
                    int x = *x_iter;
                    int y = *y_iter;

                    std::stringstream file_name_ss;
                    file_name_ss << x << '.' << y << ".json";
                    std::string file_name = file_name_ss.str();
                    
                    Benchmark::SeKeyLevel &file_dict
                        = tool_dict[file_name.c_str()];

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
    std::string output_path;
    Benchmark::SeToolLevel se_dict;

    std::list<std::string> key_list;
    std::set<int> x_set, y_set;
};

