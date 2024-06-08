#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
        string filename = argv[1];
        fstream inOut (filename.c_str(), ios::in | ios::out);
        int count = 0;
        char ch;

        inOut.seekg(0);
        while ( inOut.get(ch) )
        {
                cout.put(ch);
                count++;
                if (ch == '\n')
                {
                        long mark = inOut.tellg();
                        inOut.seekp(0, ios::end);
                        inOut << count << " ";
                        inOut.seekg(mark);
                }
        }
        inOut.clear();
        inOut << count << endl;

        cout << "[ " << count << " ]" << endl;
        return 0;
}

