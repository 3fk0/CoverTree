#include <Eigen/Core>

#include "cover_tree.h"
#include "utils.h"

Eigen::MatrixXd readPointFile(std::string fileName);

std::vector<pointType> readPointFileList(std::string fileName);