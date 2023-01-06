///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// @file miniJson.cpp
/// @author gleite
/// @version 1.0
///
/// @mainpage DESCRIPTION
///
/// @brief This is a simple and easy to use json reader. It reads a json file and loads to an objext,
/// which can be printed or singel values accessed throw the keys. There are alse sample files
/// main.cpp and config.json for demonstration purpose only. You must chang them according to your
/// meeds.
///
/// The simple example as console application shown in the file main.cpp should be self explanatory. The sample
/// uses the generated json object and its methods to print or access the json elements throw its keys. The
/// usage cases are commented in the main.cpp example. Have fun!
///
///
/// @author gleite
/// @version 1.0
///
///
/// @copyright 
///
/// Licensed under the MIT License (the "License"); you may not use this file except
/// in compliance with the License. You may obtain a copy of the License at
///
/// http://opensource.org/licenses/MIT
///
/// Unless required by applicable law or agreed to in writing, software distributed
/// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
/// CONDITIONS OF ANY KIND, either express or implied. See the License for the
/// specific language governing permissions and limitations under the License."
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <memory>

#include "miniJson.h"
#include "JNode.h"
#include "JToken.h"

using std::cout;
using std::endl;

miniJson::miniJson(/* args */) {
    // constructor generates the starting json node as head
    head = std::make_shared<JNode>();
}

miniJson::~miniJson() {
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief reads the json file and parses the json object
/// @param filename 
///
void miniJson::jReader(std::string filename){
    // reset initial json node head to empty AND objLevel to zero as starting point for parsing process 
    head->setEmpty();
    objLevel = 0;

    // reads json file and loads tokes to JToken class object
    JToken myTokens(filename);

    while(myTokens.availableToken()) {
        // parse tokens searching for value (bool,..., array or object)
        parseValue(head, &myTokens);
    }

    // check for error in the levels of objects parsing
    if (objLevel != 0) {
        cout << "ERROR: parsing process" << endl;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief parses actual token of vector<tokens at actual pos(ition)
/// @return returns shared pointer 
///
void miniJson::parseValue(shared_ptr<JNode> callingNode, JToken* ptrTokens) {
    switch (ptrTokens->getTokenType()) {
    case JToken::TokenType::CURLY_BRACKET_OPEN: {
        // '{' detected
        // creates new json object map<string key, JNode value>
        std::shared_ptr<JNode::JObject> newObj = std::make_shared<JNode::JObject>();
        // save sh_ptr of JObject to calling node
        callingNode->setObject(newObj);
        objLevel++;

        // parses tokens until either END OF TOKENS or '}' is found
        while ( ptrTokens->availableToken() && (ptrTokens->getTokenType() != JToken::TokenType::CURLY_BRACKET_CLOSE) ) {
            // go to next token and get key value
            ptrTokens->goToNextToken();

            string keyStr;
            shared_ptr<JNode> valueNode;
            keyStr= ptrTokens->getTokenStr();
            valueNode = std::make_shared<JNode>();

            // go to next token and get ':'
            ptrTokens->goToNextToken();
            // go to next token and get Node Value           
            ptrTokens->goToNextToken();      
            parseValue(valueNode, ptrTokens);

            // assign key:value pair to map (json object)
            newObj->insert( {keyStr, valueNode} );      
            // go to next token and get ',' or '}'
        }
        objLevel--;
        break;
    }
    case JToken::TokenType::SQUARE_BRACKET_OPEN: {
//    } else if (ptrTokens->getTokenType() == JToken::TokenType::SQUARE_BRACKET_OPEN) {
        // '[' detected 
        // creates new json array vector<JNode value>
        std::shared_ptr<JNode::JArray> newArr = std::make_shared<JNode::JArray>();
        // save sh_ptr of JObject to calling node
        callingNode->setArray(newArr);

        // parses tokens until either END OF TOKENS or ']' is found
        while ( ptrTokens->availableToken() && (ptrTokens->getTokenType() != JToken::TokenType::SQUARE_BRACKET_CLOSE) ) {
            // go to next token and get value
            ptrTokens->goToNextToken();

            shared_ptr<JNode> valueNode;
            valueNode = std::make_shared<JNode>();
            parseValue(valueNode, ptrTokens);            

            // assign value to vector (json array)
            newArr->push_back(valueNode);

            // go to next token and get ',' or ']'
        }
        break;
    }
    case JToken::TokenType::STRING_VALUE: {
        // creates new string AND shared pointer to it
        std::shared_ptr<string> newVal = std::make_shared<string>();
        *newVal = ptrTokens->getTokenStr();
        // save sh_ptr of value to calling node
        callingNode->setString(newVal);
        break;
    }
    case JToken::TokenType::LONG_NUMBER: {
        // save value to calling node
        callingNode->setLong(stol(ptrTokens->getTokenStr()));
        break;
    }
    case JToken::TokenType::DOUBLE_NUMBER: {
        // save value to calling node
        callingNode->setDouble(stod(ptrTokens->getTokenStr()));
        break;
    }
    case JToken::TokenType::BOOL_TRUE: {
        // save value to calling node
        callingNode->setBool(true);
        break;
    }
    case JToken::TokenType::BOOL_FALSE: {
        // save sh_ptr of value to calling node
        callingNode->setBool(false);
        break;
    }
    case JToken::TokenType::NULL_TOKEN: {
        // save sh_ptr of value to calling node
        callingNode->setEmpty();
        break;
    }
    default:
        break;
    }
    ptrTokens->goToNextToken();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief prints the complete json object including all sub objects and arrays with identation for better visualization.
/// 
///
void miniJson::JPrint() {
    head->JObjPrint();
    cout << endl;
    //
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief generates vector of strings with the keys of the json objects to be searched recursivelly.
/// 
/// @param keys string
/// @return vector<string> 
///
vector<string> miniJson::getKeys(string keys) {
    size_t prev = 0;
    size_t pos = 0;
    vector<string> vec;
    do
    {
        pos = keys.find(":", pos + 1);               
        // several keys
        if (pos!=std::string::npos) {
            vec.push_back( keys.substr(prev, (pos-prev)) );
            prev = pos + 1;
        }
    } while (pos != string::npos);
    vec.push_back( keys.substr(prev, keys.size()) );
    return vec;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief reads the bool value accorsing to the keys (key1:key2:...:keyN) from the loaded json file.
/// 
/// @param keys 
/// @return true 
/// @return false 
///
bool miniJson::JGetBool(string keys) {
    // generate vector with keys
    vector<string> keyVec;
    shared_ptr<JNode> resultNode;
    keyVec = getKeys(keys);

    // search Obj for matching key recursivelly
    resultNode = head->JSearchObj(keyVec);

    if (!resultNode->checkTypeBool()) {
        cout << "ERROR: type mismatch getting bool value." << endl;
        return false;
    } else {
        return resultNode->getBool();
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief reads the long long int value accorsing to the keys (key1:key2:...:keyN) from the loaded json file.
/// 
/// @param keys 
/// @return long long int 
///
long long int miniJson::JGetLong(string keys) {
    // generate vector with keys
    vector<string> keyVec;
    shared_ptr<JNode> resultNode;
    keyVec = getKeys(keys);

    // search Obj for matching key recursivelly
    resultNode = head->JSearchObj(keyVec);

    if (!resultNode->checkTypeLong()) {
        cout << "ERROR: type mismatch getting long value." << endl;
        return 999999999999999999;
    } else {
        return resultNode->getLong();
    }    
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief reads the double value accorsing to the keys (key1:key2:...:keyN) from the loaded json file.
/// 
/// @param keys 
/// @return double 
///
double miniJson::JGetDouble(string keys) {
    // generate vector with keys
    vector<string> keyVec;
    shared_ptr<JNode> resultNode;
    keyVec = getKeys(keys);

    // search Obj for matching key recursivelly
    resultNode = head->JSearchObj(keyVec);

    if (!resultNode->checkTypeDouble()) {
        cout << "ERROR: type mismatch getting double value." << endl;
        return 9.99999e+99;
    } else {
    return resultNode->getDouble();
    }    
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief reads the string value accorsing to the keys (key1:key2:...:keyN) from the loaded json file.
/// 
/// @param keys 
/// @return string 
///
string miniJson::JGetString(string keys) {
    // generate vector with keys
    vector<string> keyVec;
    shared_ptr<JNode> resultNode;
    keyVec = getKeys(keys);

    // search Obj for matching key recursivelly
    resultNode = head->JSearchObj(keyVec);

    if (!resultNode->checkTypeString()) {
        cout << "ERROR: type mismatch getting string value." << endl;
        return "ERROR";
    } else {
    return *(resultNode->getString());
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief reads the array of bool values accorsing to the keys (key1:key2:...:keyN) from the loaded json file.
/// 
/// @param keys 
/// @return vector<bool> 
///
vector<bool> miniJson::JGetBoolArray(string keys) {
    // generate vector with keys
    vector<string> keyVec;
    shared_ptr<JNode> resultNode;
    keyVec = getKeys(keys);

    // vector for results
    shared_ptr<vector<bool>> boolVec = std::make_shared<vector<bool>>();

    // search Obj for matching key recursivelly
    resultNode = head->JSearchObj(keyVec);

    for (auto itr = resultNode->getArray()->begin(); itr != resultNode->getArray()->end() ;itr++ ) {
        if (!(*itr)->checkTypeBool()) {
            cout << "ERROR: type mismatch getting bool value in array." << endl;
            //return false;
            boolVec->push_back( false );
        } else {
            //return resultNode
            boolVec->push_back( (*itr)->getBool() );
        }
    }
    return *boolVec;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief reads the array of long long int values accorsing to the keys (key1:key2:...:keyN) from the loaded json file.
/// 
/// @param keys 
/// @return vector<long long int> 
///
vector<long long int> miniJson::JGetLongArray(string keys) {
    // generate vector with keys
    vector<string> keyVec;
    shared_ptr<JNode> resultNode;
    keyVec = getKeys(keys);

    // vector for results
    shared_ptr<vector<long long int>> longVec = std::make_shared<vector<long long int>>();

    // search Obj for matching key recursivelly
    resultNode = head->JSearchObj(keyVec);

    for (auto itr = resultNode->getArray()->begin(); itr != resultNode->getArray()->end() ;itr++ ) {
        if (!(*itr)->checkTypeLong()) {
            cout << "ERROR: type mismatch getting long value in array." << endl;
            //return 999999999999999999;
            longVec->push_back( 999999999999999999 );
        } else {
            //return resultNode
            longVec->push_back( (*itr)->getLong() );
        }
    }
    return *longVec;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief reads the array of double values accorsing to the keys (key1:key2:...:keyN) from the loaded json file.
/// 
/// @param keys 
/// @return vector<double> 
///
vector<double> miniJson::JGetDoubleArray(string keys) {
    // generate vector with keys
    vector<string> keyVec;
    shared_ptr<JNode> resultNode;
    keyVec = getKeys(keys);

    // vector for results
    shared_ptr<vector<double>> doubleVec = std::make_shared<vector<double>>();

    // search Obj for matching key recursivelly
    resultNode = head->JSearchObj(keyVec);

    for (auto itr = resultNode->getArray()->begin(); itr != resultNode->getArray()->end() ;itr++ ) {
        if (!(*itr)->checkTypeDouble()) {
            cout << "ERROR: type mismatch getting double value in array." << endl;
            //return 9.99999e+99
            doubleVec->push_back( 9.99999e+99 );
        } else {
            //return resultNode
            doubleVec->push_back( (*itr)->getDouble() );
        }
    }
    return *doubleVec;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief reads the array of string values accorsing to the keys (key1:key2:...:keyN) from the loaded json file.
/// 
/// @param keys 
/// @return vector<string> 
///
vector<string> miniJson::JGetStringArray(string keys) {
    // generate vector with keys
    vector<string> keyVec;
    shared_ptr<JNode> resultNode;
    keyVec = getKeys(keys);

    // vector for results
    shared_ptr<vector<string>> stringVec = std::make_shared<vector<string>>();

    // search Obj for matching key recursivelly
    resultNode = head->JSearchObj(keyVec);

    for (auto itr = resultNode->getArray()->begin(); itr != resultNode->getArray()->end() ;itr++ ) {
        if (!(*itr)->checkTypeString()) {
            cout << "ERROR: type mismatch getting string value in array." << endl;
            //return "ERROR";
            stringVec->push_back( "ERROR" );
        } else {
            //return resultNode
            stringVec->push_back( *( (*itr)->getString() ) );
        }
    }
    return *stringVec;
}