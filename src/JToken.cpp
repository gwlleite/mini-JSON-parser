#include <iostream>

#include "JToken.h"

using std::cout;
using std::endl;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief contructor loads the json file and gererates vector of tokens(string and type)
/// @param filename 
///
JToken::JToken(const string filename)
{
    std::ifstream myIFS;
    std::string line;
    
    myIFS.open(filename);
    if(myIFS.is_open())
    {
        // reads json-file and generates vector of tokens
        while(getline(myIFS, line)) {
            jTokenizer(deleteSpaces(line));
            //countLine++;
        }
    }
    myIFS.close();
    cout << "MESSAGE: json file loaded successfully" << endl;
}

JToken::~JToken() {
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief translate input string to map with tokens and type of token (curly brackets, colom, number, string,...)
/// funcion update vector<struct{string, TokenType}>
/// @param inputString 
/// @return -
///
void JToken::jTokenizer(string inputString) {
    size_t j = 0;
    string tmp;
    for (size_t i = 0; i < inputString.size(); i++) {
        switch (inputString[i])
        {   
        case '{':
            tmpToken.str = inputString.substr(i, 1);
            tmpToken.type = TokenType::CURLY_BRACKET_OPEN;
            tokens.push_back(tmpToken);
            break;
        case '}':
            tmpToken.str = inputString.substr(i, 1);
            tmpToken.type = TokenType::CURLY_BRACKET_CLOSE;
            tokens.push_back(tmpToken);
            break;
        case '[':
            tmpToken.str = inputString.substr(i, 1);
            tmpToken.type = TokenType::SQUARE_BRACKET_OPEN;
            tokens.push_back(tmpToken);
            break;
        case ']':
            tmpToken.str = inputString.substr(i, 1);
            tmpToken.type = TokenType::SQUARE_BRACKET_CLOSE;
            tokens.push_back(tmpToken);
            break;
        case ':':
            tmpToken.str = inputString.substr(i, 1);
            tmpToken.type = TokenType::COLON;
            tokens.push_back(tmpToken);
            break;
        case ',':
            tmpToken.str = inputString.substr(i, 1);
            tmpToken.type = TokenType::COMMA;
            tokens.push_back(tmpToken);
break;
        case '"':
            j = strEndPos(inputString, i);
            tmpToken.str = inputString.substr(i + 1, j - i - 1);
            tmpToken.type = TokenType::STRING_VALUE;
            tokens.push_back(tmpToken);
            i = j;
            break;
        case '0' ... '9': // range: . / 0 1 2 3 4 5 6 7 8 9
            j = numEndPos(inputString, i);
            //cout << "NUMsubsstr: " << inputString.substr(i, j - i) << endl;
            tmpToken.str = inputString.substr(i, j - i);
            tmpToken.type = TokenType::LONG_NUMBER;
            //cout << "tmp: " << tmpToken.str << endl;
            for (auto flag:inputString.substr(i, j - i)) {
                if (flag == '.') {
                    tmpToken.type = TokenType::DOUBLE_NUMBER;
                }
            }
            //cout << "tmp: " << tmpToken.str << endl;
            tokens.push_back(tmpToken);
            i = j - 1;
            break;
        case 'f':
        case 'F':
        case 't':
        case 'T':
            j = numEndPos(inputString, i);
            if (inputString.substr(i, j - i) == "false" || inputString.substr(i, j - i) == "FALSE") {
                tmpToken.str = inputString.substr(i, j - 1);
                tmpToken.type = TokenType::BOOL_FALSE;
                tokens.push_back(tmpToken);
            } else if (inputString.substr(i, j - i) == "true" || inputString.substr(i, j - i) == "TRUE") {
                tmpToken.str = inputString.substr(i, j - 1);
                tmpToken.type = TokenType::BOOL_TRUE;
                tokens.push_back(tmpToken);
            }
            i = j - 1;
            break;
        default:
            cout << "ERROR: object or TokenType not identified" << endl;
            break;
        }
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief delete all spaces outside "" 
/// @param line 
/// @return string without spaces 
///
string JToken::deleteSpaces(string line) {
    string sTmp = "";
    bool quoteFlag = false;
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == ' ') {
            if (quoteFlag) {
                // keep space between quotes
                sTmp = sTmp + line[i];
            } else {
                // eliminate spaces outside quotes
            }
        } else if (line[i] == '\"') {
            quoteFlag = !quoteFlag;
            sTmp = sTmp + line[i];
        } else {
            // alle andere charactere
            sTmp = sTmp + line[i];
        }
    }
    return sTmp;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief returns true if the char ist one of the defined JSON TokenTypes, if not retuns false 
/// @param c 
/// @return bool
///
bool JToken::checkSeparator(char c) {
    switch (c) {
    case '{':
        //cout << "(TokenType { gefunden)" << endl;
        return true;
    case '}':
        //cout << "(TokenType } gefunden)" << endl;
        return true;
    case '[':
        //cout << "(TokenType [ gefunden)" << endl;
        return true;
    case ']':
        //cout << "(TokenType ] gefunden)" << endl;
        return true;
    case ':':
        //cout << "(TokenType : gefunden)" << endl;
        return true;
    case ',':
        //cout << "(TokenType , gefunden)" << endl;
        return true;
    case '"':
        //cout << "(TokenType \" gefunden)" << endl;
        return true;
    default:
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief searches for the last character of the number in int or float representation in the string
/// @param str 
/// @param startPos 
/// @return position of the last character of the number written in the string
///
size_t JToken::numEndPos(string str, size_t startPos) {
    size_t j;
    j = startPos + 1;
    //cout << "i: " << startPos << " | j: " << j << endl;
    while (!checkSeparator(str[j]) && j < str.size()) {
        //cout << str[j];
        j++;
    };
    //cout << "NumberSubsstr: " << str.substr(startPos+1, j-startPos-1) << endl;
    return j;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief searches for the last character of the string before the next separator (spaces are allowed)
/// @param str 
/// @param startPos 
/// @return position of the last character of the string
///
size_t JToken::strEndPos(string str, size_t startPos) {
    size_t j;
    j = startPos + 1;
    //cout << "i: " << startPos << " | j: " << j << endl;
    while (!checkSeparator(str[j]) && j < str.size()) {
        //cout << str[j];
        j++;
    };
    //cout << "stringSubsstr: " << str.substr(startPos+1, j-startPos-1) << endl;
    return j;
}


void JToken::printTokens() {
    cout << "printTokens: " << endl;
    for (const auto& elem : tokens) {
        cout << "tokens: " << elem.str << " - " << (int)elem.type << endl;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief gets the token and then increments the position counter in vector tokens to the next position
/// @return token string
/// 
string JToken::getTokenStr() {
    if (!availableToken()) {
        cout << "ERROR: there are no further tokens available" << endl;
        return "ERROR";
    }
    //pos++;
    return tokens[pos].str;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief gets the token type of the actual position in the vector of tokens to be retuned by getNextToken()
/// @return token type enum class TokenType
/// 
JToken::TokenType JToken::getTokenType() {
    return tokens[pos].type;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief checks if there are not readen tokens in the vector tokens
/// @return true if tokens are still available to be returned by getNextToken(), if not returns false 
///
bool JToken::availableToken()
{
    return (pos < tokens.size()) ? true : false;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief increase pos(ition) and points to next pointer
///
void JToken::goToNextToken()
{
    pos++;
}