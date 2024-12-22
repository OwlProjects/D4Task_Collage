#include "PixmapListWidgetItem.h"
#include "Tools/ImageUtils.h"

#include <QFileInfo>

PixmapListWidgetItem::PixmapListWidgetItem(const ImagePixmap& imgPxm, QListWidget *parent):
    QListWidgetItem(parent), _imgPxm(imgPxm)
{
    _imgPxm = imgPxm;
    setIcon(QIcon(imgPxm.pixmap()));
    setText(QFileInfo(imgPxm.fileName()).baseName());
    setToolTip(ImageUtils::imageInfo(imgPxm.pixmap(), imgPxm.fileName()));
}

void PixmapListWidgetItem::setFileName(const QString &fileName)
{
    _imgPxm.setFileName(fileName);
}

void PixmapListWidgetItem::setPixmap(const QPixmap &image)
{
    _imgPxm.setPixmap(image);
}

