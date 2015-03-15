/*
 * File:   bunch_tester.cpp
 * Author: daniele
 *
 * Created on 27 novembre 2014, 22.17
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <boldlib.h>

#include <pcl/io/pcd_io.h>
#include <pcl/common/common_headers.h>
#include <pcl/common/centroid.h>
#include <pcl/common/intersections.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/impl/point_types.hpp>
#include <pcl/common/centroid.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/io/ply_io.h>
#include <pcl/console/parse.h>
#include <pcl/recognition/cg/geometric_consistency.h>
#include <pcl/common/transforms.h>
#include <pcl/features/integral_image_normal.h>


#include <sstream>
#include <string>
#include <fstream>

#include <tools.h>
#include <opencv2/core/mat.hpp>

#include "Extractor.h"
#include "Bold3DExtractor.h"
#include "IrosDataset.h"
#include "extractors/extrators_utils.h"
#include "Descriptor.h"
#include "Bold3DDescriptorMultiBunch.h"
#include "Detector.h"
#include "Bold3DMDetector.h"
#include "Bold3DRDetector.h"
#include "BoldDetector.h"

using namespace std;
using namespace BoldLib;


/*
LineSegmentDetection( int * n_out,
double * img, int X, int Y,
double scale, double sigma_scale, double quant,
double ang_th, double log_eps, double density_th,
int n_bins,
int ** reg_img, int * reg_x, int * reg_y );
 */

/*
 *
 */
int
main (int argc, char** argv)
{
  pcl::console::setVerbosityLevel(pcl::console::L_ALWAYS);
  std::vector<float> sizes;
  sizes.push_back(15);
  sizes.push_back(25);
  sizes.push_back(30);

  visy::detectors::Detector * detector = new visy::detectors::Bold3DMDetector(sizes, 8);
  pcl::visualization::PCLVisualizer * viewer;
  viewer = new pcl::visualization::PCLVisualizer("Bunch Tester Viewer");

  visy::dataset::IrosDataset dataset;


  while (true)
  {



    std::vector<visy::extractors::KeyPoint3D> keypoints;
    cv::Mat descriptor;
    pcl::PointCloud<PointType>::Ptr model_cloud(new pcl::PointCloud<PointType>());
    Eigen::Matrix4f model_pose;
    dataset.fetchFullModel("asus_box", 37, keypoints, descriptor, model_cloud,model_pose, detector);



    viewer->addPointCloud(model_cloud, "scene");
    visy::extractors::KeyPoint3D::draw3DKeyPoints3D(*viewer, keypoints, cv::Scalar(0, 255, 0), "ciao", true);

    //  for (int i = 0; i < rototranslations.size(); i++)
    //  {
    //    pcl::PointCloud<PointType>::Ptr model_projection(new pcl::PointCloud<PointType>());
    //    pcl::transformPointCloud(*model_cloud, *model_projection, rototranslations[i]);
    //    std::stringstream ss;
    //    ss << "Instance_" << i << "_cloud";
    //    visy::tools::displayCloud(*viewer, model_projection, 0, 255, 0, 3.0f, ss.str());
    //  }

    //  cv::namedWindow("out", cv::WINDOW_NORMAL);
    //  cv::imshow("out", out);
    //  cv::namedWindow("out_scene", cv::WINDOW_NORMAL);
    //  cv::imshow("out_scene", out_scene);

    while (!viewer->wasStopped())
    {
      cv::waitKey(100);
      viewer->spinOnce();
    }


  }

  return 1;
}
