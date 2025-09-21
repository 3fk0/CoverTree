#include "oprations_wrapper.h"

CoverTree *cover_tree_build(std::vector<pointType> pointList)
{
    std::chrono::high_resolution_clock::time_point ts, tn;
    CoverTree *cTree;

    ts = std::chrono::high_resolution_clock::now();

    cTree = CoverTree::from_points(pointList, -1, false);

    tn = std::chrono::high_resolution_clock::now();

    std::cout << "Insert time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count() << std::endl;
    std::cout << "Size: " << cTree->msg_size() << " after INSERT" << std::endl;
    return cTree;
}

void kNearNeighbors(CoverTree *cTree, std::vector<pointType> &testPointList)
{
    std::chrono::high_resolution_clock::time_point ts, tn;
    Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "", "");

    for (int k = 0; k <= 100; k += 5)
    {
        for (int i = 0; i < testPointList.size(); ++i)
        {
            std::cout << "Query " << i << "." << (k == 0 ? 1 : k) << std::endl;
            ts = std::chrono::high_resolution_clock::now();

            pointType &queryPt = testPointList[i];
            std::vector<std::pair<CoverTree::Node *, double>> ct_nn = cTree->kNearestNeighbours(queryPt, k == 0 ? 1 : k);

            tn = std::chrono::high_resolution_clock::now();

            std::cout << "Query time: "
                      << std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count()
                      << std::endl;

            for (int j = 0; j < ct_nn.size(); j++)
            {
                const pointType &point = ct_nn[j].first->_p;

                if (point.size() == 0)
                {
                    continue;
                }
                else
                {
                    std::cout << point.format(CommaInitFmt) << std::endl;
                }
            }
        }
    }
}

void insertPoints(CoverTree *cTree, std::vector<pointType> &insertPointList)
{
    std::chrono::high_resolution_clock::time_point ts, tn;
    ts = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < insertPointList.size(); ++i)
    {
        pointType &queryPt = insertPointList[i];
        cTree->insert(queryPt);
    }
    tn = std::chrono::high_resolution_clock::now();

    std::cout << "Insert time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count() << std::endl;
    std::cout << "Size: " << cTree->msg_size() << " after INSERT" << std::endl;
}

void deletePoints(CoverTree *cTree, std::vector<pointType> &deletePointList)
{
    std::chrono::high_resolution_clock::time_point ts, tn;
    ts = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < deletePointList.size(); ++i)
    {

        pointType &queryPt = deletePointList[i];
        cTree->remove(queryPt);
    }
    tn = std::chrono::high_resolution_clock::now();
    std::cout << "Delete time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count() << std::endl;
    std::cout << "Size: " << cTree->msg_size() << " after DELETE" << std::endl;
}