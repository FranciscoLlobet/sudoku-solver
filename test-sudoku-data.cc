#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest/doctest.h"

#include <fstream>
#include <iostream>
#include <string>

#include "sudoku_engine.h"

using namespace std;

TEST_CASE("Test")
{

    ifstream test_data_file;

    test_data_file.open("../sudoku17.txt");

    SudokuPuzzle_P p = Sudoku_MallocPuzzle();
    string data_array;

    for (unsigned int i = 0; i < 1000; i++)
    {
        getline(test_data_file, data_array);
      
        if (81 == data_array.length())
        {
            Sudoku_InitializePuzzle(p);
            Sudoku_InitializeFromArray(p, data_array.c_str());
            int rc = Sudoku_Solve(p);

            //Sudoku_PrintPuzzle(p);
            CHECK(0 == rc);
        }
        else
        {
            cout << data_array.length() << endl;
        }
    }
    Sudoku_FreePuzzle(p);

    test_data_file.close();
}