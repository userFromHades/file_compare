#include "Ui.h"

#include <future>
#include <QUrl>

#include "dir_tool.h"


UI::UI( QObject *parent): QAbstractListModel(parent){}
UI::~UI(){}

void UI::compare (const QString& t_dirA, const QString& t_dirB){

	//Todo check dirs

	auto dirA = (QUrl(t_dirA)).toLocalFile().toStdString();
	auto dirB = (QUrl(t_dirB)).toLocalFile().toStdString();

	auto futureA = std::async(findAllFiles, dirA);
	auto futureB = std::async(findAllFiles, dirB);

	auto filesA = futureA.get();
	auto filesB = futureB.get();

	auto hash_a = calcCrcMt(filesA, 8); //Todo return future
	auto hash_b = calcCrcMt(filesB, 8); //Todo return future

	auto pairs = findTheSameCrcPairs (hash_a, hash_b); //Todo mt

	for (auto [first, second] : pairs){
		if (compareFiles(first,second)){
			beginInsertRows(QModelIndex(), pairsOfFiles.size(), pairsOfFiles.size());
			pairsOfFiles.push_back(std::make_pair(first.u8string(), second.u8string()));
			endInsertRows();
		}
	}
}

QVariant UI::data(const QModelIndex & index, int role) const {
	if (index.row() < 0 or
	    index.row() >= pairsOfFiles.count())
		return QVariant();

	auto& p = pairsOfFiles[index.row()];
	switch (role){
	    case dirA: return QString::fromUtf8( p.first.c_str() );
	    case dirB: return QString::fromUtf8( p.second.c_str() );
	}
	return QVariant();
}

bool UI::setData(const QModelIndex &index, const QVariant &value, int role){
	return false;
}


int UI::rowCount(const QModelIndex & parent) const {
	return pairsOfFiles.size();
}

qreal UI::getCrcProgres () const {
	return 0.0f;
}

QHash<int, QByteArray> UI::roleNames() const
{
	QHash<int, QByteArray> roles;

	roles[dirA]        = "dirA";
	roles[dirB]        = "dirB";

	return roles;
}
