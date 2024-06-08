#include <fstream>
#include <iostream>
#include <set>
using namespace std;

string getSize(string filename);
void matchingItems (fstream& file1, fstream& file2);

int main()
{
	cout << getSize("file1")<<endl;
	return 0;

}

string getSize(string filename)
{
	fstream inFile(filename.c_str(), ios::in);
	inFile.seekg(0, ios::end);
	return to_string(inFile.tellg());
//	return inFile.tellg();
}

void matchingItems (fstream& file1, fstream& file2)
{
	set<string> sorted;
	sorted.insert("hello");
	sorted.insert("wylcome");
	sorted.insert("world");
	set<string> :: iterator sortedI;

	for(sortedI = sorted.begin(); sortedI != sorted.end(); ++sortedI)
	{
		cout << sortedI << endl;
	}
	//cout << sorted[1];	
}
