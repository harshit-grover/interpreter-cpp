#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

std::string read_file_contents(const std::string& filename);

int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    // std::cerr << "Logs from your program will appear here!" << std::endl;

    if (argc < 3) {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);

        bool haderror=false ;
        int line=1;
        
        // Uncomment this block to pass the first stage
        // 
        if (!file_contents.empty()) {

            for (int  i = 0; i < file_contents.length(); i++) {
                char ch = file_contents[i];
        
                switch (ch) {
                    case '(': cout << "LEFT_PAREN ( null" << endl; break;
                    case ')': cout << "RIGHT_PAREN ) null" << endl; break;
                    case '{': cout << "LEFT_BRACE { null" << endl; break;
                    case '}': cout << "RIGHT_BRACE } null" << endl; break;
                    case ',': cout << "COMMA , null" << endl; break;
                    case '.': cout << "DOT . null" << endl; break;
                    case '-': cout << "MINUS - null" << endl; break;
                    case '+': cout << "PLUS + null" << endl; break;
                    case ';': cout << "SEMICOLON ; null" << endl; break;
                    case '*': cout << "STAR * null" << endl; break;
                    //case '\n': line++; break; // Increment line number on newline
                    
                    // Handling `=` and `==`
                    case '=':
                        if (i + 1 < file_contents.length() && file_contents[i + 1] == '=') {
                            cout << "EQUAL_EQUAL == null" << endl;
                            i++; // Skip next character since we already consumed it
                        } else {
                            cout << "EQUAL = null" << endl;
                        }
                        break;
        
                    case '!':
                        if(i+1<file_contents.length() && file_contents[i+1]=='='){
                            cout<<"BANG_EQUAL != null"<<endl;
                            i++;
                            break;
                        }
                        else {
                            cout<<"BANG ! null"<<endl;
                            break;
                        }

                    case '<':
                        if(i+1<file_contents.length() && file_contents[i+1]=='='){
                            cout<<"LESS_EQUAL <= null"<<endl;
                            i++;
                            break;
                        }
                        else {
                            cout<<"LESS < null"<<endl;
                            break;
                        }

                    case '>':
                        if(i+1<file_contents.length() && file_contents[i+1]=='='){
                            cout<<"GREATER_EQUAL >= null"<<endl;
                            i++;
                            break;
                        }
                        else {
                            cout<<"GREATER > null"<<endl;
                            break;
                        }

                    case '/':
                        if(i+1<file_contents.length() && file_contents[i+1]=='/'){
                            i++;
                            // we have to skip everything 
                           while(i<file_contents.length() && file_contents[i]!='\n'){
                                i++;
                           }
                           line++;
                        }
                        else {
                            cout<<"SLASH / null"<<endl;
                            
                        }
                        break;

                    case ' ': break;
                    case '\t':break;
                    case '\n':
                        line++;
                        break;

                    // scanning the string literal 
                    case '"':{
                        string s="";
                        i++;
                        while(i<file_contents.length() && file_contents[i]!='"'){
                            if(file_contents[i]=='\n'){
                                line++;
                            }
                            s.push_back(file_contents[i]);
                            i++;
                        }

                        if(i>=file_contents.length()){
                            haderror=true;
                            cerr << "[line " << line << "] Error: Unterminated string." << endl;
                            break;
                        }

                        cout << "STRING \"" << s << "\" " << s << endl;
                        break;
                    }

                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9': {
                        string num;
                        size_t start = i;
                        bool hasDecimal = false;
        
                        while (i < file_contents.length() && (isdigit(file_contents[i]) || file_contents[i] == '.')) {
                            if (file_contents[i] == '.') {
                                if (hasDecimal) break; // Prevent multiple decimal points
                                hasDecimal = true;
                            }
                            num += file_contents[i];
                            i++;
                        }
                        i--; // Roll back one step as loop overshoots
        
                        cout << "NUMBER " << num << " ";
                        bool isInteger = true;
                        if (hasDecimal) {
                            size_t decimalPos = num.find('.');
                            string decimalPart = num.substr(decimalPos + 1);
                            for (char c : decimalPart) {
                                if (c != '0') {
                                    isInteger = false;
                                    break;
                                }
                            }
                        }

                        if (isInteger) {
                            // Print as an integer
                            cout << stoi(num) <<".0"<< endl;
                        } else {
                            // Print as a floating-point number with the original precision
                            cout << stod(num) << endl;
                        }
                        break;
                    }
                        
                    default:
                        cerr << "[line "<<line <<"] Error: Unexpected character: " << ch << endl;
                        haderror = true;
                        break;
                }
            }
            std::cout << "EOF  null" << std::endl;
            return haderror ? 65 : 0;
        }
        std::cout << "EOF  null" << std::endl;
        
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
