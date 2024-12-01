#include "utilities.h"

void Utilities::startSectionTime(string section) {
    auto time = chrono::system_clock::now();
    sectionTimes[section] = time;
}

int Utilities::endSectionTime(string section, string unit) {
    auto time = chrono::system_clock::now();
    auto duration = time - sectionTimes[section];

    if (unit == "nanoseconds") return chrono::duration_cast<chrono::nanoseconds>(duration).count();
    if (unit == "milliseconds") return chrono::duration_cast<chrono::milliseconds>(duration).count();
    if (unit == "seconds") return chrono::duration_cast<chrono::seconds>(duration).count();
}

void Utilities::displayHeader(string section) {
    cout << section << endl;
    cout << sectionDivder << endl;
}

void Utilities::displaySectionDivder() {
    cout << sectionDivder << endl;
}

void Utilities::displayText(string text) {
    cout << text << endl;
}