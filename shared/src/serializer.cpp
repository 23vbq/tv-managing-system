#include "serializer.h"

// Constructors

Serializer::Serializer(){
    m_serialized = "";
    m_serializedPtr = 0;
}
Serializer::Serializer(std::string dataString){
    m_serialized = dataString;
    m_serializedPtr = 0;
}

// Public functions

void Serializer::AddValue(std::string value){
    m_parser.str("");
    m_parser<<value.length()<<':'<<value;
    m_data.push_back(m_parser.str());
}

void Serializer::AddList(std::vector<std::string> list){
    AddValue<size_t>(list.size());
    for (const std::string& x : list){
        AddValue(x);
    }
}

std::string Serializer::Serialize(){
    m_serialized = "";
    for (const std::string& element : m_data){
        m_serialized += '"' + element + '"';
    }
    m_serializedPtr = 0;
    return m_serialized;
}

std::string Serializer::DeserializeNext(){
    // Check occurence of first "
    size_t sPos = m_serialized.find('"', m_serializedPtr);
    if (sPos == std::string::npos)
        throw "Deserializer hits end of string";
    // Get length of string
    size_t len = 0;
    m_parser.str(m_serialized.substr(++sPos));
    m_parser.clear();
    m_parser >> len;
    // Find string start
    sPos = m_serialized.find(':', sPos);
    // Get string
    std::string result = m_serialized.substr(++sPos, len);
    // Set pointer
    m_serializedPtr = sPos + len + 1;
    return result;
}