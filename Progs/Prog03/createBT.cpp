#include <iostream>
#include <fstream>
#include "btree.h"
#include "album.h"

using namespace std;

int main(int argc, char* argv[])
{
    fstream inFile(argv[1], ios::in);
    BTree btree;
    Album album;

    btree.reset(argv[2]);
    //btree.printTree();

    while (inFile >> album)
    {
        cout << "Inserting " << album << endl;
        btree.insert(album);
    }

    cout << endl;
    btree.printTree();
    //cout << endl;
    //btree.totalio();

    inFile.close();
    btree.close();

    return 0;
}