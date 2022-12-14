#include "bp_tree/bp_tree.h"
#include "storage/disk_storage.h"
#include "storage/helper_types.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <unordered_map>
#include <map>

using namespace std;

int main()
{
    // =============== Read in data and save on disk ======================= //
    // User can set the block size
    cout << "Welcome! First enter '1' or '2' to select the Block Size " << endl;
    cout << "(1) 200 Bytes" << endl;
    cout << "(2) 500 Bytes" << endl;
    int input;
    cin >> input;
    int BLOCKSIZE = (input - 1) ? 500 : 200;

    // Error handling by setting the Block Size to 200 by default
    if (BLOCKSIZE != 200 && BLOCKSIZE != 500)
    {
        cout << "\nInvalid Block Size, it will default to 200 Bytes" << endl;
        BLOCKSIZE = 200;
    }
    cout << "Success! Block Size: " << BLOCKSIZE << "B" << endl;

    // Create a 150MB disk
    DiskStorage disk(150000000, BLOCKSIZE);

    // Creating the tree
    BPTree bptree = BPTree(BLOCKSIZE, &disk);
    // cout << "\nMax keys in node of B+ tree (n): " << bptree.getMaxDataKey() << endl;
    // cout << "Height of the B+ tree: " << bptree.getHeight() << endl;

    // Reset the no. of blocks accessed
    disk.resetNumBlocksAccessed();

    // Read in data
    ifstream file("data/data.tsv");

    // Check if file is open
    if (file.is_open())
    {
        cout << "\nReading in data..." << endl;
        cout << "Building B+ Index simultaneously..." << endl;

        cout << "\nBe patient this takes up to 10 mins, progress below out of 107 units (each unit = 10,000 records):" << endl;
        // Each line in the file will be read into this variable
        string line;

        // Counter
        int recordNum = 0;

        // Flag to check if it is the first line of the tsv
        bool firstLine = true;

        while (getline(file, line))
        {
            // Throw away the first line because it is just column names
            if (firstLine)
            {
                firstLine = false;
                continue;
            }

            // Data will be saved into a Record
            Record record;

            stringstream linestream(line);
            string data;

            // Saving the tconst value
            strcpy(record.tconst, line.substr(0, line.find("\t")).c_str());
            getline(linestream, data, '\t');

            // Saving averageRating and numVotes into the record
            linestream >> record.averageRating >> record.numVotes;

            // Write this record to the disk
            Address address = disk.writeToDisk(&record, sizeof(Record));

            // Building B+ tree index on numVotes by inserting the records sequentially
            bptree.insertKey(address, record.numVotes);

            recordNum++;

            // Progress Counter
            if (recordNum % 10000 == 0)
            {
                cout << (int)(recordNum / 10000) << " ";
            }
        }
        cout << "\nNumber of records read: " << recordNum << endl;
        file.close();
    }

    // Experiment 1
    cout << "\n\n============================== Experiment 1 ==============================" << endl;
    cout << "\nStoring the data on the disk...                  " << endl;
    cout << "\nNumber of blocks used for disk storage         : " << disk.getNumBlocksAllocated() << endl;
    cout << "Size of database used by allocated blocks      : " << disk.getTotalDiskUsage() << "B or " << disk.getTotalDiskUsage() / pow(10, 6) << "MB" << endl;
    // cout << "Actual size of database used by saved records  : " << disk.getDiskUsage() << "B or " << disk.getDiskUsage() / pow(10, 6) << "MB" << endl;
    cout << "__________________________________________________________________________" << endl;

    // Reset the no. of blocks accessed for the next experiment
    disk.resetNumBlocksAccessed();

    // Experiment 2
    cout << "\n\n\n============================== Experiment 2 ==============================" << endl;
    cout << "\nB+ Tree was constructed on numVotes. Parameters...       " << endl;
    cout << "\nParameter n of the B+ Tree     : " << bptree.getMaxDataKey() << endl;
    cout << "Number of nodes of the B+ tree : " << bptree.getNumOfNodes() << endl;
    cout << "Height of the B+ tree          : " << bptree.getHeight() << endl;
    cout << "Content of Root Node" << endl;
    bptree.displayNode(bptree.getRoot());
    cout << "\nContent of Root Node's 1st child" << endl;
    bptree.displayNode(bptree.getRoot()->getFirstChild());
    cout << "__________________________________________________________________________" << endl;

    // Reset the no. of blocks accessed for the next experiment
    disk.resetNumBlocksAccessed();

    // Experiment 3
    cout << "\n\n\n============================== Experiment 3 ==============================" << endl;
    cout << "\nRetrieving movies with 'numVotes'equal to 500\n"
         << endl;
    // We call search with lower = upper to perform a specific key query
    bptree.search(500, 500);
    cout << "\nTotal number of index nodes accessed by the search         : " << bptree.getNumIndexNodesAccessed() << endl;
    cout << "Total number of data blocks accessed by the search         : " << disk.getNumBlocksAccessed() << endl;
    cout << "Average of 'averageRating' attribute of records retrieved  : " << bptree.getAverageOfAverageRatings() << endl;
    cout << "__________________________________________________________________________" << endl;

    // Reset the no. of blocks and no. of index nodes accessed for the next experiment
    disk.resetNumBlocksAccessed();
    bptree.resetNumIndexNodesAccessed();

    // Experiment 4
    cout << "\n\n\n============================== Experiment 4 ==============================" << endl;
    cout << "\nRetrieving movies with 'numVotes' from 30,000 to 40,000, both inclusive\n"
         << endl;
    // We call search with lower < upper to perform a range query on the keys
    bptree.search(30000, 40000);
    cout << "\nTotal number of index nodes accessed by the search         : " << bptree.getNumIndexNodesAccessed() << endl;
    cout << "Total number of data blocks accessed by the search         : " << disk.getNumBlocksAccessed() << endl;
    cout << "Average of 'averageRating' attribute of records retrieved  : " << bptree.getAverageOfAverageRatings() << endl;
    cout << "__________________________________________________________________________" << endl;

    // Reset the no. of blocks accessed for the next experiment
    disk.resetNumBlocksAccessed();
    bptree.resetNumIndexNodesAccessed();

    // Experiment 5
    cout << "\n\n\n============================== Experiment 5 ==============================" << endl;
    cout << "\nDeleting movies with 'numVotes' equal to 1,000" << endl;
    int numDeleted = bptree.remove(1000);
    cout << "\nNumber of times that a node is deleted (or two nodes are merged)   : " << numDeleted << endl;
    cout << "Number of nodes of the updated B+ tree                             : " << bptree.getNumOfNodes() << endl;
    cout << "Height of the updated B+ tree                                      : " << bptree.getHeight() << endl;
    cout << "\nContent of Root Node of the updated B+ tree" << endl;
    bptree.displayNode(bptree.getRoot());
    cout << "Content of Root Node's 1st child of the updated B+ tree" << endl;
    bptree.displayNode(bptree.getRoot()->getFirstChild());
    cout << "__________________________________________________________________________" << endl;

    // Reset the no. of blocks accessed. It's a good habit!
    disk.resetNumBlocksAccessed();
    bptree.resetNumIndexNodesAccessed();

    cout << "\nAll experiments for block size " << BLOCKSIZE << "B completed. Have a nice day!" << endl;

    return 0;
}