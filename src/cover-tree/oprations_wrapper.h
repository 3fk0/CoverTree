#include "cover_tree.h"
#include "utils.h"


CoverTree* cover_tree_build(std::vector<pointType> pointList);

void kNearNeighbors(CoverTree* cTree, std::vector<pointType>& testPointList, int queryID);

void insertPoints(CoverTree* cTree, std::vector<pointType>& insertPointList, int insertID);

void deletePoints(CoverTree* cTree, std::vector<pointType>& deletePointList, int deleteID);