#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <fstream>  

class Node {
public:
    std::string value;
    std::vector<Node*> children;
    

    Node(std::string val);
};

#endif // NODE_H    