#ifndef IMAGELISTWIDGET_H
#define IMAGELISTWIDGET_H

#include "ImagePixmap.h"

#include <QWidget>
#include <QFutureWatcher>
#include <QProgressDialog>

namespace Ui {
class ImageListWidget;
}

class ImageListWidget : public QWidget
{
    Q_OBJECT

public: // c-tors/d-tors
    explicit ImageListWidget(QWidget *parent = nullptr);
    ~ImageListWidget();

private slots:
    void on_pushButtonAddImages_clicked();

    void on_pushButtonAddResImages_clicked();

    void on_pushButtonDeleteImage_clicked();

    void on_pushButtonDeleteAllImages_clicked();

public: // functions
    static ImagePixmap loadImage(const QString& fileName);
    QList<QPixmap> getPhotos() const;

private: // fields
    Ui::ImageListWidget *ui;
    QFutureWatcher<ImagePixmap>* m_imagesWatcher;
    QStringList m_addedImagesList;
    QStringList m_nonAddedImagesList;
    QProgressDialog* m_progressDialog;

private: // functions
    void addImages(const QStringList& fileNames);
    bool addImage(const ImagePixmap &imgPxm);
    void photoLoadedAt(int n);
    void addPhotosFinished();
    void setProgressDialog(const QString & labelText, const QString & cancelButtonText, int minimum, int maximum);
    void setListStateEmpty(bool isEmpty);
    void removeSelectedImages();
    void removeAllImages();

};

#endif // IMAGELISTWIDGET_H
