#ifndef PIXMAPLISTWIDGETITEM_H
#define PIXMAPLISTWIDGETITEM_H

#include "ImagePixmap.h"

#include <QPixmap>
#include <QListWidgetItem>

class PixmapListWidgetItem : public QListWidgetItem
{

public:
    PixmapListWidgetItem(const ImagePixmap& imgPxm, QListWidget *parent = 0);
    ~PixmapListWidgetItem(){}

    QString fileName()  const   {return _imgPxm.fileName();}
    QPixmap pixmap()   const   {return _imgPxm.pixmap();}

    void setFileName(const QString &fileName);
    void setPixmap(const QPixmap &pixmap);

private:
    ImagePixmap _imgPxm;

};


#endif // PIXMAPLISTWIDGETITEM_H
