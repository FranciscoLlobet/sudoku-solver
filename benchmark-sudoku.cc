#include <benchmark/benchmark.h>

#include "sudoku.hh"
#include "test-sudoku.hh"

static void Sudoku_Puzzles0(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)Process_File("../data/puzzles0_kaggle");

        ResetMaxLevel();
        ResetSolveCalls();
    }
}

static void Sudoku_Puzzles1(benchmark::State &state)
{
    for (auto _ : state)
    {

        (void)Process_File("../data/puzzles1_unbiased");

        ResetMaxLevel();
        ResetSolveCalls();
    }
}

static void Sudoku_Puzzles2(benchmark::State &state)
{
    for (auto _ : state)
    {

        (void)Process_File("../data/puzzles2_17_clue");

        ResetMaxLevel();
        ResetSolveCalls();
    }
}

static void Sudoku_Puzzles3(benchmark::State &state)
{
    for (auto _ : state)
    {

        (void)Process_File("../data/puzzles3_magictour_top1465");

        ResetMaxLevel();
        ResetSolveCalls();
    }
}

static void Sudoku_Puzzles6(benchmark::State &state)
{
    for (auto _ : state)
    {

        (void)Process_File("../data/puzzles6_forum_hardest_1106");

        ResetMaxLevel();
        ResetSolveCalls();
    }
}

BENCHMARK(Sudoku_Puzzles0)->Unit(benchmark::kSecond)->Iterations(1)->Repetitions(1);
// BENCHMARK(Sudoku_Puzzles1)->Unit(benchmark::kSecond)->Iterations(1)->Repetitions(1);
// BENCHMARK(Sudoku_Puzzles2)->Unit(benchmark::kSecond)->Iterations(1)->Repetitions(1);
BENCHMARK(Sudoku_Puzzles3)->Unit(benchmark::kSecond)->Iterations(1)->Repetitions(3);
BENCHMARK(Sudoku_Puzzles6)->Unit(benchmark::kSecond)->Iterations(1)->Repetitions(3);

BENCHMARK_MAIN();