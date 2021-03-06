/* 
 * File:   Voxy.h
 * Author: daniele
 *
 * Created on July 23, 2015, 6:38 PM
 */

#ifndef VOXY_H
#define	VOXY_H
#include "commons.h"
#include <pcl/point_cloud.h>


namespace visy {

    struct VoxyLabel {
        static int LABEL_COUNTER;
        int id;

        VoxyLabel() {
            id = LABEL_COUNTER++;
        }
    };

    class Voxy {
    public:

        Voxy(double edge_size, double edge_meters, double distance_sigma, Eigen::Vector3f& offset);
        virtual ~Voxy();

        
        void addPointCloud(pcl::PointCloud<PointType>::Ptr cloud, Eigen::Vector3f& pov);
        void addPoint(Eigen::Vector3f& point, Eigen::Vector3f& pov);
        bool pointToIndex(Eigen::Vector3f& point, int& index, bool reverse = false);
        bool pointToIndex(PointType& point, int& index, bool reverse = false);
        double truncatedDistance(Eigen::Vector3f& p1, Eigen::Vector3f& p2);

        int addCluster(pcl::PointCloud<PointType>::Ptr cluster);
        int labelsCount();
        
        void voxelToCloudZeroCrossing(pcl::PointCloud<PointType>::Ptr& cloud_out,bool bicolor=false);
        void voxelToCloud(pcl::PointCloud<PointType>::Ptr& cloud_out,std::vector<int>& labels);

        double* voxel_data;
        bool* voxel_data_pin;
        int* voxel_data_counter;
        int* voxel_labels;
    private:
        int round_counter;
        double edge_size;
        double edge_square_size;
        double edge_full_size;
        double edge_meters;
        double voxel_size;
        double distance_sigma;
        Eigen::Vector3f offset;
    };
}
#endif	/* VOXY_H */

