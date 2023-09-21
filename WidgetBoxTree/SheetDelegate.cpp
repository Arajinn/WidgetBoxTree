#include "SheetDelegate.h"

#include <QtGui/QPainter>
#include <QtWidgets/QTreeView>

SheetDelegate::SheetDelegate(QTreeView* view, QWidget* parent)
	: QStyledItemDelegate(parent),
	m_view(view)
{
}

void SheetDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QAbstractItemModel* model = index.model();
	if (!model->parent(index).isValid())
	{
		QStyleOptionButton buttonOption;
		buttonOption.state = option.state;
		buttonOption.state &= ~QStyle::State_HasFocus;
		buttonOption.rect = option.rect;
		buttonOption.palette = option.palette;
		buttonOption.features = QStyleOptionButton::None;
		painter->save();

		QColor buttonColor(230, 230, 230);
		QBrush buttonBrush = option.palette.button();
		if (!buttonBrush.gradient() && buttonBrush.texture().isNull())
		{
			buttonColor = buttonBrush.color();
		}

		QModelIndex previousIndex = model->index(index.row() - 1, index.column());
		bool drawTopline = (index.row() == 0) || (index.row() > 0 && m_view->isExpanded(previousIndex));
		int highlightOffset = drawTopline ? 1 : 0;
		QLinearGradient gradient(option.rect.topLeft(), option.rect.bottomLeft());
		gradient.setColorAt(0, buttonColor.lighter(102));
		gradient.setColorAt(1, buttonColor.darker(106));
		painter->setPen(Qt::NoPen);
		painter->setBrush(gradient);
		painter->drawRect(option.rect);

		const QColor highlightColor = buttonColor.lighter(130);
		painter->setPen(highlightColor);
		painter->drawLine(option.rect.topLeft() + QPoint(0, highlightOffset),
			option.rect.topRight() + QPoint(0, highlightOffset));

		const QColor outlineColor = buttonColor.darker(150);
		painter->setPen(outlineColor);
		if (drawTopline)
		{
			painter->drawLine(option.rect.topLeft(), option.rect.topRight());
		}
		painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
		painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
		painter->drawLine(option.rect.topRight(), option.rect.bottomRight());

		painter->restore();

		QStyleOption branchOption;
		static const int i = 9; // ### hardcoded in qcommonstyle.cpp
		QRect r = option.rect;
		branchOption.rect = QRect(r.left() + i / 2, r.top() + (r.height() - i) / 2, i, i);
		branchOption.palette = option.palette;
		branchOption.state = option.state;
		branchOption.state &= ~QStyle::State_HasFocus;
		if (m_view->isExpanded(index))
		{
			m_view->style()->drawPrimitive(QStyle::PE_IndicatorArrowDown, &branchOption, painter, m_view);
		}
		else
		{
			m_view->style()->drawPrimitive(QStyle::PE_IndicatorArrowRight, &branchOption, painter, m_view);
		}

		QRect textrect = QRect(r.left() + i * 2, r.top(), r.width() - ((5 * i) / 2), r.height());
		QString text = option.fontMetrics.elidedText(model->data(index, Qt::DisplayRole).toString(),
			Qt::ElideMiddle, textrect.width());
		auto font = painter->font();
		//font.setBold(true);
		font.setWeight(QFont::DemiBold);
		font.setPointSize(10);
		painter->setFont(font);
		m_view->style()->drawItemText(painter, textrect, Qt::AlignLeft | Qt::AlignVCenter, option.palette, m_view->isEnabled(), text);
	}
	else
	{
		auto new_option = option;
		//new_option.state &= ~QStyle::State_HasFocus;
		new_option.state = 0;
		QStyledItemDelegate::paint(painter, new_option, index);
	}
}

QSize SheetDelegate::sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
	QSize sz = QStyledItemDelegate::sizeHint(opt, index) + QSize(2, 2);
	return sz;
}