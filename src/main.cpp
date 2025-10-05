  #include <iostream>
  #include <sstream>
  #include <string>
  #include <filesystem>
  #include <cstdlib>
  namespace fs=std::filesystem;


    fs::path find_executable_in_path(std::string &word2){
      const char* path_env = std::getenv("PATH");
      if(!path_env)return {};

      std::istringstream path_stream(path_env);
      std::string dir;

      #ifdef _WIN32
        const char delimeter = ';';
      #else 
        const char delimeter = ':';
      #endif
        fs::path candidate;
        while (std::getline(path_stream,dir,delimeter)){
          if(!fs::exists(dir))continue;
          candidate = fs::path(dir)/word2;
          if(fs::exists(candidate)&&fs::is_regular_file(candidate)&&(fs::status(candidate).permissions()&fs::perms::owner_exec)!=fs::perms::none){
            return fs::canonical(candidate);
          }
        }
      #ifdef _WIN32
        fs::path exe_candidate = candidate;
        exe_candidate +=".exe";
        if(fs::exists(exe_candidate)&&fs::is_regular_file(exe_candidate)){
          return fs::canonical(exe_candidate);
        }
      #endif
        return {};
    }

  void handleType(std::string &remaining){
    std::string word2;
    std::string remainingType;
    std::istringstream ist(remaining);
    ist>>word2;
    std::getline(ist,remainingType);

    if((word2=="type"||word2=="echo"||word2=="exit")&&(remainingType.empty()||remainingType.find_first_not_of(" \t\n\r")==std::string::npos)){
      std::cout<< word2 << " is a shell builtin"<<std::endl;
    }else{

      fs::path exec_path = find_executable_in_path(word2);
      if(!exec_path.empty()){
        std::cout<< word2 << " is " << exec_path.string() << std::endl;
      }
      else{
        std::cout<< word2 << ": not found"<<std::endl;
      }
    }
  }

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

      if(word=="type"){
        handleType(remaining);
        continue;
      }

      std::cout << input << ": command not found" << std::endl;
    }

    return 0;
  }