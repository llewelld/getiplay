#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>

class Control : public QObject
{
    Q_OBJECT

    // Configuration properties
    Q_PROPERTY (bool viewTv READ getViewTv WRITE setViewTv NOTIFY viewTvChanged)

public:
    explicit Control(QObject *parent = 0);
    bool getViewTv() const;

private:
    bool viewTv;
    void settingsSetValue(QString key, bool value);

signals:
    void viewTvChanged(bool viewTv);

public slots:
    void setViewTv(bool value);
};

#endif // CONTROL_H
