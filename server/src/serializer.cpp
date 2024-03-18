#include "serializer.h"

// Constructors

Serializer::Serializer(){
    m_serialized = "";
    m_serializedPtr = 0;
}
Serializer::Serializer(std::string dataString){

}

// Public functions

void Serializer::AddValue(std::string value){
    m_parser.str("");
    m_parser<<value.length()<<':'<<value;
    m_data.push_back(m_parser.str());
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
        throw "Serialized string is empty";
    // Get length of string
    size_t len;
    m_parser.str(m_serialized.substr(++sPos));
    m_parser >> len;
    // Find string start
    sPos = m_serialized.find(':', sPos);
    // Get string
    std::string result = m_serialized.substr(++sPos, len);
    // Set pointer
    m_serializedPtr = sPos + len + 1;
    return result;
}