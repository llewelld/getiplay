#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <sailfishapp.h>
#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider {
public:
    explicit ImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
private:
    double pixelRatio;
    QString imageDir;
};

#endif // IMAGEPROVIDER_H
