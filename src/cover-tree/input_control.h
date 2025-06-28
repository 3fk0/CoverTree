#include <chrono>
#include <iostream>
#include <exception>

#include "cover_tree.h"
#include "utils.h"
#include "loader.h"

enum input_type {
    BUILD,
    QUERY,
    INSERT,
    DELETE,
};

struct input_operation {
    input_type type;
    char* vector_file;
};


input_operation* parse_input(int argc, char* argv[]);

void input_operation_destructor(input_operation* op);

void help();
