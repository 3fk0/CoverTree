#include "oprations_wrapper.h"

CoverTree* cover_tree_build(std::vector<pointType> pointList) {
    std::chrono::high_resolution_clock::time_point ts, tn;
    CoverTree* cTree;

    ts = std::chrono::high_resolution_clock::now();
    
    cTree = CoverTree::from_points(pointList, -1, false);

    tn = std::chrono::high_resolution_clock::now();

    std::cout << "Build time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count() << std::endl;
    std::cout << "Index size: " << cTree->msg_size() << std::endl;
    return cTree;
}


void kNearNeighbors(CoverTree* cTree, int kSize, std::vector<pointType>& testPointList, int queryID) {
    std::chrono::high_resolution_clock::time_point ts, tn;
    std::vector<std::vector<pointType>> ct_neighbors(testPointList.size());
    Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "", "");

    for (int i = 0; i < testPointList.size(); ++i) {
        std::cout << "Query " << queryID << "." << i << std::endl;
        ts = std::chrono::high_resolution_clock::now();

        pointType& queryPt = testPointList[i];
        std::vector<std::pair<CoverTree::Node*, double>> ct_nn = cTree->kNearestNeighbours(queryPt, kSize);

        tn = std::chrono::high_resolution_clock::now();

        std::vector<pointType> nnList(ct_nn.size());
        for (int j = 0; j < ct_nn.size(); j++) {
            std::cout << ct_nn[j].first->_p.format(CommaInitFmt) << std::endl;
        }

        std::cout << "Query time: "
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count()
                  << std::endl;
    }
}


void insertPoints(CoverTree* cTree, std::vector<pointType>& insertPointList, int insertID) {
    std::chrono::high_resolution_clock::time_point ts, tn;
    std::cout << "Insert " << insertID << std::endl;
    ts = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < insertPointList.size(); ++i) {
        pointType& queryPt = insertPointList[i];
        cTree->insert(queryPt);
    }
    tn = std::chrono::high_resolution_clock::now();

    std::cout << "Insert time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count() << std::endl;
}


void deletePoints(CoverTree* cTree, std::vector<pointType>& deletePointList, int deleteID) {
    std::chrono::high_resolution_clock::time_point ts, tn;
    std::cout << "Delete " << deleteID << std::endl;
    ts = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < deletePointList.size(); ++i) {

        pointType& queryPt = deletePointList[i];
        cTree->remove(queryPt);
    }
    tn = std::chrono::high_resolution_clock::now();
    std::cout << "Delete time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count() << std::endl;
    std::cout << "Index size: " << cTree->msg_size() << std::endl;

}