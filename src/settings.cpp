#include "settings.h"
#include "ui_settings.h"

#define FLOAT_EPSILON 0.001

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    // Use QApplication properties to save the settings.
    // This properties are accessible from all components (video-player, waveform, subtable ...)
    qApp->setProperty("prop_SubMinInterval_frame", ui->intervalMinSpinBox->value());
    qApp->setProperty("prop_SubMinDuration_ms", ui->displayTimeMinSpinBox->value());
    qApp->setProperty("prop_FrameRate_fps", ui->frameRateComboBox->currentText().toDouble());
    qApp->setProperty("prop_MaxCharPerLine", ui->maxCharPerLineSpinBox->value());
    qApp->setProperty("prop_MaxCharPerSec", ui->maxCharPerSecDoubleSpinBox->value());

    qApp->setProperty("prop_LeftMargin_percent", ui->leftMarginSpinBox->value());
    qApp->setProperty("prop_RightMargin_percent", ui->rightMarginSpinBox->value());
    qApp->setProperty("prop_TopMargin_percent", ui->topMarginSpinBox->value());
    qApp->setProperty("prop_BottomMargin_percent", ui->bottomMarginSpinBox->value());

    mCurrentProperties.setFrameRate(ui->frameRateComboBox->currentText().toDouble());

    mSettingsChangedBySoft = false;
}

Settings::~Settings()
{
    delete ui;
}

void Settings::updateDisplayAll() {

    ui->intervalMinSpinBox->setValue( qApp->property("prop_SubMinInterval_frame").toInt());
    ui->displayTimeMinSpinBox->setValue( qApp->property("prop_SubMinDuration_ms").toInt());
    this->setFrameRate(qApp->property("prop_FrameRate_fps").toDouble());
    ui->maxCharPerLineSpinBox->setValue(qApp->property("prop_MaxCharPerLine").toInt());
    ui->maxCharPerSecDoubleSpinBox->setValue(qApp->property("prop_MaxCharPerSec").toDouble());

    ui->leftMarginSpinBox->setValue( qApp->property("prop_LeftMargin_percent").toDouble());
    ui->rightMarginSpinBox->setValue( qApp->property("prop_RightMargin_percent").toDouble());
    ui->topMarginSpinBox->setValue( qApp->property("prop_TopMargin_percent").toDouble());
    ui->bottomMarginSpinBox->setValue( qApp->property("prop_BottomMargin_percent").toDouble());
}

void Settings::on_displayTimeMinSpinBox_valueChanged(int value) {

    qApp->setProperty("prop_SubMinDuration_ms", value);
}

void Settings::on_intervalMinSpinBox_valueChanged(int value) {

    qApp->setProperty("prop_SubMinInterval_frame", value);
}

void Settings::on_maxCharPerLineSpinBox_valueChanged(int value) {

    qApp->setProperty("prop_MaxCharPerLine", value);
}

void Settings::on_maxCharPerSecDoubleSpinBox_valueChanged(double value) {

    qApp->setProperty("prop_MaxCharPerSec", value);

    emit marginChanged();
}

void Settings::on_leftMarginSpinBox_valueChanged(double arg1) {

    qApp->setProperty("prop_LeftMargin_percent", arg1);

    emit marginChanged();
}

void Settings::on_rightMarginSpinBox_valueChanged(double arg1) {

    qApp->setProperty("prop_RightMargin_percent", arg1);

    emit marginChanged();
}

void Settings::on_topMarginSpinBox_valueChanged(double arg1) {

    qApp->setProperty("prop_TopMargin_percent", arg1);

    emit marginChanged();
}

void Settings::on_bottomMarginSpinBox_valueChanged(double arg1) {

    qApp->setProperty("prop_BottomMargin_percent", arg1);
}


void Settings::on_frameRateComboBox_currentIndexChanged(int index) {

    qreal frame_rate;

    if ( mSettingsChangedBySoft == true ) {
        mSettingsChangedBySoft = false;
        return;
    }

    ui->frameRateSpinBox->setEnabled(false);

    switch (index) {
    case 0:
        frame_rate = 23.976;
        break;
    case 1:
        frame_rate = 24;
        break;
    case 2:
        frame_rate = 25;
        break;
    case 3:
        frame_rate = 29.97;
        break;
    case 4:
        frame_rate = 30;
        break;
    case 5:
        ui->frameRateSpinBox->setEnabled(true);
        frame_rate = ui->frameRateSpinBox->value();
        break;
    default:
        frame_rate = 25;
        break;
    }

    if ( index != 5 ) {

        qApp->setProperty("prop_FrameRate_fps", frame_rate);

        mCurrentProperties.setFrameRate(frame_rate);

        emit frameRateChanged(frame_rate);
    }
}

void Settings::on_frameRateSpinBox_editingFinished() {

    qreal frame_rate = ui->frameRateSpinBox->value();

    if ( qFuzzyCompare(frame_rate, qApp->property("prop_FrameRate_fps").toDouble())  == false ) {

        qApp->setProperty("prop_FrameRate_fps", frame_rate);
        mCurrentProperties.setFrameRate(frame_rate);
        emit frameRateChanged(frame_rate);
    }
}

void Settings::setFrameRate(qreal frameRate) {

    int index;

    ui->frameRateSpinBox->setEnabled(false);

    if ( qAbs(frameRate - 23.976) < FLOAT_EPSILON ) {
        index = 0;
    }
    else if ( qAbs(frameRate - 24) < FLOAT_EPSILON ) {
        index = 1;
    }
    else if ( qAbs(frameRate - 25) < FLOAT_EPSILON ) {
        index = 2;
    }
    else if ( qAbs(frameRate - 29.97) < FLOAT_EPSILON ) {
        index = 3;
    }
    else if ( qAbs(frameRate - 30) < FLOAT_EPSILON) {
        index = 4;
    }
    else {
        index = 5;
        ui->frameRateSpinBox->setEnabled(true);
        ui->frameRateSpinBox->setValue(frameRate);
    }

    mSettingsChangedBySoft = true;

    ui->frameRateComboBox->setCurrentIndex(index);

    mCurrentProperties.setFrameRate(frameRate);

    qApp->setProperty("prop_FrameRate_fps", frameRate);
}

Properties Settings::getCurrentProp() {
    return mCurrentProperties;
}

//*************************************************//
//               Class Properties                //
//*************************************************//

Properties::Properties() {
    mFrameRate = 0.0;
}

void Properties::setFrameRate(qreal frameRate) {
    mFrameRate = frameRate;
}

qreal Properties::frameRate() {
    return mFrameRate;
}
