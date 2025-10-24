#include "cover_tree.h"
#include "utils.h"
#include <sstream>

CoverTree *cover_tree_build(std::vector<pointType> pointList, FILE *output_file);

void kNearNeighbors(CoverTree *cTree, std::vector<pointType> &testPointList, std::vector<int> ks_to_query, FILE *output_file);

void insertPoints(CoverTree *cTree, std::vector<pointType> &insertPointList, FILE *output_file);

void deletePoints(CoverTree *cTree, std::vector<pointType> &deletePointList, FILE *output_file);
