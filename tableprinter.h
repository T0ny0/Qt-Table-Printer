#ifndef TABLEPRINTER_H
#define TABLEPRINTER_H

#include <QPen>
#include <QFont>

class QPrinter;
class QPainter;
class QAbstractItemModel;

/**
 * @brief The PagePrepare Abstract class - base class for
 * classes that will print something like headers, borders...
 * on each page with table
 */
class PagePrepare {
public:
    virtual void preparePage(QPainter *painter) = 0;
    virtual ~PagePrepare() {}
};

class TablePrinter
{
public:
  TablePrinter(QPainter *painter, QPrinter *printer);
  bool printTable(const QAbstractItemModel* model, const QVector<int> columnStretch, const QVector<QString> headers = QVector<QString>());
  QString lastError();
  void setCellMargin(int left = 10, int right = 5, int top = 5, int bottom = 5);
  void setPageMargin(int left = 50, int right = 20, int top = 20, int bottom = 20);
  void setPagePrepare(PagePrepare *prepare);
  void setPen(QPen pen);
  void setHeadersFont(QFont font);
  void setContentFont(QFont font);
  void setHeaderColor(QColor color);
  void setContentColor(QColor color);
private:
  QPainter *painter;
  QPrinter *printer;
  PagePrepare *prepare;
  QPen pen; // for table borders
  QFont headersFont;
  QFont contentFont;
  QColor headerColor;
  QColor contentColor;
  // cell margins
  int topMargin;
  int bottomMargin;
  int leftMargin;
  int rightMargin;

  // margins for table
  int headerHeight;
  int bottomHeight;
  int leftBlank;
  int rightBlank;

  int MaxRowHeight;

  QString error;
};

#endif // TABLEPRINTER_H
