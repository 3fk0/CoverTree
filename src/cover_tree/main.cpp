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

# include <chrono>
# include <iostream>
# include <exception>

#include <future>
# include <thread>

# include <Eigen/Core>
#define EIGEN_DONT_PARALLELIZE

// User header
# include "cover_tree.h"
# include "utils.h"


Eigen::MatrixXd readPointFile(std::string fileName)
{
    std::ifstream fin(fileName, std::ios::in|std::ios::binary);

    // Check for existance for file
    if (!fin)
        throw std::runtime_error("File not found : " + fileName);

    // Read the header for number of points, dimensions
    unsigned numPoints = 0;
    unsigned numDims = 0;
    fin.read((char *)&numPoints, sizeof(int));
    fin.read((char *)&numDims, sizeof(int));

    // Printing for debugging
    std::cout << "\nNumber of points: " << numPoints << "\nNumber of dims : " << numDims << std::endl;

    // Matrix of points
    Eigen::MatrixXd pointMatrix(numDims, numPoints);
    std::cout<<"IsRowMajor?: "<<pointMatrix.IsRowMajor << std::endl;

    // Read the points, one by one
    double *tmp_point = new double[numDims];
    for (unsigned ptIter = 0; ptIter < numPoints; ptIter++)
    {
        // new point to be read
        fin.read((char *)tmp_point, sizeof(double)*numDims);
        for (unsigned dim = 0; dim < numDims; dim++)
        {
            pointMatrix(dim, ptIter) = tmp_point[dim];
        }
    }
    // Close the file
    fin.close();

    std::cout<<pointMatrix.rows() << " " << pointMatrix.cols() << std::endl;
    std::cout<<pointMatrix(0,0) << " " << pointMatrix(0,1) << " " << pointMatrix(1,0) << std::endl;

    return pointMatrix;
}

std::vector<pointType> readPointFileList(std::string fileName)
{
    std::ifstream fin(fileName, std::ios::in|std::ios::binary);

    // Check for existance for file
    if (!fin)
        throw std::runtime_error("File not found : " + fileName);

    // Read the header for number of points, dimensions
    unsigned numPoints = 0;
    unsigned numDims = 0;
    fin.read((char *)&numPoints, sizeof(int));
    fin.read((char *)&numDims, sizeof(int));

    // Printing for debugging
    std::cout << "\nNumber of points: " << numPoints << "\nNumber of dims : " << numDims << std::endl;

    // List of points
    std::vector<pointType> pointList;
    pointList.reserve(numPoints);

    // Read the points, one by one
    double *tmp_point = new double[numDims];
    for (unsigned ptIter = 0; ptIter < numPoints; ptIter++)
    {
        // new point to be read
        fin.read((char *)tmp_point, sizeof(double)*numDims);
        pointType newPt = pointType(numDims);

        for (unsigned dim = 0; dim < numDims; dim++)
        {
            newPt[dim] = tmp_point[dim];
        }

        // Add the point to the list
        pointList.push_back(newPt);
    }
    // Close the file
    fin.close();

    std::cout<<pointList[0][0] << " " << pointList[0][1] << " " << pointList[1][0] << std::endl;

    return pointList;
}

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
    if (argv < 2)
        throw std::runtime_error("Usage:\n./main <path_to_train_points> <path_to_test_points>");

    std::cout << argc[1] << std::endl;
    std::cout << argc[2] << std::endl;

    Eigen::setNbThreads(1);
    std::cout << "Number of OpenMP threads: " << Eigen::nbThreads();

    Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "[", "]");
    std::chrono::high_resolution_clock::time_point ts, tn;

    // Reading the file for points
    //Eigen::MatrixXd pointMatrix = readPointFile(argc[1]);
    std::vector<pointType> pointList = readPointFileList(argc[1]);

    // Print the points
    for (int i = 0; i < pointList.size(); i++)
    {
        std::cout << pointList[i].format(CommaInitFmt) << std::endl;
    }

    CoverTree* cTree;
    // Parallel Cover tree construction
    ts = std::chrono::high_resolution_clock::now();
    cTree = CoverTree::from_points(pointList, -1, false);
    //cTree = CoverTree::from_matrix(pointMatrix, -1, false);
    tn = std::chrono::high_resolution_clock::now();
    std::cout << "Build time: " << std::chrono::duration_cast<std::chrono::milliseconds>(tn - ts).count() << std::endl;
    std::cout << "Number of points in the tree: " << cTree->count_points() << std::endl;

    // find the nearest neighbor
    std::vector<pointType> testPointList = readPointFileList(argc[2]);

    //Serial search
    //double tsum = 0;
    // std::cout << "Querying serially" << std::endl;
    // ts = std::chrono::high_resolution_clock::now();
    // //for (const auto& queryPt : testPointList)
    // int run_till = testPointList.size();
    // for (int i = 1; i < run_till; ++i)
    // {
        // utils::progressbar(i, run_till);
        // pointType& queryPt = testPointList[i];
        // std::pair<CoverTree::Node*, double> ct_nn = cTree->NearestNeighbour(queryPt);
        // tsum += ct_nn.first->_p[0];
        // pointType bf_nn = bruteForceNeighbor(pointList, queryPt);
        // if (!ct_nn.first->_p.isApprox(bf_nn))
        // {
            // std::cout << "Something is wrong" << std::endl;
            // std::cout << ct_nn.first->_p.format(CommaInitFmt) << " " << bf_nn.format(CommaInitFmt) << " " << queryPt.format(CommaInitFmt) << std::endl;
            // std::cout << (ct_nn.first->_p - queryPt).norm() << " ";
            // std::cout << (bf_nn - queryPt).norm() << std::endl;
        // }
    // }
    // tn = std::chrono::high_resolution_clock::now();
    // std::cout << "Query time: " << std::chrono::duration_cast<std::chrono::milliseconds>(tn - ts).count() << std::endl;
    // std::cout << tsum << std::endl;

    // Parallel search (async)
    std::cout << "Quering parallely" << std::endl;
    ts = std::chrono::high_resolution_clock::now();
    std::vector<pointType> ct_neighbors(testPointList.size());
    utils::parallel_for_progressbar(0, testPointList.size(), [&](int i)->void{
        pointType& queryPt = testPointList[i];
        std::pair<CoverTree::Node*, double> ct_nn = cTree->NearestNeighbour(queryPt);
        ct_neighbors[i] = ct_nn.first->_p;
    });

    tn = std::chrono::high_resolution_clock::now();
    std::cout << "Query time: " << std::chrono::duration_cast<std::chrono::milliseconds>(tn - ts).count() << std::endl;

    std::cout << "Quering parallely" << std::endl;
    ts = std::chrono::high_resolution_clock::now();
    std::vector<pointType> bt_neighbors(testPointList.size());
    utils::parallel_for_progressbar(0, testPointList.size(), [&](int i)->void{
        pointType& queryPt = testPointList[i];
        bt_neighbors[i] = bruteForceNeighbor(pointList, queryPt);
    });
    tn = std::chrono::high_resolution_clock::now();
    std::cout << "Query time: " << std::chrono::duration_cast<std::chrono::milliseconds>(tn - ts).count() << std::endl;

    //Match answers
    int problems = 0;
    std::vector<pointType> test1(1);
    std::vector<pointType> test2(1);
    for(size_t i=0; i<testPointList.size(); ++i)
    {
        if (!ct_neighbors[i].isApprox(bt_neighbors[i]))
        {
            problems += 1;
            std::cout << "Something is wrong" << std::endl;
            std::cout << ct_neighbors[i].format(CommaInitFmt) << " " << bt_neighbors[i].format(CommaInitFmt) << " " << testPointList[i].format(CommaInitFmt) << std::endl;
            std::cout << (ct_neighbors[i] - testPointList[i]).norm() << " ";
            std::cout << (bt_neighbors[i] - testPointList[i]).norm() << std::endl;
        }
        test1[0] = ct_neighbors[i];
        test2[0] = bt_neighbors[i];
    }
    
    std::cout << "Two: " << std::endl;
    for (int i = 0; i < test1[0].size(); i++) {
        std::cout << test1[0][i] << " " << test2[0][i] << std::endl;
    }

    if (problems)
        std::cout << "Nearest Neighbour test failed!" << std::endl;
    else
        std::cout << "Nearest Neighbour test passed!" << std::endl;

    //std::cout << "k-NN serially" << std::endl;
    //for (const auto& queryPt : testPointList)
    //{
    //  std::vector<point> nnList = cTree->nearNeighbors(queryPt, 2);
        //  nearNeighborBruteForce(pointList, queryPt, 2, nnList);
    //}

    //std::cout << "range serially" << std::endl;
    //std::vector<point> nnList = cTree->rangeNeighbors(testPointList[0], 10);
        //rangeBruteForce(pointList, testPointList[0], 10, nnList);

    //tn = std::chrono::high_resolution_clock::now();
    //std::cout << "Query time: " << std::chrono::duration_cast<std::chrono::milliseconds>(tn - ts).count() << std::endl;

    std::cout << "Finnished!" << std::endl;


    std::cout << "Quering parallely" << std::endl;
    std::vector<pointType> kn_neighbors(25);
    utils::parallel_for_progressbar(0, 1,
        [&](int i) -> void {
            pointType& queryPt = testPointList[i];
            std::vector<std::pair<CoverTree::Node*, double>> ct_nn = cTree->kNearestNeighbours(queryPt, 25);
            for (int i = 0; i < ct_nn.size(); i++)
            {
                kn_neighbors[i] = ct_nn[i].first->_p;
            }
        }
    );

    pointType& queryPt = testPointList[0];
    std::cout << queryPt.format(CommaInitFmt) << std::endl;
    std::vector<pointType> kn_brute_neighbors;
    utils::parallel_for_progressbar(0, 1,
        [&](int i) -> void {
            nearNeighborBruteForce(pointList, queryPt, 25, kn_brute_neighbors);
        }
    );

    std::cout << kn_brute_neighbors.size() << std::endl;

    for (int i = 0; i < kn_neighbors.size(); i++)
    {
        std::cout << kn_neighbors[i].isApprox(kn_brute_neighbors[i]) << std::endl;
    }

    utils::pause();

    queryPt = testPointList[0];
    std::cout << queryPt.format(CommaInitFmt) << std::endl;
    std::pair<CoverTree::Node*, double> ct_nn = cTree->NearestNeighbour(queryPt);
    std::cout << "Before Delete: " << ct_nn.first->_p.format(CommaInitFmt) << std::endl;
    cTree->remove(queryPt);
    ct_nn = cTree->NearestNeighbour(queryPt);
    std::cout << "After Delete: " << ct_nn.first->_p.format(CommaInitFmt) << std::endl;

    cTree->insert(queryPt);
    ct_nn = cTree->NearestNeighbour(queryPt);
    std::cout << "After Insert: " << ct_nn.first->_p.format(CommaInitFmt) << std::endl;

    // Success
    return 0;
}

