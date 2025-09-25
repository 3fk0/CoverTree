#include "cover_tree.h"
#include "utils.h"
#include <sstream>


CoverTree* cover_tree_build(std::vector<pointType> pointList);

void kNearNeighbors(CoverTree* cTree, std::vector<pointType>& testPointList);

void insertPoints(CoverTree* cTree, std::vector<pointType>& insertPointList);

void deletePoints(CoverTree* cTree, std::vector<pointType>& deletePointList);