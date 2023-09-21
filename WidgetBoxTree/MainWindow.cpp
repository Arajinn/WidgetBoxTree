#include "MainWindow.h"

#include <QtWidgets/QTableWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass())
{
    ui->setupUi(this);

    {
        QTableWidget* widget = new QTableWidget(this);
        widget->setRowCount(5);
        widget->setColumnCount(5);
        widget->setMinimumSize(QSize(0, 0));
        widget->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
        ui->treeWidget->add_item("Table1", widget);
    }
    {
        QTableWidget* widget = new QTableWidget(this);
        widget->setRowCount(4);
        widget->setColumnCount(10);
        widget->setMinimumSize(QSize(0, 0));
        widget->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
        ui->treeWidget->add_item("Table2", widget);
    }
    {
        QTableWidget* widget = new QTableWidget(this);
        widget->setRowCount(2);
        widget->setColumnCount(2);
        widget->setMinimumSize(QSize(0, 0));
        widget->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
        ui->treeWidget->add_item("Table3", widget);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
