#include <chrono>
#include <iostream>
#include <exception>

#include "cover_tree.h"
#include "utils.h"
#include "loader.h"

enum input_type
{
    BUILD,
    QUERY,
    INSERT,
    DELETE,
};

struct input_operation
{
    input_type type;
    std::string vector_file;
};

struct input_control {
    std::vector<input_operation> operations;
    std::string output_file;
    std::vector<int> ks_to_query;
};

input_control parse_input(int argc, char* argv[]);

void input_operation_destructor(input_control* ctrl);

void help();
