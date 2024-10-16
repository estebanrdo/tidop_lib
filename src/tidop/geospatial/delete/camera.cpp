/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "camera.h"

#include <stdexcept>

#include <tidop/math/algebra/rotation_convert.h>

namespace tl
{

	
/// Cámaras predefinidas

class CalibrationRadial1
  : public Calibration
{

public:

  CalibrationRadial1()
    : Calibration(Calibration::CameraModel::radial1){}
  virtual ~CalibrationRadial1() override {}

  std::string name() const override {
    return std::string("Radial 1");
  }

};

class CalibrationRadial2
  : public Calibration
{

public:

  CalibrationRadial2()
    : Calibration(Calibration::CameraModel::radial2){}
  virtual ~CalibrationRadial2() override {}

  std::string name() const override {
    return std::string("Radial 2");
  }

};

class CalibrationRadial3
  : public Calibration
{

public:

  CalibrationRadial3()
    : Calibration(Calibration::CameraModel::radial3){}
  virtual ~CalibrationRadial3() override {}

  std::string name() const override {
    return std::string("Radial 3");
  }

};

class CalibrationSimpleRadialFisheye
  : public Calibration
{

public:

  CalibrationSimpleRadialFisheye()
    : Calibration(Calibration::CameraModel::simple_radial_fisheye){}
  virtual ~CalibrationSimpleRadialFisheye() override {}

  std::string name() const override {
    return std::string("Radial Fisheye 1");
  }

};


class CalibrationRadialFisheye
  : public Calibration
{

public:

  CalibrationRadialFisheye()
    : Calibration(Calibration::CameraModel::radial_fisheye){}
  virtual ~CalibrationRadialFisheye() override {}

  std::string name() const override {
    return std::string("Radial Fisheye 2");
  }

};


class CalibrationOpenCV
  : public Calibration
{

public:

  CalibrationOpenCV()
    : Calibration(Calibration::CameraModel::opencv){}
  virtual ~CalibrationOpenCV() override {}

  std::string name() const override {
    return std::string("OpenCV 1");
  }

};


class CalibrationOpenCVFisheye
  : public Calibration
{

public:

  CalibrationOpenCVFisheye()
    : Calibration(Calibration::CameraModel::opencv_fisheye){}
  virtual ~CalibrationOpenCVFisheye() override {}

  std::string name() const override {
    return std::string("OpenCV Fisheye");
  }

};


class CalibrationOpenCVFull
  : public Calibration
{

public:

  CalibrationOpenCVFull()
    : Calibration(Calibration::CameraModel::opencv_full){}
  virtual ~CalibrationOpenCVFull() override {}

  std::string name() const override {
    return std::string("OpenCV 2");
  }

};


class CalibrationSimplePinhole
  : public Calibration
{

public:

  CalibrationSimplePinhole()
    : Calibration(Calibration::CameraModel::simple_pinhole){}
  virtual ~CalibrationSimplePinhole() override {}

  std::string name() const override {
    return std::string("Pinhole 1");
  }

};



class CalibrationPinhole
  : public Calibration
{

public:

  CalibrationPinhole()
    : Calibration(Calibration::CameraModel::pinhole){}
  virtual ~CalibrationPinhole() override {}

  std::string name() const override {
    return std::string("Pinhole 2");
  }

};



Calibration::Calibration(CameraModel cameraModel)
  : mCameraModel(cameraModel)
{

  if (mCameraModel.isActive(convertFlags(Parameters::focalx)) &&
      mCameraModel.isActive(convertFlags(Parameters::focaly))){
    mParameters[Parameters::focalx] = 0.;
    mParameters[Parameters::focaly] = 0.;
  } else {
    mParameters[Parameters::focal] = 0.;
  }

  mParameters[Parameters::cx] = 0.;
  mParameters[Parameters::cy] = 0.;

  if (mCameraModel.isActive(convertFlags(Parameters::k1))) {
    mParameters[Parameters::k1] = 0.;
  }

  if (mCameraModel.isActive(convertFlags(Parameters::k2))) {
    mParameters[Parameters::k2] = 0.;
  }

  if (mCameraModel.isActive(convertFlags(Parameters::k3))) {
    mParameters[Parameters::k3] = 0.;
  }

  if (mCameraModel.isActive(convertFlags(Parameters::k4))) {
    mParameters[Parameters::k4] = 0.;
  }

  if (mCameraModel.isActive(convertFlags(Parameters::k5))) {
    mParameters[Parameters::k5] = 0.;
  }

  if (mCameraModel.isActive(convertFlags(Parameters::k6))) {
    mParameters[Parameters::k6] = 0.;
  }

  if (mCameraModel.isActive(convertFlags(Parameters::p1))) {
    mParameters[Parameters::p1] = 0.;
  }

  if (mCameraModel.isActive(convertFlags(Parameters::p2))) {
    mParameters[Parameters::p2] = 0.;
  }
}

Calibration::Calibration(const Calibration &calibration)
  : mCameraModel(calibration.mCameraModel),
    mParameters(calibration.mParameters)
{
}

Calibration::Calibration(Calibration &&calibration) TL_NOEXCEPT
  : mCameraModel(std::move(calibration.mCameraModel)),
    mParameters(std::move(calibration.mParameters))
{
}

Calibration::CameraModel Calibration::cameraModel() const
{
  return mCameraModel.flags();
}

//std::shared_ptr<Calibration> Calibration::create(Calibration::CameraModel cameraModel)
//{
//  std::shared_ptr<Calibration> calib;
//  switch (cameraModel)
//  {
//  case Calibration::CameraModel::radial1:
//    calib = std::make_shared<CalibrationRadial1>();
//    break;
//  case Calibration::CameraModel::radial2:
//    calib =std::make_shared<CalibrationRadial2>();
//    break;
//  case Calibration::CameraModel::radial3:
//    calib = std::make_shared<CalibrationRadial3>();
//    break;
//  case Calibration::CameraModel::simple_radial_fisheye:
//    calib = std::make_shared<CalibrationSimpleRadialFisheye>();
//    break;
//  case Calibration::CameraModel::radial_fisheye:
//    calib = std::make_shared<CalibrationRadialFisheye>();
//    break;
//  case Calibration::CameraModel::opencv:
//    calib = std::make_shared<CalibrationOpenCV>();
//    break;
//  case Calibration::CameraModel::opencv_fisheye:
//    calib = std::make_shared<CalibrationOpenCVFisheye>();
//    break;
//  case Calibration::CameraModel::opencv_full:
//    calib = std::make_shared<CalibrationOpenCVFull>();
//    break;
//  case Calibration::CameraModel::simple_pinhole:
//    calib = std::make_shared<CalibrationSimplePinhole>();
//    break;
//  case Calibration::CameraModel::pinhole:
//    calib = std::make_shared<CalibrationPinhole>();
//    break;
//  }
//  return calib;
//}

int Calibration::cameraModelId() const
{
  return static_cast<int>(mCameraModel.flags());
}

Calibration &Calibration::operator = (const Calibration &calibration)
{
  if (this != &calibration) {
    mCameraModel = calibration.mCameraModel;
    mParameters = calibration.mParameters;
  }
  return *this;
}

Calibration::CameraModel Calibration::convertFlags(Parameters parameter) const
{
  return static_cast<CameraModel>(static_cast<std::underlying_type<CameraModel>::type>(parameter));
}

Calibration::CameraModel Calibration::convertFlags(CameraType cameraType) const
{
  return static_cast<CameraModel>(static_cast<std::underlying_type<CameraModel>::type>(cameraType));
}

Calibration &Calibration::operator = (Calibration &&calibration) TL_NOEXCEPT
{
  if (this != &calibration) {
    mCameraModel = std::move(calibration.mCameraModel);
    mParameters = std::move(calibration.mParameters);
  }
  return *this;
}

Calibration::parameter_iterator Calibration::parametersBegin()
{
  return mParameters.begin();
}

Calibration::parameter_const_iterator Calibration::parametersBegin() const
{
  return mParameters.cbegin();
}

Calibration::parameter_iterator Calibration::parametersEnd()
{
  return mParameters.end();
}

Calibration::parameter_const_iterator Calibration::parametersEnd() const
{
  return mParameters.cend();
}

//Calibration::parameter_iterator Calibration::find(Parameters parameter)
//{
//  return mParameters.find(parameter);
//}

//Calibration::parameter_const_iterator Calibration::find(Parameters parameter) const
//{
//  return parameter_const_iterator();
//}

std::string Calibration::parameterName(Parameters parameter) const
{
  std::string parameter_name;
  switch (parameter)
  {
  case Calibration::Parameters::focal:
    parameter_name = "f";
    break;
  case Calibration::Parameters::focalx:
    parameter_name = "fx";
    break;
  case Calibration::Parameters::focaly:
    parameter_name = "fy";
    break;
  case Calibration::Parameters::cx:
    parameter_name = "cx";
    break;
  case Calibration::Parameters::cy:
    parameter_name = "cy";
    break;
  case Calibration::Parameters::k1:
    parameter_name = "k1";
    break;
  case Calibration::Parameters::k2:
    parameter_name = "k2";
    break;
  case Calibration::Parameters::k3:
    parameter_name = "k3";
    break;
  case Calibration::Parameters::k4:
    parameter_name = "k4";
    break;
  case Calibration::Parameters::k5:
    parameter_name = "k5";
    break;
  case Calibration::Parameters::k6:
    parameter_name = "k6";
    break;
  case Calibration::Parameters::p1:
    parameter_name = "p1";
    break;
  case Calibration::Parameters::p2:
    parameter_name = "p2";
    break;
  }
  return parameter_name;
}

bool Calibration::existParameter(Calibration::Parameters parameter) const
{
  return mParameters.find(parameter) != mParameters.end();
}

double Calibration::parameter(Calibration::Parameters parameter) const
{
  return this->existParameter(parameter) ? mParameters.at(parameter) : 0.0;
}

void Calibration::setParameter(Calibration::Parameters parameter, double value)
{
  if (this->existParameter(parameter))
    mParameters.at(parameter) = value;
}

bool tl::Calibration::checkCameraType(CameraType camera_type) const
{
  return mCameraModel.isActive(convertFlags(camera_type));
}




/* ------------------------------------------------------------------ */



std::shared_ptr<Calibration> CalibrationFactory::create(const std::string &cameraType)
{
  std::shared_ptr<Calibration> calibration;
  
  try {
  
    if (cameraType.compare("Pinhole 1") == 0) {
      calibration = std::make_shared<CalibrationSimplePinhole>();
    } else if (cameraType.compare("Pinhole 2") == 0) {
      calibration = std::make_shared<CalibrationPinhole>();
    } else if (cameraType.compare("Radial 1") == 0) {
      calibration = std::make_shared<CalibrationRadial1>();
    } else if (cameraType.compare("Radial 2") == 0) {
      calibration = std::make_shared<CalibrationRadial2>();
    } else if (cameraType.compare("OpenCV 1") == 0) {
      calibration = std::make_shared<CalibrationOpenCV>();
    } else if (cameraType.compare("OpenCV Fisheye") == 0) {
      calibration = std::make_shared<CalibrationOpenCVFisheye>();
    } else if (cameraType.compare("OpenCV 2") == 0) {
      calibration = std::make_shared<CalibrationOpenCVFull>();
    } else if (cameraType.compare("Radial Fisheye 1") == 0) {
      calibration = std::make_shared<CalibrationSimpleRadialFisheye>();
    } else if (cameraType.compare("Radial Fisheye 2") == 0) {
      calibration = std::make_shared<CalibrationRadialFisheye>();
    } else if (cameraType.compare("Radial 3") == 0) {
      calibration = std::make_shared<CalibrationRadial3>();
    } else {
      TL_THROW_EXCEPTION("Invalid Camera Type: '%s'.", cameraType.c_str());
    }

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

  return calibration;
}

std::shared_ptr<Calibration> tl::CalibrationFactory::create(Calibration::CameraModel cameraModel)
{
  std::shared_ptr<Calibration> calibration;
  switch (cameraModel)
  {
  case Calibration::CameraModel::radial1:
    calibration = std::make_shared<CalibrationRadial1>();
    break;
  case Calibration::CameraModel::radial2:
    calibration =std::make_shared<CalibrationRadial2>();
    break;
  case Calibration::CameraModel::radial3:
    calibration = std::make_shared<CalibrationRadial3>();
    break;
  case Calibration::CameraModel::simple_radial_fisheye:
    calibration = std::make_shared<CalibrationSimpleRadialFisheye>();
    break;
  case Calibration::CameraModel::radial_fisheye:
    calibration = std::make_shared<CalibrationRadialFisheye>();
    break;
  case Calibration::CameraModel::opencv:
    calibration = std::make_shared<CalibrationOpenCV>();
    break;
  case Calibration::CameraModel::opencv_fisheye:
    calibration = std::make_shared<CalibrationOpenCVFisheye>();
    break;
  case Calibration::CameraModel::opencv_full:
    calibration = std::make_shared<CalibrationOpenCVFull>();
    break;
  case Calibration::CameraModel::simple_pinhole:
    calibration = std::make_shared<CalibrationSimplePinhole>();
    break;
  case Calibration::CameraModel::pinhole:
    calibration = std::make_shared<CalibrationPinhole>();
    break;
  }
  return calibration;
}



/* ------------------------------------------------------------------ */



Camera::Camera()
    : mMake(""),
      mModel(""),
      mType("OpenCV 1"),
      mFocal(1.),
      mWidth(0),
      mHeight(0),
      mSensorSize(1.),
      mCalibration(nullptr)
{
  init();
}

Camera::Camera(const std::string &make, const std::string &model)
    : mMake(make),
      mModel(model),
      mType("OpenCV 1"),
      mFocal(1.),
      mWidth(0),
      mHeight(0),
      mSensorSize(1.),
      mCalibration(nullptr)
{
  init();
}

Camera::Camera(const Camera &camera)
  : mMake(camera.mMake),
    mModel(camera.mModel),
    mType(camera.mType),
    mFocal(camera.mFocal),
    mWidth(camera.mWidth),
    mHeight(camera.mHeight),
    mSensorSize(camera.mSensorSize),
    mCalibration(camera.mCalibration)
{
}

std::string Camera::make() const
{
  return mMake;
}

void Camera::setMake(const std::string &make)
{
  mMake = make;
}

std::string Camera::model() const
{
  return mModel;
}

void Camera::setModel(const std::string &model)
{
  mModel = model;
}

std::string Camera::type() const
{
  return mType;
}

void Camera::setType(const std::string &type)
{
  mType = type;
}

double Camera::focal() const
{
  return mFocal;
}

void Camera::setFocal(double focal)
{
  mFocal = focal;
}

int Camera::width() const
{
  return mWidth;
}

void Camera::setWidth(int width)
{
  mWidth = width;
}

int Camera::height() const
{
  return mHeight;
}

void Camera::setHeight(int height)
{
  mHeight = height;
}

double Camera::sensorSize() const
{
    return mSensorSize;
}

void Camera::setSensorSize(double sensorSize)
{
  mSensorSize = sensorSize;
}

std::shared_ptr<Calibration> Camera::calibration() const
{
  return mCalibration;
}

void Camera::setCalibration(std::shared_ptr<Calibration> &calibration)
{
  mCalibration = calibration;
}

Camera &Camera::operator =(const Camera &camera)
{
  if (this != &camera){
    this->mMake = camera.mMake;
    this->mModel = camera.mModel;
    this->mType = camera.mType;
    this->mFocal = camera.mFocal;
    this->mWidth = camera.mWidth;
    this->mHeight = camera.mHeight;
    this->mSensorSize = camera.mSensorSize;
    this->mCalibration = camera.mCalibration;
  }
  return *this;
}

void Camera::init()
{
}



/* ------------------------------------------------------------------ */



CameraPose::CameraPose()
  : mPosition(),
    mRotation(nullptr)
{
}

CameraPose::CameraPose(double x, double y, double z, 
                       const math::RotationMatrix<double> &rotationMatrix)
  : mPosition(x, y, z),
    mRotation(new math::RotationMatrix<double>(rotationMatrix))
{
}

CameraPose::CameraPose(const Point3D &center,
                       const math::RotationMatrix<double> &rotationMatrix)
  : mPosition(center),
    mRotation(new math::RotationMatrix<double>(rotationMatrix))
{
}

CameraPose::CameraPose(double x, double y, double z,
                       const math::Quaternion<double> &quaternion)
  : mPosition(x, y, z),
    mRotation(new math::Quaternion<double>(quaternion))
{
}

CameraPose::CameraPose(const Point3D &center,
                       const math::Quaternion<double> &quaternion)
  : mPosition(center),
    mRotation(new math::Quaternion<double>(quaternion))
{
}

CameraPose::~CameraPose()
{
}

Point3D CameraPose::position() const
{
  return mPosition;
}

void CameraPose::setPosition(const Point3D &position)
{
  mPosition = position;
}

math::Quaterniond tl::CameraPose::quaternion() const
{
  math::Quaterniond quaternion = math::Quaterniond::zero();

  if (mRotation) {

    math::Rotation::Type type = mRotation->rotationType();
    if (type == math::Rotation::Type::quaternion) {
      quaternion = *dynamic_cast<math::Quaterniond *>(mRotation.get());
    } else if (type == math::Rotation::Type::rotation_matrix) {
      math::RotationConverter<double>::convert(*dynamic_cast<math::RotationMatrix<double> *>(mRotation.get()), quaternion);
    }

  }

  return quaternion;
}

void CameraPose::setQuaternion(const math::Quaterniond &quaternion)
{
  mRotation = std::make_shared<math::Quaternion<double>>(quaternion);
}

math::RotationMatrix<double> CameraPose::rotationMatrix() const
{
  math::RotationMatrix<double> rotation_matrix = math::RotationMatrix<double>::zero();

  if (mRotation) {
    math::Rotation::Type type = mRotation->rotationType();
    if (type == math::Rotation::Type::quaternion) {
      math::RotationConverter<double>::convert(*dynamic_cast<math::Quaternion<double> *>(mRotation.get()), rotation_matrix);
    } else if (type == math::Rotation::Type::rotation_matrix) {
      rotation_matrix = *dynamic_cast<math::RotationMatrix<double> *>(mRotation.get());
    }
  }

  return rotation_matrix;
}

void CameraPose::setRotationMatrix(const math::RotationMatrix<double> &rotationMatrix)
{
  mRotation = std::make_shared<math::RotationMatrix<double>>(rotationMatrix);
}

bool CameraPose::isEmpty() const
{
  return mPosition == Point3D();
}



/* ------------------------------------------------------------------ */



cv::Mat openCVCameraMatrix(const Calibration &calibration)
{
  float focal_x{};
  float focal_y{};

  if (calibration.existParameter(tl::Calibration::Parameters::focal)) {
    focal_x = static_cast<float>(calibration.parameter(tl::Calibration::Parameters::focal));
    focal_y = static_cast<float>(calibration.parameter(tl::Calibration::Parameters::focal));
  } else {
    focal_x = static_cast<float>(calibration.parameter(tl::Calibration::Parameters::focalx));
    focal_y = static_cast<float>(calibration.parameter(tl::Calibration::Parameters::focaly));
  }

  float ppx = static_cast<float>(calibration.parameter(tl::Calibration::Parameters::cx));
  float ppy = static_cast<float>(calibration.parameter(tl::Calibration::Parameters::cy));

  std::array<std::array<float, 3>, 3> camera_matrix_data = {focal_x, 0.f, ppx,
                                                            0.f, focal_y, ppy,
                                                            0.f, 0.f, 1.f};
  cv::Mat cameraMatrix(3, 3, CV_32F, camera_matrix_data.data());

  return cameraMatrix.clone();
}

cv::Mat openCVDistortionCoefficients(const Calibration &calibration)
{
  tl::Calibration::CameraModel camera_model = calibration.cameraModel();
  bool b_fisheye = calibration.checkCameraType(tl::Calibration::CameraType::fisheye);

  size_t distCoeffs_size = 4;

  switch (camera_model) {
    case tl::Calibration::CameraModel::radial1:
      distCoeffs_size = 4;
      break;
    case tl::Calibration::CameraModel::radial2:
      distCoeffs_size = 4;
      break;
    case tl::Calibration::CameraModel::radial3:
      distCoeffs_size = 5;
      break;
    case tl::Calibration::CameraModel::simple_radial_fisheye:
      distCoeffs_size = 4;
      break;
    case tl::Calibration::CameraModel::radial_fisheye:
      distCoeffs_size = 4;
      break;
    case tl::Calibration::CameraModel::opencv:
      distCoeffs_size = 4;
      break;
    case tl::Calibration::CameraModel::opencv_fisheye:
      distCoeffs_size = 4;
      break;
    case tl::Calibration::CameraModel::opencv_full:
      distCoeffs_size = 8;
      break;
    case tl::Calibration::CameraModel::simple_pinhole:
      distCoeffs_size = 4;
      break;
    case tl::Calibration::CameraModel::pinhole:
      distCoeffs_size = 4;
      break;
    default:
      break;
  }

  cv::Mat distCoeffs = cv::Mat::zeros(1, distCoeffs_size, CV_32F);

  distCoeffs.at<float>(0) = calibration.existParameter(tl::Calibration::Parameters::k1) ?
    static_cast<float>(calibration.parameter(tl::Calibration::Parameters::k1)) : 0.f;
  distCoeffs.at<float>(1) = calibration.existParameter(tl::Calibration::Parameters::k2) ?
    static_cast<float>(calibration.parameter(tl::Calibration::Parameters::k2)) : 0.f;
  if (b_fisheye) {
    distCoeffs.at<float>(2) = calibration.existParameter(tl::Calibration::Parameters::k3) ?
      static_cast<float>(calibration.parameter(tl::Calibration::Parameters::k3)) : 0.f;
    distCoeffs.at<float>(3) = calibration.existParameter(tl::Calibration::Parameters::k4) ?
      static_cast<float>(calibration.parameter(tl::Calibration::Parameters::k4)) : 0.f;
  } else {

    distCoeffs.at<float>(2) = calibration.existParameter(tl::Calibration::Parameters::p1) ?
      static_cast<float>(calibration.parameter(tl::Calibration::Parameters::p1)) : 0.f;
    distCoeffs.at<float>(3) = calibration.existParameter(tl::Calibration::Parameters::p2) ?
      static_cast<float>(calibration.parameter(tl::Calibration::Parameters::p2)) : 0.f;

    if (camera_model == tl::Calibration::CameraModel::opencv_full ||
        camera_model == tl::Calibration::CameraModel::radial3) {
      distCoeffs.at<float>(4) = calibration.existParameter(tl::Calibration::Parameters::k3) ?
        static_cast<float>(calibration.parameter(tl::Calibration::Parameters::k3)) : 0.f;
    }

    if (camera_model == tl::Calibration::CameraModel::opencv_full) {
      distCoeffs.at<float>(5) = calibration.existParameter(tl::Calibration::Parameters::k4) ?
        static_cast<float>(calibration.parameter(tl::Calibration::Parameters::k4)) : 0.f;
      distCoeffs.at<float>(6) = calibration.existParameter(tl::Calibration::Parameters::k5) ?
        static_cast<float>(calibration.parameter(tl::Calibration::Parameters::k5)) : 0.f;
      distCoeffs.at<float>(7) = calibration.existParameter(tl::Calibration::Parameters::k6) ?
        static_cast<float>(calibration.parameter(tl::Calibration::Parameters::k6)) : 0.f;
    }
  }

  return distCoeffs;
}

} // namespace tl
