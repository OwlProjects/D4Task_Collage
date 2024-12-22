#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define NON_WORKING_COLLAGE

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFutureWatcher>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionViewShowWindowImages_triggered();

    void on_actionViewShowWindowSettings_triggered();

private: // fields
    Ui::MainWindow *ui;
    QList<QPixmap>  m_photos;
    QGraphicsScene* m_graphicsScene;
    QFutureWatcher<void>* m_collageWatcher;

private: // functions
#ifndef NON_WORKING_COLLAGE
    void previewCollage();
#endif
};
#endif // MAINWINDOW_H
