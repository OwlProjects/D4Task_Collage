#include "ImagePixmap.h"

#include <QFileInfo>

ImagePixmap::ImagePixmap(const QString& fileName):
    m_fileName(fileName), m_pixmap(QPixmap(fileName))
{

}

ImagePixmap::ImagePixmap(const ImagePixmap& pw):
    m_fileName(pw.fileName()), m_pixmap(pw.pixmap())
{

}

ImagePixmap::ImagePixmap()
{

}

void ImagePixmap::setFileName(const QString& fileName)
{
    m_fileName = fileName;
}

void ImagePixmap::setPixmap(const QPixmap& pixmap)
{
    m_pixmap = pixmap;
}

ImagePixmap &ImagePixmap::operator=(const ImagePixmap &pixmap)
{
    setPixmap(pixmap.pixmap());
    setFileName(pixmap.fileName());
    return *this;
}
