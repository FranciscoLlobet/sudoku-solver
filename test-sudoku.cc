#include <fstream>
#include <iostream>

#include "sudoku.hh"
#include "test-sudoku.hh"

using namespace std;

tuple<unsigned int, unsigned int, unsigned int, unsigned int> Process_File(string file_name)
{
    unsigned int success = 0;
    unsigned int prune = 0;
    unsigned int error = 0;
    unsigned int count = 0;

    ifstream test_data_file;
    string data_array;

    test_data_file.open(file_name);
    while (getline(test_data_file, data_array))
    {
        if (81 == data_array.length())
        {
            SudokuPuzzle p(data_array);

            auto rc = p.Solve();
            switch (rc)
            {
            case SUDOKU_RC_SUCCESS:
                success++;
                break;
            case SUDOKU_RC_PRUNE:
                prune++;
                break;
            default:
                error++;
            }
            count++;
        }
    }
    test_data_file.close();

    return make_tuple(success, prune, error, count);
}
