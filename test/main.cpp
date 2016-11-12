#include <iostream>
#include <string>
#include <cstring>
#include "FileHandler.h"

using namespace std;

void testFileHandler() {
    bool e;
    string filename("file.txt"), extension("txt"), wrong("bmp");
    e = FileHandler::verifyExtension(filename, extension);
    cout << "Test 1 of verifyExtension(): " << ((e) ? "SUCCEED" : "FAILED") << endl;
    e = FileHandler::verifyExtension(filename.c_str(), extension.c_str());
    cout << "Test 2 of verifyExtension(): " << ((e) ? "SUCCEED" : "FAILED") << endl;
    e = !(FileHandler::verifyExtension(filename.c_str(), strlen(filename.c_str()), wrong.c_str(), strlen(wrong.c_str())));
    cout << "Test 3 of verifyExtension(): " << ((e) ? "SUCCEED" : "FAILED") << endl;
}

int main() {
    cout << "Testing.." << endl;
    testFileHandler();
}