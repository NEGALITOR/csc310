#include <bits/stdc++.h>
#include "btree.h"

//static int splitCount = 0;

//Constructor
BTree::BTree()
{
    height = 0;
    read = 0;
    write = 0;
}

/*-------------------------------------------------------------------------------------------------------------
/    In: char* filename
/   Out: N/A
/
/   Reads in and prints the root to screen
/   1 Read Total
/------------------------------------------------------------------------------------------------------------*/
void BTree::writeHeader(char* filename)
{
    treeFile.open(filename, ios::in | ios::out | ios::binary);
    BTNode containNode;

    treeFile.seekg(0, ios::beg);
    treeFile.read((char *) &containNode, sizeof(BTNode));
    //cout << containNode.child[0] << endl;

    rootAddr = containNode.child[0];
    root = getNode(rootAddr);
    read++;

}

/*-------------------------------------------------------------------------------------------------------------
/    In: keyType key
/        int recAddr
/   Out: N/A
/
/   Inserts the key within the recAddress node
/   Checks if node is a leaf and not full and inserts accordingly
/   If node isn't a leaf, recurse until a leaf is found
/   If node is a leaf and less than max capacity (4) then insert based on comparison
/   If none of these conditions are satisfied (node is full), then it splits the node
/   1 Write Total
/------------------------------------------------------------------------------------------------------------*/
void BTree::insert(keyType key)
{
    insert(key, rootAddr);
}

void BTree::insert(keyType key, int recAddr)
{
    BTNode containNode = getNode(recAddr);
    int loc;

    if (!isLeaf(recAddr))
    {
        for (loc = 0; loc < containNode.currSize; loc++)
        {
            if (key < containNode.contents[loc])
            {
                insert(key, containNode.child[loc]);
                return;
            }
        }
        insert(key, containNode.child[containNode.currSize]);
        return;

    }
    else if(containNode.currSize < 4)
    {
        
        for (loc = containNode.currSize-1; loc >= 0 && key < containNode.contents[loc]; loc--)
            containNode.contents[loc+1] = containNode.contents[loc];
            
        containNode.contents[loc+1] = key;
        containNode.currSize++;

        treeFile.seekg(recAddr, ios::beg);
        treeFile.write((char *) &containNode, sizeof(BTNode));
        write++;

        if (recAddr == rootAddr) root = containNode;
    }
    else
    {
        cout << "Now Splitting" << endl;
        //splitCount++;
        //cout << splitCount << endl;
        splitNode(key, recAddr, -1, -1);
    }
}

/*-------------------------------------------------------------------------------------------------------------
/    In: keyType &key
/        int recAddr
/        int oneAddr
/        int twoAddr
/   Out: N/A
/
/   Splits node into two seperate nodes (left and right)
/   Leaves two of the contents in  the left node and moves the other 2 plus the added node to the right
/   Writes out to file
/   2 Writes Total
/------------------------------------------------------------------------------------------------------------*/
void BTree::splitNode(keyType& key, int recAddr, int oneAddr, int twoAddr)
{
    BTNode left = getNode(recAddr);
    BTNode parent = getNode(findpAddr(key, root, rootAddr, recAddr));

    if (key < left.contents[3])
    {
        swap(left.contents[3], key);
        swap(left.child[4], twoAddr);

        for (int i = left.currSize-1; i > 0 && left.contents[i] < left.contents[i-1]; i--)
        {
            swap(left.contents[i], left.contents[i-1]);
            swap(left.child[i+1], left.child[i]);
        }
    }

    left.currSize = 2;

    BTNode right;
    right.currSize = 2;
    right.contents[0] = left.contents[3];
    right.contents[1] = key;

    key = left.contents[2];

    right.child[0] = left.child[3];
    right.child[1] = left.child[4];
    right.child[2] = twoAddr;
    right.child[3] = -1;
    right.child[4] = -1;

    treeFile.clear();
    treeFile.seekg(recAddr, ios::beg);
    treeFile.write((char *) &left, sizeof(BTNode));

    treeFile.clear();
    treeFile.seekg(0, ios::end);
    int rightAddr = treeFile.tellg();
    treeFile.write((char *) &right, sizeof(BTNode));

    write+=2;

    placeNode(key, findpAddr(key, root, rootAddr, recAddr), recAddr, rightAddr);
    
}

/*-------------------------------------------------------------------------------------------------------------
/    In: keyType k
/        int recAddr
/        int oneAddr
/        int two Addr
/   Out: N/A
/
/   Promotes the middle value to the parent node
/   Checks if the node has parent or not (root)
/   If a root node, adjust the root
/   Checks if parent is at max compacity or not
/   If not full, inserts the key into correct location as well as adjusting the children
/   If full, split node
/   1 Write Total (excludes other function's writes)
/------------------------------------------------------------------------------------------------------------*/
void BTree::placeNode(keyType k, int recAddr, int oneAddr, int twoAddr)
{
    BTNode parent = getNode(recAddr);
    BTNode left = getNode(oneAddr);
    BTNode right = getNode(twoAddr);

    if (recAddr == -1)
    {
        cout << "Now adjusting root" << endl;
        height++;

        adjRoot(k, oneAddr, twoAddr);
    }
    else if (parent.currSize < 4)
    {
        int i;

        for (i = parent.currSize-1; i >= 0 && k < parent.contents[i]; i--)
        {
            parent.contents[i+1] = parent.contents[i];
            parent.child[i+2] = parent.child[i+1];
        }
        parent.contents[i+1] = k;
        parent.child[i+2] = twoAddr;
        parent.currSize++;

        treeFile.seekg(recAddr, ios::beg);
        treeFile.write((char *) &parent, sizeof(BTNode));
        write++;

        if (recAddr == rootAddr) root = parent;
    }
    else
        splitNode(k, recAddr, oneAddr, twoAddr);
}

/*-------------------------------------------------------------------------------------------------------------
/    In: keyType rootElem
/        int oneAddr
/        int twoAddr
/   Out: N/A
/
/   Moves key to a new node and assigning the oneAddr and twoAddr as its children
/   Set new root to original root variable and write to file at the end
/   Write the rootAddr as the child of the new node and write to file at the beginning
/   2 Writes Total
/------------------------------------------------------------------------------------------------------------*/
void BTree::adjRoot(keyType rootElem, int oneAddr, int twoAddr)
{
    BTNode nRoot;
    nRoot.contents[0] = rootElem;
    nRoot.currSize = 1;

    nRoot.child[0] = oneAddr;
    nRoot.child[1] = twoAddr;
    nRoot.child[2] = -1;
    nRoot.child[3] = -1;
    nRoot.child[4] = -1;

    root = nRoot;
    root.currSize = 1;
    
    treeFile.clear();
    treeFile.seekg(0, ios::end);
    rootAddr = treeFile.tellp();
    treeFile.write((char *) &nRoot, sizeof(BTNode));

    treeFile.clear();
    treeFile.seekg(0, ios::beg);

    BTNode containNode;
    containNode.child[0] = rootAddr;
    treeFile.write((char *) &containNode, sizeof(BTNode));

    write+=2;
}

/*-------------------------------------------------------------------------------------------------------------
/    In: keyType rootElem
/        int oneAddr
/        int twoAddr
/   Out: int tAddr
/ 
/   Checks if parent or current node is the root node
/   Check through the children for node t till an address that the parent matches to is found
/   Checks though t node and compares its contents to the key
/   When the key is less than the content and the child is not -1 recurse or return -1 if child is -1
/   The leaf portion is to just check the right most child
/------------------------------------------------------------------------------------------------------------*/
int BTree::findpAddr(keyType key, BTNode t, int tAddr, int recAddr)
{
    if (tAddr == -1 || recAddr == rootAddr) return -1;

    for (int i = 0; i <= t.currSize; i++) 
        if (recAddr == t.child[i]) return tAddr;
    
    for (int i = 0; i < t.currSize; i++)
        if (key < t.contents[i])
        {
            if(t.child[i] != -1)
                return findpAddr(key, getNode(t.child[i]), t.child[i], recAddr);
            else
                return -1;
        }
    
    if (isLeaf(t))
        return -1;
    else
        return findpAddr(key, getNode(t.child[t.currSize]), t.child[t.currSize], recAddr);

    return -1;
}

/*-------------------------------------------------------------------------------------------------------------
/    In: string key
/        BTNode t
/        int tAddr
/   Out: BoolKeyType foundKey
/
/   Searches through the Btree for a specific key and returns struct BoolKeyType
/   BoolKeyType serves the purpose of performing both search() and retrieve() at once
/   BoolKeytype holds bool found and the keyType key
/------------------------------------------------------------------------------------------------------------*/
BoolKeyType BTree::retrieve(string key)
{
    return retrieve(key, root, rootAddr);
}

BoolKeyType BTree::retrieve(string key, BTNode t, int tAddr)
{
    BoolKeyType foundKey;
    //keyType notFound;
    
    for (int i = 0; i < t.currSize; i++)
    {
        if (strcmp(key.c_str(), t.contents[i].getUPC().c_str()) == 0)
        {
            foundKey.found  = true;
            foundKey.key = t.contents[i];
            return foundKey;
        }
    }
    if (!isLeaf(tAddr))
    {
        for(int i = 0; i < t.currSize; i++)
        {
            if (key < t.contents[i].getUPC())
                return retrieve(key, getNode(t.child[i]), t.child[i]);
        }
        return retrieve(key, getNode(t.child[t.currSize]), t.child[t.currSize]);
    }
    foundKey.found = false;
    return foundKey;
    
}

/*-------------------------------------------------------------------------------------------------------------
/    In: char* filename
/   Out: N/A
/
/   Resets the treeFile for a new B-Tree with a dummy node (containNode)
/   2 Writes Total
/------------------------------------------------------------------------------------------------------------*/
void BTree::reset(char* filename)
{
    BTNode containNode;

    treeFile.open(filename, ios::in | ios::out | ios::binary | ios::trunc);
    treeFile.seekg(0, ios::beg);

    containNode.child[0] = sizeof(BTNode);
    rootAddr = containNode.child[0];

    root.currSize = 0;
    for (int i = 0; i < ORDER; i++)
        root.child[i] = -1;
    
    treeFile.write((char * ) &containNode, sizeof(BTNode));
    treeFile.write((char * ) &root, sizeof(BTNode));
    write+=2;

}

/*-------------------------------------------------------------------------------------------------------------
/    In: int recAddr
/   Out: int numLeaves
/
/   Returns the number of leaves in the B-Tree
/------------------------------------------------------------------------------------------------------------*/
int BTree::countLeaves()
{
    return countLeaves(rootAddr);
}

int BTree::countLeaves(int recAddr)
{
    if (recAddr == -1) return 0;
    
    int numLeaves = 0;
    if(recAddr != -1)
    {
        BTNode containNode = getNode(recAddr);

        for (int i = 0; i <= containNode.currSize; i++)
            numLeaves += countLeaves(containNode.child[i]);
    }

    return numLeaves;
}

/*-------------------------------------------------------------------------------------------------------------
/    In: int rootAddr
/   Out: N/A
/
/   Prints the B-Tree while utilizing printNode()
/------------------------------------------------------------------------------------------------------------*/
void BTree::printTree()
{
    cout << "------------ B-Tree of Height " << height << " ------------" <<  endl;
    printTree(rootAddr);
}

void BTree::printTree(int rootAddr)
{
    if (rootAddr != -1)
    {
        BTNode containNode = getNode(rootAddr);
        printNode(rootAddr);
        for (int i = 0; i <= containNode.currSize; i++)
            printTree(containNode.child[i]);
    }
}

/*-------------------------------------------------------------------------------------------------------------
/    In: int recAddr
/   Out: BTNode containNode
/
/   Gets the node based on the given recAddr
/   1 Read Total
/------------------------------------------------------------------------------------------------------------*/
BTNode BTree::getNode(int recAddr)
{

    BTNode containNode;

    treeFile.seekg(recAddr, ios::beg);
    treeFile.read((char *) &containNode, sizeof(BTNode));
    read++;

    return containNode;
    
}

/*-------------------------------------------------------------------------------------------------------------
/    In: int recAddr
/   Out: N/A
/
/   Prints the node provided by the recAddr
/------------------------------------------------------------------------------------------------------------*/
void BTree::printNode(int recAddr)
{
    BTNode containNode;

    treeFile.seekg(recAddr, ios::beg);
    treeFile.read((char *) &containNode, sizeof(BTNode));
    read++;

    cout << " *** node of size " << containNode.currSize << " *** " << endl;
    for (int i = 0; i < containNode.currSize; i++)
        cout << containNode.contents[i] << endl;
        
}

/*-------------------------------------------------------------------------------------------------------------
/    In: int recAddr
/   Out: bool
/
/   Checks if node is a leaf by checking if left most child is -1 or not
/------------------------------------------------------------------------------------------------------------*/
bool BTree::isLeaf(int recAddr)
{
    return isLeaf(getNode(recAddr));
}

bool BTree:: isLeaf(BTNode t)
{
    return (t.child[0] == -1) ? true : false;
}


/*-------------------------------------------------------------------------------------------------------------
/    In: N/A
/   Out: int height
/
/   Returns height of B-Tree
/------------------------------------------------------------------------------------------------------------*/
int BTree::getHeight()
{
    return height;
}


/*-------------------------------------------------------------------------------------------------------------
/    In: N/A
/   Out: N/A
/
/   Closes treeFile file
/------------------------------------------------------------------------------------------------------------*/
void BTree::close()
{
    treeFile.close();
}

/*-------------------------------------------------------------------------------------------------------------
/    In: N/A
/   Out: N/A
/
/   Prints total io performed onto screen
/------------------------------------------------------------------------------------------------------------*/
void BTree::totalio() const
{
    cout << "Total Reads: " << read << endl;
    cout << "Total Writes: " << write << endl;
}

