#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cctype>

using namespace std;

void compareFiles(string fileOne, string fileTwo, fstream &oneIn, fstream &twoIn, bool charFlag, bool numFlag, int bytesToIgnore);
void dirErrorCheck(string fileOne, string fileTwo);
void errorCheck(int argc, char *argv[], fstream &oneIn, fstream &twoIn, string fileOne, string fileTwo, bool &charFlag, bool &numFlag, int &bytesToIgnore);

int main(int argc, char *argv[])
{
    string fileOne = argv[argc-2];
    string fileTwo = argv[argc-1];
    dirErrorCheck(fileOne, fileTwo);
    
    fstream oneIn(fileOne.c_str(), ios::in);
    fstream twoIn(fileTwo.c_str(), ios::in);

    bool charFlag = false, numFlag = false;
    int bytesToIgnore = 0;

    errorCheck(argc, argv, oneIn, twoIn, fileOne, fileTwo, charFlag, numFlag, bytesToIgnore);
    compareFiles(fileOne, fileTwo, oneIn, twoIn, charFlag, numFlag, bytesToIgnore);

    return 0;
}

//Compares the two files
void compareFiles(string fileOne, string fileTwo, fstream &oneIn, fstream &twoIn, bool charFlag, bool numFlag, int bytesToIgnore)
{
    char chOne, chTwo;
    int line = 1;
    int currByte = 0;
    
    if (numFlag)
    {
        currByte = bytesToIgnore;
        oneIn.seekg(bytesToIgnore);
        twoIn.seekg(bytesToIgnore);
    }

    while (oneIn.get(chOne))
    {
        twoIn.get(chTwo);

        currByte++;

        //Checks when eof is reach in any of the files
        if (oneIn.eof())
        {
            cout << "EOF reached on " << fileOne << endl;
            exit(0);
        }
        if (twoIn.eof())
        {
            cout << "EOF reached on " << fileTwo << endl;
            exit(0);
        }


        //Comparison and line incrementation
        if (chOne == '\n' && chTwo == '\n') line++;
        bool comparedChar = (chOne == chTwo) ? true : false;
        
        
        //Comparison fails and return location
        if (!comparedChar)
        {
            if (charFlag)
                cout << fileOne << " " << fileTwo << " differ: char " << currByte << ", line " << line << " is " << oct << (int) chOne << " " << chOne << " " << oct << (int) chTwo << " " << chTwo << endl;
            else
                cout << fileOne << " " << fileTwo << " differ: byte " << currByte << ", line " << line << endl;
            exit(0);
        }
            

    }
    cout << "They Are The Same!" << endl;

}

//Checks files opening is not a directory
void dirErrorCheck(string fileOne, string fileTwo)
{
    struct stat dirFileCheck;
    stat(fileOne.c_str(), &dirFileCheck);
    if(S_ISDIR(dirFileCheck.st_mode))
    {
        cerr << fileOne << " Is A Directory" << endl;
        exit(0);
    }

    stat(fileTwo.c_str(), &dirFileCheck);
    if(S_ISDIR(dirFileCheck.st_mode))
    {
        cerr << fileTwo << " Is A Directory" << endl;
        exit(0);
    }

}

//Checks error cases: incorrect argument count, files not existing, and incorrect arguments
void errorCheck(int argc, char *argv[], fstream &oneIn, fstream &twoIn, string fileOne, string fileTwo, bool &charFlag, bool &numFlag, int &bytesToIgnore)
{
    //Arg count check
    if (argc < 3 || argc > 6)
    {
        cerr << "Incorrect Number of Arguments." << endl;
        exit(0);
    }

    //File existing check
    if (oneIn.fail())
    {
        cerr << fileOne.c_str() << " Does Not Exist. Please Try Again." << endl;
        exit(0);
    }
    if (twoIn.fail())
    {
        cerr << fileTwo.c_str() << " Does Not Exist. Please Try Again." << endl;
        exit(0);
    }


    //Incorrect argument checking
    string argOne;
    string argTwo;

    //Checks based on 4 arguments
    if (argc == 4)
    {
        argOne = argv[1];
        if (argOne == "-c")
            charFlag = true; 
        else if (argOne == "-i")
        {
            cerr << "No Byte Count Given." << endl;
            exit(0);
        }
        else
        {
            cerr << "Invalid Argument." << endl;
            exit(0);
        }
    }

    //Checks based on 5 arguments
    if (argc == 5)
    {
        argOne = argv[1];
        if (argOne == "-i")
        {
            numFlag = true;
            if (isdigit(argv[2][0]))
                bytesToIgnore = atoi(argv[2]);
            else
            {
                cerr << "Incorrect Byte Count Type." << endl;
                exit(0);
            }
        }
        else if (argOne == "-c")
        {
            cerr << "Invalid Number of Arguments." << endl;
            exit(0);
        }
        else
        {
            cerr << "Invalid Arguments." << endl;
            exit(0);
        }
        
        
    }

    //Checks based on 6 arguments
    if (argc == 6)
    {
        argOne = argv[1];
        if (argOne == "-c")
        {
            charFlag = true;
            argTwo = argv[2];
            
            if (argTwo == "-i")
            {
                numFlag = true;
                
                if (isdigit(argv[3][0]))
                    bytesToIgnore = atoi(argv[3]);
                else
                {
                    cerr << "Incorrect Byte Count Type." << endl;
                    exit(0);
                }
            }
            else
            {
                cerr << "Invalid Arguments." << endl;
                exit(0);
            }
        }
        else if (argOne == "-i")
        {
            numFlag = true;

            if(isdigit(argv[2][0]))
            {
                bytesToIgnore = atoi(argv[2]);
                argTwo = argv[3];

                if (argTwo == "-c")
                    charFlag = true;
                else
                {
                    cerr << "Invalid Arguments." << endl;
                    exit(0);
                }
            }
            else
            {
                cerr << "Incorrect Byte Count Type." << endl;
                exit(0);
            }
        }
    }

}