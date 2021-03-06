#include <QtGui>
#include <math.h>
#include "spinslider.h"

SpinSlider::SpinSlider(QString text, QString type, QString dataType, qreal min, qreal max, QWidget* parent)
    : QWidget(parent)
{
    value = 1.0;
    this->type = type;
    this->dataType = dataType;
    this->min = min;
    this->max = max;
    QPushButton* resetBtn = new QPushButton(tr("Reset"),this);
    connect(resetBtn, SIGNAL(clicked()), this, SLOT(resetValue()));
    QLabel* label = new QLabel(text+": ");
    label->setFont( QFont("Helvetica", 10) );
    valueLabel = new QLabel("--");
    valueLabel->setFont( QFont("Helvetica", 10) );
    //valueLabel->setFixedWidth(30);
    valueLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(0);
    slider->setMaximum(100);
    //slider->setFixedHeight(16);
    QGridLayout *lay = new QGridLayout();
    lay->setMargin(2);
    lay->setSpacing(2);
    //lay->setColumnStretch(0,1);
    lay->addWidget(label,0,0,1,1);
    lay->addWidget(valueLabel,0,1,1,1);
    lay->addWidget(slider,1,0,1,3);
    lay->addWidget(resetBtn, 0, 2, 1, 1);
    setLayout(lay);
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(changeValue(int)));
    connect(slider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(sliderMoved(int)));
}

void SpinSlider::changeValue(qreal value)
{
    this->value = value;
    if(dataType == "integer")
    {

        valueLabel->setText( QString::number(qRound(value)) );
    }
    else
    {
        valueLabel->setText( QLocale::system().toString(value,'f', 2) );
    }
}

void SpinSlider::changeValue(int value)
{
    qreal value2 = 0.0;
    if(type=="linear")
        value2 = min + value*(max-min)/100;
    if(type=="log")
        value2 = min*exp( value*log(max/min)/100 );
    changeValue(value2);
}

void SpinSlider::setValue(qreal value)
{
    qreal value2 = 0.0;
    if(type=="linear")
        value2 = qRound( 100*(value-min)/(max-min) );
    if(type=="log")
        value2 = qRound( 100.0*log(value/0.2)/log(1000.0) );
    slider->setSliderPosition(value2);
    changeValue(value);
}

void SpinSlider::sliderReleased()
{
    emit valueChanged(this->value);
}

void SpinSlider::sliderMoved(int value)
{
    changeValue(value);
    emit valueChanged(this->value);
}

void SpinSlider::resetValue()
{
    setValue(1.00);
    emit valueChanged(this->value);
}
