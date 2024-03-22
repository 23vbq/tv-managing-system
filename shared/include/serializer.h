#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <vector>
#include <string>
#include <sstream>

class Serializer{
private:
    std::vector<std::string> m_data; // Data provided to serializer
    std::stringstream m_parser; // Any data type parser

    std::string m_serialized; // Serialized data
    size_t m_serializedPtr; // Position in `m_serialized` that is used to get DeserializeNext()
public:
    /**
     * Initialize empty serializer class
    */
    Serializer();
    /**
     * Initialize serialized class from string containing serialized data.
     * It allows to deserialize string
    */
    Serializer(std::string);

    /**
     * Adds string value to serializer data
     * Added string is in format `length:text`
     * @param value string to add
    */
    void AddValue(std::string);
    /**
     * Adds any type of data to serialized class
     * Data is parsed to string using stringstream
     * @param value variable to add
    */
    template <typename T>
    void AddValue(T value){
        m_parser.str("");
        m_parser<<value;
        m_data.push_back(m_parser.str());
    }

    /**
     * Adds vector list of string values to serializer data
     * Added list is in format `vector_size` `length:text`...
    */
    void AddList(std::vector<std::string>);
    /**
     * Adds vector list of any type of data to serializer data
     * Added list is in format `vector_size` `variable`...
    */
    template <typename T>
    void AddList(std::vector<T> list){
        AddValue<size_t>(list.size());
        for (const T& x : list){
            AddValue<T>(x);
        }
    }

    /**
     * Serializes all objects added to serializer into one string
     * @return Serialized data in string
    */
    std::string Serialize();

    /**
     * Deserializes string from serialized data provided to serializer
    */
    std::string DeserializeNext();
    /**
     * Deserializes any type of data from serialized data provided to serializer
    */
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