#ifndef J_TOKEN
#define J_TOKEN

#include <fstream>
#include <vector>

using std::string;
using std::vector;

class JToken {

    public:
    enum class TokenType {
        CURLY_BRACKET_OPEN,     // {
        CURLY_BRACKET_CLOSE,    // }
        SQUARE_BRACKET_OPEN,    // [
        SQUARE_BRACKET_CLOSE,   // ]
        COMMA,                  // ,
        COLON,                  // :
        QUOTATION_OPEN,         // "
        QUOTATION_CLOSE,        // "
        STRING_VALUE,           // string
        LONG_NUMBER,            // long long
        DOUBLE_NUMBER,          // double
        BOOL_TRUE,              // true
        BOOL_FALSE,             // false
        NULL_TOKEN              // no value token
    };

    private:
        struct token {
            string str;
            TokenType type;
        };
        size_t pos = 0;
        token tmpToken;
        vector<token> tokens;

    public:
        JToken(const string filename);
        ~JToken();

    private:
        // internal helper functions
        string deleteSpaces(string line);
        void jTokenizer(string inputString);
        bool checkSeparator(char c);
        size_t numEndPos(string str, size_t startPos);
        size_t strEndPos(string str, size_t startPos);

    public:
        void printTokens();
        string getTokenStr();
        JToken::TokenType getTokenType();
        bool availableToken();
        void goToNextToken();

};

#endif // J_TOKEN