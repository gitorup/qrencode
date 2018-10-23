#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QPainter>

#include "qrencode.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle(tr("QRtool"));

    ui->qrLabel->clear();
    margin = 10;
    qrcode_rate = 0;
    image = new QImage(ui->qrLabel->width(), ui->qrLabel->height(), QImage::Format_ARGB32);
    connect(ui->genBtn, SIGNAL(clicked()), this, SLOT(genBtnTriggered()));
    connect(ui->saveBtn, SIGNAL(clicked()), this, SLOT(saveBtnTriggered()));
    connect(ui->rateComBox, SIGNAL(currentTextChanged(QString)), this, SLOT(rateChanged(QString)));
    connect(ui->sizeComBox, SIGNAL(currentTextChanged(QString)), this, SLOT(sizeChanged(QString)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::saveBtnTriggered(void)
{
    QString filename;

    filename = QFileDialog::getSaveFileName(this, tr("Save Image"),
        "", tr("Images (*.png *.bmp *.jpg)"));
    if (!filename.isEmpty()) {
        image->save(filename);
    }
}

void Widget::genQrcodeImage(char *qr_str, int width, int height)
{
    QRcode *qrcode = NULL;

    if (qr_str == NULL || width <= 0 || height <= 0)
        return ;

    qrcode = QRcode_encodeString(qr_str, qrcode_rate, QR_ECLEVEL_L, QR_MODE_8, 1);
    if(qrcode != NULL) {
        QPainter painter(image);
        unsigned char *point = qrcode->data;
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawRect(0, 0, width, height);
        double scale = (width - 2.0 * margin) / qrcode->width;
        painter.setBrush(Qt::black);
        for (int y = 0; y < qrcode->width; y++) {
            for (int x = 0; x < qrcode->width; x++) {
                if (*point & 1) {
                    QRectF r(margin + x * scale, margin + y * scale, scale, scale);
                    painter.drawRects(&r, 1);
                }
                point++;
            }
        }
        point = NULL;
        QRcode_free(qrcode);
        qrcode = NULL;
    }
}

void Widget::showQrcodeImage(void)
{
    QString text;
    QByteArray ba;
    char *text_str;
    QPixmap pixmap;
    text = ui->textEdit->text();
    if (text.isNull() || text.isEmpty()) {
        QMessageBox::information(this, tr("Error"), tr("Please input serial number!"));
        return ;
    }

    ba = text.toLatin1();
    text_str = ba.data();

    genQrcodeImage(text_str, ui->qrLabel->width(), ui->qrLabel->height());
    pixmap = QPixmap::fromImage(*image);
    if (pixmap.isNull()) {
        QMessageBox::information(this, tr("Error"), tr("Generate qrcode failed!"));
        return ;
    }

    ui->qrLabel->setPixmap(pixmap);
}

void Widget::genBtnTriggered(void)
{
    showQrcodeImage();
}

void Widget::sizeChanged(QString index)
{
    qDebug() << "sizeChanged: " << index;
    if (index.compare(index, "100%") == 0) {
        margin = 10;
    } else if (index.compare(index, "75%") == 0) {
        margin = 25;
    } else if (index.compare(index, "50%") == 0) {
        margin = 50;
    } else if (index.compare(index, "25%") == 0) {
        margin = 75;
    } else {
        margin = 10;
    }

    showQrcodeImage();
}

void Widget::rateChanged(QString index)
{
    qDebug() << "rateChanged: " << index;
    if (index.compare(index, "70%") == 0) {
        qrcode_rate = 3;
    } else if (index.compare(index, "50%") == 0) {
        qrcode_rate = 2;
    } else if (index.compare(index, "30%") == 0) {
        qrcode_rate = 1;
    } else {
        qrcode_rate = 1;
    }
    showQrcodeImage();
}
