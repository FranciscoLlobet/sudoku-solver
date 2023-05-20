import os
import sys

from behave import given, then, when

# Add the build directory to the system path so that the sudoku_solver module can be imported
sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "build"))
)

from sudoku_solver import SudokuPuzzle, SudokuRC


@given('a Sudoku puzzle string "{puzzle_str}"')
def step_given_a_sudoku_puzzle_string(context, puzzle_str):
    context.puzzle = SudokuPuzzle(puzzle_str)


@when("I try to solve the Sudoku puzzle")
def step_when_i_solve_the_sudoku_puzzle(context):
    context.solve_rc = context.puzzle.solve()


@then("There is no solution")
def step_when_there_is_no_solution(context):
    assert SudokuRC.SUDOKU_RC_SUCCESS != context.solve_rc


@then('the solution should match the expected result "{expected_solution}"')
def step_then_the_solution_should_match_the_expected_result(context, expected_solution):
    assert SudokuRC.SUDOKU_RC_SUCCESS == context.solve_rc
    assert context.puzzle.get_puzzle() == expected_solution
