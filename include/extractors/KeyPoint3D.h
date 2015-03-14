/* 
 * File:   KeyPoint3D.h
 * Author: daniele
 *
 * Created on 9 marzo 2015, 18.03
 */

#ifndef KEYPOINT3D_H
#define	KEYPOINT3D_H

#include <iostream>
#include <vector>
#include <math.h>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boldlib.h>

#include <pcl/common/common_headers.h>
#include <pcl/common/centroid.h>
#include <pcl/common/intersections.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/impl/point_types.hpp>
#include <pcl/common/centroid.h>

#include "tools.h"
#include "commons.h"

namespace visy {
    namespace extractors {

        class KeyPoint3D : public cv::KeyPoint {
        public:
            
            
            static const int KEYPOINT3D_TYPE_UNKNOWN = 0;
            static const int KEYPOINT3D_TYPE_EDGE_SURFACE = 1000;
            static const int KEYPOINT3D_TYPE_EDGE_TEXTURE = 1001;
            static const int KEYPOINT3D_TYPE_EDGE_OCCLUSION = 1002;
            
            KeyPoint3D(cv::Vec4f line, pcl::PointCloud<PointType>::Ptr cloud);
            KeyPoint3D();
            virtual ~KeyPoint3D();
            static void draw3DKeyPoints(cv::Mat& out, std::vector<visy::extractors::KeyPoint3D>& keypoints, cv::Scalar color, float tick,bool force_color = false);
            static void draw3DKeyPointsWithAreas(cv::Mat& out, std::vector<KeyPoint3D>& keypoints, cv::Scalar color, float tick,float radius, float slice);
            static void draw3DKeyPoints3D(pcl::visualization::PCLVisualizer &viewer, std::vector<visy::extractors::KeyPoint3D>& keypoints, cv::Scalar color,std::string name);
            
            cv::Point2i pt1;
            cv::Point2i pt2;
            
            cv::Point3f pt3D;
            cv::Point3f pt3D_1;
            cv::Point3f pt3D_2;
            cv::Point3f direction_x;
            cv::Point3f direction_y;
            cv::Point3f direction_z;
            
            int type;
            
            KeyPoint3D clone();
            static void transformKeyPoint3D(KeyPoint3D& in, KeyPoint3D& out,Eigen::Matrix4f& transform);
        protected:

        };
    }
}
#endif	/* KEYPOINT3D_H */

