protoc --cpp_out=. *.proto
g++ -std=c++11 -pedantic -Wall -Wextra -static -O2 -isystem $(protoc_dir)/include -isystem $(boost_dir)/include -L $(protoc_dir)/lib *.cc *.cpp -lprotobuf -lpthread -o strategy
