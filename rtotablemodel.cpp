#include <QFont>
#include <QColor>
#include <QDebug>
#include <QDate>

#include "rtotablemodel.h"
#include "sqlmodel.h"
#include "weekcalc.h"

QVariant RTOTableModel::data(const QModelIndex &index, int role) const
{
    QModelIndex rowidx;
    int week, year, status;

    if (role == Qt::FontRole) {
        QFont font;
        font.setFamily("Arial");
        font.setPixelSize(12);
        if (index.column() == RTO_WEEK_TEXT) {

            rowidx = index.model()->index(index.row(), RTO_WEEK, QModelIndex());
            week = index.model()->data(rowidx).toInt();

            rowidx = index.model()->index(index.row(), RTO_YEAR, QModelIndex());
            year = index.model()->data(rowidx).toInt();

            rowidx = index.model()->index(index.row(), RTO_STATUS_VAL, QModelIndex());
            status = index.model()->data(rowidx).toInt();

            if (status == RTO_NDONE) {
                if ( ((week <= QDate::currentDate().weekNumber()) && (year == QDate::currentDate().year())) || (year < QDate::currentDate().year()) ) {
                   font.setBold(true);
                   return font;
                }
            }
            font.setBold(false);

            return font;
        } else { // if index.column
            return font;
        }

    } else if (role == Qt::ForegroundRole) {
        if (index.column() == RTO_WEEK_TEXT) {
            rowidx = index.model()->index(index.row(), RTO_WEEK, QModelIndex());
            week = index.model()->data(rowidx).toInt();
            QString tmp = index.model()->data(rowidx).toString();
            rowidx = index.model()->index(index.row(), RTO_YEAR, QModelIndex());
            year = index.model()->data(rowidx).toInt();
            rowidx = index.model()->index(index.row(), RTO_STATUS_VAL, QModelIndex());
            status = index.model()->data(rowidx).toInt();

            if (status == RTO_NDONE) {
                if ( ((week < QDate::currentDate().weekNumber()) && (year == QDate::currentDate().year())) || (year < QDate::currentDate().year()) ) {
                    return QColor(Qt::red);
                } else if ( (week == QDate::currentDate().weekNumber()) && (year == QDate::currentDate().year()) ) {
                    return QColor(Qt::darkYellow);
                }
            }
            return SqlModel::data(index, role);
        } else {
            return SqlModel::data(index, role);
        }

    } else if (role == Qt::TextAlignmentRole) {
        if (index.column() != RTO_COMMENT) {
            return Qt::AlignCenter;
        } else {
            return SqlModel::data(index, role);
        }

    } else if (role == Qt::DisplayRole) {
        if (index.column() == RTO_WEEK_TEXT) {
            rowidx = index.model()->index(index.row(), RTO_YEAR, QModelIndex());
            year = index.model()->data(rowidx).toInt();

            rowidx = index.model()->index(index.row(), RTO_WEEK, QModelIndex());
            week = index.model()->data(rowidx).toInt();

            return QVariant(QString::number(year) + ", " + QString::number(week) + " : " + WeekCalc::getWeekRange(week, year, year));
        } else {
            return SqlModel::data(index, role);
        }
    } else {
        return SqlModel::data(index, role);
    }
}
