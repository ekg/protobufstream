#ifndef STREAM_H
#define STREAM_H

#include <cassert>
#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <list>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/gzip_stream.h>
#include <google/protobuf/io/coded_stream.h>

namespace pbs {

template <typename T> class Stream {

public:

    ::google::protobuf::io::OstreamOutputStream* raw_out;
    ::google::protobuf::io::CodedOutputStream* coded_out;

    ::google::protobuf::io::IstreamInputStream* raw_in;
    ::google::protobuf::io::CodedInputStream* coded_in;

    void init(void) {
        coded_out = NULL;
        raw_out = NULL;
        coded_in = NULL;
        raw_in = NULL;
    }

    Stream(std::ostream* out) {
        init();
        raw_out = new ::google::protobuf::io::OstreamOutputStream(out);
        coded_out = new ::google::protobuf::io::CodedOutputStream(raw_out);
    }

    Stream(std::istream* in) {
        init();
        raw_in = new ::google::protobuf::io::IstreamInputStream(in);
        coded_in = new ::google::protobuf::io::CodedInputStream(raw_in);
    }

    ~Stream(void) {
        close();
    }

    void close(void) {
        if (coded_out) delete coded_out;
        if (raw_out) delete raw_out;
        if (coded_in) delete coded_in;
        if (raw_in) delete raw_in;
    }

    void write(const T& message) {
        message.ByteSize(); // cache serialized size
        message.SerializeWithCachedSizes(coded_out);
    }

    void for_each(const std::function<void(T&)>& lambda) {
        bool good = true;
        do {
            // prevents overflow ... inefficiently
            if (coded_in) delete coded_in;
            coded_in = new ::google::protobuf::io::CodedInputStream(raw_in);
            T message;
            good = message.MergePartialFromCodedStream(coded_in);
            lambda(message);
        } while (good);
    }

};

}

#endif
