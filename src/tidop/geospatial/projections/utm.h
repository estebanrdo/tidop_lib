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

#pragma once

#include "tidop/core/defs.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geospatial/ellipsoid.h"
#include "tidop/geospatial/util.h"

namespace tl
{

/*!
 * \addtogroup geospatial
 *
 * \{
 */

constexpr double k0 = 0.9996;


Point<double> fwd(double lat, double lon, Ellipsoid ellipsoid)
{
	Point<double> pt_utm;
  // Calcular el huso UTM
  auto zone = utmZoneFromLonLat(lon, lat);
  // Calcular la longitud central del huso
  double lon0 = (zone.first - 1) * 6.0 - 180.0 + 3.0;
  
  double a = ellipsoid.a;
  double e = ellipsoid.eccentricity();
  
  double N = a / std::sqrt(1 - e * e * std::sin(lat) * std::sin(lat));
    double T = std::tan(lat) * std::tan(lat);
    double C = e * e * std::cos(lat) * std::cos(lat) / (1 - e * e);
    double A = (lon - lon0) * std::cos(lat);
    double M = a * ((1 - e * e / 4 - 3 * e * e * e * e / 64 - 5 * e * e * e * e * e * e / 256) * lat
            - (3 * e * e / 8 + 3 * e * e * e * e / 32 + 45 * e * e * e * e * e * e / 1024) * std::sin(2 * lat)
            + (15 * e * e * e / 256 + 45 * e * e * e * e / 1024) * std::sin(4 * lat)
            - (35 * e * e * e * e / 3072) * std::sin(6 * lat));
    pt_utm.x = k0 * N * (A + (1 - T + C) * A * A * A / 6
            + (5 - 18 * T + T * T + 72 * C - 58 * e * e) * A * A * A * A * A / 120) + 500000;
    pt_utm.y = (k0 * (M + N * std::tan(lat) * (A * A / 2 + (5 - T + (9 * C) + (4 * C * C)) * A * A * A * A / 24 + (61 - (58 * T) + (T * T) + (600 * C) - (330 * e * e)) * A * A * A * A * A * A / 720)));
    if (lat < 0) {
        pt_utm.y += 10000000
    }
	
	return pt_utm;
}


Point<double> inv(Point<double> pt_utm, Ellipsoid ellipsoid, zone) {

    double a = ellipsoid.a;
    double e = ellipsoid.e2;
	double e2 = e * e;
    double B = e2 / (1 - e2);
    double g = (1 - std::sqrt(1 - e2)) / (1 + std::sqrt(1 - e2));
    double x = pt_utm.x - 500000;
    double y = pt_utm.y;
	// Calcular la longitud central del huso
    double lon0 = (zone - 1) * 6.0 - 180.0 + 3.0;
  
    var u = y / k0;
    var q = u / (a * (1 - e2 / 4 - 3 * e2 * e2 / 64 - 5 * e2*e2*e2 / 256));
    var w = q + (3 * g / 2 - 27 * g * g * g / 32) * std::sin(2 * q) + (21 * g * g / 16 - 55 * std::pow(g, 4) / 32) * Math.sin(4 * q) + (151 * Math.pow(g, 3) / 96) * std::sin(6 * q);
    var s = a / std::sqrt(1 - e2 * std::sin(w) * std::sin(w));
    var l = std::tan(w) * std::tan(w);
    var A = B * std::cos(w) * std::cos(w);
    var o = a * (1 - e2) / Math.pow(1 - e2 * std::sin(w) * std::sin(w), 1.5);
    var z = x / (s * k0);
    var p = (w - (s * Math.tan(w) / o) * (z * z / 2 - (5 + (3 * l) + (10 * A) - (4 * A * A) - (9 * B)) * Math.pow(z, 4) / 24 + (61 + (90 * l) + (298 * A) + (45 * l * l) - (252 * B) - (3 * A * A)) * Math.pow(z, 6) / 720)) * (180 / Math.PI);
    var k = lon0 + ((z - (1 + 2 * l + A) * Math.pow(z, 3) / 6 + (5 - (2 * A) + (28 * l) - (3 * A * A) + (8 * B) + (24 * l * l)) * Math.pow(z, 5) / 120) / Math.cos(w)) * (180 / Math.PI);
    return {
        lon: k,
        lat: p
    }
}


/*! \} */ // end of geospatial

} // End namespace tl

