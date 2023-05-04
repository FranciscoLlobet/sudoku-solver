#ifndef TEST_SUDOKU_HH_INCLUDED
#define TEST_SUDOKU_HH_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#include <string>
#include <array>
#include <vector>
#include <tuple>

const std::vector<std::string> validTestPuzzles = {
    "974236158638591742125487936316754289742918563589362417867125394253649871491873625", /* Already solved puzzle */
    "2564891733746159829817234565932748617128.6549468591327635147298127958634849362715", /* Almost solved puzzle. One Element left */
    "3.542.81.4879.15.6.29.5637485.793.416132.8957.74.6528.2413.9.655.867.192.965124.8", /* Almost solved puzzle. Naked Singles */
    "..2.3...8.....8....31.2.....6..5.27..1.....5.2.4.6..31....8.6.5.......13..531.4..", /* Solvable puzzle. Hidden Singles */
};

const std::vector<std::string> invalidTestPuzzles = {
    "11...............................................................................", /* Invalid puzzle. Same Row  */
    "1........1.......................................................................", /* Invalid puzzle. Same Column */
    "1.........1.........1............................................................", /* Invalid puzzle. Same Subgrid */
    "534678912672195348198342567859761423426853791713924856961537284287119635345286179", /* Invalid puzzle */
};

const std::vector<std::string> subgridTest = {
    "123000000546000000789000000000000000000000000000000000000000000000000000000000000", /* Full subgid mask */
    "123000000506000000789000000000000000000000000000000000000000000000000000000000000" /* Missing only one value */
};

const std::vector<std::string> testFileNames = {
    "../data/puzzles0_kaggle",
    "../data/puzzles1_unbiased",
    "../data/puzzles2_17_clue",
    "../data/puzzles3_magictour_top1465",
    "../data/puzzles4_forum_hardest_1905",
    "../data/puzzles5_forum_hardest_1905_11+",
    "../data/puzzles6_forum_hardest_1106",
    "../data/puzzles7_serg_benchmark",
    "../data/puzzles8_gen_puzzles"
};

std::tuple<unsigned int, unsigned int, unsigned int, unsigned int>Process_File(std::string file_name);

#endif // TEST_SUDOKU_HH_INCLUDED