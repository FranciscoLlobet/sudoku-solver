#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "sudoku.hh"

namespace py = pybind11;

PYBIND11_MODULE(sudoku_solver, m) {
    m.doc() = "Sudoku Solver C++ Library wrapped with pybind11";

    py::class_<SudokuPuzzle>(m, "SudokuPuzzle")
        .def(py::init<>())
        .def(py::init<const std::string &>())
        .def(py::init<const SudokuPuzzle &>())
        .def("__copy__", &SudokuPuzzle::operator=)
        .def("initialize_puzzle", py::overload_cast<const std::string &>(&SudokuPuzzle::InitializePuzzle))
        .def("set_value", py::overload_cast<Sudoku_Row_Index_T, Sudoku_Column_Index_T, Sudoku_Values_T>(&SudokuPuzzle::SetValue))
        .def("get_value", &SudokuPuzzle::GetValue)
        .def("solve", py::overload_cast<>(&SudokuPuzzle::Solve))
        .def("get_puzzle", &SudokuPuzzle::GetPuzzleAsString)
        .def("check", &SudokuPuzzle::Check);

    py::enum_<Sudoku_RC_T>(m, "SudokuRC")
        .value("SUDOKU_RC_ERROR", Sudoku_RC_E::SUDOKU_RC_ERROR)
        .value("SUDOKU_RC_NULL_POINTER",Sudoku_RC_E::SUDOKU_RC_NULL_POINTER)
        .value("SUDOKU_RC_INVALID_INPUT",Sudoku_RC_E::SUDOKU_RC_INVALID_INPUT) 
        .value("SUDOKU_RC_INVALID_VALUE", Sudoku_RC_E::SUDOKU_RC_INVALID_VALUE) 
        .value("SUDOKU_RC_NOT_SOLVABLE", Sudoku_RC_E::SUDOKU_RC_NOT_SOLVABLE)
        .value("SUDOKU_RC_SUCCESS", Sudoku_RC_E::SUDOKU_RC_SUCCESS)  
        .value("SUDOKU_RC_PRUNE", Sudoku_RC_E::SUDOKU_RC_NULL_POINTER)
        .export_values();

    py::enum_<SudokuValues_E>(m, "SudokuValues")
        .value("SUDOKU_VALUE_1", SudokuValues_E::SUDOKU_VALUE_1)
        .value("SUDOKU_VALUE_2", SudokuValues_E::SUDOKU_VALUE_2)
        .value("SUDOKU_VALUE_3", SudokuValues_E::SUDOKU_VALUE_3)
        .value("SUDOKU_VALUE_4", SudokuValues_E::SUDOKU_VALUE_4)
        .value("SUDOKU_VALUE_5", SudokuValues_E::SUDOKU_VALUE_5)
        .value("SUDOKU_VALUE_6", SudokuValues_E::SUDOKU_VALUE_6)
        .value("SUDOKU_VALUE_7", SudokuValues_E::SUDOKU_VALUE_7)
        .value("SUDOKU_VALUE_8", SudokuValues_E::SUDOKU_VALUE_8)
        .value("SUDOKU_VALUE_9", SudokuValues_E::SUDOKU_VALUE_9)
        .export_values();
}