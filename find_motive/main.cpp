#include <iostream>
#include <string.h>
#include <fstream>
#include <filesystem>
#include <unistd.h>


using namespace std ;

int main(int argc, char const *argv[])
{

    if(argc != 3){
        cout << "Erreur d'arguments" << endl ;
        return -1 ;
    }

    string path = argv[1] ;
    int compt = 0 ;
    
    fstream my_file;
	my_file.open(path, ios::in);
	if (!my_file) {
		cout << "The file " << path << " could not be opened." << endl;
        return 1 ;
	}

    string word = argv[2] ;

    string ch;
    while (1) {
        my_file >> ch;
        if(ch.find(word) != std::string::npos) compt ++ ;
        if (my_file.eof()) break;
    }
    

	my_file.close();

    cout << "The file " << path << " contains " << compt << " words containing the motive " << word << endl ;

	return 0;
    
}
