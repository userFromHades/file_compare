#ifndef UI_H
#define UI_H


#include <QAbstractListModel>


class UI : public QAbstractListModel
{
	Q_OBJECT

	Q_PROPERTY (qreal crcProgress READ getCrcProgres
	                NOTIFY crcProgressChanged)

public:
	UI( QObject *parent = 0);
	virtual ~UI();

	Q_INVOKABLE void compare (const QString&, const QString&);

	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
	int rowCount(const QModelIndex & parent = QModelIndex()) const override;

	qreal getCrcProgres () const;
protected:

	enum Roles {

		dirA = Qt::UserRole + 1,
		dirB,
	};

	QHash<int, QByteArray> roleNames() const;

	QList<std::pair<std::string, std::string>> pairsOfFiles;

signals:
	void crcProgressChanged ();

};

#endif //UI_H
