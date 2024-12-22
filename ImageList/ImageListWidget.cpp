#include "ImageListWidget.h"
#include "ui_ImageListWidget.h"
#include "Tools/ImageUtils.h"
#include "ImagePixmap.h"
#include "PixmapListWidgetItem.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QtConcurrent>
#include <QMessageBox>

ImageListWidget::ImageListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ImageListWidget)
{
    ui->setupUi(this);

    m_imagesWatcher = new QFutureWatcher<ImagePixmap>(this);


    // m_progressDialog = new QProgressDialog(this);
    // m_progressDialog->cancel();
    // m_progressDialog->setWindowModality(Qt::WindowModal);

    connect(m_imagesWatcher, &QFutureWatcher<ImagePixmap>::resultReadyAt,
            this, &ImageListWidget::photoLoadedAt);
    connect(m_imagesWatcher, &QFutureWatcher<ImagePixmap>::finished,
            this, &ImageListWidget::addPhotosFinished);
    // connect(_photoAddingWatcher,SIGNAL(progressValueChanged(int)),_progressDialog,SLOT(setValue(int)));
    // connect(_progressDialog,SIGNAL(canceled()),_photoAddingWatcher,SLOT(cancel()));
    // connect(m_imagesWatcher, &QFutureWatcher<ImagePixmap>::progressValueChanged,
    //         m_progressDialog, &QProgressDialog::setValue);
    // connect(_progressDialog, &QProgressDialog::canceled,
    //         m_imagesWatcher, &QFutureWatcher<ImagePixmap>::cancel);
    // connect(m_imagesWatcher, &QFutureWatcher<ImagePixmap>::finished,
    //         m_progressDialog, &QProgressDialog::deleteLater);

}

ImageListWidget::~ImageListWidget()
{
    delete ui;

    m_imagesWatcher->cancel();
    m_imagesWatcher->waitForFinished();
}

void ImageListWidget::on_pushButtonAddImages_clicked()
{
    QString filter = ImageUtils::supportedImageFormatsFilter();
    QStringList imagesFileNames = QFileDialog::getOpenFileNames(
        this, "Выберите изображения",
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), filter);
    if (!imagesFileNames.isEmpty()) {
        addImages(imagesFileNames);
    }
}

void ImageListWidget::photoLoadedAt(int n)
{
    addImage(m_imagesWatcher->resultAt(n));
}

bool ImageListWidget::addImage(const ImagePixmap &imgPxm)
{
    if(imgPxm.pixmap().isNull())
    {
        m_nonAddedImagesList << "\t<b>" << imgPxm.fileName() << "</b>";
        return false;
    }

    if(m_addedImagesList.contains(imgPxm.fileName()))
        return true;
    else
        m_addedImagesList<<imgPxm.fileName();

    ui->listWidgetImages->addItem(new PixmapListWidgetItem(imgPxm));

    // ui->labelNbPhotos->setText(tr("%n Photo(s)","",ui->listWidgetPhotos->count()));

    return true;
}

void ImageListWidget::addImages(const QStringList &fileNames)
{
    int imgCnt = fileNames.count();

    setProgressDialog("Добавление...", "Отмена", 0, imgCnt);

    if  (ui->comboBoxMethod->currentText() == "QtConcurrent") {

        m_imagesWatcher->setFuture(QtConcurrent::mapped(fileNames, &ImageListWidget::loadImage));

    } else {

        for (int i = 0; i < imgCnt; i++)
        {
            // m_progressDialog->setValue(i);
            // if (_progressDialog->wasCanceled()) {
            //     break;
            // }
            addImage(loadImage(fileNames[i]));
            qApp->processEvents();
        }

        // m_progressDialog->setValue(imgCnt);
    }
}

ImagePixmap ImageListWidget::loadImage(const QString &fileName)
{
    return ImagePixmap(fileName);
}

void ImageListWidget::addPhotosFinished()
{
    ui->pushButtonAddImages->setEnabled(true);

    if (!m_nonAddedImagesList.isEmpty()) {
        QMessageBox::warning(this, "Список изображений",
                             QString("Следующие изображения не добавились:\n<b>%1</b>").arg(m_nonAddedImagesList.join("\n")));
    }

    m_nonAddedImagesList.clear();
    setListStateEmpty(ui->listWidgetImages->count() == 0);
}

void ImageListWidget::setProgressDialog(const QString & labelText, const QString & cancelButtonText, int minimum, int maximum)
{
    // m_progressDialog->setLabelText(labelText);
    // m_progressDialog->setCancelButtonText(cancelButtonText);
    // m_progressDialog->setRange(minimum, maximum);
}

void ImageListWidget::setListStateEmpty(bool isEmpty)
{
    ui->pushButtonDeleteImage->setDisabled(isEmpty);
    ui->pushButtonDeleteImage->setDisabled(isEmpty);
}

void ImageListWidget::on_pushButtonAddResImages_clicked()
{
    QStringList imagesFileNames;
    QDirIterator it(":/res/Ryazan/");
    while (it.hasNext()) {
        imagesFileNames << it.next();
    }
    if (!imagesFileNames.isEmpty()) {
        addImages(imagesFileNames);
    }
}

QList<QPixmap> ImageListWidget::getPhotos() const
{
    QList<QPixmap> photos;
    for(int i = 0; i < ui->listWidgetImages->count(); i++)
    {
        PixmapListWidgetItem* item = static_cast<PixmapListWidgetItem*>(ui->listWidgetImages->item(i));
        photos << item->pixmap();

    }

    return photos;

}

void ImageListWidget::on_pushButtonDeleteImage_clicked()
{
    removeSelectedImages();
}

void ImageListWidget::removeSelectedImages()
{
    int nbPhotos = ui->listWidgetImages->selectedItems().count();
    QList<QListWidgetItem*> selectedPhotos = ui->listWidgetImages->selectedItems();

    for (int i = 0; i < nbPhotos; i++)
    {
        // m_progressDialog->setValue(i);

        // if (m_progressDialog->wasCanceled()) break; //removing photos cancelled

        PixmapListWidgetItem * imageItem = static_cast<PixmapListWidgetItem*>(selectedPhotos.at(i));
        m_addedImagesList.removeOne(imageItem->fileName());

        delete ui->listWidgetImages->takeItem(ui->listWidgetImages->row(imageItem));
    }

    // m_progressDialog->setValue(nbPhotos);

    setListStateEmpty(ui->listWidgetImages->count() == 0);
}

void ImageListWidget::removeAllImages()
{
    if(QMessageBox::question(this,tr("Список изображений"),
                              tr("Удалить всё?"),
                              QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        m_addedImagesList.clear();
        ui->listWidgetImages->clear();
        setListStateEmpty(true);
    }
}

void ImageListWidget::on_pushButtonDeleteAllImages_clicked()
{
    removeAllImages();
}

