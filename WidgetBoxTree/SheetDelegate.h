#pragma once

#include <QtWidgets/QStyledItemDelegate>

class QTreeView;

class SheetDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	SheetDelegate(QTreeView* view, QWidget* parent);
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& index) const override;
private:
	QTreeView* m_view;
};

