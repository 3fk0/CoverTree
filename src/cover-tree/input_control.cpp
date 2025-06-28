#include "input_control.h"

input_operation* parse_input(int argc, char* argv[]) {
    if (argc < 2) {
        throw std::runtime_error("[Input Control]: Not enough arguments");
    }
    std::cout << argv[1] << std::endl;
    if (argv[1] != std::string("-b")) {
        throw std::runtime_error("[Input Control]: First operation must be -b");
    }

    input_operation* op = (input_operation*) malloc(sizeof(input_operation)*(argc -1));
    bool has_build = false;


    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-b") == 0) {
            if (has_build) {
                throw std::runtime_error("[Input Control]: Multiple build operations");
            }
            has_build = true;
            op[i-1].type = BUILD;
        } else if (strcmp(argv[i], "-q") == 0) {
            op[i-1].type = QUERY;
        } else if (strcmp(argv[i], "-i") == 0) {
            op[i-1].type = INSERT;
        } else if (strcmp(argv[i], "-d") == 0) {
            op[i-1].type = DELETE;
        } else {
            help();
            throw std::runtime_error("[Input Control]: Unknown operation");
        }
        op[i-1].vector_file = argv[i+1];
        i++;
    }

    return op;
}

void input_operation_destructor(input_operation* op) {
    free(op);
}

void help() {
    std::cout << "Usage: ./main -b <path_to_train_points> [[-q <path_to_test_points>] [-i <path_to_insert_points>] [-d <path_to_delete_points>]]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-b : Build the cover tree from the given points" << std::endl;
    std::cout << "-q : Query the cover tree with the given points" << std::endl;
    std::cout << "-i : Insert the given points into the cover tree" << std::endl;
    std::cout << "-d : Delete the given points from the cover tree" << std::endl;
    std::cout << "-h : Show this help message" << std::endl;
}