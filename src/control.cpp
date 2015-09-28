#include "control.h"
#include <QSettings>

Control::Control(QObject *parent) :
    QObject(parent),
    viewTv(false)
{
    QSettings settings;
    viewTv = settings.value("viewTv", true).toBool();
}

bool Control::getViewTv() const {
    return viewTv;
}

void Control::setViewTv(bool value) {
    if (value != viewTv) {
        viewTv = value;
        settingsSetValue("viewTv", value);
        emit viewTvChanged(value);
    }
}

void Control::settingsSetValue(QString key, bool value) {
    QSettings settings;

    settings.setValue(key, value);
}
