#include "JNode.h"
#include <iostream>

using std::cout;
using std::endl;

JNode::JNode() {
    type = NodeType::EMPTY_NODE;
}

JNode::~JNode() {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief set node type to empty
///
void JNode::setEmpty() {
    type = NodeType::EMPTY_NODE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief setter
/// @param inputBool 
///
void JNode::setBool(bool inputBool) {
    value = inputBool;
    type = NodeType::BOOLEAN;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief setter
/// @param inputLong 
///
void JNode::setLong(long long inputLong) {
    value = inputLong;
    type = NodeType::LONG;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief setter
/// @param inputDouble 
///
void JNode::setDouble(double inputDouble) {
    value = inputDouble;
    type = NodeType::DOUBLE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief setter
/// @param inputString 
///
void JNode::setString(shared_ptr<string> inputString) {
    value = inputString;
    type = NodeType::STRING;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief setter
/// @param inputArray 
///
void JNode::setArray(shared_ptr<JNode::JArray> inputArray) {
    value = inputArray;
    type = NodeType::ARRAY;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief setter
/// @param inputObject 
///
void JNode::setObject(shared_ptr<JNode::JObject> inputObject) {
    value = inputObject;
    type = NodeType::OBJECT;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief getter
/// @return bool value
///
bool JNode::getBool() {
    return std::get<bool>(value);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief getter
/// @return long long int value
///
long long int JNode::getLong() {
    return std::get<long long int>(value);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief getter
/// @return double value
///
double JNode::getDouble() {
    return std::get<double>(value);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief getter
/// @return shared pointer to string value
///
shared_ptr<string> JNode::getString() {
    return std::get<shared_ptr<string>>(value);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief getter
/// @return shared pointer to json array
///
shared_ptr<JNode::JArray> JNode::getArray() {
    return std::get<shared_ptr<JNode::JArray>>(value);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief getter
/// @return shared pointer to json object
///
shared_ptr<JNode::JObject> JNode::getObject() {
    return std::get<shared_ptr<JNode::JObject>>(value);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief checks if the type of the node mathes to bool.
/// 
/// @return true 
/// @return false 
///
bool JNode::checkTypeBool() {
    if (type == NodeType::BOOLEAN) {
        return true;
    } else {
        return false;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief checks if the type of the node mathes to long.
/// 
/// @return true 
/// @return false 
///
bool JNode::checkTypeLong() {
    if (type == NodeType::LONG) {
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief checks if the type of the node mathes to double.
/// 
/// @return true 
/// @return false 
///
bool JNode::checkTypeDouble() {
    if (type == NodeType::DOUBLE) {
        return true;
    } else {
        return false;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief checks if the type of the node mathes to pointer to string.
/// 
/// @return true 
/// @return false 
///
bool JNode::checkTypeString() {
    if (type == NodeType::STRING) {
        return true;
    } else {
        return false;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief checks if the type of the node mathes to array.
/// 
/// @return true 
/// @return false 
///
bool JNode::checkTypeArray() {
    if (type == NodeType::ARRAY) {
        return true;
    } else {
        return false;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief prints the value of node and pass identation level if value is json array or json object
/// @param identLevel
/// 
void JNode::JNodePrint(size_t identLevel) {
    //cout << "INDEX: " << value.index() << ": ";

    switch (value.index())
    {
    case 0: {
        // Bool
        if (getBool()) {
            cout << "true";
        } else {
            cout << "false";
        }
        break;
    }
    case 1: {
        // Long Long
        cout << getLong();
        break;
    }
    case 2: {
        // Double
        cout << getDouble();
        break;
    }
    case 3: {
        // String*
        cout << "\"" << *getString() << "\"";
        break;
    }
    case 4: {
        // JArray*
        JArrPrint(identLevel + 1);
    }
        break;
    case 5: {
        // JObecjt*
        JObjPrint(identLevel + 1);
        break;
    }
    default:
        break;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief prints all values of json array with increased identation level
/// @param identLevel
///
void JNode::JArrPrint(size_t identLevel) {
    cout << endl << string((identLevel - 1) * 4, ' ');
    cout << "[" << endl;
    for (auto it = getArray()->begin(); it < getArray()->end(); it++) {
        cout << string(identLevel * 4, ' ');
        (*it)->JNodePrint(identLevel);
        if (next(it) != getArray()->end()) {
            cout << "," << endl; 
        } else {
            cout << endl;
        }
    }
    cout << string((identLevel - 1) * 4, ' ');
    cout << "]";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief wrapper for JObjPrint with standard initial identation used for first json object.
/// 
///
void JNode::JObjPrint() {
    JObjPrint(1);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief prints all values of json object with increased identation level
/// @param identLevel 
///
void JNode::JObjPrint(size_t identLevel) {
    cout << endl << string((identLevel - 1) * 4, ' ');
    cout << "{" << endl;
    for (auto it = getObject()->begin(); it != getObject()->end(); it++) {
        cout << string(identLevel * 4, ' ');
        cout << "\"" << (*it).first << "\"" <<" : ";
        (*it).second->JNodePrint(identLevel);
        if (next(it) != getObject()->end() ) {
            cout << "," << endl; 
        } else {
            cout << endl;
        }
    }
    cout << string((identLevel - 1) * 4, ' ');
    cout << "}";
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief searches json with the keys in keyVec and returns a pointer (shared_ptr) to the node with the result.
/// 
/// @param keyVec vector with the keys
/// @return shared_ptr<JNode> to the node with the resul of the search
///
 shared_ptr<JNode> JNode::JSearchObj(vector<string> keyVec) {

    shared_ptr<JNode> resultNode;
    vector<string> nextVec;
    bool match = false;

    if (keyVec.size() > 1) {
        // vector size > 1: search further with remaining keys
        for ( size_t i = 1; i < keyVec.size(); i++) {
            nextVec.push_back(keyVec[i]);
        }
    }

    // search object keys for match
    for ( auto it = getObject()->begin(); it != getObject()->end(); it++) {
        if ( (*it).first == keyVec[0] ) {
            if (keyVec.size() > 1) {
                // vector size > 1: search further with remaining keys        
                resultNode = (*it).second->JSearchObj(nextVec);
                
            } else {
                // vector size = 1 last object: return shared pointer to JNode to retrieve actual value
                match = true;
                resultNode = (*it).second;
            }
        }
    }
    if ( !match && (keyVec.size() == 1) ) {
        cout << "ERROR: no match with given keys in json" << endl;
    }
    return resultNode;
 }