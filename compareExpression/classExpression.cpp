#include "classExpression.h"


/*!
* Конструктор
*/

QMap<QString, exprNodeType> returnExprNodeTypeAtQString;  //хнанит все соответствия типов epxrNodeType из QString


exprNode::exprNode()
{
	type = exprNodeType(0);
	namevar='\0';
	numbervalue = 0;
	leftOperand = NULL;
	rightOperand = NULL;
	fatherOperand = NULL;

}
/*!
* Сравнить два значение данных DataVariable
* \param[in] tempValue1 - первое значение данных DataVariable
* \param[in] tempValue2 - второе значение данных DataVariable
* \return true - две значения одиноковые
* \return false - две значения не одиноковые
*/
bool operator ==(const DataVariable& tempValue1, const DataVariable& tempValue2)
{
	return (tempValue1.typevar == tempValue2.typevar&&tempValue1.lowEdge == tempValue2.lowEdge&&tempValue1.highEdge == tempValue2.highEdge);
}

/*!
* Заполнить значении QString может быть в xml файл, для преобразование на exprNodeType
* \param[out] returnExprNodeTypeAtQString - хнанит все соответствия типов epxrNodeType из QString
*/
void initQMapReturnExprNode(QMap<QString, exprNodeType>&returnExprNodeTypeAtQString)
{
	returnExprNodeTypeAtQString.insert("const", const1);
	returnExprNodeTypeAtQString.insert("var", var1);
	returnExprNodeTypeAtQString.insert("plus", plus1);
	returnExprNodeTypeAtQString.insert("minus", minus);
	returnExprNodeTypeAtQString.insert("multi", multi);
	returnExprNodeTypeAtQString.insert("divide", divide);
	returnExprNodeTypeAtQString.insert("mod", mod);
	returnExprNodeTypeAtQString.insert("prefixInc", prefixInc);
	returnExprNodeTypeAtQString.insert("prefixDec", prefixDec);
	returnExprNodeTypeAtQString.insert("postfixInc", postfixInc);
	returnExprNodeTypeAtQString.insert("postfixDec", postfixDec);
	returnExprNodeTypeAtQString.insert("assignment", assignment);
	returnExprNodeTypeAtQString.insert("addAssign", addAssign);
	returnExprNodeTypeAtQString.insert("subAssign", subAssign);
	returnExprNodeTypeAtQString.insert("multiAssign", multiAssign);
	returnExprNodeTypeAtQString.insert("divideAssign", divideAssign);
	returnExprNodeTypeAtQString.insert("abs", abs1);
	returnExprNodeTypeAtQString.insert("pow", pow1);
	returnExprNodeTypeAtQString.insert("sqrt", sqrt1);
	returnExprNodeTypeAtQString.insert("sqrt", sqrt1);
	returnExprNodeTypeAtQString.insert("fmax", fmax1);
	returnExprNodeTypeAtQString.insert("fmin", fmin1);
}




