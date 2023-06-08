#include <stdint.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <commdlg.h>
#include <span>
#include <memory>
#include <functional>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
std::string string_hash(std::string d){

    auto get_hex = [](std::size_t number){
         std::ostringstream oss;
          oss << std::hex << std::setw(16) << std::setfill('0') << number;
        return oss.str();
    };

    std::hash<std::string> stringHasher;  // Example for hashing strings
    
    std::size_t stringHashValue = stringHasher(d);
    return get_hex(stringHashValue);

}


#define DEBUG 1
int constexpr length(const char* str)
{
    return *str ? 1 + length(str + 1) : 0;
}

std::string scegli_file(){


    OPENFILENAMEA ofn; // Structure for the Open dialog box
    CHAR szFile[MAX_PATH] = {0}; // Buffer to store the selected file path

    // Initialize the OPENFILENAME structure
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All Files\0*.*\0"; // Filter to show all files
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box
    if (GetOpenFileNameA(&ofn))
    {
        // The user selected a file, you can use szFile to access the selected file path
        printf("File scelto: %s\n", szFile);
    }
    else
    {
        // The user canceled the dialog or an error occurred
        DWORD error = CommDlgExtendedError();
        if (error != 0)
        {
            // Handle the error
            printf("Errore: %lu\n", error);
        }
        else
        {
            // User canceled the dialog
            printf("Dialog canceled.\n");
        }
    }

  

    return szFile;
}



void codifica_byte(const void* byte_val, const void* byte_key, void* return_byte){
    uint8_t& val = *((uint8_t*)byte_val);
    uint8_t& key = *((uint8_t*)byte_key);
    uint8_t res = val ^ key;
    uint8_t& r_val = *((uint8_t*)return_byte);
    r_val = res;
    
}


void codifica_stringa(std::span<char> buff,std::span<char> key_buff, char output_buff[]){
    for(int i = 0; i < buff.size(); i++ ){
        char output_byte;
        uint32_t key_idx = i % key_buff.size();
        codifica_byte(&buff[i],&key_buff[key_idx],&output_byte);
        output_buff[i]= output_byte;


        #if DEBUG
        std::cout << i <<": (not encoded["<<(int)buff[i] <<"]) (encoded[" <<(int)output_byte << "])\n";
        std::cout << "key_idx = " << key_idx << "\n";
        #endif
    }


}


void codifica(){
    std::cout << "scegli un file in cui salvare le modifiche:\n";
    std::ofstream output_file(scegli_file(), std::ios::binary);
    
    std::string buff;
    std::string key;
    std::cout << "valore da codificare: ";
    std::getline(std::cin,buff);
    std::cout << "scegliere una password: ";
    std::getline(std::cin,key);
    std::cout << "sto codificando: "<< buff << "\n";
    char* output_buff= new char[length(buff.c_str())+1];
    codifica_stringa(buff,key,output_buff);

    
    for(int i = 0; i < length(buff.c_str());i++){
        std::cout << "["<< (int)output_buff[i] << "]";
        output_file.put(output_buff[i]);
    }
    std::cout << "\n";
    

    output_file.close();
    

}



std::span<char> leggi_file(std::string path){

    // Open the file
    std::ifstream file(path, std::ios::binary);
    
    // Get the size of the file
 
    std::vector<char> buff(std::istreambuf_iterator<char>(file),{});
    #if DEBUG
    for(char i:buff){
        std::cout<< (int)i<< " ";
    }
    std::cout <<  "\n";
    #endif

    
    // Close the file
    file.close();
    
    return buff;
 }
void decodifica(){
    std::cout << "scegli un file da cui decoficare:\n";
    std::span<char> file = leggi_file(scegli_file());
    std::cout << "immettere la pasword: ";
    std::string key;
    std::getline(std::cin,key);
    char* output_buff= new char[file.size()];
    codifica_stringa(file,key,output_buff);
    std::cout << "\ndecodifica in forma di byte:\n";
    for(int i = 0; i < file.size();i++){
        std::cout << (int)output_buff[i] << " ";
        
    }
    
    std::cout<< "\ndecodifica:\n";
    for(int i = 0; i < file.size();i++){
        std::cout << output_buff[i];
        
    }

    std::cout << "\n";
}

int main(){
    while(true){
        std::cout << "vuoi decodificare o codificare una stringa di testo? (d/c): ";
        std::string opzione;
        std::getline(std::cin,opzione);
        if(opzione == "d")
            decodifica();

        else if(opzione == "c"  )
            codifica();
        else{
            break;
            }
        std::cout << "\n";
        
    
    }
    return 0;


}