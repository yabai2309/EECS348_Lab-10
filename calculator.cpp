#include <string>
#include <sstream> 
#include <iostream>
#include <cctype>   //For isdigit

using namespace std;

//Helper function that return the absolute value of a long long int
long long my_abs(long long n) {
    return (n < 0) ? -n : n;//If it smaller than 0, return -n, else return n to make sure it's always positive
}

//Helper function to calculate 10^n 
long long powerOf10(int n) {
    if (n < 0) return 0; //Handle the edge case of 0
    if (n == 0) return 1;//Handle the edge case of 1
    long long returnVal = 1; //A dummy value to start with
    for (int i = 0; i < n; ++i) {//Iterate n time
        //Basic overflow check
        long long temp = returnVal; //Store previous value
        returnVal *= 10;
        if (returnVal / 10 != temp) { //Check if multiplication overflowed
            return -1; //Indicate overflow
        }
    }
    return returnVal;//Return back the value
}
//Function to process number into into sign, integer part, fractional part and precision
void processNumber(const string &num, long long &intPart, long long &fracPart, int &sign, int &precision) {
    //Initialize outputs to 0 to make sure the initial value is correct
    intPart = 0;
    fracPart = 0; 
    sign = 1;
    precision = 0; 

    size_t i = 0; //Track the position in the number string
    //Handle sign
    if (i < num.length() && (num[i] == '+' || num[i] == '-')) { //Check if there's a sign
        sign = (num[i] == '-') ? -1 : 1; //Set the sign to -1 if '-' is found, else 1
        i++; //Move to the next position after the sign
    }
    //Process the integer part
    while (i < num.length() && isdigit(num[i])) { //While there's a digit, process the integer part
        //Basic overflow check (could be improved)
        if (intPart > (LLONG_MAX / 10)) { /* Overflow potential */ }
        intPart = intPart * 10 + (num[i] - '0'); //Multiply previous value by 10 and add the current digit
        i++; //Move to the next character
    }
    //If there's a decimal point, process the fractional part
    if (i < num.length() && num[i] == '.') { //If a decimal point is found
        i++; //Skip over the decimal point
        while (i < num.length() && isdigit(num[i])) { //While there's a digit, process the fractional part
            //Basic overflow check for fracPart
            if (fracPart > (LLONG_MAX) / 10) { /* Overflow potential */ }
            fracPart = fracPart * 10 + (num[i] - '0'); //Multiply previous value by 10 and add the current digit
            precision++; //Increment precision for each digit in the fractional part
            i++; //Move to the next character
        }
    }
}
//Function to check if a string is a valid double
bool isValidDouble(const string& num_str) {
    bool hasDecimalPoint = false; // Flag to check if decimal point has been found
    bool hasDigit = false; // Flag to check if at least one digit has been found
    
    if (num_str.empty()) { // If the string is empty, it's invalid
        return false; // Return false for an invalid number
    }
    int i = 0; // Initialize the index for processing the string
    if (num_str[0] == '+' || num_str[0] == '-') { // Handle optional sign
        i++; // Move past the sign
        // If only the sign exists, it's an invalid input
        if (i == num_str.length()) {
            return false; // Return false if no number exists after the sign
        }
    }

    for (i; i < num_str.length(); ++i) { // Iterate through the characters of the number
        if (isdigit(num_str[i])) { // If the current character is a digit
            hasDigit = true; // Set the flag to true
        } else if (num_str[i] == '.') { // If the character is a decimal point
            if (hasDecimalPoint) { // If a decimal point is already found, it's invalid
                return false; // Return false for multiple decimal points
            }
            hasDecimalPoint = true; // Mark that a decimal point is found
        } else { // If the character is neither a digit nor a decimal point
            return false; // Return false for invalid characters
        }
    }
    return hasDigit; // Return true if at least one digit was found
}
//Helper function to format the fraction part
string formatFractional(long long fracPart, int precision) {
    if (precision == 0) return ""; //No fractional part to format
    stringstream ss;
    ss << fracPart; //Convert fractional part to string
    string fracStr = ss.str(); //Store the string version of the fractional part

    int leadingZerosNeeded = precision - fracStr.length(); //Calculate the number of zeros needed to pad
    if (leadingZerosNeeded < 0) leadingZerosNeeded = 0; //Should not happen with correct logic

    string padding = ""; //Initialize the padding string
    for (int k = 0; k < leadingZerosNeeded; ++k) { //Add leading zeros to the padding string
        padding += '0'; //Append a zero to the padding string
    }

    return padding + fracStr; //Return the padded fractional part
}


//Add function
string add(const string& num1_str, const string& num2_str) {
    if (!isValidDouble(num1_str) || !isValidDouble(num2_str)) { //If the input numbers are invalid
        return "Error: Invalid input"; //Return an error message
    }

    long long int1 = 0, int2 = 0, frac1 = 0, frac2 = 0; //Initialize parts of the numbers
    int sign1 = 1, sign2 = 1, prec1 = 0, prec2 = 0; //Initialize sign and precision values

    processNumber(num1_str, int1, frac1, sign1, prec1); //Process the first number
    processNumber(num2_str, int2, frac2, sign2, prec2); //Process the second number

    //Determine max precision
    int maxPrecision = (prec1 > prec2) ? prec1 : prec2; //Find the maximum precision needed

    //Get scaling factors (with basic overflow check)
    long long scale1_pow = powerOf10(maxPrecision - prec1); //Get scaling factor for the first number
    long long scale2_pow = powerOf10(maxPrecision - prec2); //Get scaling factor for the second number
    long long max_prec_pow = powerOf10(maxPrecision); //Get scaling factor for max precision

    if (scale1_pow == -1 || scale2_pow == -1 || max_prec_pow == -1) { //Check for overflow in scaling factor
        return "Error: Overflow in scaling factor"; //Return error if overflow detected
    }

    //Represent both numbers as integers scaled to maxPrecision
    long long scaledFrac1 = frac1 * scale1_pow; //Scale the fractional part of the first number
    long long scaledFrac2 = frac2 * scale2_pow; //Scale the fractional part of the second number

    long long scaledNum1 = sign1 * (int1 * max_prec_pow + scaledFrac1); //Scale the integer and fractional part of the first number
    long long scaledNum2 = sign2 * (int2 * max_prec_pow + scaledFrac2); //Scale the integer and fractional part of the second number

    //Perform addition (check potential overflow)
    long long scaledResult = scaledNum1 + scaledNum2; //Add the scaled numbers

    //Extract sign, integer, and fractional parts
    int resultSign = (scaledResult < 0) ? -1 : 1; //Determine the sign of the result
    long long absScaledResult = my_abs(scaledResult); //Get the absolute value of the result

    long long resultInt = absScaledResult / max_prec_pow; //Extract the integer part of the result
    long long resultFrac = absScaledResult % max_prec_pow; //Extract the fractional part of the result

    stringstream ss_res;
    if (resultSign == -1 && (resultInt != 0 || resultFrac != 0)) { //Handle negative sign
        ss_res << "-"; //Append the negative sign
    }
    ss_res << resultInt; //Append the integer part

    if (maxPrecision > 0) { //If there’s a fractional part to display
        ss_res << "."; //Append the decimal point
        ss_res << formatFractional(resultFrac, maxPrecision); //Append the formatted fractional part
    }

    string finalResult = ss_res.str(); //Convert the result to a string
    size_t decimalPointPos = finalResult.find('.'); //Find the position of the decimal point
    if (decimalPointPos != string::npos) { //If there's a decimal point
        finalResult.erase(finalResult.find_last_not_of('0') + 1, string::npos); //Remove trailing zeros
        if (finalResult.back() == '.') { //If only the decimal point remains
            finalResult.pop_back(); //Remove the decimal point
        }
    }

    if (finalResult == "-0") return "0"; //If the result is "-0", return "0"

    return finalResult; //Return the final result
}

//Function to calculate the multiply 
string multiply(const string& num1_str, const string& num2_str) {
    if (!isValidDouble(num1_str) || !isValidDouble(num2_str)) {//If the input numbers are invalid
        return "Error: Invalid input";//Return an error message
    }
    long long int1 = 0, int2 = 0, frac1 = 0, frac2 = 0;//Initialize parts of the numbers
    int sign1 = 1, sign2 = 1, prec1 = 0, prec2 = 0; //Initialize sign and precision values
    processNumber(num1_str, int1, frac1, sign1, prec1); //Process the first number
    processNumber(num2_str, int2, frac2, sign2, prec2); //Process the second number
    int totalPrecision = prec1 + prec2;//Calculate the total precision needed

    //Get scaling factors (with basic overflow check)
    long long scale1_pow = powerOf10(prec1); //Get scaling factor for the first number
    long long scale2_pow = powerOf10(prec2); //Get scaling factor for the second number
    long long total_prec_pow = powerOf10(totalPrecision); //Get scaling factor for total precision

    if (scale1_pow == -1 || scale2_pow == -1 || total_prec_pow == -1) {//Check for overflow in scaling factor
        return "Error: Overflow in scaling factor";//Return error if overflow detected
    }
    //Represent numbers as scaled integers
    long long scaledNum1_int = int1 * scale1_pow;//Scale the integer part of the first number
    long long scaledNum1 = scaledNum1_int + frac1;//Scale the fractional part of the first number

    long long scaledNum2_int = int2 * scale2_pow; //Scale the integer part of the second number
    long long scaledNum2 = scaledNum2_int + frac2;//Scale the fractional part of the second number

    //Perform multiplication (check potential overflow)
    long long scaledResult = scaledNum1 * scaledNum2;//Multiply the scaled numbers

    int resultSign = sign1 * sign2; //Determine the sign of the result

    long long absScaledResult = my_abs(scaledResult);//Get the absolute value of the result
    long long resultInt = absScaledResult / total_prec_pow;//Extract the integer part of the result
    long long resultFrac = absScaledResult % total_prec_pow;//Extract the fractional part of the result

    stringstream ss_res;
    if (resultSign == -1 && (resultInt != 0 || resultFrac != 0)) {//Handle negative sign
        ss_res << "-";//Append the negative sign
    }
    ss_res << resultInt;//Append the integer part

    if (totalPrecision > 0) {//If there’s a fractional part to display
        ss_res << ".";//Append the decimal point
        ss_res << formatFractional(resultFrac, totalPrecision); //Append the formatted fractional part
    }

    string finalResult = ss_res.str(); //Convert the result to a string
    size_t decimalPointPos = finalResult.find('.'); //Find the position of the decimal point
    if (decimalPointPos != string::npos) { //If there's a decimal point
        finalResult.erase(finalResult.find_last_not_of('0') + 1, string::npos); //Remove trailing zeros
        if (finalResult.back() == '.') { //If only the decimal point remains
            finalResult.pop_back(); //Remove the decimal point
        }
    }
    if (finalResult == "-0") return "0"; //If the result is "-0", return "0"

    return finalResult; //Return the final result
}