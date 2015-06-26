
#include <QtCore/QCoreApplication>
#include <qfile.h>
#include <locale>
#include<QTextCodec>
#include "functions.h"
#include <QtTest\qtest.h>

/*!
* Выполняет программу
* \param[in] argc - количество переданных аргументов командной строки
* \param[in] argv[1] - имя файла вводных данных
* \return 0 - при успешном выполнении
* \return 1 - в противном случае
*/
int main(int argc, char *argv[])
{

	QCoreApplication a(argc, argv);
	
	setlocale(LC_ALL, "rus");
	QTextCodec * codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(codec);

	initQMapReturnExprNode(returnExprNodeTypeAtQString);    //Создать начальные значение у returnExprNodeTypeAtQString

	QFile output("errors.txt");
	output.open(QIODevice::WriteOnly);


	QTextStream out(&output);
	// 1. Проверить аргументы командной строки на наличие входных данных. Если не достает какого – либо файла, сформировать сообщение об ошибке
	if (argc != 2)
	{
		out << QString::fromLocal8Bit("Ошибка при выполнении программы.\nНеправильные аргументы.\n");
		return 1;
	}
	QString filein = QString(argv[1]);


	exprNode* expr1 = new exprNode;
	exprNode* expr2 = new exprNode;
	QMap<QString, DataVariable> listVar;
	QList< QMap< QString, DataVariable >> listRange;

	// 2. Считать входные данные
	try
	{
		// 2.1. Считать входные данные с помощью функции readXML и проверить их на корректность
		readXML(filein, expr1, expr2, listVar);
	}
	catch (const QString& message)
	{
		// 2.2. Если возникла ошибка, то вывести ошибок в файл 
		out << message;
		return 0;
	}

	bool equivalent; //флаг эквивалентности
	try
	{
		//Сравнить две выражение
		equivalent = compareExpression(listVar, expr1, expr2, listRange);
	}
	catch (const QString& message)
	{
		out<< message;  //Если возникла ошибка, то вывести ошибок в файл 
		return 0;
	}
	//Группировать список наборов переменных, на которых выражения принимают различные значения
	if (listRange.size() > 0)
		groupRangeData(listRange);
	//Вывести резултаты в файл 
	printResult(equivalent, listRange);

	output.close(); //закрыть файл
	//}
	//return 0;
}

