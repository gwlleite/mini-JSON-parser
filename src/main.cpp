#include <iostream>
#include <string>
#include <vector>

#include "miniJson.h"
#include "jNode.h"
#include "JToken.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main() {
    cout << "---------------------------------MINIJSON-----------------------------------------\n";
    cout << "c++ standard: " << __cplusplus << endl;
    cout << "---------------------------------------------------------------------------------\n";
    miniJson myJson;
    myJson.jReader("config.json");
    cout << "---------------------------------------------------------------------------------\n";
    myJson.JPrint();
    cout << "---------------------------------------------------------------------------------\n";

    // key: "trainingData : Obj2 : File" / value: "testData.csv" ,
    string myStr = myJson.JGetString("trainingData:Obj2:File");
    cout << "RESULT: " << myStr << endl;

    // key: "trainingData : Obj2 : key10" / value: 11.11 ,
    double myDbl = myJson.JGetDouble("trainingData:Obj2:key10");
    cout << "RESULT: " << myDbl << endl;

    // key: "trainingData : Obj2 : key20" / value: 2222 ,
    long long int myLong = myJson.JGetLong("trainingData:Obj2:key20");
    cout << "RESULT: " << myLong << endl;

    // key: "trainingData : Obj2 : key30" / value: true ,
    bool myBool = myJson.JGetBool("trainingData:Obj2:key30");
    cout << "RESULT: " << myBool << endl;

    //"numberPerceptronsEachLayer1" : [true , true, true, false],
    vector<bool> myBool2 = myJson.JGetBoolArray("numberPerceptronsEachLayer1");
    for (auto a:myBool2) {
        cout << "RESULTARRAY: " << a << endl;
    }

    //"numberPerceptronsEachLayer2" : [16 , 17, 16, 5],
    vector<long long int> myLong2 = myJson.JGetLongArray("numberPerceptronsEachLayer2");
    for (auto a:myLong2) {
        cout << "RESULTARRAY: " << a << endl;
    }

    //"numberPerceptronsEachLayer3" : [16.1 , 17.2, 16.3, 5.5],
    vector<double> myDouble2 = myJson.JGetDoubleArray("numberPerceptronsEachLayer3");
    for (auto a:myDouble2) {
        cout << "RESULTARRAY: " << a << endl;
    }

    //"numberPerceptronsEachLayer4" : ["value1", "value2", "value3", "value4"],
    vector<string> myString2 = myJson.JGetStringArray("numberPerceptronsEachLayer4");
    for (auto a:myString2) {
        cout << "RESULTARRAY: " << a << endl;
    }

    cout << "---------------------------------------------------------------------------------\n";

    return EXIT_SUCCESS;
}