#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <vector>
#include <string>
#include <sstream>

class Serializer{
private:
    std::vector<std::string> m_data;
    std::stringstream m_parser;

    std::string m_serialized;
    size_t m_serializedPtr;
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

    std::string DeserializeNext();
    template <typename T>
    T DeserializeNext(){
        size_t sPos = m_serialized.find('"', m_serializedPtr);
        if (sPos == std::string::npos)
            throw "Deserializer hits end of string";
        sPos++;
        size_t ePos = m_serialized.find('"', sPos);
        T result;
        m_parser.str(m_serialized.substr(sPos, ePos - sPos));
        m_parser.clear();
        m_parser >> result;
        m_serializedPtr = ePos + 1;
        return result;
    }
};

#endif