#ifndef CLASSEXPRESSION_H
#define CLASSEXPRESSION_H

#include<QMap>
#include<QString>


/*!
* Объект перечень содержит информацию об операциях, поддерживаемы в программе.
*/
enum exprNodeType
{
	formula1, /**< Первое выражение  */
	formula2, /**< Второе выражение  */
	const1, /**< Константа  */
	var1,   /**< Переменная  */
	plus1,   /**< Сложение  */
	minus,   /**< Вычитание  */
	multi,   /**< Умножение  */
	divide,  /**< Деление  */
	mod,    /**< Остаток от деления  */
	prefixInc,  /**< Префиксный инкремент  */
	prefixDec,  /**< Префиксный декремент  */
	postfixInc,  /**< Префиксный инкремент  */
	postfixDec,  /**< Постфиксный декремент  */
	assignment,  /**< Присваивание  */
	addAssign,   /**< Присваивание со сложением  */
	subAssign,  /**< Присваивание с вычитанием  */
	multiAssign,  /**< Присваивание с умножением  */
	divideAssign,  /**< Присваивание с делением  */
	abs1,        /**< Модуль вычисления   */
	pow1,      /**< Возведение в степень   */
	sqrt1,     /**< Извлечение квадратного корня   */
	fmax1,    /**< Наибольшее значение среди двух значений  */
	fmin1    /**< Наименьшее значение среди двух значений  */

};

/*!
* Класс Хранит деревья разбора двух выражений, данные о типах переменных, функциях, классах, диапазонах значений переменных.
*/
class exprNode
{
public:
	exprNodeType type; /**< типы данных в узле  */ 
	long long numbervalue;  /**< значение узла, если в нем находится число  */ 
	QString namevar;  /**< латинская буква, если в узле находятся переменные  */ 
	exprNode* leftOperand;  /**< левый узел  */ 
	exprNode* rightOperand;  /**< правый узел  */
	exprNode* fatherOperand; /**< указатель на родителя  */ 
	/*!
	* Конструктор
	*/
	exprNode();   
	
};



/*!
* Класс хранит переменные с их данными и диапазоном.
*/
class DataVariable
{
public:
	QString typevar;   /**< тип переменной  */  
	long long lowEdge;   /**< значение нижнего предела в диапазоне переменных  */ 
	long long highEdge;   /**< значение верхнего предела в диапазоне переменных  */
	/*!
	* Сравнить два значение данных DataVariable
	* \param[in] tempValue1 - первое значение данных DataVariable
	* \param[in] tempValue2 - второе значение данных DataVariable
	* \return true - две значения одиноковые
	* \return false - две значения не одиноковые
	*/
	bool friend operator ==(const DataVariable& tempValue1, const DataVariable& tempValue2);
};

extern QMap<QString, exprNodeType> returnExprNodeTypeAtQString;  //хнанит все соответствия типов epxrNodeType из QString
void initQMapReturnExprNode(QMap<QString, exprNodeType>&);  //заполнить значении QString может быть в xml файл, для преобразование на exprNodeType
#endif 