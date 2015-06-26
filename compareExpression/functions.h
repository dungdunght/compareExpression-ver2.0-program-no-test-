#ifndef FUNCTIONS_H
#define FUNCTIONS_H



#include <qfile.h>
#include <QXmlStreamReader>
#include <qdebug.h>
#include<math.h>
#include"classExpression.h"


/*!
* Считывает из XML-файла данные о тестах данной задачи и заполняет ими контейнер
* \param[in] filename - Файл XML.
* \param[out] expr1 - дерево, описывающее первое выражение.
* \param[out] expr2 - дерево, описывающее второе выражение.
* \param[out] listVar - список переменных и их данных с диапазонами.
*/
void readXML(QString filename, exprNode* expr1, exprNode* expr2, QMap<QString, DataVariable>& argc); //Считывает из XML-файла данные о тестах данной задачи и заполняет ими контейнер
/*!
* Сравнивает два выражения и сообщает о наличии эквивалентности
* \param[in] expr1 - дерево, описывающее первое выражение.
* \param[in] expr2 - дерево, описывающее второе выражение.
* \param[in] listVar- список переменных и их данных для двух выражений.
* \param[out] listRange - список наборов переменных, на которых выражения принимают различные значения.
* \return - Знак об эквивалентности true - выражения эквивалентны
* \return -						    false - выражения не эквивалентны
*/
bool compareExpression(QMap<QString, DataVariable>& listVar, exprNode* expr1, exprNode* expr2, QList< QMap< QString, DataVariable >>& listRange); //Сравнивает два выражения и сообщает о наличии эквивалентности
/*!
* Вычисляет значение функции с конкретными значениями переменных
* \param[in] current - Разбор дерева выражения
* \param[in] variantValue  – Список конкретных значений для каждой переменной
* \return - Результат вычисления
*/
long long calcExpression(exprNode* current, QMap<QString, long long> variantValue);  //Вычисляет значение функции с конкретными значениями переменных
/*!
* Группирует набор интервалов значений переменных, которые дают разные результаты
* \param[in] listRange - список наборов переменных, на которых выражения принимают различные значения.
* \param[out] listRange - список наборов переменных, на которых выражения принимают различные значения.
*/
void groupRangeData(QList< QMap< QString, DataVariable >>& listRange); //Группирует набор интервалов значений переменных, которые дают разные результаты
/*!
* Выводит результаты в файл .txt
* \param[in] listRange - список наборов переменных, которые выдают различные значения
* \param[in] equivalent - знак эквивалентности
* \param[out] - файл result.txt с результатами
*/
void printResult(bool equivalent, QList< QMap< QString, DataVariable >> listRange);  //Выводит результаты в файл .txt
#endif 