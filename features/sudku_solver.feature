Feature: Sudoku Solver
  Scenario: Solving a Sudoku puzzle
    Given a Sudoku puzzle string "003020600900305001001806400008102900700000008006708200002609500800203009005010300"
    When I try to solve the Sudoku puzzle
    Then the solution should match the expected result "483921657967345821251876493548132976729564138136798245372689514814253769695417382"

  Scenario: Solving a Sudoku puzzle in mixed dots and zeros notation
    Given a Sudoku puzzle string "..3.2.6..9..3.5..1..18.64....81.29..7000000080067.82....26.95..8..2.3..9..5.1.3.."
    When I try to solve the Sudoku puzzle
    Then the solution should match the expected result "483921657967345821251876493548132976729564138136798245372689514814253769695417382"

  Scenario: Getting an impossible sudoku puzzle
    Given a Sudoku puzzle string "11..............................................................................."
    When I try to solve the Sudoku puzzle
    Then there is no solution

  Scenario: Getting a incorrect sudoku puzzle
    Given a Sudoku puzzle string "534678912672195348198342567859761423426853791713924856961537284287119635345286179"
    When I try to solve the Sudoku puzzle
    Then there is no solution