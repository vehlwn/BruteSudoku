#pragma once

#include "src/LinearChartWithAverage.h"

#include <QCloseEvent>
#include <QWidget>

namespace Ui {
class TimeChartForm;
}

class TimeChartForm : public QWidget
{
    Q_OBJECT

public:
    explicit TimeChartForm(QWidget* parent = nullptr);
    ~TimeChartForm();

public slots:
    void append(double y);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void on_pushButtonClear_clicked();

private:
    Ui::TimeChartForm*      ui{};
    LinearChartWithAverage* m_chart{};
};
