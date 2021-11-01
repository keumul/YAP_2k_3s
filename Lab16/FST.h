#pragma once

namespace FST
{
	struct RELATION //ребро:символ -> вершина графа переходов КА
	{
		char symbol; // символ перехода
		short nnode; // номер смежной вершины
		RELATION(
			char c = 0x00, // символ перехода
			short ns = NULL // новое состояние
		);
	};

	struct NODE // вершина графа переходов
	{
		short n_relation; // количество инцидентных ребер
		RELATION* relations; // инцидентные ребра
		NODE();
		NODE(
			short n, // количество инцидентных ребер
			RELATION rel, ... // список ребер
		);
	};

	struct FST // недетерминированный конечный автомат
	{
		char* string; // цепочка(строка, завершается 0х00)
		short position; // текущая позиция в цепочке
		short nstates; // количество состояний автомата
		NODE* nodes; // граф переходов : [0] - начальное состояние, [nstate-1] - конечное
		short* rstates; // возможные состояния автомата на данной позиции
		FST(
			char* s, // цепочка(строка, завершается 0х00)
			short ns, // количество состояний автомата
			NODE n, ... // список состояний (граф переходов)
		);
	};

	bool execute( // выполнить распознание цепочки
		FST& fst // недетерминированный КА
	);
}

#define MAIN(string) string, 5,			  \
			FST::NODE(1, FST::RELATION('m', 1)),  \
			FST::NODE(1, FST::RELATION('a', 2)),  \
			FST::NODE(1, FST::RELATION('i', 3)),  \
			FST::NODE(1, FST::RELATION('n', 4)),  \
			FST::NODE()	

#define FUNCTION(string) string, 9,			  \
			FST::NODE(1, FST::RELATION('f', 1)),  \
			FST::NODE(1, FST::RELATION('u', 2)),  \
			FST::NODE(1, FST::RELATION('n', 3)),  \
			FST::NODE(1, FST::RELATION('c', 4)),  \
			FST::NODE(1, FST::RELATION('t', 5)),  \
			FST::NODE(1, FST::RELATION('i', 6)),  \
			FST::NODE(1, FST::RELATION('o', 7)),  \
			FST::NODE(1, FST::RELATION('n', 8)),  \
			FST::NODE()	

#define INTEGER(string) string, 8,			  \
			FST::NODE(1, FST::RELATION('i', 1)),  \
			FST::NODE(1, FST::RELATION('n', 2)),  \
			FST::NODE(1, FST::RELATION('t', 3)),  \
			FST::NODE(1, FST::RELATION('e', 4)),  \
			FST::NODE(1, FST::RELATION('g', 5)),  \
			FST::NODE(1, FST::RELATION('e', 6)),  \
			FST::NODE(1, FST::RELATION('r', 7)),  \
			FST::NODE()		

#define STRING(string) string, 7,				  \
			FST::NODE(1, FST::RELATION('s', 1)),  \
			FST::NODE(1, FST::RELATION('t', 2)),  \
			FST::NODE(1, FST::RELATION('r', 3)),  \
			FST::NODE(1, FST::RELATION('i', 4)),  \
			FST::NODE(1, FST::RELATION('n', 5)),  \
			FST::NODE(1, FST::RELATION('g', 6)),  \
			FST::NODE()		

#define DECLARE(string) string, 8,			  \
			FST::NODE(1, FST::RELATION('d', 1)),  \
			FST::NODE(1, FST::RELATION('e', 2)),  \
			FST::NODE(1, FST::RELATION('c', 3)),  \
			FST::NODE(1, FST::RELATION('l', 4)),  \
			FST::NODE(1, FST::RELATION('a', 5)),  \
			FST::NODE(1, FST::RELATION('r', 6)),  \
			FST::NODE(1, FST::RELATION('e', 7)),  \
			FST::NODE()	

#define RETURN(string) string, 7,				  \
			FST::NODE(1, FST::RELATION('r', 1)),  \
			FST::NODE(1, FST::RELATION('e', 2)),  \
			FST::NODE(1, FST::RELATION('t', 3)),  \
			FST::NODE(1, FST::RELATION('u', 4)),  \
			FST::NODE(1, FST::RELATION('r', 5)),  \
			FST::NODE(1, FST::RELATION('n', 6)),  \
			FST::NODE()	

#define PRINT(string) string, 6,				  \
			FST::NODE(1, FST::RELATION('p', 1)),  \
			FST::NODE(1, FST::RELATION('r', 2)),  \
			FST::NODE(1, FST::RELATION('i', 3)),  \
			FST::NODE(1, FST::RELATION('n', 4)),  \
			FST::NODE(1, FST::RELATION('t', 5)),  \
			FST::NODE()	

#define IDENTIFICATOR(string) string, 1,	\
				FST::NODE(26,			\
				FST::RELATION('a', 0),	\
				FST::RELATION('b', 0),	\
				FST::RELATION('c', 0),	\
				FST::RELATION('d', 0),	\
				FST::RELATION('e', 0),	\
				FST::RELATION('f', 0),	\
				FST::RELATION('g', 0),	\
				FST::RELATION('h', 0),	\
				FST::RELATION('i', 0),	\
				FST::RELATION('j', 0),	\
				FST::RELATION('k', 0),	\
				FST::RELATION('l', 0),	\
				FST::RELATION('m', 0),	\
				FST::RELATION('n', 0),	\
				FST::RELATION('o', 0),	\
				FST::RELATION('p', 0),	\
				FST::RELATION('q', 0),	\
				FST::RELATION('r', 0),	\
				FST::RELATION('s', 0),	\
				FST::RELATION('t', 0),	\
				FST::RELATION('u', 0),	\
				FST::RELATION('v', 0),	\
				FST::RELATION('w', 0),	\
				FST::RELATION('x', 0),	\
				FST::RELATION('y', 0),	\
				FST::RELATION('z', 0)),	\
			FST::NODE()	

#define INTEGER_LITERAL(string) string, 1,	\
		FST::NODE(10,	FST::RELATION('0', 0),	\
						FST::RELATION('1', 0),	\
						FST::RELATION('2', 0),	\
						FST::RELATION('3', 0),	\
						FST::RELATION('4', 0),	\
						FST::RELATION('5', 0),	\
						FST::RELATION('6', 0),	\
						FST::RELATION('7', 0),	\
						FST::RELATION('8', 0),	\
						FST::RELATION('9', 0)),	\
			FST::NODE()

#define STRING_LITERAL(string) string, 3,	\
		FST::NODE(1, FST::RELATION('\'', 1)),	\
		FST::NODE(85,	FST::RELATION('a', 1),	\
						FST::RELATION('b', 1),	\
						FST::RELATION('c', 1),	\
						FST::RELATION('d', 1),	\
						FST::RELATION('e', 1),	\
						FST::RELATION('f', 1),	\
						FST::RELATION('g', 1),	\
						FST::RELATION('h', 1),	\
						FST::RELATION('i', 1),	\
						FST::RELATION('j', 1),	\
						FST::RELATION('k', 1),	\
						FST::RELATION('l', 1),	\
						FST::RELATION('m', 1),	\
						FST::RELATION('n', 1),	\
						FST::RELATION('o', 1),	\
						FST::RELATION('p', 1),	\
						FST::RELATION('q', 1),	\
						FST::RELATION('r', 1),	\
						FST::RELATION('s', 1),	\
						FST::RELATION('t', 1),	\
						FST::RELATION('u', 1),	\
						FST::RELATION('v', 1),	\
						FST::RELATION('w', 1),	\
						FST::RELATION('x', 1),	\
						FST::RELATION('y', 1),	\
						FST::RELATION('z', 1),	\
						FST::RELATION('0', 1),	\
						FST::RELATION('1', 1),	\
						FST::RELATION('2', 1),	\
						FST::RELATION('3', 1),	\
						FST::RELATION('4', 1),	\
						FST::RELATION('5', 1),	\
						FST::RELATION('6', 1),	\
						FST::RELATION('7', 1),	\
						FST::RELATION('8', 1),	\
						FST::RELATION('9', 1),	\
						FST::RELATION('а', 1),	\
						FST::RELATION('б', 1),	\
						FST::RELATION('в', 1),	\
						FST::RELATION('г', 1),	\
						FST::RELATION('д', 1),	\
						FST::RELATION('е', 1),	\
						FST::RELATION('ё', 1),	\
						FST::RELATION('ж', 1),	\
						FST::RELATION('з', 1),	\
						FST::RELATION('и', 1),	\
						FST::RELATION('й', 1),	\
						FST::RELATION('к', 1),	\
						FST::RELATION('л', 1),	\
						FST::RELATION('м', 1),	\
						FST::RELATION('н', 1),	\
						FST::RELATION('о', 1),	\
						FST::RELATION('п', 1),	\
						FST::RELATION('р', 1),	\
						FST::RELATION('с', 1),	\
						FST::RELATION('т', 1),	\
						FST::RELATION('у', 1),	\
						FST::RELATION('ф', 1),	\
						FST::RELATION('х', 1),	\
						FST::RELATION('ц', 1),	\
						FST::RELATION('ч', 1),	\
						FST::RELATION('ш', 1),	\
						FST::RELATION('щ', 1),	\
						FST::RELATION('ь', 1),	\
						FST::RELATION('ъ', 1),	\
						FST::RELATION('ы', 1),	\
						FST::RELATION('э', 1),	\
						FST::RELATION('ю', 1),	\
						FST::RELATION('я', 1),	\
						FST::RELATION(' ', 1),	\
						FST::RELATION('=', 1),	\
						FST::RELATION('+', 1),	\
						FST::RELATION('-', 1),	\
						FST::RELATION('*', 1),	\
						FST::RELATION('/', 1),	\
						FST::RELATION('(', 1),	\
						FST::RELATION(')', 1),	\
						FST::RELATION('{', 1),	\
						FST::RELATION('}', 1),	\
						FST::RELATION(';', 1),	\
						FST::RELATION(',', 1),	\
						FST::RELATION('?', 1),	\
						FST::RELATION('!', 1),	\
						FST::RELATION('\'', 2)),\
	FST::NODE()