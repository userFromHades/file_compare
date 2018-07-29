
#include "crc32.h"

#include <iostream>

#include <vector>
#include <queue>
#include <map>

#include <fstream>
#include <thread>
#include <atomic>
#include <mutex>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem::v1;



#include <QApplication>
#include <QQmlApplicationEngine>

#include <QQuickPaintedItem>
#include <QQuickStyle>
#include <QTimer>
#include <QMessageBox>


#include "Ui.h"

int main(int argc, char *argv[]){

	QApplication app(argc, argv);

	QQuickStyle::setStyle("Material");

	QQmlApplicationEngine engine;

	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	UI ui;

	engine.rootContext()->setContextProperty("ui", &ui);

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	QTimer timer;

	QObject::connect(&timer,&QTimer::timeout,[&](){


	});
	timer.start(20);


	return app.exec();
}
