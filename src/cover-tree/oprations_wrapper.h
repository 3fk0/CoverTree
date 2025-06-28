#include "cover_tree.h"
#include "utils.h"


CoverTree* cover_tree_build(std::vector<pointType> pointList);

void nearNeighbor(CoverTree* cTree, std::vector<pointType>& testPointList);

void kNearNeighbors(CoverTree* cTree, int kSize, std::vector<pointType>& testPointList, int queryID);

void resultPrint(int index, std::vector<pointType>& resultList);

void insertPoints(CoverTree* cTree, std::vector<pointType>& insertPointList, int insertID);

void deletePoints(CoverTree* cTree, std::vector<pointType>& deletePointList, int deleteID);