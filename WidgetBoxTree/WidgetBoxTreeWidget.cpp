#include "WidgetBoxTreeWidget.h"
#include "SheetDelegate.h"

#include <QtGui/QResizeEvent>
#include <QtWidgets/QProxyStyle>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QApplication>

#include <cmath>

class Style_tweaks : public QProxyStyle
{
public:
	void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
		QPainter* painter, const QWidget* widget) const
	{
		if (element == QStyle::PE_FrameFocusRect) return;

		QProxyStyle::drawPrimitive(element, option, painter, widget);
	}
};

WidgetBoxTreeWidget::WidgetBoxTreeWidget(QWidget* parent)
	: QTreeWidget(parent)
{
	setFocusPolicy(Qt::NoFocus);
	setIndentation(0);
	setRootIsDecorated(false);
	setColumnCount(1);
	header()->hide();
	header()->setSectionResizeMode(QHeaderView::Stretch);
	setTextElideMode(Qt::ElideMiddle);
	setVerticalScrollMode(ScrollPerPixel);
	setItemDelegate(new SheetDelegate(this, this));
	setStyle(new Style_tweaks);
	QObject::connect(this, &QTreeWidget::itemPressed, this, &WidgetBoxTreeWidget::onMousePress);
	QObject::connect(this, &QTreeWidget::itemCollapsed, this, &WidgetBoxTreeWidget::onExpand);
	QObject::connect(this, &QTreeWidget::itemExpanded, this, &WidgetBoxTreeWidget::onExpand);

	updateGeometries();
}

void WidgetBoxTreeWidget::onMousePress(QTreeWidgetItem* item)
{
	if (item == nullptr)
		return;

	if (QApplication::mouseButtons() != Qt::LeftButton)
		return;

	if (item->parent() == nullptr)
	{
		item->setExpanded(!item->isExpanded());
		return;
	}
}

void WidgetBoxTreeWidget::expand_item(const int& index)
{
	topLevelItem(index)->setExpanded(true);
}

void WidgetBoxTreeWidget::add_item(const QString& name, QWidget* wgt)
{
	auto* cat_item = new QTreeWidgetItem();
	cat_item->setText(0, name);
	const auto index = topLevelItemCount();
	insertTopLevelItem(index, cat_item);
	cat_item->setExpanded(false);
	cat_item->setSizeHint(0, QSize(cat_item->sizeHint(0).width(), 30));

	auto* child_item = new QTreeWidgetItem();

	cat_item->addChild(child_item);

	setItemWidget(child_item, 0, wgt);

	if (index == 0)
		size_hints.append(wgt->sizeHint().height());
	else
		size_hints.append(wgt->sizeHint().height());
}

void WidgetBoxTreeWidget::update_size(int new_size, const bool& change_expand)
{
	resizeColumnToContents(0);
	doItemsLayout();

	new_size -= 32 * topLevelItemCount();

	int summ = 0;
	for (int i = 0, isize = topLevelItemCount(); i < isize; i++)
	{
		auto* cat_item = topLevelItem(i);
		if (!cat_item->isExpanded())
			continue;

		auto* child_item = cat_item->child(0);
		auto* widget = itemWidget(child_item, 0);
		if (widget == nullptr)
			continue;

		const auto hh = size_hints.at(i);

		summ += hh;
	}

	if (!change_expand && (std::abs(new_size - summ) < 2))
		return;

	if (new_size > summ)
	{
		const double ratio = double(new_size) / double(summ);

		for (int i = 0, isize = topLevelItemCount(); i < isize; i++)
		{
			auto* cat_item = topLevelItem(i);

			auto* child_item = cat_item->child(0);
			auto* widget = itemWidget(child_item, 0);
			if (widget == nullptr)
				continue;

			if (cat_item->isExpanded())
			{
				auto hh = size_hints.at(i);
				auto nh = std::floor(double(hh) * ratio);
				widget->setFixedHeight(nh);
			}
		}
	}
	else
	{
		const double ratio = double(new_size) / double(summ);

		for (int i = 0, isize = topLevelItemCount(); i < isize; i++)
		{
			auto* cat_item = topLevelItem(i);

			auto* child_item = cat_item->child(0);
			auto* widget = itemWidget(child_item, 0);
			if (widget == nullptr)
				continue;

			if (cat_item->isExpanded())
			{
				auto hh = size_hints.at(i);
				//auto nh = std::floor(double(hh) * ratio);
				widget->setFixedHeight(hh);
			}
		}
	}

	resizeColumnToContents(0);
	doItemsLayout();
}

void WidgetBoxTreeWidget::resizeEvent(QResizeEvent* event)
{
	const auto new_size = event->size().height();
	update_size(new_size, false);
}

void WidgetBoxTreeWidget::onExpand(QTreeWidgetItem* item)
{
	if (item == nullptr)
		return;

	if (item->parent() == nullptr)
	{
		const auto new_size = height();
		update_size(new_size, true);
		return;
	}
}