#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>

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

void readInMapOut(map<unsigned int, unsigned int> &books, BookRec br);
void transactFile(map<unsigned int, unsigned int> &books, BookRec br, string transactFileName, string outputFileName);
void addRecord(BookRec br, map<unsigned int, unsigned int> &books, int transactionNumber, fstream &outCopyBin, fstream &errorFile);
void deleteRecord(BookRec br, map<unsigned int, unsigned int> &books, int transactionNumber, fstream &outCopyBin, fstream &errorFile);
void changeOnHand(BookRec br, map<unsigned int, unsigned int> &books, int transactionNumber, fstream &outCopyBin, fstream &errorFile);
void changePrice(BookRec br, map<unsigned int, unsigned int> &books, int transactionNumber, fstream &outCopyBin, fstream &errorFile);
void makeUpdatedRecords(BookRec br, map<unsigned int, unsigned int> books, fstream& outCopyBin, string outputFileName);

int main(int argc, char *argv[])
{
    if (argc < 4) return 0;

    string fileToCopyName = argv[1];
    string transactFileName = argv[2];
    string outputFileName = argv[3];
    map<unsigned int, unsigned int> books;
    BookRec br;

    system("clear");

    system(("cp " + fileToCopyName + " fileCopy.out").c_str());
    readInMapOut(books, br);
    transactFile(books, br, transactFileName, outputFileName);
    system("rm fileCopy.out");

    return 0;
}

/*----------------------------------------------------------------------------------------------------------------------
Function: readInMapOut()
In:     1) Map of books with an unsigned int isbn as the key and an unsigned int byte offset
        2) BookRec record
        3) File name to read/write output file

Out:    Opens copied file and reads in intital file byte offsets of each BookRec to map
------------------------------------------------------------------------------------------------------------------------*/
void readInMapOut(map<unsigned int, unsigned int> &books, BookRec br)
{
    fstream bInFile("fileCopy.out", ios::in | ios::binary);
    

    while(bInFile.read((char *) &br, sizeof(br)))
    {
        unsigned int byteOffset = (bInFile.tellg() - (long) sizeof(br));
        books[br.isbn] = byteOffset;
    }

    bInFile.close();
}

/*----------------------------------------------------------------------------------------------------------------------
Function: transactFile()
In:     1) Map of books with an unsigned int isbn as the key and an unsigned int byte offset
        2) BookRec record
        3) File name to read/binary in both the transact file and output file
        4) File name to pass thorugh to makeUpdatedRecords().

Out:    Performs transactions (add, delete, changeOnHand, changePrice) to the copy file based on the transaction file.
        Creates a map of the locations of valid entries not marked for deletion
        Writes them into the output file via makeUpdatedRecords()
------------------------------------------------------------------------------------------------------------------------*/
void transactFile(map<unsigned int, unsigned int> &books, BookRec br, string transactFileName, string outputFileName)
{
    fstream bInFile(transactFileName.c_str(), ios::in | ios::binary);
    fstream outCopyBin("fileCopy.out", ios::in | ios::out | ios::binary);
    fstream errorFile("ERROR", ios::out);
    TransactionRec tr;
    int transactionNumber = 0;

    while (bInFile.read((char *) &tr, sizeof(tr)))
    {

        transactionNumber++;
        switch(tr.ToDo)
        {
            case 0:
                addRecord(tr.B, books, transactionNumber, outCopyBin, errorFile);
                break;
            case 1:
                deleteRecord(tr.B, books, transactionNumber, outCopyBin, errorFile);
                break;
            case 2:
                changeOnHand(tr.B, books, transactionNumber, outCopyBin, errorFile);
                break;
            case 3:
                changePrice(tr.B, books, transactionNumber, outCopyBin, errorFile);
                break;
        }
        
    }
    
    bInFile.close();
    errorFile.close();

    makeUpdatedRecords(br, books, outCopyBin, outputFileName);
    outCopyBin.close();

    
}

/*----------------------------------------------------------------------------------------------------------------------
Function: addRecord()
In:     1) BookRec record
        2) Map of books with an unsigned int isbn as the key and an unsigned int byte offset
        3) Current transaction line
        4/5) the fstreams opened in transactFile() of both the copyFile and errorFile

Out:    Checks if isbn exists in file. If so, returns error of duplicate key
        Seeks to end of file, gets byteOffset, inputs to map, and writes to copied file
------------------------------------------------------------------------------------------------------------------------*/
void addRecord(BookRec br, map<unsigned int, unsigned int> &books, int transactionNumber, fstream& outCopyBin, fstream &errorFile)
{
    if (books.find(br.isbn) != books.end())
    {
        errorFile << "Error in transaction number " << transactionNumber << ": cannot add---duplicate key " << br.isbn << endl;
        return;
    }    

    outCopyBin.seekp(0, ios::end);
    unsigned int byteOffset = outCopyBin.tellg();
    books[br.isbn] = byteOffset;
    outCopyBin.write((char *) &br, sizeof(BookRec));
    
}

/*----------------------------------------------------------------------------------------------------------------------
Function: deleteRecord()
In:     1) BookRec record
        2) Map of books with an unsigned int isbn as the key and an unsigned int byte offset
        3) Current transaction line
        4/5) the fstreams opened in transactFile() of both the copyFile and errorFile

Out:    Checks if isbn exists in file. If not, returns error of missing key
        Erases the map item with that isbn
------------------------------------------------------------------------------------------------------------------------*/
void deleteRecord(BookRec br, map<unsigned int, unsigned int> &books, int transactionNumber, fstream& outCopyBin, fstream &errorFile)
{
    if (books.find(br.isbn) == books.end())
    {
        errorFile << "Error in transaction number " << transactionNumber << ": cannot delete---missing key " << br.isbn << endl;
        return;
    }

    books.erase(br.isbn);

}

/*----------------------------------------------------------------------------------------------------------------------
Function: changeOnHand()
In:     1) BookRec record
        2) Map of books with an unsigned int isbn as the key and an unsigned int byte offset
        3) Current transaction line
        4/5) the fstreams opened in transactFile() of both the copyFile and errorFile

Out:    Checks if isbn exists in file. If not, returns error of missing key
        Creates a temporary BookRec, seek in copyFile to isbn byteOffset and read into temp BookRec
        Add temp Bookrec.onhand to passed through BookRec.onhand
        Checks if onhand is negative after adding. If so, print error and set passed through onhand to 0
        Seek to byte offset of BookRec and write to copyFile
------------------------------------------------------------------------------------------------------------------------*/
void changeOnHand(BookRec br, map<unsigned int, unsigned int> &books, int transactionNumber, fstream& outCopyBin, fstream &errorFile)
{
    if (books.find(br.isbn) == books.end())
    {
        errorFile << "Error in transaction number " << transactionNumber << ": cannot change on hand---missing key " << br.isbn << endl;
        return;
    }
    
    BookRec tempBr;

    outCopyBin.seekg(books[br.isbn]);
    outCopyBin.read((char *) &tempBr, sizeof(BookRec));
    br.onhand += tempBr.onhand;

    if (br.onhand < 0) 
    {
        errorFile << "Error in transaction number " << transactionNumber << ": count = " << br.onhand << " for key " << br.isbn << endl;
        br.onhand = 0;
    }

    outCopyBin.seekg(books[br.isbn]);
    outCopyBin.write((char *) &br, sizeof(BookRec));

}

/*----------------------------------------------------------------------------------------------------------------------
Function: changePrice()
In:     1) BookRec record
        2) Map of books with an unsigned int isbn as the key and an unsigned int byte offset
        3) Current transaction line
        4/5) the fstreams opened in transactFile() of both the copyFile and errorFile

Out:    Checks if isbn exists in file. If so, returns error of missing key
        Seek to isbn offset and write to copyFile
------------------------------------------------------------------------------------------------------------------------*/
void changePrice(BookRec br, map<unsigned int, unsigned int> &books, int transactionNumber, fstream& outCopyBin, fstream &errorFile)
{
    
    if (books.find(br.isbn) == books.end())
    {
        errorFile << "Error in transaction number " << transactionNumber << ": cannot change price---missing key " << br.isbn << endl;
        return;
    }

    outCopyBin.seekg(books[br.isbn]);
    outCopyBin.write((char *) &br, sizeof(BookRec));

}

/*----------------------------------------------------------------------------------------------------------------------
Function: makeUpdatedRecords()
In:     1) BookRec record
        2) Map of books with an unsigned int isbn as the key and an unsigned int byte offset
        3) the fstreams opened in transactFile() of both the copyFile and errorFile
        4) File name to write/binary in output file

Out:    Prints out to console in formatted way.
        Places items into output file by utilizing map to get offsets from copyfile that arent marked for deletion
------------------------------------------------------------------------------------------------------------------------*/
void makeUpdatedRecords(BookRec br, map<unsigned int, unsigned int> books, fstream& outCopyBin, string outputFileName)
{
    
    fstream outMasterBin(outputFileName.c_str(), ios::out | ios::binary);
    
    cout << "-----------------------------------------------------------------------------------" << endl;
    cout << "| Book Records" << setw(69) << setfill(' ') << "|" <<endl;
    cout << "-----------------------------------------------------------------------------------" << endl;
    map<unsigned int, unsigned int>::iterator j;
    for (j = books.begin(); j != books.end();  ++j)
    {
        outCopyBin.seekg(j->second);

        outCopyBin.read((char *) &br, sizeof(BookRec));
        outMasterBin.write((char *) &br, sizeof(BookRec));

        cout << "| " << setw(10) << setfill('0') << br.isbn
        << setw(25) << setfill(' ') << br.name
        << setw(25) << br.author
        << setw(3) << br.onhand
        << setw(6) << br.price
        << setw(10) << br.type << " |" << endl;
    }
    outMasterBin.close();
    cout << "-----------------------------------------------------------------------------------" << endl;
    
}