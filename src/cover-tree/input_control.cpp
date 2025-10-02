#include "input_control.h"

std::vector<input_operation> parse_input(int argc, char *argv[])
{
    if (argc < 2)
    {
        throw std::runtime_error("[Input Control]: Not enough arguments");
    }

    std::vector<input_operation> op;
    std::vector<int> ks_to_query;
    bool has_build = false;

    for (int i = 1; i < argc; i += 2)
    {
        input_operation new_op;
        if (strcmp(argv[i], "-k") == 0)
        {
            int k = std::stoi(argv[i + 1]);
            if (k <= 0)
            {
                throw std::runtime_error("[Input Control]: k must be a positive integer");
            }
            ks_to_query.push_back(k);
            continue;
        }
        else if (strcmp(argv[i], "-b") == 0)
        {
            if (has_build)
            {
                throw std::runtime_error("[Input Control]: Multiple build operations");
            }
            has_build = true;
            new_op.type = BUILD;
        }
        else if (strcmp(argv[i], "-q") == 0)
        {
            new_op.type = QUERY;
        }
        else if (strcmp(argv[i], "-i") == 0)
        {
            new_op.type = INSERT;
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            new_op.type = DELETE;
        }
        else
        {
            help();
            throw std::runtime_error("[Input Control]: Unknown operation " + std::string(argv[i]));
        }
        new_op.vector_file = argv[i + 1];
        new_op.ks_to_query = ks_to_query;
        op.push_back(new_op);
    }

    return op;
}

void input_operation_destructor(input_operation *op)
{
    free(op);
}

void help()
{
    std::cout << "Usage: ./main [-k <number>] -b <path_to_train_points> [[-q <path_to_test_points>] [-i <path_to_insert_points>] [-d <path_to_delete_points>]]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-k : Specify the k values for k-NN queries (can be used multiple times)" << std::endl;
    std::cout << "-b : Build the cover tree from the given points" << std::endl;
    std::cout << "-q : Query the cover tree with the given points" << std::endl;
    std::cout << "-i : Insert the given points into the cover tree" << std::endl;
    std::cout << "-d : Delete the given points from the cover tree" << std::endl;
    std::cout << "-h : Show this help message" << std::endl;
}