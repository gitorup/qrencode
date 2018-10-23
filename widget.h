#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QFileInfo>
#include <QString>
#include <QPaintEvent>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QImage *image;
    int margin;
    int qrcode_rate;
    void showQrcodeImage(void);
    void genQrcodeImage(char *qr_str, int width, int height);

public slots:
    void genBtnTriggered(void);
    void saveBtnTriggered(void);
    void rateChanged(QString index);
    void sizeChanged(QString index);
};

#endif // WIDGET_H
