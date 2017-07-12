# Qt Table Printer
## About
TablePainter is a simple class for qt-based applications that can print
all kinds of tables inherited from QAbstractItemModel such QSqlTableModel,
QSqlQueryModel, QTableWidget (using QTableWidget::model() method) or your custom
table model. TablePainter works using QPrinter class.

## Features
 * border style
 * table margins
 * cell margins
 * column width
 * column headers
 * column headers font and color
 * content font and color
 * printing of standart elements on each page containing table (page number, border, service information etc.)
 * maximum row height

## Usage
The simplest usage may look like:

    QPrinter printer
    // printer setup
    QPainter painter;
    painter.begin(&printer);
    TablePrinter tablePrinter(&painter, &printer);
    QVector<int> columnStretch = QVector<int>() << 1 << 5 << 5 << 5;
    tablePrinter.printTable(model, columnStretch);
    painter.end();
Where model is a child of QAbstractItemModel you want to print.
TablePrinter starts to print table from point (tablePrinter.leftblank, current_Y_position) so if you have some content above the table you need to set initial vertical position with

    painter.translate(0, dy);
After `printTable()` call `painter.transform.dx()` equals to `painter.transform.dx()` before printing, and `painter.transform.dy()`
equals to `y` coordinate of last horizontal line of printed table.
If wrong data was passed to TablePrinter then table will not be printed and printTable returns false. You can get an error description from TablePrinter::lastError() method.

    if(!tablePrinter.printTable(model, columnStretch)) {
        qDebug() << tablePrinter.lastError();
    }
If you need to print some data on each page, like page numbers, you need to create new class inherited from PagePrepare and reimplement preparePage(QPainter *painter) function.

You can find two examples of TablePrinter usage (one of which demonstrates applying of all TablePrinter settings but looks pretty wierd) in /example catalog.
## License
[BSD License](./LICENSE)