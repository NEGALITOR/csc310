## B-Tree Class
---
Note - Alot of these have a public and private version.
```
BTree() 
- constructor that creates the initial root node with address to it
```
```
writeHeader()
    In: char* filename
   Out: N/A

   Reads in and prints the root to screen
   1 Read Total
```
```
insert() 
    In: keyType key
        int recAddr
   Out: N/A

   Inserts the key within the recAddress node
   Checks if node is a leaf and not full and inserts accordingly
   If node isn't a leaf, recurse until a leaf is found
   If node is a leaf and less than max capacity (4) then insert based on comparison
   If none of these conditions are satisfied (node is full), then it splits the node
   1 Write Total
```
```
splitNode()
    In: keyType &key
        int recAddr
        int oneAddr
        int twoAddr
   Out: N/A

   Splits node into two seperate nodes (left and right)
   Leaves two of the contents in  the left node and moves the other 2 plus the added node to the right
   Writes out to file
   2 Writes Total
```
```
placeNode()
    In: keyType k
        int recAddr
        int oneAddr
        int two Addr
   Out: N/A

   Promotes the middle value to the parent node
   Checks if the node has parent or not (root)
   If a root node, adjust the root
   Checks if parent is at max compacity or not
   If not full, inserts the key into correct location as well as adjusting the children
   If full, split node
   1 Write Total (excludes other function's writes)
```
```
adjRoot()
    In: keyType rootElem
        int oneAddr
        int twoAddr
   Out: N/A

   Moves key to a new node and assigning the oneAddr and twoAddr as its children
   Set new root to original root variable and write to file at the end
   Write the rootAddr as the child of the new node and write to file at the beginning
   2 Writes Total
```
```
findpAddr()
    In: keyType rootElem
        int oneAddr
        int twoAddr
   Out: int tAddr
 
   Checks if parent or current node is the root node
   Check through the children for node t till an address that the parent matches to is found
   Checks though t node and compares its contents to the key
   When the key is less than the content and the child is not -1 recurse or return -1 if child is -1
   The leaf portion is to just check the right most child
```
```
retrieve()
    In: string key
        BTNode t
        int tAddr
   Out: BoolKeyType foundKey

   Searches through the Btree for a specific key and returns struct BoolKeyType
   BoolKeyType serves the purpose of performing both search() and retrieve() at once
   BoolKeytype holds bool found and the keyType key
```
```
reset()
    In: char* filename
   Out: N/A

   Resets the treeFile for a new B-Tree with a dummy node (containNode)
   2 Writes Total
```
```
countLeaves()
    In: int recAddr
   Out: int numLeaves

   Returns the number of leaves in the B-Tree
```
```
printTree()
    In: int rootAddr
   Out: N/A

   Prints the B-Tree while utilizing printNode()
```
```
getNode()
    In: int recAddr
   Out: BTNode containNode

   Gets the node based on the given recAddr
   1 Read Total
```
```
printNode()
    In: int recAddr
   Out: N/A

   Prints the node provided by the recAddr
```
```
isLeaf()
    In: int recAddr
   Out: bool

   Checks if node is a leaf by checking if left most child is -1 or not
```
```
getHeight()
    In: N/A
   Out: int height

   Returns height of B-Tree
```
```
close()
    In: N/A
   Out: N/A

   Closes treeFile file
```
```
totalio()
    In: N/A
   Out: N/A

   Prints total io performed onto screen
```