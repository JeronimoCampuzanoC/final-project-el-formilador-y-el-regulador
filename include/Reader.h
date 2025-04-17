#ifndef READER_H
#define READER_H

#include <string>
#include "Grammar.h"

class Reader {
public:
    Grammar readFile(string fileRoute);

private:
    std::string content;
};

#endif 