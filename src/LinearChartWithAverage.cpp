#include "LinearChartWithAverage.h"

LinearChartWithAverage::LinearChartWithAverage(const QString& dataSeriesName)
    : m_dataSeries{new QLineSeries}
    , m_averageSeries{new QLineSeries}
    , m_axisX{new QValueAxis}
    , m_axisY{new QValueAxis}
{
    m_dataSeries->setName(dataSeriesName);
    m_averageSeries->setName("Average");
    m_dataSeries->setPointLabelsVisible(false);
    m_dataSeries->setPointsVisible(true);

    m_axisX->setTitleText("N of points");
    m_axisX->setTitleVisible(true);
    m_axisY->setTitleText(dataSeriesName);
    m_axisY->setTitleVisible(true);

    addSeries(m_dataSeries);
    addSeries(m_averageSeries);
    addAxis(m_axisX, Qt::AlignBottom);
    addAxis(m_axisY, Qt::AlignLeft);
    legend()->setVisible(true);

    for(auto&& s : {m_dataSeries, m_averageSeries})
    {
        s->attachAxis(m_axisX);
        s->attachAxis(m_axisY);
    }
    *m_averageSeries << QPointF{0, 0} << QPointF{0, 0};

    connect(m_dataSeries, &QXYSeries::pointAdded, this, [this](int index) {
        const QPointF& p = m_dataSeries->at(index);
        m_minX = std::min(m_minX, p.x());
        m_maxX = std::max(m_maxX, p.x());
        m_minY = std::min(m_minY, p.y());
        m_maxY = std::max(m_maxY, p.y());
        m_axisX->setRange(m_minX, m_maxX);
        m_axisY->setRange(m_minY, m_maxY);

        const int n = m_dataSeries->count();
        (m_accumulatedAverage *= (n - 1.) / n) += p.y() / n;
        m_averageSeries->replace(0, QPointF{m_minX, m_accumulatedAverage});
        m_averageSeries->replace(1, QPointF{m_maxX, m_accumulatedAverage});
        emit accumulatedAverage(m_accumulatedAverage);
    });
}

void LinearChartWithAverage::append(double y)
{
    const double x = m_dataSeries->count();
    m_dataSeries->append(x, y);
}

void LinearChartWithAverage::clear()
{
    m_dataSeries->clear();
    m_averageSeries->replace(0, QPointF{0., 0.});
    m_averageSeries->replace(1, QPointF{0., 0.});
    m_minX = m_maxX = m_minY = m_maxY = 0;
    m_axisX->setRange(m_minX, m_maxX);
    m_axisY->setRange(m_minY, m_maxY);
    m_accumulatedAverage = 0.;
    emit accumulatedAverage(m_accumulatedAverage);
}
