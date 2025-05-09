#include "oprations_wrapper.h"

void nearNeighbor(CoverTree* cTree, std::vector<pointType>& testPointList) {
    std::cout << "Quering parallely" << std::endl;

    std::chrono::high_resolution_clock::time_point ts, tn;
    ts = std::chrono::high_resolution_clock::now();
    std::vector<pointType> ct_neighbors(testPointList.size());
    for (int i = 0; i < testPointList.size(); ++i) {
        pointType& queryPt = testPointList[i];
        std::pair<CoverTree::Node*, double> ct_nn = cTree->NearestNeighbour(queryPt);
        ct_neighbors[i] = ct_nn.first->_p;
    }
    tn = std::chrono::high_resolution_clock::now();
    std::cout << "Query time: " << std::chrono::duration_cast<std::chrono::milliseconds>(tn - ts).count() << std::endl;
}


void kNearNeighbors(CoverTree* cTree, int kSize, std::vector<pointType>& testPointList) {
    std::cout << "Quering parallely" << std::endl;

    std::chrono::high_resolution_clock::time_point ts, tn;
    ts = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<pointType>> ct_neighbors(testPointList.size());
    for (int i = 0; i < testPointList.size(); ++i) {
        pointType& queryPt = testPointList[i];
        std::vector<std::pair<CoverTree::Node*, double>> ct_nn = cTree->kNearestNeighbours(queryPt, kSize);
        ct_neighbors[i].resize(ct_nn.size());
        for (int j = 0; j < ct_nn.size(); j++) {
            ct_neighbors[i][j] = ct_nn[j].first->_p;
        }
    }
    tn = std::chrono::high_resolution_clock::now();

    
    std::cout << "Query time: " << std::chrono::duration_cast<std::chrono::milliseconds>(tn - ts).count() << std::endl;
    for (int i = 0; i < ct_neighbors.size(); i++) {
        resultPrint(i, ct_neighbors[i]);
    }
}

void resultPrint(int index, std::vector<pointType> &resultList) {
    std::cout << "Result for index " << index << ": " << std::endl;
    for (int i = 0; i < resultList.size(); i++) {
        for (int j = 0; j < resultList[i].size(); j++) {
            std::cout << resultList[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void insertPoints(CoverTree* cTree, std::vector<pointType>& insertPointList) {
    std::cout << "Inserting points" << std::endl;

    std::chrono::high_resolution_clock::time_point ts, tn;
    ts = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < insertPointList.size(); ++i) {
        pointType& queryPt = insertPointList[i];
        cTree->insert(queryPt);
    }
    tn = std::chrono::high_resolution_clock::now();

    std::cout << "Insert time: " << std::chrono::duration_cast<std::chrono::milliseconds>(tn - ts).count() << std::endl;
}


void deletePoints(CoverTree* cTree, std::vector<pointType>& deletePointList) {
    std::cout << "Deleting points" << std::endl;

    std::chrono::high_resolution_clock::time_point ts, tn;
    ts = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < deletePointList.size(); ++i) {
        pointType& queryPt = deletePointList[i];
        cTree->remove(queryPt);
    }
    tn = std::chrono::high_resolution_clock::now();

    std::cout << "Delete time: " << std::chrono::duration_cast<std::chrono::milliseconds>(tn - ts).count() << std::endl;
}