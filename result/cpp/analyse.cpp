#include <cstdlib>

#include <iostream>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>

#include "../../lib/cpp/basic_lib.hpp"


using namespace std;

int main(int argc, char *argv[])
{
    Benchmark::BenchmarkDict bd;
    Benchmark::Serializable se_obj;
    ifstream serialized_file(bd.get_result_file_path().c_str());
    boost::archive::text_iarchive ia(serialized_file);

    ia >> se_obj;

    serialized_file.close();

    const Benchmark::SeToolLevel &ds = se_obj.se_obj;


    ;

    exit(0);
}
