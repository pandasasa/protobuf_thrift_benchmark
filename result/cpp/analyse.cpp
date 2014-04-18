#include <cstdlib>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <boost/archive/text_iarchive.hpp>

#include "../../lib/cpp/basic_lib.hpp"


using namespace std;


class Analyser
{
public:
    Analyser(const string &serialization_file_path,
            const string &config_file_path,
            const string &output_path) : output_path(output_path)
    {
        Benchmark::BenchmarkDict bd;
        Benchmark::Serializable se_obj;
        ifstream serialized_file(serialization_file_path.c_str());

        boost::archive::text_iarchive ia(serialized_file);
        serialized_file.close();

        ia >> se_obj;
        this->se_dict = se_obj.se_obj;

        ifstream config_file(config_file_path.c_str());
        string line;

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
        for (list<string>::iterator key = this->key_list.begin();
            key != this->key_list.end();
            ++key)
            this->write_data(*key);
    }

    void write_data(const string &key)
    {
        for (Benchmark::SeToolLevel::iterator tool = this->se_dict.begin();
            tool != this->se_dict.end();
            ++tool)
        {
            Benchmark::SeFileLevel &tool_dict = this->se_dict[tool->first];
            string out_path = this->output_path + "/" + tool->first + "."
                + key + ".result";

            ofstream write_file(out_path.c_str());
            
            for (int x = 0; x < this->x; x++)
            {
                for (int y = 0; y < this->y; y++)
                {
                    stringstream file_name_ss;
                    file_name_ss << x << '.' << y << ".json";
                    string file_name = file_name_ss.str();
                    Benchmark::SeKeyLevel &file_dict
                        = tool_dict[file_name.c_str()];
                    double value;

                    if (key == "seed_file_size")
                        write_file << file_dict[key.c_str()] << '\t';
                    else
                        write_file << file_dict[key.c_str()] / 1000.0
                            << '\t';
                }

                write_file << endl;
            }

            write_file.close();
        }
    }

private:
    int x, y;
    string output_path;
    Benchmark::SeToolLevel se_dict;
    
    list<string> key_list;
};


int main(int argc, char *argv[])
{
    string pickle_file_path("./original_dict.pickle");
    string config_file_path("../../gen.config");
    string output_path("./");

    Analyser analyser(pickle_file_path, config_file_path, output_path);
    analyser.default_write();

    exit(0);
}
