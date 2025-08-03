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

//# define EIGEN_USE_MKL_ALL        //uncomment if available

#include <chrono>
#include <iostream>
#include <exception>

#include <future>
#include <thread>

#include <Eigen/Core>
#define EIGEN_DONT_PARALLELIZE

// User header
#include "input_control.h"
#include "oprations_wrapper.h"


// Compute the nearest neighbor using brute force, O(n)
pointType bruteForceNeighbor(std::vector<pointType>& pointList, pointType queryPt)
{
    Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "[", "]");

    pointType ridiculous = 1e200 * queryPt;
    pointType minPoint = ridiculous;
    double minDist = 1e300, curDist; // ridiculously high number


    for (const auto& p : pointList)
    {
        curDist = (queryPt-p).norm();
        // Re-assign minimum
        if (minDist > curDist)
        {
            minDist = curDist;
            minPoint = p;
        }
    }
    //std::cout << "Min Point: " << minPoint.format(CommaInitFmt)
    //        << " for query " << queryPt.format(CommaInitFmt) << std::endl;

    if (minPoint == ridiculous)
    {
        throw std::runtime_error("Something is wrong! Brute force neighbor failed!\n");
    }

    return minPoint;
}

void rangeBruteForce(std::vector<pointType>& pointList, pointType queryPt, double range, std::vector<pointType>& nnList)
{
    // Check for correctness
    for (const auto& node: nnList){
        if ( (node-queryPt).norm() > range){
            throw std::runtime_error( "Error in correctness - range!\n" );
        }
    }

    // Check for completeness
    int numPoints = 0;
    for (const auto& pt: pointList)
        if ((queryPt - pt).norm() < range)
            numPoints++;

    if (numPoints != nnList.size()){
    throw std::runtime_error( "Error in completeness - range!\n Brute force: " + std::to_string( numPoints ) + " Tree: " + std::to_string(  nnList.size() ) );
    }
}


void nearNeighborBruteForce(std::vector<pointType> pointList, pointType queryPt, int numNbrs, std::vector<pointType>& nnList)
{
    std::vector<std::pair<double, pointType>> distances;

    for (const auto& pt : pointList) {
        double dist = (pt - queryPt).norm();
        distances.emplace_back(dist, pt);
    }

    std::sort(distances.begin(), distances.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });

    nnList.clear();
    for (int i = 0; i < std::min(numNbrs, (int)distances.size()); ++i) {
        nnList.push_back(distances[i].second);
    }

    return;
}


int main(int argv, char** argc)
{
    Eigen::setNbThreads(1);
    Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "[", "]");
    
    input_operation* ops = parse_input(argv, argc);
    CoverTree* cTree;
    int queryID = 0;
    int deleteID = 0;
    int insertID = 0;

    for (int i = 0; i < argv - 1; i += 2) {
        std::vector<pointType> pointList = readPointFileList(ops[i].vector_file);
        
        switch (ops[i].type) {
            case BUILD:
                cTree = cover_tree_build(pointList);
                break;
            case QUERY:
                kNearNeighbors(cTree, pointList, queryID++);
                break;
            case INSERT:
                insertPoints(cTree, pointList, insertID++);
                break;
            case DELETE:
                deletePoints(cTree, pointList, deleteID++);
                break;
            default:
                throw std::runtime_error("Unknown operation");
        }
    }

    return 0;
}

