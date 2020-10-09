#include "digitalosc.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
    DigitalOsc osc;
    string str(osc.wrRdData("*IDN?"));
    if (str.substr(0, 10) != ",ADS-6142H")
        return -1;

    if (0) {
        vector<string> list {
            "MAX", "MIN", "PKPK", "VTOP", "VBASe", "VAMP", "AVERage",
            "SQUAresum", "CYCRms", "CURSorrms", "OVERShoot",
            "PREShoot", "PERiod", "FREQuency", "RTime", "FTime",
            "PWIDth", "NWIDth", "PDUTy", "NDUTy", "SCREenduty",
            "FRR", "FRF", "FFR", "FFF", "LRR", "LRF", "LFR", "LFF", "RDELay",
            "FDELay", "RPHAse", "PPULsenum", "NPULsenum",
            "RISEedgenum", "FALLedgenum", "AREA", "CYCLearea",
            "HARDfrequency"
        };
        for (auto var : list) {
            cout << osc.wrRdData(":MEASUrement:CH2:" + var + "?");
        }
    }
    getline(std::cin, str);
    while (str != "exit") {
        cout << osc.wrRdData(str);
        getline(std::cin, str);
    }
    return 0;
}
