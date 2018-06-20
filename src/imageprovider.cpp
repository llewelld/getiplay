#include <sailfishapp.h>
#include <QQuickImageProvider>
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <mlite5/MGConfItem>

#include "imageprovider.h"

ImageProvider::ImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {
    QScopedPointer<MGConfItem> ratioItem(new MGConfItem("/desktop/sailfish/silica/theme_pixel_ratio"));
    pixelRatio = ratioItem->value(1.0).toDouble();
    QString dir;
    if (pixelRatio > 1.75) {
        dir = "2.0";
    }
    else if (pixelRatio > 1.5) {
        dir = "1.75";
    }
    else if (pixelRatio > 1.25) {
        dir = "1.5";
    }
    else if (pixelRatio > 1.0) {
        dir = "1.25";
    }
    else {
        dir = "1.0";
    }

    imageDir = SailfishApp::pathTo("qml/images/z" + dir).toString(QUrl::RemoveScheme) + "/";
    qDebug() << "Image folder: " << imageDir;
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {
    QPixmap image;
    QStringList parts = id.split('?');
    QPixmap sourcePixmap(imageDir + parts.at(0) + ".png");

    if (size) {
        *size = sourcePixmap.size();
    }
    if (parts.length() > 1) {
        if (QColor::isValidColor(parts.at(1))) {
            QPainter painter(&sourcePixmap);
            painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            painter.fillRect(sourcePixmap.rect(), parts.at(1));
            painter.end();
        }
    }
    if (requestedSize.width() > 0 && requestedSize.height() > 0) {
        image = sourcePixmap.scaled(requestedSize.width(), requestedSize.height(), Qt::IgnoreAspectRatio);
    }
    else {
        image = sourcePixmap;
    }

    return image;
}
