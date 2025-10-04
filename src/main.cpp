#include <iostream>
#include <sstream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while(true){
    std::cout<<"$ ";
    std::string input;
    std::string word;
    std::string remaining;
    std::getline(std::cin, input);
    std::istringstream iss(input);
    iss>>word;
    std::getline(iss,remaining);

    if(input=="exit 0"){
      break;
    }
    if(word=="echo"){
      if(!remaining.empty()&&remaining[0]==' '){
        remaining.erase(0,remaining.find_first_not_of(' '));
        std::cout<<remaining<<std::endl;
      }
      continue;
    }
    std::cout << input << ": command not found" << std::endl;
  }

  return 0;
}