benchmark: 
	g++ -o benchmark -lboost_system -lboost_filesystem -lthrift -lprotobuf ./benchmark.cpp ./address_book_protobuf.pb.o ./address_book_thrift_constants.o ./address_book_thrift_types.o

benchmark: benchmark.o address_book.o basic_lib.o \
	address_book_protobuf.pb.o address_book_thrift_constants.o \
	address_book_thrift_types.o
	gcc -o benchmark benchmark.o address_book.o basic_lib.o \
		address_book_protobuf.pb.o address_book_thrift_constants.o \
		address_book_thrift_types.o

benchmark.o: benchmark.cpp lib/cpp/address_book.hpp lib/cpp/basic_lib.hpp \
	lib/cpp/address_book_protobuf.pb.h lib/cpp/address_book_thrift_types.h
	gcc -c -lboost_system -lboost_file_system benchmark.cpp

address_book.o: lib/cpp/address_book.hpp lib/cpp/basic_lib.hpp \
	lib/cpp/address_book_protobuf.pb.h lib/cpp/address_book_thrift_types.h
	gcc -o address_book.o -c lib/cpp/address_book.hpp -lthrift -lprotobuf

basic_lib.o: lib/cpp/basic_lib.hpp
	gcc -o basic_lib.o -c lib/cpp/basic_lib.hpp

address_book_protobuf.pb.o: lib/cpp/address_book_protobuf.pb.cc \
	lib/cpp/address_book_protobuf.pb.h
	gcc -o address_book_protobuf.pb.o -c lib/cpp/address_book_protobuf.pb.cc

address_book_thrift_constants.o: lib/cpp/address_book_thrift_constants.cpp \
	lib/cpp/address_book_thrift_constants.h lib/cpp/address_book_thrift_types.h
	gcc -o address_book_thrift_constants.o -c lib/cpp/address_book_thrift_constants.cpp

address_book_thrift_types.o: lib/cpp/address_book_thrift_types.cpp \
	lib/cpp/address_book_thrift_types.h
	gcc -o address_book_thrift_types.o -c lib/cpp/address_book_thrift_types.cpp

clean:
	rm benchmark: benchmark.o lib/cpp/address_book.o lib/cpp/basic_lib.o \
		lib/cpp/address_book_protobuf.pb.o \
		lib/cpp/address_book_thrift_constants.o \
		lib/cpp/address_book_thrift_types.o
