#pragma once

#include <QtWidgets/QTreeWidget>

class WidgetBoxCategoryListView;

class WidgetBoxTreeWidget : public QTreeWidget
{
	Q_OBJECT
public:
	explicit WidgetBoxTreeWidget(QWidget* parent = nullptr);

	void add_item(const QString& name, QWidget* wgt);

	void expand_item(const int& index);
protected:
	void resizeEvent(QResizeEvent* event);
private:
	void update_size(int new_size, const bool& change_expand);

	QList<int> size_hints;
public slots:
	void onMousePress(QTreeWidgetItem* item);
	void onExpand(QTreeWidgetItem* item);
signals:
};

