#include "functions.h"




/*!
* Считывает из XML-файла данные о тестах данной задачи и заполняет ими контейнер
* \param[in] filename - Файл XML.
* \param[out] expr1 - дерево, описывающее первое выражение.
* \param[out] expr2 - дерево, описывающее второе выражение.
* \param[out] listVar - список переменных и их данных с диапазонами.
*/

void readXML(QString filename, exprNode* expr1, exprNode* expr2, QMap<QString, DataVariable>& listVar)
{

	//Данных для вывести ошибки
	QString russia = "йцукенгшщзхъфывапролджэячсмитьбюёЙЦУКЕНГШЩЗХЪЁФЫВАПРОЛДЖЭЯЧСМИТЬБЮ"; //русские буквы
	
	QString message; // сообщение об ошибке
	QString formula;  //индекс выражения
	int lineOpenOperator;  //индекс строки начального тега оператора
	int lineCloseOperator; //индекс строки конечного тега оператора

	QFile input(filename);
	if (!input.open(QFile::ReadOnly | QFile::Text))
	{
		throw "file don't extsting";
	}


	QXmlStreamAttributes attr;
	//5.	Считывание данных из XML-файла происходит средствами библиотеки 
	//Qt методом потокового чтения QXmlStreamReader. 
	
	QXmlStreamReader xmlStream;
	xmlStream.setDevice(&input);

	//Прочитать первую строку в XML файл
	xmlStream.readNext();
	xmlStream.readNext();


	xmlStream.readNext();
	xmlStream.readNext();
	


	//6.Создать указатель currentNode, который будет указывать на текущий узел.
	exprNode* currentNode;
	
	//7.	До конца XML файла
	while (!xmlStream.atEnd())
	{
		if (xmlStream.name().toString() == "test1" && xmlStream.isEndElement()) break;
		
		//7.1.	Если полученный начальный тег является тегом “formula1” или “formula2”, 
		//то соответственно выбирать правильный разбор дерева expr1, expr2 для сохранения. 
		//Присвоить currentNode на expr1 или expr2 соответственно.
		if (xmlStream.name().toString() == "formula1" || xmlStream.name().toString() == "formula2")
		{
			//Если полученный начальный тег является тегом “formula1”
			if (xmlStream.isStartElement())
			{
				if (xmlStream.name().toString() == "formula1")
				{
					currentNode = expr1;
					formula = "1";
				}
				//Если полученный начальный тег является тегом “formula2”,
				else
				{
					currentNode = expr2;
					formula = "2";
				}
				
				//Присвоить curerntNode на текущий опретор
				xmlStream.readNext();
				xmlStream.readNext();

				
				
				currentNode->type = returnExprNodeTypeAtQString[xmlStream.name().toString()];
				lineOpenOperator = xmlStream.lineNumber();  //Обновить индекс начального тега
				// Перейти к следующему тегу	
				xmlStream.readNext();
				xmlStream.readNext();
			}
			//Если полученный тег является последним, то идти дальше
			else if (xmlStream.isEndElement())
			{
				xmlStream.readNext();
				xmlStream.readNext();
				
			}

		}

		//7.2.Если полученный тег является операцией
		else if (xmlStream.name().toString() == "plus" || xmlStream.name().toString() == "minus" || xmlStream.name().toString() == "multi" || xmlStream.name().toString() == "devide" || xmlStream.name().toString() == "mod" || xmlStream.name().toString() == "prefixInc" ||
			xmlStream.name().toString() == "prefixDec" || xmlStream.name().toString() == "postfixInc" || xmlStream.name().toString() == "postfixDec" ||
			xmlStream.name().toString() == "assignment" || xmlStream.name().toString() == "addAssign" || xmlStream.name().toString() == "subAssign" ||
			xmlStream.name().toString() == "multiAssign" || xmlStream.name().toString() == "divideAssign" || xmlStream.name().toString() == "abs" ||
			xmlStream.name().toString() == "pow" || xmlStream.name().toString() == "sqrt" || xmlStream.name().toString() == "fmax" || xmlStream.name().toString() == "fmin")

		{
			//Если полученный тег является начальной
			if (xmlStream.isStartElement())
			{

				lineOpenOperator = xmlStream.lineNumber(); //Обновить индекс начального тега
				//7.2.1.Создать новый узел newNode у класса exprNode, который сохраняет данные из тега.
				exprNode* newNode=new exprNode;
			

				//7.2.2.	Указать родительский узел у узла newNode на currentNode.
				newNode->fatherOperand = currentNode;
				//7.2.3.	Данный узел будет находиться в левом currentNode, если указатель leftOperand свободен, иначе в правом rightOpernand.
				if (currentNode->leftOperand == NULL) currentNode->leftOperand = newNode;
				else
					currentNode->rightOperand = newNode;
				//Присвоить тип оператора в newNode->type
				newNode->type = returnExprNodeTypeAtQString[xmlStream.name().toString()];
				//Присвоить текущий узел на newNode
				currentNode = newNode;

				// Перейти к следующему тегу	
				xmlStream.readNext();
				xmlStream.readNext();
			}
			//7.3.Если полученный тег является последним
			else if (xmlStream.isEndElement())  
			{
				lineCloseOperator = xmlStream.lineNumber(); //Обновить индекс конечного тега

				//Ошибка.Выражение введено не полностью
				if (xmlStream.name().toString() == "prefixInc" || xmlStream.name().toString() == "prefixDec" || xmlStream.name().toString() == "postfixInc" || xmlStream.name().toString() == "postfixDec" || xmlStream.name().toString() == "abs" || xmlStream.name().toString() == "sqrt")
				{

					if (currentNode->leftOperand == NULL)
					{
						message = "В формуле " + formula + ", на строке " + QString::number(lineOpenOperator) + "-" + QString::number(lineCloseOperator) + ", в операции " + xmlStream.name().toString() + " не хватает операторов";
						throw(message);
					}
				}
				else
				{
					if (currentNode->leftOperand == NULL || currentNode->rightOperand == NULL)
					{
						message = "В формуле " + formula + ", на строке " + QString::number(lineOpenOperator) + "-" + QString::number(lineCloseOperator) + ", в операции " + xmlStream.name().toString() + " не хватает операторов";
						throw(message);
					}
				}
				//// Перейти к следующему тегу
				xmlStream.readNext();
				xmlStream.readNext();
				
				currentNode = currentNode->fatherOperand;  //вернуть указатель currentNode на родителей узел
			}

		}

		else
		{

			exprNode* newNode = new exprNode;
			//7.2.4.Если полученный тег обозначает число
			if (xmlStream.name().toString() == "const")
			{
				attr = xmlStream.attributes(); //Получать все атрибуты внутри тега

				//Ошибка.Присваивания при константном правом операнде (не l-value)
				if (currentNode->type == prefixInc || currentNode->type == prefixDec || currentNode->type == postfixInc ||
					currentNode->type == postfixDec || currentNode->type == assignment || currentNode->type == addAssign ||
					currentNode->type == subAssign || currentNode->type == multiAssign || currentNode->type == divideAssign)
					if (currentNode->leftOperand == NULL)
					{
						int currentLine = xmlStream.lineNumber();
						message = "В формуле " + formula + ", в строке " + QString::number(currentLine) + ", присваивается значение константа";
						throw(message);
					}


				//7.2.4.2.Установить поле numbervalue и type у newNode в значение числа и типа данных.
				newNode->type = returnExprNodeTypeAtQString[xmlStream.name().toString()];
				newNode->numbervalue = attr.value("value").toLongLong();
				
				//// Перейти к следующему тегу
				xmlStream.readNext();
				xmlStream.readNext();
				xmlStream.readNext();

			


			}
			//7.2.5.	Если полученный тег обозначает переменную
			else if (xmlStream.name().toString() == "var")
			{
				attr = xmlStream.attributes();//Получать все атрибуты внутри тега


				newNode->type = returnExprNodeTypeAtQString[xmlStream.name().toString()]; //Установить поле type у newNode в значение типа данных.
				newNode->namevar = attr.value("name").toString();  //Добавить имена переменные в поле namevar у newNode. 

				//Ошибка.  В названии переменных используются нелатинские буквы.
				if (russia.contains(newNode->namevar))
				{
					message = "В формуле " + formula + " имя переменной " + newNode->namevar  + " должно быть задано латинскими буквами";
					throw(message);
				}

				//Ошибка. Заданные диапазоны не соответствуют типам данных
				if (attr.value("type").toString() != "char" && attr.value("type").toString() != "unsigned char" && attr.value("type").toString() != "int" && attr.value("type").toString() != "long" && attr.value("type").toString() != "unsigned long")
				{
					message = "В формуле " + formula + ", у переменной "  + newNode->namevar + " тип данных задан не верно";
					throw(message);
				}

				//Ошибка.Переменные введены без типа данных
				if (attr.value("type") == "")
				{
					message = "В формуле " + formula + ", переменная «а» задана без типа данных";
					throw(message);
				}
				//Ошибка.Переменные введены без диапазона 
				if (attr.value("from") == "" || attr.value("to") == "")
				{
					message = "В формуле " + formula + ", переменная "+newNode->namevar +" задана без диапазона";
					throw(message);
				}



				//Получить информации о диапозоне и типе данных в теге 
				DataVariable tempvar;
				tempvar.typevar = attr.value("type").toString();
				tempvar.lowEdge = attr.value("from").toLongLong();
				tempvar.highEdge = attr.value("to").toLongLong();


				//Ошибка.Нелогично задан интервал
				if (tempvar.highEdge < tempvar.lowEdge)
				{
					message = "В формуле " + formula + ", у переменной "  + newNode->namevar + " диапазон задан не верно";
					throw (message);
				}

				//Ошибка.Задается одна и та же переменная с разными типами данных или с разными диапазонами в двух выражениях
				if (listVar.contains(newNode->namevar))
				{
					if (listVar.value(newNode->namevar).lowEdge != tempvar.lowEdge || listVar.value(newNode->namevar).highEdge != tempvar.highEdge)
					{
						message = "Переменная " + newNode->namevar + " в разных выражениях определяется по-разному";
						throw (message);
					}
				}
				else
					//7.2.5.4.	Добавить в список переменных listVar имена переменные с диапазоном и типом данных.  
					listVar.insert(newNode->namevar, tempvar);

				//// Перейти к следующему тегу
				xmlStream.readNext();
				xmlStream.readNext();
				xmlStream.readNext();

				


			}
			//7.2.3.	Указать родительский узел у узла newNode на currentNode.
			newNode->fatherOperand = currentNode;
			//7.2.2.	Данный узел будет находиться в левом currentNode, если указатель leftOperand свободен, иначе в правом rightOpernand
			if (currentNode->leftOperand == NULL) currentNode->leftOperand = newNode;
			else
				currentNode->rightOperand = newNode;
		}
	}


}


/*!
* Сравнивает два выражения и сообщает о наличии эквивалентности
* \param[in] expr1 - дерево, описывающее первое выражение.
* \param[in] expr2 - дерево, описывающее второе выражение.
* \param[in] listVar- список переменных и их данных для двух выражений.
* \param[out] listRange - список наборов переменных, на которых выражения принимают различные значения.
* \return - Знак об эквивалентности true - выражения эквивалентны
* \return -						    false - выражения не эквивалентны
*/


bool compareExpression(QMap<QString, DataVariable>& listVar, exprNode* expr1, exprNode* expr2, QList< QMap< QString, DataVariable >>& listRange)
{
 	bool equivalent=true;  //флаг эквивалентности

	int n = listVar.size();  //количество эелементы в listVar


	QMap<QString, long long> variantValue;  //список переменные с конкретными значенями


	QMap<QString, DataVariable>::Iterator itempValue;  //интератор на listVar

	QMap<QString, DataVariable> tempValues;  //временный список переменные и их пределы

	QMap<QString, long long>::Iterator temp;   //интератор на variantValue

	bool lastCombination = false;  // флаг lastCombination, который отмечает является ли текущая комбинация последней

	DataVariable tempRange;  


	itempValue = listVar.begin();


	int i;
	//1.3.Заполнить все переменные в listVar на variantValue с помощью интератор itempValue, их начальные значения будут являться itempValue.value().lowEdge.
	for (i = 1; i <= n; i++)
	{

		variantValue.insert(itempValue.key(), itempValue.value().lowEdge);
		itempValue++;

	}

	//4.	До тех пор, пока не достигнута последняя комбинация
	while (!lastCombination)
	{
		
		temp = variantValue.begin(); //Указать итератор temp на начальный элемент variantValue
		lastCombination = true;  //	Установить флаг lastCombination в истину
		//4.1.Сравнить переменные при текущей комбинация, 
		//если каждая из переменных в текущей комбинации больше или равна максимальному значению из своего диапазона, 
		//то это - последняя комбинация.
		for (int ii = 1; ii <= n; ii++)
		{
			//4.1.3.	C помощью temp, сравнить каждый элемент у variantValue с его верхней пределом highEdge, 
			//если у какого-либо элемента значение меньше чем highEdge, то сбросить флаг lastCombination.

			if (temp.value() < listVar[temp.key()].highEdge)
				lastCombination = false;
			temp++;
		}

		//4.1.4.C помощью функции calcExpression, сравнить два выражения при текущей комбинации variantValue.
		long long s1, s2;
		try
		{
			s1 = calcExpression(expr1, variantValue);
			s2 = calcExpression(expr2, variantValue);
		}
		catch (const QString& message)
		{
			throw (message);
		}
		//4.1.4.1.	Если выражения не равны  
		if (s1 != s2)
		{
			equivalent = false;
			if (variantValue.size() > 0)
			{
				//4.1.4.1.1.	Создать группу с различными переменными и 
				//их пределами lowEdge и highEdge с помощью tempValues, тип QMap<QString, DataVariable>. 
				tempValues.clear();
				temp = variantValue.begin();
				for (int ii = 1; ii <= n; ii++)
				{
					tempRange.typevar = listVar[temp.key()].typevar;
					tempRange.lowEdge = temp.value();
					tempRange.highEdge = temp.value();
					tempValues.insert(temp.key(), tempRange);
					temp++;
				}
				//4.1.4.1.2.	 Добавить tempValues в выходной контейнер listRange
				listRange.append(tempValues);
			}
		}

		//4.2.Перейти к следующей комбинации
		if (variantValue.size() > 0)
		{
			//4.2.1.	Установить temp на последнюю переменную комбинации.
			temp =variantValue.end(); 
			temp--;
			bool stop = false;
			//4.2.2.	Двигаться назад по массиву переменных variantValue, когда достигли предела highEdge по одному или несколько измерениям.
			while (temp.value() == listVar[temp.key()].highEdge&&!stop) //4.2.2.1.	Если текущие значение переменной у итератора temp равно его предел highEdge.
			{

				//4.2.2.2.Присвоить данной переменной значение её нижнего предела lowEdge.
				temp.value() = listVar[temp.key()].lowEdge;
				//4.2.2.3.Перейти к предыдущей переменной(temp--) и вернуть на шаг 4.2.2.1.
				if (temp == variantValue.begin()) stop = true;
				else
					temp--;
			}
			//4.2.3.	Если значение переменной у итератора temp меньше его верхнего предела (highEdge)
			//4.2.3.1.Увеличить её значение temp.value() на 1.
			temp.value()++;
		}


	}
	return equivalent; //Вернуть знак эквивалентности

}
/*!
* Вычисляет значение функции с конкретными значениями переменных
* \param[in] current - Разбор дерева выражения
* \param[in] variantValue  – Список конкретных значений для каждой переменной
* \return - Результат вычисления
*/
long long calcExpression(exprNode* current, QMap<QString, long long> variantValue)
{

	QMap<QString, long long>::iterator i;
	switch (current->type)
	{
	//1.1.	Если данный узел содержит константу, то вернуть само это значение.
	case const1:
	{	
		return current->numbervalue;
	}
	//1.2.	Если данный узел содержит переменную, то вернуть значение этой переменной, которое берется из списка variantValue.
	case var1:
		return variantValue.value(current->namevar);
	//1.3.	Если данный узел содержит оператор, то выполнить вычисление по этому оператору с результатами дочернего узла и возвращает это значение.
	case plus1: //Сложение
		return calcExpression(current->leftOperand, variantValue) + calcExpression(current->rightOperand, variantValue);
	case minus: //Вычитание
		return calcExpression(current->leftOperand, variantValue) - calcExpression(current->rightOperand, variantValue);
	case multi: //Умножение
		return calcExpression(current->leftOperand, variantValue) * calcExpression(current->rightOperand, variantValue);
	case divide: //Деление
	{
		
		int y=calcExpression(current->rightOperand, variantValue);
		// Ошибки. Если в операции деления делитель является нулем
		if (y == 0)
		{
			QString message;
			QString lineValue;
			for (i = variantValue.begin(); i != variantValue.end(); ++i)
				lineValue += (i.key()+ "="+QString::number(i.value())+", ");
			if (lineValue.size() == 0) message = "В процессе вычисления произошло деление на ноль";
			else
				message = "При переменных "+lineValue+"в процессе вычисления произошло деление на ноль";
			throw (message); 
		}
		return llround(calcExpression(current->leftOperand, variantValue) / y);
	}  
	case mod:   //Получение остатка от деления
		return calcExpression(current->leftOperand, variantValue) % calcExpression(current->rightOperand, variantValue);
	case prefixInc:  //Префиксный инкремент
		return calcExpression(current->leftOperand, variantValue) +1;

	case postfixInc: //Постфиксный инкремент
		return calcExpression(current->leftOperand, variantValue);
	case prefixDec:  //Префиксный декремент
		return calcExpression(current->leftOperand, variantValue) - 1;
	case postfixDec: //Постфиксный декремент
		return calcExpression(current->leftOperand, variantValue);
	case assignment: //Присваивание 
	{
		variantValue[current->leftOperand->namevar] = calcExpression(current->rightOperand, variantValue);
		return variantValue[current->leftOperand->namevar];
	}
	case addAssign:  //Присваивание со сложением
	{

		variantValue[current->leftOperand->namevar] += calcExpression(current->rightOperand, variantValue);
		return variantValue[current->leftOperand->namevar];
	}
	case subAssign:  //Присваивание с вычитанием
	{
		variantValue[current->leftOperand->namevar] -= calcExpression(current->rightOperand, variantValue);
		return variantValue[current->leftOperand->namevar];
	}
	case multiAssign:  //Присваивание с умножением
	{
	
		variantValue[current->leftOperand->namevar] *= calcExpression(current->rightOperand, variantValue);
		return variantValue[current->leftOperand->namevar];
	}
	case divideAssign:  //Присваивание с делением
	{
		int y = calcExpression(current->rightOperand, variantValue);
		// Ошибки. Если в операции деления делитель является нулем
		if (y == 0)
		{
			QString message;
			QString lineValue;
			QMap<QString, long long>::iterator endOfValue = variantValue.end(); //отимизация
			for (i = variantValue.begin(); i != endOfValue; ++i)
				lineValue += (i.key() + "=" + QString::number(i.value()) + ", ");
			if (lineValue.size() == 0) message = "В процессе вычисления произошло деление на ноль";
			else
				message = "При переменных " + lineValue + "в процессе вычисления произошло деление на ноль";
			throw (message);
		}
		variantValue[current->leftOperand->namevar] = llround(variantValue.value(current->leftOperand->namevar) / y);
		return variantValue[current->leftOperand->namevar];
	}
	case abs1: //Вычисление модуля 
		return abs(calcExpression(current->leftOperand, variantValue));
	case pow1: //Возведение в степень
		return pow(calcExpression(current->leftOperand,variantValue), calcExpression(current->rightOperand,variantValue));
	case sqrt1: // Извлечение квадратного корня
			return llround(sqrt(calcExpression(current->leftOperand, variantValue)));
	case fmax1: //Наибольшее значение среди двух значений
		return fmax(calcExpression(current->leftOperand, variantValue), calcExpression(current->rightOperand, variantValue));
	case fmin1:  //Наименьшее значение среди двух значений
		return fmin(calcExpression(current->leftOperand, variantValue), calcExpression(current->rightOperand, variantValue));

	}
	//2.	Переход вверх : заполнить возвращенный результат для вычисления значения родительского узла.
	//3.	Конечный результат: возвращаемое значение для корня дерева.
}


/*!
* Группирует набор интервалов значений переменных, которые дают разные результаты
* \param[in] listRange - список наборов переменных, на которых выражения принимают различные значения.
* \param[out] listRange - список наборов переменных, на которых выражения принимают различные значения.
*/

void groupRangeData(QList< QMap< QString, DataVariable >>& listRange)
{
	bool haveChange = true;
	


	while (haveChange)
	{
		haveChange = false;
		//2.2.	Проверить, можно ли группировать данный набор с другими наборами значений, 
		//которые находятся после данного набора и были отмечены для рассмотрения.
		for (int i = 0; i < listRange.size() && haveChange == false; ++i)
		{
			
				for (int j = i + 1; j < listRange.size() && haveChange == false; ++j)
				{

					QMapIterator<QString, DataVariable> tempValue(listRange.at(i));
					int countValue = 0;  //2.2.1.1.	Инициализировать countValue для рассчитан количество переменных не совпадают интервал значений.
					QString addressValueMerge='\0';
					bool flagValue = true;

					while (tempValue.hasNext() && flagValue)
					{

						tempValue.next();
						//Проверить по каждому переменных, совпадет ли интервал значений, если нет, то увеличивать значение countValue.
						if (!(listRange[j][tempValue.key()] == tempValue.value()))
						{

							countValue++;
							if (countValue > 1) flagValue = false; //2.2.1.3.	Если countValue>1, то остановить проверку
							//2.2.1.4.	Если два переменные не совпадают, проверять значение highEdge у первого элемемнта и значение lowEdge у второго элемента. 
							//Если lowEdge=highEdge+1, то два эти набор можно объединять. 
							//Сохранять адрес этого переменные в addressValueMerge.
							if (tempValue.value().highEdge == listRange[j][tempValue.key()].lowEdge - 1)
							{
								addressValueMerge = tempValue.key();
							}

						}

					}
					//2.2.2.	Если два данные набор отмечают что можно группировать (countValue должна равна 1), 
					//то группирует два разных интервала переменные у два наборов, расширяя один из наборов значений.
					if (flagValue&&addressValueMerge.size()!=0)
					{
						QMap<QString, DataVariable>::iterator addtempValue;
						
						//2.2.2.1.	Найти адрес этого интервал из addressValueMerge.
						//2.2.2.2.	Увеличить значение highEdge у первого интервал до highEdge у второго интервал.   
						listRange[i][addressValueMerge].highEdge = listRange.at(j).value(addressValueMerge).highEdge;
						

						//2.2.2.3.	Удалить лишний набор значений из контейнера.
						listRange.removeAt(j);
						haveChange = true;
						//2.2.3.Вернуть к начальному набору значений и повторить действие
					}

				}
			/*}*/
		}
	}
}
/*!
* Выводит результаты в файл .txt
* \param[in] listRange - список наборов переменных, которые выдают различные значения
* \param[in] equivalent - знак эквивалентности
* \param[out] - файл result.txt с результатами
*/

void printResult(bool equivalent,QList< QMap< QString, DataVariable >> listRange)
{
	// 1. Открыть XML файл с именем result.txt с режимом записи.
	QFile output("result.txt"); 
	output.open(QIODevice::WriteOnly);


	QTextStream out(&output);
	//3.1.	Вывести сообщение о наличии эквивалентности.
	if (equivalent)
		out << QString::fromLocal8Bit("Выражения эквивалентны.") << endl;
	else
	{
		out << QString::fromLocal8Bit("Выражения не эквивалентны.") <<endl;
		int m = listRange.size();
		//3.2.	Вывести все наборы значений переменных, если контейнер наборов значений переменных содержит элементы
		for (int i = 0; i < m; i++)
		{   
			out<<"[";
			QMap<QString, DataVariable>::iterator temp;
			//Вывести имена переменных
			temp = listRange[i].begin();
			while (temp != listRange[i].end())
			{
				out<<temp.key().toUtf8().data();
				++temp; 
				if (temp != listRange[i].end()) out<<"|";
			}
			out<<"]=[";
			//Вывести наборы значений переменных
			temp = listRange[i].begin();
			while (temp != listRange[i].end())
			{

				out << temp.value().lowEdge << ".." << temp.value().highEdge;
				++temp;
				if (temp != listRange[i].end()) out<<"|";
			}
			out << "]"<<endl;
			

		}
	}
	output.close(); //4.	Закрыть файл result.txt
}