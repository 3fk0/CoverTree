#include "loader.h"

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

    // Matrix of points
    Eigen::MatrixXd pointMatrix(numDims, numPoints);

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
    return pointList;
}