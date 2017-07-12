#include "widget.h"
#include "ui_widget.h"
#include "../tableprinter.h"

#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>

// ----------------- example of inharitance from PagePrepare ---------------------

class PrintBorder : public PagePrepare {
public:
    virtual void preparePage(QPainter *painter);
    static int pageNumber;
};

int PrintBorder::pageNumber = 0;

void PrintBorder::preparePage(QPainter *painter) { // print a border on each page
    QRect rec = painter->viewport();
    painter->setPen(QPen(QColor(0, 0, 0), 1));
    painter->drawRect(rec);
    painter->translate(10, painter->viewport().height() - 10);
    painter->drawText(0, 0, QString("Page %1").arg(pageNumber));
    pageNumber += 1;
}

// --------------------------------------------------------------------------------

Widget::Widget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Widget) {
    ui->setupUi(this);
    initDb();
    model = new QSqlTableModel;
    model->setTable("exampleTable");
    model->select();
    ui->tableView->setModel(model);
}

Widget::~Widget() {
    delete ui;
    delete model;
}

void Widget::print(QPrinter *printer) {

    // ------------------ simplest example --------------------------

    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << "can't start printer";
        return;
    }
    // print table
    TablePrinter tablePrinter(&painter, printer);
    QVector<int> columnStretch = QVector<int>() << 2 << 5 << 10 << 15;
    if(!tablePrinter.printTable(ui->tableView->model(), columnStretch)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();
}

void Widget::print_two_tables(QPrinter *printer) {

    // ------------------ two tables example --------------------------

    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << "can't start printer";
        return;
    }
    // print table
    TablePrinter tablePrinter(&painter, printer);
    QVector<int> columnStretch = QVector<int>() << 2 << 5 << 10 << 15;
    if(!tablePrinter.printTable(ui->tableView->model(), columnStretch)) {
        qDebug() << tablePrinter.lastError();
    }
    // print second table
    painter.translate(0, 100);
    if(!tablePrinter.printTable(ui->tableView->model(), columnStretch)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();
}

void Widget::uglyPrint(QPrinter *printer) {

    // ---------------- death-to-designers example ------------------

    QPainter uglyPainter;
    if(!uglyPainter.begin(printer)) {
        qWarning() << "can't start printer";
        return;
    }
    TablePrinter uglyTablePrinter(&uglyPainter, printer);
    QVector<int> colStretch = QVector<int>() << 5 << 5 << 0 << 10;
    uglyTablePrinter.setPen(QPen(QColor(0, 100, 255), 3, Qt::DotLine)); // pen for borders
    uglyTablePrinter.setHeaderColor(Qt::red);
    uglyTablePrinter.setContentColor(Qt::green);
    QFont font1; // font for headers
    font1.setBold(true);
    QFont font2; // font for content
    font2.setItalic(true);
    uglyTablePrinter.setHeadersFont(font1);
    uglyTablePrinter.setContentFont(font2);
    PrintBorder *printB = new PrintBorder;
    printB->pageNumber = 1;
    uglyTablePrinter.setPagePrepare(printB);
    QVector<QString> headers = QVector<QString>() << "Header 1" << "Header 2" << "Header 3" << "Header 4";
    uglyPainter.setPen(QPen(Qt::yellow));
    uglyPainter.drawText(uglyPainter.viewport().width()/2 - 40, 40, "TABLE NAME");
    uglyPainter.translate(0, 60); // start print point
    uglyTablePrinter.setCellMargin(10, 5, 5, 5);
    uglyTablePrinter.setPageMargin(100, 40, 40, 40);
    if(!uglyTablePrinter.printTable(ui->tableView->model(), colStretch, headers)) {
        qDebug() << uglyTablePrinter.lastError();
    }
    uglyPainter.end();
    delete printB;
}

// ----------------------- create and populate the table ------------
void Widget::initDb() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("exampleDb");
    if(!db.open()) {
        qWarning() << "can't open db" << db.lastError().text();
    }
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS exampleTable(Column_0 INT, Column_1 TEXT, Column_2 TEXT, Column_3 TEXT);");
    query.exec("DELETE FROM exampleTable;");
    QSqlDatabase::database().transaction();
    for(int i = 1; i < 20; i++) {
        query.exec(QString("INSERT INTO exampleTable VALUES (%1, '%2', '%3', '%4');")
                   .arg(i).arg("text in column 1").arg("text in column 2").arg("text in column 3"));
    }
    query.exec(QString("INSERT INTO exampleTable VALUES(%1, '%2', '%3', '%4');")
               .arg(20).arg("long text results in column with big height one two three four five six seven eigth nine ten")
               .arg("text in column 2").arg("text in column 3"));
    query.exec(QString("INSERT INTO exampleTable VALUES(%1, '%2', '%3', '%4');")
               .arg(21).arg("text in column 1")
               .arg("text in column 2")
               .arg("long text results in column with big height one two three four five six seven eigth nine ten"));
    for(int i = 22; i < 31; i++) {
        query.exec(QString("INSERT INTO  exampleTable VALUES(%1, '%2', '%3', '%4');")
                   .arg(i).arg("text in column 1").arg("text in column 2").arg("text in column 3"));
    }
    QSqlDatabase::database().commit();
}

void Widget::on_pushButton_2_clicked() {
    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(uglyPrint(QPrinter*)));
    dialog.exec();
}

void Widget::on_pushButton_clicked() {
    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print(QPrinter*)));
    dialog.exec();
}

void Widget::on_pushButton_3_clicked() {
    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print_two_tables(QPrinter*)));
    dialog.exec();
}
