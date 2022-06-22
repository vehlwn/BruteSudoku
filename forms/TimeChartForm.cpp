#include "TimeChartForm.h"

#include "ui_TimeChartForm.h"

TimeChartForm::TimeChartForm(QWidget* parent)
    : QWidget{parent}
    , ui{new Ui::TimeChartForm}
    , m_chart{new LinearChartWithAverage{"Solution time, s"}}

{
    ui->setupUi(this);
    ui->chartView->setChart(m_chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    connect(m_chart, &LinearChartWithAverage::accumulatedAverage, this,
        [this](double avg) { this->ui->lineEditAverage->setText(QString::number(avg)); });
}

TimeChartForm::~TimeChartForm()
{
    delete ui;
}

void TimeChartForm::append(double y)
{
    m_chart->append(y);
}

void TimeChartForm::closeEvent(QCloseEvent* event)
{
    hide();
    event->ignore();
}

void TimeChartForm::on_pushButtonClear_clicked()
{
    m_chart->clear();
}
