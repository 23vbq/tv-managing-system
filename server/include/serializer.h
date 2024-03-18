#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <vector>
#include <string>
#include <sstream>

class Serializer{
private:
    std::vector<std::string> m_data;
    std::stringstream m_parser;
public:
    Serializer();
    Serializer(std::string);

    void AddValue(std::string);
    template <typename T>
    void AddValue(T value){
        m_parser.str("");
        m_parser<<value;
        m_data.push_back(m_parser.str());
    }
    std::string Serialize();
};

#endif