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
    image = new QImage(ui->qrLabel->width(), ui->qrLabel->height(), QImage::Format_ARGB32);
    connect(ui->genBtn, SIGNAL(clicked()), this, SLOT(genBtnTriggered()));
    connect(ui->saveBtn, SIGNAL(clicked()), this, SLOT(saveBtnTriggered()));
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

void Widget::genBtnTriggered(void)
{
    QRcode *qrcode = NULL;
    QString text;
    QByteArray ba;
    char *text_str;
    int margin = 10;
    QPixmap pixmap;

    text = ui->textEdit->text();
    if (text.isNull() || text.isEmpty()) {
        QMessageBox::information(this, tr("Error"), tr("Please input serial number!"));
        return ;
    }

    ba = text.toLatin1();
    text_str = ba.data();

    qrcode = QRcode_encodeString(text_str, 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    if(qrcode != NULL) {
        QPainter painter(image);
        unsigned char *point = qrcode->data;
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawRect(0, 0, ui->qrLabel->width(), ui->qrLabel->height());
        double scale = (ui->qrLabel->width() - 2.0 * margin) / qrcode->width;
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
    }
    qrcode = NULL;

    pixmap = QPixmap::fromImage(*image);
    ui->qrLabel->setPixmap(pixmap);
}
