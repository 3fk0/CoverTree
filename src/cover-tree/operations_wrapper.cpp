#include "oprations_wrapper.h"
#include <sys/resource.h>

// Helper function to get current memory usage
long getCurrentMemoryUsage()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // in KB on Linux
}

CoverTree *cover_tree_build(std::vector<pointType> pointList, FILE *output_file)
{
    std::chrono::high_resolution_clock::time_point ts, tn;
    CoverTree *cTree;

    ts = std::chrono::high_resolution_clock::now();

    cTree = CoverTree::from_points(pointList, -1, false);

    tn = std::chrono::high_resolution_clock::now();

    fprintf(output_file, "Insert time: %ld\n", std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count());
    fprintf(output_file, "Size: %d after INSERT\n", cTree->msg_size());
    return cTree;
}

void kNearNeighbors(CoverTree *cTree, std::vector<pointType> &testPointList, std::vector<int> ks_to_query, FILE *output_file)
{
    std::chrono::high_resolution_clock::time_point ts, tn;
    Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "", "");

    // Pre-allocate vector to avoid repeated allocations
    std::vector<std::pair<CoverTree::Node *, double>> ct_nn;
    ct_nn.reserve(100); // Reserve space for maximum k value

    for (int k : ks_to_query)
    {
        for (int i = 0; i < testPointList.size(); ++i)
        {
            fprintf(output_file, "Query %d.%d\n", i, k);
            ts = std::chrono::high_resolution_clock::now();

            pointType &queryPt = testPointList[i];

            // Clear previous results and get new ones
            ct_nn.clear();
            ct_nn = cTree->kNearestNeighbours(queryPt, k);

            tn = std::chrono::high_resolution_clock::now();

            fprintf(output_file, "Query time: %ld\n", std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count());

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
                    std::stringstream ss;
                    ss << point.format(CommaInitFmt);
                    fprintf(output_file, "%s\n", ss.str().c_str());
                }
            }
        }
    }
}

void insertPoints(CoverTree *cTree, std::vector<pointType> &insertPointList, FILE *output_file)
{
    std::chrono::high_resolution_clock::time_point ts, tn;
    ts = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < insertPointList.size(); ++i)
    {
        pointType &queryPt = insertPointList[i];
        cTree->insert(queryPt);
    }
    tn = std::chrono::high_resolution_clock::now();

    fprintf(output_file, "Insert time: %ld\n", std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count());
    fprintf(output_file, "Size: %d after INSERT\n", cTree->msg_size());
}

void deletePoints(CoverTree *cTree, std::vector<pointType> &deletePointList, FILE *output_file)
{
    std::chrono::high_resolution_clock::time_point ts, tn;
    ts = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < deletePointList.size(); ++i)
    {

        pointType &queryPt = deletePointList[i];
        cTree->remove(queryPt);
    }
    tn = std::chrono::high_resolution_clock::now();
    fprintf(output_file, "Delete time: %ld\n", std::chrono::duration_cast<std::chrono::nanoseconds>(tn - ts).count());
    fprintf(output_file, "Size: %d after DELETE\n", cTree->msg_size());
}