#include <fstream>
#include <iostream>
#include "btree.h"

using namespace std;

int main(int argc, char* argv[])
{
    fstream inFile(argv[2], ios::in);
    BTree btree;
    string str, key, temp;

    btree.writeHeader(argv[1]);

    while(getline(inFile, str))
    {
        BoolKeyType keyFound;
        //keyFound.found = false;

        if (str[0] == 'S')
        {
            getline(inFile, key);
            keyFound = btree.retrieve(key);

            if (keyFound.found)
                cout << "\t" << keyFound.key << endl;
            else
                cout << "\t\"" << key << "\" not found" << endl;
        }
        else
        {
            Album album;
            inFile >> album;
            cout << "Now Inserting " << album << endl;
            btree.insert(album);
        }
    }

    cout << endl;
    btree.totalio();

    btree.close();
    inFile.close();

    return 0;
    
}
