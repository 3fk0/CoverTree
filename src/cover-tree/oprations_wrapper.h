#include "cover_tree.h"
#include "utils.h"
#include <sstream>

CoverTree *cover_tree_build(std::vector<pointType> pointList);

void kNearNeighbors(CoverTree *cTree, std::vector<pointType> &testPointList, std::vector<int> ks_to_query);

void insertPoints(CoverTree *cTree, std::vector<pointType> &insertPointList);

void deletePoints(CoverTree *cTree, std::vector<pointType> &deletePointList);