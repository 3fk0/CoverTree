/*
 * Copyright (c) 2017 Manzil Zaheer All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// # define EIGEN_USE_MKL_ALL        //uncomment if available

#include <chrono>
#include <iostream>
#include <exception>
#include <string.h>

#include <future>
#include <thread>

#include <Eigen/Core>
#define EIGEN_DONT_PARALLELIZE

// User header
#include "input_control.h"
#include "oprations_wrapper.h"

int main(int argv, char **argc)
{
    Eigen::setNbThreads(1);
    Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "[", "]");
    
    input_control ctrl = parse_input(argv, argc);
    std::ofstream output_file = std::ofstream(ctrl.output_file);
    CoverTree* cTree;
    int iteration = -1;

    for (input_operation operation : ctrl.operations) {
        std::string filePath = operation.vector_file;
        size_t lastSlash = filePath.find_last_of('/');
        std::string fileName = (lastSlash == std::string::npos) ? filePath : filePath.substr(lastSlash + 1);
        size_t dashPos = fileName.find('-');
        int fileNumber = std::stoi(fileName.substr(0, dashPos));

        if (iteration < fileNumber)
        {
            iteration = fileNumber;
            output_file << "Iteration: " << iteration << std::endl;
        }

        std::vector<pointType> pointList = readPointFileList(operation.vector_file);

        switch (operation.type) {
            case BUILD:
                cTree = cover_tree_build(pointList, output_file);
                break;
            case QUERY:
                kNearNeighbors(cTree, pointList, ctrl.ks_to_query, output_file);
                break;
            case INSERT:
                insertPoints(cTree, pointList, output_file);
                break;
            case DELETE:
                deletePoints(cTree, pointList, output_file);
                break;
            default:
                throw std::runtime_error("Unknown operation");
        }
    }

    return 0;
}
