#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_graphicsScene = new QGraphicsScene(this);
    m_collageWatcher = new QFutureWatcher<void>(this);

    connect(ui->dockWidgetImages, &QDockWidget::visibilityChanged,
            ui->actionViewShowWindowImages, &QAction::setChecked);
    connect(ui->dockWidgetSettings, &QDockWidget::visibilityChanged,
            ui->actionViewShowWindowSettings, &QAction::setChecked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionViewShowWindowImages_triggered()
{
    ui->actionViewShowWindowImages->isChecked()
        ? ui->dockWidgetImages->show()
        : ui->dockWidgetImages->hide();
}


void MainWindow::on_actionViewShowWindowSettings_triggered()
{
    ui->actionViewShowWindowSettings->isChecked()
        ? ui->dockWidgetSettings->show()
        : ui->dockWidgetSettings->hide();
}

#ifndef NON_WORKING_COLLAGE

//Previews the collage
void MainWindow::previewCollage()
{
    _photos = ui->imageListWidget->getPhotos();

    if(_photos.isEmpty())
    {
        QMessageBox::critical(this,tr("Коллаж"),tr("Нет добавленных изображений"),
                              QMessageBox::Ok);
        return ;
    }


    // if(!_mainWindow->getCollageParameters())
    // {
    //     return;
    // }

    //we get the photos

     m_graphicsScene->clear();


    if(_collageSizeAuto)
    {

        foreach(QPixmap photo, _photos)
        {
            if(photo.width()>_collageSize.width())
                _collageSize.setWidth(photo.width());

            if(photo.height()>_collageSize.height())
                _collageSize.setHeight(photo.height());
        }
        _collageSize += QSize(30,30);
    }


    ui->labelCollageSize->setVisible(true);
    ui->labelCollageNbPhotos->setVisible(true);

    ui->labelCollageNbPhotos->setText( tr("Number of Photos : %1").arg(_photos.count()));
    ui->labelCollageSize->setText(tr("Collage Size: %1 x %2").arg(_collageSize.width()).arg(_collageSize.height()));

     m_graphicsScene->setSceneRect(0,0,_collageSize.width(),_collageSize.height());


    switch(_background)
    {
    case Color:
    case Transparent:    m_graphicsScene->setBackgroundBrush(QBrush(_bgColor));break;
    default :            m_graphicsScene->addPixmap(_bgPixmap);
    }

    ui->labelCollageProgression->setText(tr("Analyzing photos..."));
    ui->progressBarCollageCreation->setRange(1, _nbPhotos);


    QList<GraphicsPixmapItem *> items;
    for (int i = 0; i < _nbPhotos; ++i)
    {
        QPixmap photo =  toCollagePhoto(_photos[i % _photos.count()]); //we use '%' as _nbPhotos >= _photos.count()
        photo = photo.scaled(_photoSize);
        GraphicsPixmapItem *item = new GraphicsPixmapItem(photo);
        item->setOffset(-photo.width(), -photo.height());
        item->setZValue(i);
        items << item;
         m_graphicsScene->addItem(item);

        ui->progressBarCollageCreation->setValue(i);
    }


    QParallelAnimationGroup *animGroup = new QParallelAnimationGroup;
    QPainterPath shapePath;


    int xOffset = _photoSize.width();
    int yOffset = _photoSize.height();


    switch(_shape)
    {

    case Random:
    case Extra:
    case Rectangle: shapePath.addRect(xOffset,yOffset,_collageSize.width()-xOffset,_collageSize.height()-yOffset);break;
    case Circle:    shapePath.addEllipse(xOffset,yOffset,_collageSize.width()-xOffset,_collageSize.height()-yOffset);break;
    case Text:      shapePath.addText(xOffset,_collageSize.height()/2,_font,_text);break;

    case Heart:     shapePath.moveTo(QPoint(_collageSize.width()/2,_collageSize.height()/3));
        shapePath.lineTo(QPoint(xOffset,yOffset));
        shapePath.lineTo(QPoint(_collageSize.width()/2,_collageSize.height()-yOffset));
        shapePath.lineTo(QPoint(_collageSize.width()-xOffset,yOffset));
        shapePath.lineTo(QPoint(_collageSize.width()/2,_collageSize.height()/3));
        break;

    }


    qreal t = 0;
    for (int i = 0; i < items.count(); ++i)
    {
        QPropertyAnimation *anim = new QPropertyAnimation(items[i], "pos");
        QPropertyAnimation *anim2 = new QPropertyAnimation(items[i], "rotation");


        anim->setEasingCurve(QEasingCurve::InOutBack);
        anim->setDuration(750 + i * 25);
        anim->setStartValue(QPointF());
        anim->setEndValue( shapePath.pointAtPercent(t));


        anim2->setEasingCurve(QEasingCurve::InOutBack);
        anim2->setDuration(750 + i * 25);
        anim2->setStartValue(0);


        QRandomGenerator gen;
        anim2->setEndValue(-20 + gen.bounded(1, 100)%20);
        // anim2->setEndValue(-20 + qrand()%20);


        t += 1.0/_nbPhotos;

        animGroup->addAnimation(anim);
        animGroup->addAnimation(anim2);

        ui->graphicsView->fitInView(_graphicsScene->sceneRect(), Qt::KeepAspectRatio);
    }

    animGroup->start();

}

#endif


