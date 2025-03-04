// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2022.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Timo Sachsenberg $
// $Authors: Christian Ehrlich, Chris Bielow $
// --------------------------------------------------------------------------

#pragma once

#include <OpenMS/CONCEPT/Types.h>
#include <OpenMS/CONCEPT/Exception.h>

#include <iterator>
#include <vector>

namespace OpenMS
{
  namespace Math
  {
    /*
      @brief Estimates model parameters for a quadratic equation

      The quadratic equation is of the form 
       y = a + b*x + c*x^2

      Weighted inputs are supported. 

    */
    class OPENMS_DLLAPI QuadraticRegression
    {
public:
      QuadraticRegression();

      /** compute the quadratic regression over 2D points */
      void computeRegression(
        std::vector<double>::const_iterator x_begin, 
        std::vector<double>::const_iterator x_end, 
        std::vector<double>::const_iterator y_begin);

      /** compute the weighted quadratic regression over 2D points */
      void computeRegressionWeighted(
        std::vector<double>::const_iterator x_begin, 
        std::vector<double>::const_iterator x_end,
        std::vector<double>::const_iterator y_begin, 
        std::vector<double>::const_iterator w_begin);

      /** evaluate the quadratic function */
      double eval(double x) const;

      /** evaluate using external coefficients */
      static double eval(double A, double B, double C, double x);

      double getA() const; /// A = the intercept
      double getB() const; /// B*X
      double getC() const; /// C*X^2
      double getChiSquared() const;

protected:
      double a_;
      double b_;
      double c_;
      double chi_squared_;
    }; //class

  } //namespace
} //namespace

