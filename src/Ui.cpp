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

	std::thread ([this, dirA, dirB](){

		auto futureA = std::async(findAllFiles, dirA);
		auto futureB = std::async(findAllFiles, dirB);

		auto filesA = futureA.get();
		auto filesB = futureB.get();

		//Todo не продолжать если нет фалов в одном из наборов

		std::function onCountA = [this, total = filesA.size()](uint32_t count){
			crcProgress = 1.0 / 3.0 * qreal(count) / qreal(total);
			emit crcProgressChanged ();
		};

		auto hash_a = calcCrcMt(filesA, onCountA, 8);


		std::function onCountB = [this, total = filesA.size()](uint32_t count){
			crcProgress = 1.0 / 3.0 *(1.0 +  qreal(count) / qreal(total));
			emit crcProgressChanged ();
		};

		auto hash_b = calcCrcMt(filesB, onCountB, 8);


		auto pairs = findTheSameCrcPairs (hash_a, hash_b); //Todo mt

		std::function onCompare = [this, total = pairs.size()] (bool equal, uint32_t count, std::pair<fs::path,fs::path> paths) -> void {

			crcProgress = 1.0 / 3.0 *(2.0 +  qreal(count) / qreal(total));
			emit crcProgressChanged ();

			if (equal){
				// Todo Is Thread safe?
				beginInsertRows(QModelIndex(), pairsOfFiles.size(), pairsOfFiles.size());
				pairsOfFiles.push_back(std::make_pair(paths.first.u8string(), paths.second.u8string()));
				endInsertRows();
			}
		};

		compareFilesMt(pairs, onCompare, 8);

		//Todo список не обнавляеться без перерисовки?

	}).detach();
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
	return crcProgress;
}

QHash<int, QByteArray> UI::roleNames() const
{
	QHash<int, QByteArray> roles;

	roles[dirA]        = "dirA";
	roles[dirB]        = "dirB";

	return roles;
}
