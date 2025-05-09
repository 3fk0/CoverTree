#include "cover_tree.h"
#include "utils.h"

void nearNeighbor(CoverTree* cTree, std::vector<pointType>& testPointList);

void kNearNeighbors(CoverTree* cTree, int kSize, std::vector<pointType>& testPointList);

void resultPrint(int index, std::vector<pointType>& resultList);

void insertPoints(CoverTree* cTree, std::vector<pointType>& insertPointList);

void deletePoints(CoverTree* cTree, std::vector<pointType>& deletePointList);