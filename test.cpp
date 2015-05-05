#include <iostream>
#include "cpp/test.pb.h"
#include "stream.hpp"

using namespace google::protobuf;
using namespace test;


int main(void) {

    Note note1;
    note1.add_idea("a fun way to idea");
    note1.add_idea("could this really be serialized");
    Note note2;
    note2.add_idea("yheaw");
    note2.set_number(42);

    std::ofstream pbsout("test.pbs");
    pbs::Stream<Note> out(&pbsout);
    out.write(note1);
    out.write(note2);
    out.close();
    pbsout.close();

    std::ifstream pbsin("test.pbs");
    pbs::Stream<Note> in(&pbsin);
    in.for_each([](Note& note) {
            std::cout << "-------------------" << std::endl;
            if (note.has_number()) {
                std::cout << note.number() << std::endl;
            }
            for (int i = 0; i < note.idea_size(); ++i) {
                std::cout << note.idea(i) << std::endl;
            }
        });

    return 0;
}
