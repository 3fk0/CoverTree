#include "oprations_wrapper.h"
#include <sys/resource.h>

// Helper function to get current memory usage
long getCurrentMemoryUsage()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // in KB on Linux
}

CoverTree *cover_tree_build(std::vector<pointType> pointList, std::ofstream &output_file)
{
    std::chrono::high_resolution_clock::time_point ts, tn;
    CoverTree *cTree;

    ts = std::chrono::high_resolution_clock::now();

    cTree = CoverTree::from_points(pointList, -1, false);

    tn = std::chrono::high_resolution_clock::now();

    output_file << "Insert time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count() << std::endl;
    output_file << "Size: " << cTree->msg_size() << " after INSERT" << std::endl;
    return cTree;
}

void kNearNeighbors(CoverTree *cTree, std::vector<pointType> &testPointList, std::vector<int> ks_to_query, std::ofstream &output_file)
{
    std::chrono::high_resolution_clock::time_point ts, tn;
    Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "", "");

    // Pre-allocate vector to avoid repeated allocations
    std::vector<std::pair<CoverTree::Node *, double>> ct_nn;
    ct_nn.reserve(100); // Reserve space for maximum k value

    for (int k : ks_to_query)
    {
        std::stringstream ss;
        for (int i = 0; i < testPointList.size(); ++i)
        {
            ss << "Query " << i << "." << k << std::endl;
            ts = std::chrono::high_resolution_clock::now();

            pointType &queryPt = testPointList[i];

            // Clear previous results and get new ones
            ct_nn.clear();
            ct_nn = cTree->kNearestNeighbours(queryPt, k);

            tn = std::chrono::high_resolution_clock::now();

            ss << "Query time: "
               << std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count()
               << std::endl;

            for (size_t j = 0; j < ct_nn.size(); j++)
            {
                // Add null pointer check to prevent crashes
                if (ct_nn[j].first == nullptr)
                {
                    continue;
                }

                const pointType &point = ct_nn[j].first->_p;

                if (point.size() == 0)
                {
                    continue;
                }
                else
                {
                    ss << point.format(CommaInitFmt) << std::endl;
                }
            }
        }
        output_file << ss.str();
    }
}

void insertPoints(CoverTree *cTree, std::vector<pointType> &insertPointList, std::ofstream &output_file)
{
    std::chrono::high_resolution_clock::time_point ts, tn;
    ts = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < insertPointList.size(); ++i)
    {
        pointType &queryPt = insertPointList[i];
        cTree->insert(queryPt);
    }
    tn = std::chrono::high_resolution_clock::now();

    output_file << "Insert time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count() << std::endl;
    output_file << "Size: " << cTree->msg_size() << " after INSERT" << std::endl;
}

void deletePoints(CoverTree *cTree, std::vector<pointType> &deletePointList, std::ofstream &output_file)
{
    std::chrono::high_resolution_clock::time_point ts, tn;
    ts = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < deletePointList.size(); ++i)
    {

        pointType &queryPt = deletePointList[i];
        cTree->remove(queryPt);
    }
    tn = std::chrono::high_resolution_clock::now();
    output_file << "Delete time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count() << std::endl;
    output_file << "Size: " << cTree->msg_size() << " after DELETE" << std::endl;
}