/*
EECS 348 Lab 09
Basic calculator program

Inputs: A file double string
Outputs: Execute the add and multiply with a constant

Collaborators: None
Other sources: None
Author's name: Anh Hoang
Creation date: 4/15/25

*/
#include <iostream>//Include necessary library
#include <fstream>
#include <sstream>
#include "calculator.hpp"
#include "calculator.cpp"
using namespace std;//Use to avoid using std::

int main(int argc, char *argv[]) {
    string filename ; //Prompt the user to put in the file
    cout << "Enter your file name: " ;
    cin >> filename;//Get the filename in
    ifstream inputFile(filename);
    
    if (!inputFile) {//If cannot open the inputFile
        cerr << "Error: Could not open the file." << endl;//Print out the error to user
        return 1;
    }

    string line;//Placeholder to read the file
    const string constant = "-123.456"; //Constant to add to the read number

   //Process each line in the file
    while (getline(inputFile, line)) {
       //Trim whitespace from both ends (if any)
        size_t start = line.find_first_not_of(" \t");
        size_t end = line.find_last_not_of(" \t");

        if (start == string::npos || end == string::npos) {
            continue;//Skip line if it is just whitespace
        }

        string trimmedLine = line.substr(start, end - start + 1);
        
       //Check if the line is a valid double
        if (isValidDouble(trimmedLine)) {
           //Perform the addition with constant (-123.456)
            string result = add(trimmedLine, constant);
            string mult = multiply(trimmedLine, constant);
            cout << trimmedLine << " + (" << constant << ") = " << result << endl;
            cout << trimmedLine << " * (" << constant << ") = " << mult << endl;
        } else {
           //If invalid, print error message
            cout << "Invalid number: " << trimmedLine << endl;
        }
    }

    inputFile.close();//Close the file
    return 0;//Return 0 and end main
        
}
