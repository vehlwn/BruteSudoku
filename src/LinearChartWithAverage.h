#pragma once

#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <memory>

QT_CHARTS_USE_NAMESPACE

class LinearChartWithAverage : public QChart
{
    Q_OBJECT
public:
    LinearChartWithAverage(const QString& dataSeriesName = "Data");

public slots:
    void append(double y);
    void clear();

signals:
    void accumulatedAverage(double avg);

private:
    QLineSeries *m_dataSeries{}, *m_averageSeries{};
    QValueAxis *m_axisX{}, *m_axisY{};
    double m_minX{}, m_maxX{}, m_minY{}, m_maxY{};
    double m_accumulatedAverage{};
};
