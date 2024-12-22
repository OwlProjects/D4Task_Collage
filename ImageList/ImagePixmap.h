#ifndef IMAGEPIXMAP_H
#define IMAGEPIXMAP_H

#include <QPixmap>

class ImagePixmap
{

public: // c-tors/d-tors
    ImagePixmap(const QString& fileName);
    ImagePixmap(const ImagePixmap& pw);
    ImagePixmap();

public: // functions
    QString fileName()  const   {return m_fileName;}
    QPixmap pixmap()   const   {return m_pixmap;}
    void setFileName(const QString &fileName);
    void setPixmap(const QPixmap &image);

    ImagePixmap& operator=(const ImagePixmap& pixmap);

private: // fields
    QString m_fileName;
    QPixmap m_pixmap;

};

#endif // IMAGEPIXMAP_H
