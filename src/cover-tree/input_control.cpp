#include "input_control.h"

input_control parse_input(int argc, char *argv[])
{
    if (argc < 2)
    {
        throw std::runtime_error("[Input Control]: Not enough arguments");
    }

    input_control ctrl;
    std::vector<input_operation> op;
    std::vector<int> ks_to_query;
    bool has_build = false;

    for (int i = 1; i < argc; i += 2)
    {
        input_operation new_op;
        std::string param = argv[i];
        std::string value = (i + 1 < argc) ? argv[i + 1] : "";
        
        if (param == "-k")
        {
            int k = std::stoi(value);
            if (k <= 0)
            {
                throw std::runtime_error("[Input Control]: k must be a positive integer");
            }
            ks_to_query.push_back(k);
            continue;
        }
        else if (param == "-b")
        {
            if (has_build)
            {
                throw std::runtime_error("[Input Control]: Multiple build operations");
            }
            op.push_back({BUILD, value});
            has_build = true;
        }
        else if (param == "-q")
        {
            op.push_back({QUERY, value});
        }
        else if (param == "-i")
        {
            op.push_back({INSERT, value});
        }
        else if (param == "-d")
        {
            op.push_back({DELETE, value});
        }
        else if (param == "-o")
        {
            ctrl.output_file = value;
        }
        else if (param == "-h")
        {
            help();
            exit(0);
        }
        else
        {
            help();
            std::cout << "[Input Control]: Unknown operation " << param << " " << value << std::endl;
            throw std::runtime_error("[Input Control]: Unknown operation " + param + " " + value);
        }
    }

    ctrl.operations = op;
    ctrl.ks_to_query = ks_to_query;
    return ctrl;
}

void input_operation_destructor(input_operation *ctrl)
{
    free(ctrl);
}

void help() {
    std::cout << "Usage: ./cover-tree -b <path_to_train_points> [[-q <path_to_test_points>] [-i <path_to_insert_points>] [-d <path_to_delete_points>]] -o <output-file>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-k : Specify the k values for k-NN queries (can be used multiple times)" << std::endl;
    std::cout << "-b : Build the cover tree from the given points" << std::endl;
    std::cout << "-q : Query the cover tree with the given points" << std::endl;
    std::cout << "-i : Insert the given points into the cover tree" << std::endl;
    std::cout << "-d : Delete the given points from the cover tree" << std::endl;
    std::cout << "-o : Specify the output file for results" << std::endl;
    std::cout << "-h : Show this help message" << std::endl;
}