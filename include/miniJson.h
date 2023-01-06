#ifndef MINI_JSON
#define MINI_JSON

#include <string>
#include <memory>

#include "JNode.h"
#include "JToken.h"

using std::string;
using std::shared_ptr;

class miniJson {

    public:
        miniJson();
        ~miniJson();

    private:
        shared_ptr<JNode> head;
        size_t objLevel;

    public:
        void jReader(string filename);
        void parseValue(shared_ptr<JNode> callingNode, JToken* token);
        void JPrint();

        bool JGetBool(string keys);
        long long int JGetLong(string keys);
        double JGetDouble(string keys);
        string JGetString(string keys);
        vector<bool> JGetBoolArray(string keys);
        vector<long long int> JGetLongArray(string keys);
        vector<double> JGetDoubleArray(string keys);
        vector<string> JGetStringArray(string keys);
        // private helper member function
        vector<string> getKeys(string keys);
};

#endif // MINI_JSON