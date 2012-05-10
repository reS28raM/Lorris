/**********************************************
**    This file is part of Lorris
**    http://tasssadar.github.com/Lorris/
**
**    See README and COPYING
***********************************************/

#include <qapplication.h>
#include <qlayout.h>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_series_data.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_text.h>
#include <qwt_math.h>
#include <qwt_plot_grid.h>
#include <qwt_legend_item.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_widget.h>

#include <QMouseEvent>
#include <QWheelEvent>

#include "../datawidget.h"
#include "graph.h"

Graph::Graph(QWidget *parent) : QwtPlot(parent)
{
    // zoom in/out with the wheel
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( canvas() );
    magnifier->setMouseButton(Qt::LeftButton);

    // panning with the left mouse button
    QwtPlotPanner *panner =  new QwtPlotPanner( canvas() );
    panner->setMouseButton(Qt::MiddleButton);

#if defined(Q_WS_X11)
    // Even if not recommended by TrollTech, Qt::WA_PaintOutsidePaintEvent
    // works on X11. This has a nice effect on the performance.

    canvas()->setAttribute(Qt::WA_PaintOutsidePaintEvent, true);
#endif

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(QPen(Qt::gray, 0.0, Qt::DotLine));
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(false);
    grid->attach(this);

    QwtLegend *legend = new QwtLegend;
    legend->setItemMode(QwtLegend::CheckableItem);
    insertLegend(legend, QwtPlot::BottomLegend);

    connect(this, SIGNAL(legendChecked(QwtPlotItem*,bool)), SLOT(showCurve(QwtPlotItem*, bool)));
    connect(axisWidget(QwtPlot::xBottom), SIGNAL(scaleDivChanged()), SIGNAL(updateSampleSize()));

    setAxisScale(QwtPlot::xBottom, -20, 20);
    setAxisScale(QwtPlot::yLeft, -20, 20);
    // to show graph appearance while dragging from add button to widget area
    replot();
}

void Graph::showCurve(QwtPlotItem *item, bool on)
{
    item->setVisible(on);
    QWidget *w = legend()->find(item);
    if ( w && w->inherits("QwtLegendItem") )
        ((QwtLegendItem *)w)->setChecked(on);

    replot();
}

void Graph::showLegend(bool show)
{
    plotLayout()->setLegendPosition(show ? QwtPlot::BottomLegend : QwtPlot::ExternalLegend);
    legend()->setVisible(show);
    replot();
}

void Graph::mousePressEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();

    if((x < RESIZE_BORDER) ||
       (x > width() - RESIZE_BORDER) ||
       (y > height() - RESIZE_BORDER))
    {
        return QWidget::mousePressEvent(event);
    }

    QwtPlot::mousePressEvent(event);
    event->accept();
}

void Graph::mouseMoveEvent(QMouseEvent *event)
{
    QwtPlot::mouseMoveEvent(event);
    QWidget::mouseMoveEvent(event);
}

void Graph::wheelEvent(QWheelEvent *event)
{
    const QPoint& pos = event->pos();
    int yPos = axisWidget(QwtPlot::yLeft)->pos().x() + axisWidget(QwtPlot::yLeft)->width();
    int xPos = axisWidget(QwtPlot::xBottom)->pos().y();

    int axis = -1;
    if(pos.x() < yPos)
        axis = QwtPlot::yLeft;
    else if(pos.y() > xPos)
        axis = QwtPlot::xBottom;

    if(axis == -1)
        return QwtPlot::wheelEvent(event);

    double max = axisScaleDiv(axis)->upperBound();
    double min = axisScaleDiv(axis)->lowerBound();

    double diff = fabs(max - min);
    if(diff == 0)
        diff = 1;

    float exp = (event->modifiers() & Qt::ShiftModifier) ? 0.01 : 0.001;
    double newDiff = fabs(diff + (diff*(exp *event->delta())))/2;

    diff /= 2;
    double newMax = (max - diff) + newDiff;
    double newMin = (min + diff) - newDiff;

    if(newMin > newMax)
        return;

    setAxisScale(axis, newMin, newMax);
    replot();
}

double Graph::XupperBound()
{
    return axisScaleDiv(QwtPlot::xBottom)->upperBound();
}
double Graph::XlowerBound()
{
    return axisScaleDiv(QwtPlot::xBottom)->lowerBound();
}
double Graph::YupperBound()
{
    return axisScaleDiv(QwtPlot::yLeft)->upperBound();
}
double Graph::YlowerBound()
{
    return axisScaleDiv(QwtPlot::yLeft)->lowerBound();
}
