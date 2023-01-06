#ifndef J_NODE
#define J_NODE

#include <vector>
#include <string>
#include <map>
#include <variant>
#include <memory>

using std::vector;
using std::string;
using std::map;
using std::variant;
using std::shared_ptr;

class JNode {
    public:
        using JObject = map<string, std::shared_ptr<JNode>>;
        using JArray = std::vector<std::shared_ptr<JNode>>;

    enum class NodeType {
        EMPTY_NODE,
        BOOLEAN,
        LONG,
        DOUBLE,
        STRING,
        ARRAY,
        OBJECT};

    private:
        variant<
            bool,
            long long int,
            double,
            shared_ptr<string>,
            shared_ptr<JArray>,
            shared_ptr<JObject>
        > value;
        NodeType type;

    public:
        JNode();
        ~JNode();

        void setEmpty();
        void setBool(bool inputBool);
        void setLong(long long inpuLong);
        void setDouble(double inputDouble);
        void setString(shared_ptr<string> inputString);
        void setArray(shared_ptr<JArray> inputArray);
        void setObject(shared_ptr<JObject> inputObject);

        bool getBool();
        long long int getLong();
        double getDouble();
        shared_ptr<string> getString();
        shared_ptr<JArray> getArray();
        shared_ptr<JObject> getObject();

        bool checkTypeBool();
        bool checkTypeLong();
        bool checkTypeDouble();
        bool checkTypeString();
        bool checkTypeArray();

        void JNodePrint(size_t identLevel);
        void JArrPrint(size_t identLevel);
        void JObjPrint();
        void JObjPrint(size_t identLevel);

        shared_ptr<JNode> JSearchObj(vector<string> keyVec);

};

#endif // J_NODE