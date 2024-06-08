#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

typedef char String[25];
struct BookRec //87 bytes
{
    unsigned int isbn; //4
    String name; //25
    String author; //25
    int onhand; //4
    float price; //4
    String type; //25
};

enum TransactionType {Add, Delete, ChangeOnhand, ChangePrice};
struct TransactionRec
{
    TransactionType ToDo;
    BookRec B;
};

int main(int argc, char *argv[])
{
    if(argc < 3) 
    {
        cout << "failed";
        return 0;
    }

    string inFileName = argv[1];
    string outFileName = argv[2];
    fstream inFile(inFileName.c_str(), ios::in);
    fstream outFile(outFileName.c_str(), ios::out | ios::binary);

    //BookRec br;
    TransactionRec tr;
    int temp;
    //char temp[4];

    while (!inFile.eof())
    {
        //inFile.getline(temp, 20, '|');
        inFile >> temp;
        inFile.seekp(1, ios::cur);
        tr.ToDo = (TransactionType) temp;
        inFile >> tr.B.isbn;
        inFile.seekp(1, ios::cur);
        inFile.getline(tr.B.name, 25, '|');
        inFile.getline(tr.B.author, 25, '|');
        inFile >> tr.B.onhand;
        inFile.seekp(1, ios::cur);
        inFile >> tr.B.price;
        inFile.seekp(1, ios::cur);
        inFile.getline(tr.B.type, 25, '\n');

        outFile.write((char *) &tr, sizeof(TransactionRec));

        cout << tr.ToDo << " | " << tr.B.isbn << " | " << tr.B.name << " | " << tr.B.author << " | " << tr.B.onhand << " | " << tr.B.price << " | " << tr.B.type << endl;
    }
}