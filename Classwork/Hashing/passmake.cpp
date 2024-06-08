#include <fstream>
#include <cstdlib>         
#include <iostream>
using namespace std;
#define RECSIZ 1024           
#define NUMREC 128            
#define FILNAME "password.dat"
                              
int main(int argc, char *argv[]) {
	ofstream newStream(FILNAME, ios::out | ios::binary) ;
	int i;
	char outBuff[RECSIZ] ;

	if (!newStream) {
		cerr << "Unable to open " << FILNAME << endl ;
		exit(1) ;
	}

	for (i=0; i<RECSIZ; ++i)
		outBuff[i] = '\0' ;

	for (i=0; i<NUMREC && newStream; ++i)
		newStream.write(outBuff, RECSIZ) ;

	if (i != NUMREC) {
		cerr << "Write failed on record " << i << endl ;
		exit(1) ;
	}

	newStream.close() ;

	cout << FILNAME << " initialized" << endl ;
        return 0;
}


