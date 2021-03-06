/* 
 * File:   Bold3DExtractor.h
 * Author: daniele
 *
 * Created on 9 marzo 2015, 18.14
 */

#ifndef BOLD3DEXTRACTOR_H
#define	BOLD3DEXTRACTOR_H

#include "Extractor.h"


namespace visy {
    namespace extractors {

        class Bold3DExtractor : public Extractor {
        public:
            Bold3DExtractor(bool filter_occlusion = true,float zone_radius = 5.0f, float zone_slice = 2.0f,float area_normals_angular_th = 25.0f, float area_max_distance=0.05f,int extraction_method = visy::tools::VISY_TOOLS_EDGEDETECTION_METHOD_BOLD_LSD, int parallels_rounds = 5, float parallels_distance = 2.0f,float near_filtering_th = 0.0f);
            Bold3DExtractor(const Bold3DExtractor& orig);
            virtual ~Bold3DExtractor();

            virtual void extract(cv::Mat& source, pcl::PointCloud<PointType>::Ptr cloud, std::vector<KeyPoint3D>& keypoints, cv::Mat* mask = NULL);
            static void draw3DKeyPointsWithAreas (cv::Mat& out, std::vector<visy::extractors::KeyPoint3D>& keypoints, cv::Scalar color, float tick, float radius, float slice);
            
            
            void checkKeyPoint3DType(cv::Mat& source, pcl::PointCloud<PointType>::Ptr cloud, visy::extractors::KeyPoint3D& keypoint);
            
             virtual  std::string buildNameImpl();
        private:
            bool filter_occlusion;
            float area_radius;
            float area_slice;
            float area_normals_angular_th;
            float area_max_distance;
            int extraction_method;
            int parallels_rounds;
            float parallels_distance;
            float near_filtering_th;

        };

    }
}
#endif	/* BOLD3DEXTRACTOR_H */

