/* 
 * File:   Descriptor.cpp
 * Author: daniele
 * 
 * Created on 10 marzo 2015, 18.40
 */

#include "Descriptor.h"

namespace visy
{
  namespace descriptors
  {

    Descriptor::Descriptor ()
    {
    }

    Descriptor::~Descriptor ()
    {
    }

    std::string
    Descriptor::buildName ()
    {
      std::stringstream ss;
      ss << "DESCRIPTOR_" << this->buildNameImpl();
      return ss.str();
    }


  }
}