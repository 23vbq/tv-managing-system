#include "serializer.h"

// Constructors

Serializer::Serializer(){

}
Serializer::Serializer(std::string dataString){

}

// Public functions

std::string Serializer::Serialize(){
    std::string result = "";
    for (const std::string& element : m_data){
        result += '"' + element + '"';
    }
    return result;
}