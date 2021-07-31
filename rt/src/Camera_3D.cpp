#include "Camera_3D.hpp"

#include "Bob.hpp"

//Camera_3D::Camera_3D() : projection(0), samples(0), aperture(-1.0), focal_length(0.0), screen(*this, Bob::getApp().picFile) {} // screen needs the camera ref
Camera_3D::Camera_3D() : projection(0), samples(0), aperture(-1.0), focal_length(0.0), screen(*this) {} // screen needs the camera ref
