#include <fstream>
#include <iostream>

using namespace std;

string tail(string fileName, int linesToPrint);

int main(int argc, char *argv[])
{
    int linesToPrint = abs(stoi(argv[1]));
    string fileName = argv[2];   
    
    cout << tail(fileName, linesToPrint) << endl;
}

string tail(string fileName, int linesToPrint)
{
    fstream inOut(fileName.c_str(), ios::in | ios::out);
    string output = "";
    int lineNumber = 0;
    int count = -1;
    char ch;

    inOut.seekg(-1, ios::end);

    inOut.get(ch);
    if (ch == '\n') count = -2;

    //inOut.seekg(0);
    while (lineNumber < linesToPrint)
    {
        long currentPos = inOut.tellg();

        if (currentPos == -1)
        {
            break;
        }

        inOut.seekg(count, ios::end);
        inOut.get(ch);
        //cout << ch;
        //cout << "< " << ch << " > " << inOut.tellg() << endl;
        

        if (ch == '\n')
        {
            lineNumber++;
        }
        
        output = ch + output;
        count--;
    }
    output = output.substr(1, output.length()-1);
    inOut.close();
    return output;
}