#include "serializer.h"

// Constructors

Serializer::Serializer(){

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
    std::string result = "";
    for (const std::string& element : m_data){
        result += '"' + element + '"';
    }
    return result;
}