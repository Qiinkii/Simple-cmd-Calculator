#include <iostream>
#include <string>
#include <limits>
#include <stack>
#include <fstream>

bool getInstruction(std::string &response) {

    if(response.empty()){
        return false;
    }

    for(int i = 0; i < response.length(); ++i) {
        if(response[i] == '#') {
            response.erase(response.begin() + i, response.end());
            break;
        }
    }

    bool end = true;
    int i = 0;
    for(; i < response.length(); ++i) {
        char c = response[i];
        if(c == ' ' || (c >= '0' && c <= '9') || c == '+' || c == '-' || c == '/' || c == '*' || c == '(' || c == ')' || c == '.') {
            continue;
        }
        else {
            end = false;
            break;
        }
    }

    if(end == true)
        return true;
    else {
        std::cout << "Unknow symbol: " << response[i];
        return false;
    }
    
}

int convertInPriority(char op) {
    switch(op) {
        case '(': return 0;
        case '+':
        case '-': return 1;
        case '/':
        case '*': return 2;
    }
}

double mathCalculate(char symbol, double a, double b) {
    switch(symbol) {
        case '+': return a+b;
        case '-': return a-b;
        case '/': return a/b;
        case '*': return a*b;
    }
}

double logicCalculator(std::string &response, bool isVerbose) {
    std::stack<double> numbers;
    std::stack<char> operators;

    for(int i = 0; i < response.length(); ++i) {
        char c = response[i];
        if(c == ' '){
            continue;
        }

        if(response[i] >= '0' && response[i] <= '9') {
            double val = 0.0;

            while(i < response.length() && (response[i] >= '0' && response[i] <= '9')) {
                val = (val * 10) + (response[i] - '0');
                i++;
            }
            if(response[i] == '.') {
                i++;
                double divisor = 10.0;

                while(i < response.length() && (response[i] >= '0' && response[i] <= '9')) {
                    val += (response[i] - '0')/divisor;
                    divisor *= 10;
                    i++;
                }
            }
            i--;

            if(isVerbose) {
                std::cerr << "[Log] Pushed number: " << val << std::endl;
            }
            numbers.push(val);
        }
        else {
            if(response[i] == '(') {
                operators.push(response[i]);
                continue;
            }
            else if(response[i] == ')') {
                while(operators.top() != '(') {
                    char symbol = operators.top();
                    operators.pop();
                    double a = numbers.top();
                    numbers.pop();
                    double b = numbers.top();
                    numbers.pop();

                    if(isVerbose) 
                        std::cerr << "[Log] Calculated: " << b << " " << symbol << " " << a << std::endl;

                    numbers.push(mathCalculate(symbol, b, a));
                }
                operators.pop();
                continue;
            }

            while(true) {
                if(operators.empty()) {
                    if(isVerbose)
                        std::cerr << "[Log] Pushed operator: " << response[i] << std::endl;

                    operators.push(response[i]);
                    break;
                }

                if(convertInPriority(operators.top()) >= convertInPriority(response[i])){
                    char symbol = operators.top();
                    operators.pop();
                    double a = numbers.top();
                    numbers.pop();
                    double b = numbers.top();
                    numbers.pop();

                    if(isVerbose) 
                        std::cerr << "[Log] Calculated: " << b << " " << symbol << " " << a << std::endl;

                    numbers.push(mathCalculate(symbol, b, a));
                }
                else if(convertInPriority(operators.top()) < convertInPriority(response[i])){
                    if(isVerbose)
                        std::cerr << "[Log] Pushed operator: " << response[i] << std::endl;

                    operators.push(response[i]);
                    break;
                }
            }
        }
    }

    while(!operators.empty()){
        char symbol = operators.top();
        operators.pop();
        double a = numbers.top();
        numbers.pop();
        double b = numbers.top();
        numbers.pop();

        if(isVerbose) 
            std::cerr << "[Log] Calculated: " << b << " " << symbol << " " << a << std::endl;

        numbers.push(mathCalculate(symbol, b, a));
    }

    if(isVerbose)
        std::cerr << "[Log] Returned number: " << numbers.top() << std::endl;

    return numbers.top();
}

int main(int argc, char* argv[]) {
    bool showHelp = false;
    bool isVerbose = false;
    bool isFile = false;
    bool isCalculate = false;
    std::string userCalculate = "";
    std::string filePath = "";

    for(int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if(arg == "-h" || arg == "--help" || arg == "-?") {
            showHelp = true;
        }
        else if(arg == "-v" || arg == "--verbose") {
            isVerbose = true;
        }
        else if(arg == "-c" || arg == "--calculate") {
            if(i+1 < argc){
                isCalculate = true;
                userCalculate = argv[i+1];
                ++i;
            }
            else {
                std::cout << "Unknow expression." << std::endl;
                return 1;
            }
        }
        else if(arg == "-f" || arg == "--file"){
            if(i+1 < argc){
                isFile = true;
                filePath = argv[i+1];
                ++i;
            }
            else{
                std::cout << "The file path has not been specified." << std::endl;
                return 1;
            }
        }
        else{
            std::cout << "Unknown argument: " << arg << std::endl;
            return 1;
        }
    }

    if(showHelp) {
        std::cout << "\t\t---- HELP MENU ----" << std::endl;
        std::cout << "Usage: ./main [-h] [-v] [-c] [your expression]" << std::endl;
        std::cout << "[-h], [--help], [-?]:     outputs useful commands" << std::endl;
        std::cout << "[-v], [--verbose]:        displays program logs" << std::endl;
        std::cout << "[-c], [--calculate]:      allows you to enter mathematical operations" << std::endl;
        std::cout << "[-f], [--file]:           You need to specify the path to the file." << std::endl;
        return 0;
    }

    if(isVerbose) {
        std::cerr << "Turn verbosing..." << std::endl;
    }

    if(isFile){
        std::ifstream inFile(filePath);

        if(!inFile.is_open()){
            std::cerr << "[Error]: Unable to open the file" << std::endl;
            return 1;
        }
        else{
            std::string line;
            while(std::getline(inFile, line)) {
                if(getInstruction(line)){
                    double a = logicCalculator(line, isVerbose);
                    std::cout << "Result: " << a << std::endl;    
                }
            }
        }
    }

    if(isCalculate){
        if(getInstruction(userCalculate)){
            double a = logicCalculator(userCalculate, isVerbose);
            std::cout << "Result: " << a << std::endl;
        }
    }

    return 0;
}