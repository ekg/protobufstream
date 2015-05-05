all: test

cpp/test.pb.h: test.proto
	mkdir -p cpp
	protoc test.proto --cpp_out=cpp

test: test.cpp cpp/test.pb.h
	g++ -std=c++11 -g -o test test.cpp cpp/test.pb.cc -lprotobuf

clean:
	rm -f test
	rm -rf cpp
