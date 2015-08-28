#ifndef PROGRAMME_H
#define PROGRAMME_H

#include <QObject>

class Programme
{
public:
    Programme();
    Programme(unsigned int progId, QString name, float length);

    QString getName () const;
    float getLength () const;
    unsigned int getProgId () const;

    void setName (const QString &value);
    void setLength (float value);
    void setProgId (unsigned int value);

private:
    QString name;
    float length;
    unsigned int progId;
};


#endif // PROGRAMME_H
