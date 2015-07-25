#include <pcl/ModelCoefficients.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/features/normal_3d.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/features/integral_image_normal.h>
#include <pcl/registration/transformation_estimation_svd.h>
#include <pcl/registration/transformation_estimation_lm.h>
#include <pcl/filters/fast_bilateral.h>
#include <pcl/filters/bilateral.h>
#include <pcl/io/ply_io.h>
#include <pcl/surface/poisson.h>
#include <pcl/io/vtk_io.h>

#include <pcl/kdtree/kdtree_flann.h>
//#include <pcl/surface/mls.h>
#include <pcl/surface/impl/mls.hpp>

#include "Parameters.h"
#include "WillowDataset.h"
#include "Voxy.h"

/** SCENE */
std::vector<visy::extractors::KeyPoint3D> scene_keypoints;
std::vector<visy::extractors::KeyPoint3D> scene_keypoints_seletected;
std::vector<visy::extractors::KeyPoint3D> scene_keypoints_parallels_all;
std::vector<visy::extractors::KeyPoint3D> scene_keypoints_parallels;
int scene_keypoints_parallels_best = -1;
std::vector<cv::Point2f> scene_keypoints_seletected_parallels;
int scene_keypoint_selected_index = -1;
cv::Mat scene_descriptor;
cv::Mat scene_rgb, scene_rgb_full;

pcl::PointCloud<PointType>::Ptr scene_cloud_filtered(new pcl::PointCloud<PointType>());

visy::Parameters * parameters;

/*
void gravityVector(pcl::PointCloud<PointType>::Ptr cloud, cv::Point3f& gravity_vector) {
    // Create the filtering object: downsample the dataset using a leaf size of 1cm
    pcl::VoxelGrid<PointType> vg;
    pcl::PointCloud<PointType>::Ptr cloud_filtered(new pcl::PointCloud<PointType>);
    pcl::PointCloud<PointType>::Ptr cloud_filtered_orig(new pcl::PointCloud<PointType>);
    pcl::PointCloud<PointType>::Ptr cloud_f(new pcl::PointCloud<PointType>);


    vg.setInputCloud(cloud);
    vg.setLeafSize(0.01f, 0.01f, 0.01f);
    vg.filter(*cloud_filtered);
    std::cout << "PointCloud after filtering has: " << cloud_filtered->points.size() << " data points." << std::endl; //*
    //    cloud_filtered = cloud;
    pcl::copyPointCloud(*cloud_filtered, *cloud_filtered_orig);

    // Create the segmentation object for the planar model and set all the parameters
    pcl::SACSegmentation<PointType> seg;
    pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
    pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
    pcl::PointCloud<PointType>::Ptr cloud_plane(new pcl::PointCloud<PointType> ());
    std::vector<pcl::PointCloud<PointType>::Ptr> cloud_planes;

    seg.setOptimizeCoefficients(true);
    seg.setModelType(pcl::SACMODEL_PLANE);
    seg.setMethodType(pcl::SAC_RANSAC);
    seg.setMaxIterations(100);
    seg.setDistanceThreshold(0.02);

    std::vector<std::vector<int> > planes_inliers;
    std::vector<std::vector<float> > planes_coeff;
    std::vector<cv::Point3f > planes_normals;

    int i = 0, nr_points = (int) cloud_filtered->points.size();
    while (cloud_filtered->points.size() > 0.3 * nr_points) {
        // Segment the largest planar component from the remaining cloud
        seg.setInputCloud(cloud_filtered);
        seg.segment(*inliers, *coefficients);
        if (inliers->indices.size() == 0) {
            std::cout << "Could not estimate a planar model for the given dataset." << std::endl;
            break;
        }

        pcl::PointCloud<PointType>::Ptr cloud_plane_iter(new pcl::PointCloud<PointType> ());
        pcl::copyPointCloud(*cloud_filtered, inliers->indices, *cloud_plane_iter);
        cloud_planes.push_back(cloud_plane_iter);


        // Extract the planar inliers from the input cloud
        pcl::ExtractIndices<PointType> extract;
        extract.setInputCloud(cloud_filtered);
        extract.setIndices(inliers);
        extract.setNegative(false);

        // Get the points associated with the planar surface
        extract.filter(*cloud_plane);


        planes_coeff.push_back(coefficients->values);
        planes_inliers.push_back(inliers->indices);

        cv::Point3f n(
                coefficients->values[0],
                coefficients->values[1],
                coefficients->values[2]
                );
        planes_normals.push_back(n);

        // Remove the planar inliers, extract the rest
        extract.setNegative(true);
        extract.filter(*cloud_f);
 *cloud_filtered = *cloud_f;
    }


    for (int i = 0; i < planes_coeff.size(); i++) {
        pcl::PointCloud<PointType>::Ptr cloud_plane(new pcl::PointCloud<PointType> ());
        pcl::copyPointCloud(*cloud_filtered, planes_inliers[i], *cloud_plane);
        std::stringstream ss;
    }

    float max_d = 10.0f * (M_PI / 180.0f);

    std::vector<int> similar_planes(planes_coeff.size());
    std::fill(similar_planes.begin(), similar_planes.end(), -1);
    for (int i = 0; i < planes_coeff.size(); i++) {
        cv::Point3f p_source = planes_normals[i];
        p_source = p_source * (1.0f / cv::norm(p_source));
        for (int j = 0; j < planes_coeff.size(); j++) {
            if (i != j) {
                cv::Point3f p_target = planes_normals[j];
                p_target = p_target * (1.0f / cv::norm(p_target));
                float pp = acos(p_source.dot(p_target));

                if (pp < max_d) {
                    similar_planes[i] = j;
                }

            }
        }
    }

    std::vector<int> planes_full_count;
    std::vector<cv::Point3f> planes_full_normals;
    std::vector<bool> graph_visit(similar_planes.size());
    std::fill(graph_visit.begin(), graph_visit.end(), false);
    bool full = false;
    while (!full) {
        int start_index = -1;
        int ref_index = -1;
        for (int i = 0; i < similar_planes.size(); i++) {
            full = true;
            if (!graph_visit[i]) {
                start_index = i;
                ref_index = i;
                graph_visit[i] = true;
                full = false;
                break;
            }
        }
        if (start_index == -1)continue;
        int counter = planes_inliers[start_index].size();
        bool visited = false;
        while (!visited) {
            start_index = similar_planes[start_index];
            if (start_index != -1 && !graph_visit[start_index]) {
                graph_visit[start_index] = true;
                counter += planes_inliers[start_index].size();
            } else {
                visited = true;
            }
        }
        planes_full_count.push_back(counter);
        planes_full_normals.push_back(cv::Point3f(
                planes_coeff[ref_index][0],
                planes_coeff[ref_index][1],
                planes_coeff[ref_index][2]
                ));

    }

    int max_plane = -1;
    int max_plane_count = 0;
    for (int i = 0; i < planes_full_count.size(); i++) {
        if (planes_full_count[i] > max_plane_count) {
            max_plane_count = planes_full_count[i];
            max_plane = i;
        }
    }

    gravity_vector.x = planes_coeff[max_plane][0];
    gravity_vector.y = planes_coeff[max_plane][1];
    gravity_vector.z = planes_coeff[max_plane][2];
}
 */


void loadCloud(int index, pcl::PointCloud<PointType>::Ptr& cloud, Eigen::Matrix4f& t) {

    std::stringstream ss;
    ss << "/home/daniele/Desktop/TSDF_Test/1437572674.743236835/";
    ss << index << ".pcd";
    if (pcl::io::loadPCDFile<PointType> (ss.str().c_str(), *cloud) == -1) //* load the file
    {
        PCL_ERROR("Couldn't read file test_pcd.pcd \n");

    }

    //LOAD MATRIX
    ss.str("");
    ss << "/home/daniele/Desktop/TSDF_Test/1437572674.743236835/";
    ss << index << ".txt";
    ifstream myReadFile;
    std::cout << "Opening: " << ss.str().c_str() << std::endl;
    myReadFile.open(ss.str().c_str());
    char output[100];
    if (myReadFile.is_open()) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                myReadFile >> output;
                t(i, j) = atof(output);
            }
        }
    } else {
        std::cout << "BOH" << std::endl;
    }
    myReadFile.close();

}

int
main(int argc, char** argv) {
    /** PARAMETERS */
    parameters = new visy::Parameters(argc, argv);
    parameters->putFloat("gc_th");
    parameters->putFloat("gc_size");
    parameters->putFloat("down");
    parameters->putString("dataset");
    parameters->putString("detector");
    parameters->putString("model");
    parameters->putString("sizes");
    parameters->putInt("set");
    parameters->putInt("scene");
    parameters->putInt("nbin");
    parameters->putInt("occlusion");
    parameters->putBool("x");
    parameters->putBool("f");
    parameters->putFloat("f_th");
    parameters->putFloat("quantum");
    parameters->putFloat("edge_th");




    pcl::visualization::PCLVisualizer* viewer = new pcl::visualization::PCLVisualizer("Bunch Tester Viewer");


    int size = 256;
    int full_size = size * size*size;
    double step = 2.0f / (double) size;

    Eigen::Vector3f offset(0.0, 1.0, 1.0);

    visy::Voxy voxy(size, 2.0, 0.01f, offset);

    Eigen::Matrix4f adjust;
    adjust <<
            1, 0, 0, -0.01,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    for (int i = 0; i <= 70; i += 10) {
        pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>());
        pcl::PointCloud<PointType>::Ptr cloud_trans(new pcl::PointCloud<PointType>());
        Eigen::Matrix4f t;
        int index = i;

        loadCloud(index, cloud, t);

        t = t*adjust;

        pcl::transformPointCloud(*cloud, *cloud_trans, t);
        Eigen::Vector3f pov(t(0, 3), t(1, 3), t(2, 3));
        voxy.addPointCloud(cloud_trans, pov);
        //        
        //        std::stringstream ss;
        //        ss << "Cloud_"<<i;
        //        viewer->addPointCloud(cloud_trans, ss.str().c_str());
    }

    pcl::PointCloud<PointType>::Ptr cloud_vox(new pcl::PointCloud<PointType>());

    voxy.voxelToCloudZeroCrossing(cloud_vox);

   
    typedef  pcl::PointNormal XYZNormalType;
    
    // Create a KD-Tree
    pcl::search::KdTree<PointType>::Ptr tree(new pcl::search::KdTree<PointType>);

    // Output has the PointNormal type in order to store the normals calculated by MLS
    pcl::PointCloud<XYZNormalType>::Ptr mls_points(new pcl::PointCloud<XYZNormalType>());;

    // Init object (second point type is for the normals, even if unused)
    pcl::MovingLeastSquares<PointType, XYZNormalType> mls;

    mls.setComputeNormals(true);

    // Set parameters
    mls.setInputCloud(cloud_vox);
    mls.setPolynomialFit(true);
    mls.setSearchMethod(tree);
    mls.setSearchRadius(0.05);
    mls.setPolynomialOrder(2);
//    mls.setUpsamplingMethod(pcl::MovingLeastSquares<PointType,XYZNormalType>::SAMPLE_LOCAL_PLANE);
//    mls.setUpsamplingRadius(0.005);
//    mls.setUpsamplingStepSize(0.003);
    // Reconstruct
    mls.process(*mls_points);

    // Save output
    pcl::io::savePCDFile("bun0-mls.pcd", *mls_points);


    viewer->addPointCloud(cloud_vox, "cloud_vox");


    //    viewer->addPointCloud(cloud_trans, "cloud");
    //    Eigen::Affine3f A;
    //    A = t;
    //    viewer->addCoordinateSystem(1.0, A);
    while (!viewer->wasStopped()) {
        viewer->spinOnce();
        cv::waitKey(10);
    }
    
    
//    pcl::Poisson<XYZNormalType> poisson;
//    poisson.setDepth (9);
//    poisson.setInputCloud (mls_points);
//    pcl::PolygonMesh mesh;
//    poisson.reconstruct (mesh);
//    pcl::io::saveVTKFile ("/home/daniele/Desktop/sreconstruc.vtk",mesh);
    
    pcl::io::savePLYFileASCII("/home/daniele/Desktop/mls.ply", *mls_points);
    pcl::io::savePLYFileASCII("/home/daniele/Desktop/output.ply", *cloud_vox);

    return (0);
}