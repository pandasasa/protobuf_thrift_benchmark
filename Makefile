# Variable definition
CC = g++

final_target = benchmark
link_lib = -lboost_system -lboost_filesystem -lthrift -lprotobuf
main_file = benchmark.cpp

ap = address_book_protobuf.pb.o
atc = address_book_thrift_constants.o
att = address_book_thrift_types.o
o_file = $(ap) $(atc) $(att)

ap_main = lib/cpp/address_book_protobuf.pb.cc
ap_h = lib/cpp/address_book_protobuf.pb.h
atc_main = lib/cpp/address_book_thrift_constants.cpp
atc_h = lib/cpp/address_book_thrift_constants.h
att_main = lib/cpp/address_book_thrift_types.cpp
att_h = lib/cpp/address_book_thrift_types.h

benchmark: ${o_file}
	$(CC) -o $(final_target) $(link_lib) $(main_file) $(o_file)
	rm $(o_file)

$(ap): $(ap_main) $(ap_h)
	$(CC) -o $(ap) -c $(ap_main)

$(atc): $(atc_main) $(atc_h) $(att_h)
	$(CC) -o $(atc) -c $(atc_main)

$(att): $(att_main) $(att_h)
	$(CC) -o $(att) -c $(att_main)

clean:
	rm $(final_target)

