#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <QObject>

class ImageUtils
{
public:
    static QString      supportedImageFormatsFilter();
    static QStringList  supportedImageFormats();
    static QString      imageInfo(const QPixmap & image, const QString &fileName);
};

#endif // IMAGEUTILS_H
