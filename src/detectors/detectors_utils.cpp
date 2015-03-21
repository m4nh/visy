/* 
 * File:   detectors_utils.cpp
 * Author: daniele
 * 
 * Created on 20 marzo 2015, 22.19
 */

#include "detectors_utils.h"
#include "Bold3DM2Detector.h"
#include "Bold3DM2MultiDetector.h"
#include "Bold3DR2Detector.h"
#include "Bold3DRDetector.h"
#include "BoldDetector.h"
#include "Bold3DXDetector.h"
#include "Bold3DMDetector.h"
#include "Bold3DR2MultiDetector.h"

namespace visy
{
  namespace detectors
  {
    namespace utils
    {
      /**
       * 
       * @param detector_name
       * @param parameters
       * @return 
       */
      Detector *
      buildDetectorFromString (std::string detector_name, visy::Parameters* parameters)
      {
        bool use_occlusion_edges = false;
        if(parameters->getInt("occlusion")>=1){
          use_occlusion_edges = true;
        }
        std::vector<float> sizes = visy::Parameters::parseFloatArray(parameters->getString("sizes"));

        visy::detectors::Detector * detector;

        if (detector_name == "BOLD3DM")
        {
          detector = new visy::detectors::Bold3DMDetector(sizes, parameters->getInt("nbin"), !use_occlusion_edges);
        }
        else if (detector_name == "BOLD3DM2")
        {
          detector = new visy::detectors::Bold3DM2Detector(sizes, parameters->getInt("nbin"), !use_occlusion_edges);
        }
        else if (detector_name == "BOLD3DM2MULTI")
        {
          detector = new visy::detectors::Bold3DM2MultiDetector(sizes, parameters->getInt("nbin"), !use_occlusion_edges);
        }
        else if (detector_name == "BOLD3DR2")
        {
          detector = new visy::detectors::Bold3DR2Detector(sizes, parameters->getInt("nbin"), !use_occlusion_edges);
        }
        else if (detector_name == "BOLD3DR2MULTI")
        {
          detector = new visy::detectors::Bold3DR2MultiDetector(sizes, parameters->getInt("nbin"), !use_occlusion_edges);
        }
        else if (detector_name == "BOLD3DR")
        {
          detector = new visy::detectors::Bold3DRDetector(sizes, parameters->getInt("nbin"), !use_occlusion_edges);
        }
        else if (detector_name == "BOLD3DX")
        {
          detector = new visy::detectors::Bold3DXDetector(sizes, parameters->getInt("nbin"), !use_occlusion_edges);
        }
        else if (detector_name == "BOLD")
        {
          detector = new visy::detectors::BoldDetector(sizes);
        }

        return detector;
      }

    }
  }
}

