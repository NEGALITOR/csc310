#include <fstream>
#include <iostream>

using namespace std;

int sumOfInts();

int main()
{
    cout << sumOfInts() << endl;
    return 0;
}

int sumOfInts()
{
    fstream inFile("input.txt", ios::in);
    int num, total;
    char nextCh;

    while (!inFile.eof())
    {
        if (inFile.peek() == ' ' || inFile.peek() == ',' || inFile.peek() == '\n') 
        {
            inFile.seekg(1, ios::cur);
            continue;
        }
        inFile >> num;
        if (inFile.fail()) return -1;
        total+=num;
        
    }
    return total;
}