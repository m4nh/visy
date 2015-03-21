/* 
 * File:   Bold3DM2Detector.cpp
 * Author: daniele
 * 
 * Created on 11 marzo 2015, 17.30
 */

#include "Bold3DM2Detector.h"
#include "Bold3DExtractor.h"
#include "Bold3DDescriptorMultiBunch.h"
#include "DFunctionB3DV2.h"

namespace visy
{
  namespace detectors
  {

    Bold3DM2Detector::Bold3DM2Detector (std::vector<float>& sizes, int n_bins, bool filter_occlusion, float zone_radius, float zone_slice, float area_normals_angular_th, float area_max_distance) : Detector ()
    {
      this->extractor = new visy::extractors::Bold3DExtractor(filter_occlusion, zone_radius, zone_slice, area_normals_angular_th, area_max_distance);
      this->descriptor = new visy::descriptors::Bold3DDescriptorMultiBunch(n_bins, sizes, new visy::descriptors::DFunctionB3DV2(n_bins), visy::descriptors::Bold3DDescriptorMultiBunch::BUNCH_METHOD_KNN);
      this->multiplication_factor = sizes.size();
    }

    Bold3DM2Detector::~Bold3DM2Detector ()
    {
    }

    void
    Bold3DM2Detector::detect (cv::Mat& source, pcl::PointCloud<PointType>::Ptr cloud, std::vector<visy::extractors::KeyPoint3D>& keypoints, cv::Mat& descriptor, cv::Mat* mask)
    {
      std::vector<visy::extractors::KeyPoint3D> temp_keypoints;
      this->extractor->extract(source, cloud, temp_keypoints, mask);
      this->descriptor->describe(source, cloud, temp_keypoints, descriptor);
      visy::extractors::utils::replicateKeypoints(temp_keypoints, keypoints, this->multiplication_factor);
    }

    std::string
    Bold3DM2Detector::buildNameImpl ()
    {
      return "BOLD3DM2";
    }


  }
}