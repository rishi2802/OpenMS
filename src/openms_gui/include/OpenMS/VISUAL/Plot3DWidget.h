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
// $Maintainer: Timo Sachsenberg$
// $Authors: Cornelia Friedle $
// --------------------------------------------------------------------------

#pragma once

// OpenMS_GUI config
#include <OpenMS/VISUAL/OpenMS_GUIConfig.h>

#include <OpenMS/VISUAL/PlotWidget.h>
#include <OpenMS/VISUAL/Plot3DCanvas.h>

namespace OpenMS
{
  class Plot3DCanvas;
  /**
      @brief Widget for 3D-visualization of map data

      @image html Plot3DWidget.png

      @ingroup PlotWidgets
  */
  class OPENMS_GUI_DLLAPI Plot3DWidget :
    public PlotWidget
  {
    Q_OBJECT

public:
    ///Constructor
    Plot3DWidget(const Param & preferences, QWidget * parent = nullptr);

    /// Destructor
    ~Plot3DWidget() override;

    /// This method is overwritten to make the class specific members accessible
    inline Plot3DCanvas * canvas()
    {
      return static_cast<Plot3DCanvas *>(canvas_);
    }

    // Docu in base class
    void recalculateAxes_() override;

    //docu in base class
    bool isLegendShown() const override;
    //docu in base class
    void showLegend(bool show) override;

signals:
    /// Requests to display all spectra in 2D plot
    void showCurrentPeaksAs2D();

public slots:
    // Docu in base class
    void showGoToDialog() override;
  };

} //namespace

