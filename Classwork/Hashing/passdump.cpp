#include <fstream>
#include <cstdlib>         
#include <iostream>
using namespace std;
#define RECSIZ 1024           
#define NUMREC 128            
#define FILNAME "password.dat"

const int population[256] = {
	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,		//  0x00 - 0x0F
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,		//  0x10 - 0x1F
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,		//  0x20 - 0x2F
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,		//  0x30 - 0x3F
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,		//  0x40 - 0x4F
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,		//  0x50 - 0x5F
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,		//  0x60 - 0x6F
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,		//  0x70 - 0x7F
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,		//  0x80 - 0x8F
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,		//  0x90 - 0x9F
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,		//  0xA0 - 0xAF
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,		//  0xB0 - 0xBF
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,		//  0xC0 - 0xCF
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,		//  0xD0 - 0xDF
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,		//  0xE0 - 0xEF
	4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8		//  0xF0 - 0xFF
} ;
	
int main(int argc, char *argv[]) {
	ifstream oldStream(FILNAME, ios::in | ios::binary) ;
	int i, j, numones ;
	unsigned char inBuff[RECSIZ] ;

	if (!oldStream) {
		cerr << "Unable to open " << FILNAME << endl ;
		exit(1) ;
	}

	numones = 0 ;

	for (i=0; i<NUMREC; ++i) {
		oldStream.read((char *) inBuff, RECSIZ) ;
		if (!oldStream) {
			cerr << "Read filed on record " << i << endl ;
			exit(1) ;
		}
		for (j=0; j<RECSIZ; ++j)
			numones += population[inBuff[j]] ;
	}

	oldStream.close() ;

	cout << numones << " ones read from " << FILNAME << endl ;
        return 0;
}


