#include <iostream>
#include <regex>
#include <algorithm>
#include <thread>
#include <mutex>
#include <iterator>
#include <string>
using namespace std;
char *crt = "\n---------------------------------------\n";
mutex resourceGuard;
class DateTime {
	int *_day, *_month, *_year, *_hours, *_minutes;
public:
	DateTime(int day = 1, int month = 1, int year = 2000, int hours = 0, int minutes = 0) {
		_day = new int(day);
		_month = new int(month);
		_year = new int(year);
		_hours = new int(hours);
		_minutes = new int(minutes);
	}
	~DateTime() {
		delete _day; _day = nullptr;
		delete _month; _month = nullptr;
		delete _year; _year = nullptr;
		delete _hours; _hours = nullptr;
		delete _minutes; _minutes = nullptr;
	}
	DateTime(const DateTime &obj) {
		_day = new int(*obj._day);
		_month = new int(*obj._month);
		_year = new int(*obj._year);
		_hours = new int(*obj._hours);
		_minutes = new int(*obj._minutes);
	}
	DateTime & operator = (const DateTime &obj) {
		if (this != &obj) {
			delete _day, _month, _year, _hours, _minutes;
			_day = new int(*obj._day);
			_month = new int(*obj._month);
			_year = new int(*obj._year);
			_hours = new int(*obj._hours);
			_minutes = new int(*obj._minutes);
		} return *this;
	}
	friend ostream& operator<< (ostream &COUT, const DateTime &obj);
	friend bool operator == (const DateTime &d1, const DateTime &d2);
};
bool operator == (const DateTime &d1, const DateTime &d2) {
	return *d1._day == *d2._day && *d1._month == *d2._month && *d1._year == *d2._year && *d1._hours == *d2._hours;
}
ostream& operator<< (ostream &COUT, const DateTime &obj) {
	COUT << *obj._day << "." << *obj._month << "." << *obj._year << " " << *obj._hours << ":" << *obj._minutes;
	return COUT;
}
template<class T1, class T2 = int>
class FITCollection {
	T1 * _elements1;
	T2 * _elements2;
	int _currently;
public:
	FITCollection() {
		_elements1 = nullptr;
		_elements2 = nullptr;
		_currently = 0;
	}
	~FITCollection() {
		delete[] _elements1; _elements1 = nullptr;
		delete[] _elements2; _elements2 = nullptr;
	}
	FITCollection(const FITCollection &obj) {
		_elements1 = new T1[obj._currently];
		_elements2 = new T2[obj._currently];
		for (int i = 0; i < obj._currently; i++) {
			_elements1[i] = obj._elements1[i];
			_elements2[i] = obj._elements2[i];
		}
		_currently = obj._currently;
	}
	void Add(T1 e1, T2 e2) {
		T1 *temp1 = new T1[_currently + 1];
		T2 *temp2 = new T2[_currently + 1];
		for (int i = 0; i < _currently; i++) {
			temp1[i] = _elements1[i];
			temp2[i] = _elements2[i];
		}
		temp1[_currently] = e1;
		temp2[_currently++] = e2;
		delete[]_elements1; delete[]_elements2;
		_elements1 = temp1; _elements2 = temp2;
	}
	void SortEl() {
		for (int i = _currently; i > 0; i--) {
			for (int j = 1; j < i; j++) {
				if (_elements1[j] < _elements1[j - 1]) {
					swap(_elements1[j], _elements1[j - 1]);
					swap(_elements2[j], _elements2[j - 1]);
				}
			}
		}
	}
	void AddOnLocation(T1 e1, T2 e2, int location) {
		if (location < 0 || location >= _currently) return;
		_currently++;
		T1 *temp1 = new T1[_currently];
		T2 *temp2 = new T2[_currently];
		for (int i = 0; i < _currently; i++) {
			if (i > location) {
				temp1[i] = _elements1[i - 1];
				temp2[i] = _elements2[i - 1];
			}
			else {
				temp1[i] = _elements1[i];
				temp2[i] = _elements2[i];
			}
		}
		temp1[location] = e1;
		temp2[location] = e2;
		delete[]_elements1; delete[]_elements2;
		_elements1 = temp1; _elements2 = temp2;
	}
	void RemoveElement(int location) {
		if (location < 0 || location >= _currently) return;
		T1 *temp1 = new T1[_currently - 1];
		T2 *temp2 = new T2[_currently - 1];
		for (int i = 0; i < _currently - 1; i++) {
			if (i < location) {
				temp1[i] = _elements1[i];
				temp2[i] = _elements2[i];
			}
			else {
				temp1[i] = _elements1[i + 1];
				temp2[i] = _elements2[i + 1];
			}
		}
		_currently--;
		delete[]_elements1; delete[]_elements2;
		_elements1 = temp1; _elements2 = temp2;
	}
	FITCollection * GetUnique() {
		FITCollection * _list = new FITCollection<T1, T2>(); bool notUnique;
		for (int i = 0; i < _currently; i++) {
			notUnique = false;
			for (int j = 0; j < _currently; j++) {
				if (i != j) {
					if (_elements1[i] == _elements1[j] && _elements2[i] == _elements2[j])
						notUnique = true;
				}
			}
			if (!notUnique) {
				for (int e = 0; e < _list->_currently; e++) {
					if (_list->_elements1[e] == _elements1[i] && _list->_elements2[e] == _elements2[i])
						notUnique = true;
				}
				if (!notUnique) _list->Add(_elements1[i], _elements2[i]);
			}
		} return _list;
	}
	FITCollection * getDuplicate() {
		FITCollection * _list = new FITCollection<T1, T2>(); bool Duplicate;
		for (int i = 0; i < _currently; i++) {
			Duplicate = false;
			for (int j = 0; j < _currently; j++) {
				if (i != j) {
					if (_elements1[i] == _elements1[j] && _elements2[i] == _elements2[j])
						Duplicate = true;
				}
			}
			if (Duplicate) {
				for (int e = 0; e < _list->_currently; e++) {
					if (_list->_elements1[e] == _elements1[i] && _list->_elements2[e] == _elements2[i])
						Duplicate = false;
				}
				if (Duplicate) _list->Add(_elements1[i], _elements2[i]);
			}
		}
		return _list;
	}
	FITCollection & operator = (const FITCollection &obj) {
		if (this != &obj) {
			delete[]_elements1;
			delete[]_elements2;
			_elements1 = new T1[obj._currently];
			_elements2 = new T2[obj._currently];
			for (int i = 0; i < obj._currently; i++) {
				_elements1[i] = obj._elements1[i];
				_elements2[i] = obj._elements2[i];
			}
			_currently = obj._currently;
		} return *this;
	}
	T1 & GetT1(int location) const { if (location >= 0 && location < _currently) return _elements1[location]; }
	T2 & GetT2(int location) const { if (location >= 0 && location < _currently) return _elements2[location]; }
	int GetCurrently() { return _currently; }
	friend ostream& operator<< <> (ostream &COUT, const FITCollection<T1, T2> &obj);
};
template<class T1, class T2>
ostream& operator<< <> (ostream &COUT, const FITCollection<T1, T2> &obj) {
	for (size_t i = 0; i < obj._currently; i++)
		COUT << obj._elements1[i] << " - " << obj._elements2[i] << endl;
	return COUT;
}
enum documentType { PDF, DOC, TXT, HTML };
char *_documentPrint[]{ "pdf", "doc", "txt", "html" };
class Document {
	documentType _type;
	string _title;
	string _content;
	int _numberOfPages;
public:
	Document() { _numberOfPages = 0; }
	Document(documentType type, string title) {
		_title = title; _type = type; _numberOfPages = 0;
	}
	Document(const Document &obj) {
		_type = obj._type;
		_title = obj._title;
		_content = obj._content;
		_numberOfPages = obj._numberOfPages;
	}
	~Document() {}
	string GetTitle() { return _title; };
	string GetContent() { return _content; };
	documentType GetType() { return _type; };
	void IncreaseNumberOfPages() { _numberOfPages++; }
	int GetNumberOfPages() { return _numberOfPages; }
	void AddContent(string content) {
		_content += content;
		_numberOfPages = (_content.size() / 30) + 1;
	}
	friend ostream & operator<< (ostream&COUT, const Document & obj);
};
ostream & operator<< (ostream&COUT, const Document & obj) {
	COUT << obj._title << "." << _documentPrint[obj._type] << " (" << obj._numberOfPages << ")\n" << obj._content << endl;
	return COUT;
}
class Device {
protected:
	char *_producer;
	char *_model;
public:
	Device(char * producer, char * model) {
		int size = strlen(producer) + 1;
		_producer = new char[size];
		strcpy_s(_producer, size, producer);
		size = strlen(model) + 1;
		_model = new char[size];
		strcpy_s(_model, size, model);
	}
	virtual ~Device() = 0 {
		delete[] _producer; _producer = nullptr;
		delete[] _model; _model = nullptr;
	}
	Device(const Device &obj) {
		int size = strlen(obj._producer) + 1;
		_producer = new char[size];
		strcpy_s(_producer, size, obj._producer);
		size = strlen(obj._model) + 1;
		_model = new char[size];
		strcpy_s(_model, size, obj._model);
	}
	Device & operator= (const Device &obj) {
		if (this != &obj) {
			delete[]_producer;
			delete[]_model;
			int size = strlen(obj._producer) + 1;
			_producer = new char[size];
			strcpy_s(_producer, size, obj._producer);
			size = strlen(obj._model) + 1;
			_model = new char[size];
			strcpy_s(_model, size, obj._model);
		} return *this;
	}
};
void Print(string documentTitle, string documentContent, DateTime &dT) {
	lock_guard<mutex> locker(resourceGuard); //~safe(lock - unlock)
											//resourceGuard.lock();
	cout << documentTitle << endl << documentContent << endl << dT << endl;
	this_thread::sleep_for(chrono::seconds(1));
	//resourceGuard.unlock();
}
class Printer : public Device {
	FITCollection<DateTime, Document> _printedDocuments;
	vector<string> _forbiddenWords;
public:
	Printer(char *producer, char * model) : Device(producer, model) {}
	virtual ~Printer() {}
	Printer(const Printer &obj) : Device(obj) {
		_printedDocuments = obj._printedDocuments;
		_forbiddenWords = obj._forbiddenWords;
	}
	Printer & operator= (const Printer &obj) {
		Device::operator=(obj);
		_printedDocuments = obj._printedDocuments;
		_forbiddenWords = obj._forbiddenWords;
		return *this;
	}
	int GetAverageNumberOfPagesPerDate(DateTime &d) {
		int numberOfPages = 0;
		for (int i = 0; i < _printedDocuments.GetCurrently(); i++) {
			if (d == _printedDocuments.GetT1(i))
				numberOfPages += _printedDocuments.GetT2(i).GetNumberOfPages();
		} return numberOfPages;
	}
	FITCollection<DateTime, Document> & GetPrintedDocuments() { return _printedDocuments; };
	vector<string> & GetForbiddenWords() { return _forbiddenWords; };
	void RemoveDocuments() {
		for (int i = 0; i < _printedDocuments.GetCurrently(); i++) {
			if (!_checkIfCorrect(_printedDocuments.GetT2(i).GetTitle(), _printedDocuments.GetT2(i).GetContent())) {
				_printedDocuments.RemoveElement(i);
			}
		}
	}
	void AddForbiddenWord(string zabranjenaRijec) {
		vector<string>::iterator it = _forbiddenWords.begin();
		for (it; it != _forbiddenWords.end(); it++) {
			if (zabranjenaRijec == *it)
				return;
		}
		_forbiddenWords.push_back(zabranjenaRijec);
	}
	string GetTopForbiddenWord() {
		int currently = 0, maximum = 0, index = 0;
		for (int j = 0; j < _forbiddenWords.size(); j++) {
			currently = 0;
			regex reg("(" + _forbiddenWords[j] + ")");
			for (int i = 0; i < _printedDocuments.GetCurrently(); i++) {
				string tekst = _printedDocuments.GetT2(i).GetContent();
				sregex_iterator its(tekst.begin(), tekst.end(), reg);
				sregex_iterator ite;
				while (its != ite) {
					its++;
					currently++;
				}
			}
			if (currently > maximum) { maximum = currently; index = j; }
		} return _forbiddenWords[index];
	}
	bool _checkIfCorrect(string title, string content) {
		string titleRule = "^([a-zA-Z]{5,15})\\.((" + (string)_documentPrint[PDF] + ")|(" + (string)_documentPrint[DOC] + ")|(" + (string)_documentPrint[TXT] + ")|(" + (string)_documentPrint[HTML] + "))";
		string contentRule = "(?:(";
		for (int i = 0; i < _forbiddenWords.size(); i++) {
			if (i == _forbiddenWords.size() - 1)
				contentRule += _forbiddenWords[i] + "))";
			else
				contentRule += _forbiddenWords[i] + "|";
		}
		return regex_match(title, regex(titleRule)) && !regex_search(content, regex(contentRule));
	}

	// SO THE PRINTER CAN PRINT A DOCUMENT FOLLOWING RULES MUST BE FULFILLED:
	// 1. docuemnt title MUST BE IN THE NEXT FORMAT, for example: examPRIII.doc
	// docuemnt title CAN CONTAIN ONLY LETTERS (MINIMUM 5, MAXIMUM 15), AND EXTENSION CAN
	// BECOME A KIND OF AN AUTHORIZED DOCUMENT
	// 2. DOCUMENT CONTENT CAN NOT CONTAIN ONE FORBIDDEN WORD
	// IF ANY OF THESE REQUIREMENTS IS NOT SATISFIED, THE FUNCTION SHOULD THROW EXCEPTION WITH
	// APPROPRIATE STAMP, BUT THE DOCUMENT WILL BE STORED IN COLLECTION _printedDocuments.
	// IF THE DOCUMENT COMPLETES ALL CONDITIONS FOR PRINTING IT IS NEEDED TO PRINT HOURS ON CONSOLE
	// FOR THE DOCUMENT THAT IS PRINTED
	// IMPLEMENT PRINTING IN PRIVATE THREADS USING sleep_for FUNCTIONS

	void PrintD(DateTime d, Document &Document) {
		_printedDocuments.Add(d, Document);
		if (_checkIfCorrect(Document.GetTitle(), Document.GetContent())) {
			thread printThread(&Print, Document.GetTitle(), Document.GetContent(), ref(d));
			printThread.join();
		}
		else throw exception("\nError! There is forbidden word in the document.\n");
	} 
	friend ostream & operator << (ostream &C, const Printer &obj);
};
ostream & operator << (ostream &C, const Printer &obj) {
	C << crt << obj._producer << " - " << obj._model << crt;
	C << obj._printedDocuments;
	C << "List of firbidden words: ";
	for (int i = 0; i < obj._forbiddenWords.size(); i++)
		C << obj._forbiddenWords[i] << " ";
	return C;
}
void main() {

#pragma region DateTimeTesting
	DateTime today(1, 2, 2017, 10, 15);
	DateTime tomorrow(today);
	DateTime dayAfterTomorrow;
	dayAfterTomorrow = today;
	cout << today << endl << tomorrow << endl << dayAfterTomorrow << crt;
#pragma endregion
#pragma region FITCollection
	int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

	FITCollection<int, int> numbers;
	numbers.Add(v196, v6);
	numbers.Add(v13, v32);
	numbers.Add(v98, v196);
	numbers.Add(v63, v13);
	numbers.Add(v98, v196);
	numbers.Add(v196, v6);

	cout << numbers << crt + 1;
	//SORTING ELEMENTS OF COLLECTION IS DONE in ascending order based on value of element type T1
	numbers.SortEl();
	cout << numbers << crt + 1;
	//NUMBER 2 MEANS LOCATION/INDEX IN THE LIST WHERE IT IS NEEDED TO ADD NEW ELEMENTS
	numbers.AddOnLocation(v109, v6, 2);
	cout << numbers << crt + 1;
	numbers.SortEl();
	cout << numbers << crt + 1;
	//METHOD GetUnique RETURNS A LIST OF UNIQUE ELEMENTS, ELEMENTS TAHT DON'T CONTAIN DUPLICATES(duplicate IS RELATED TO VALUES OF THE ELEMENTS T1 AND T2). ELEMENTS THAT ARE DUPLICATED NEED TO OCCUR IN THIS LIST JUST ONE TIME.
	FITCollection<int, int> * unique = numbers.GetUnique();
	cout << *unique << crt + 1;
	//*unique = numbers;
	//cout << *unique << crt;
	FITCollection<int, int> * Duplicates = numbers.getDuplicate();
	cout << *Duplicates << crt + 1;

	Document examPRIII(DOC, "examPRIII.doc");
	//NUMBER OF PAGES OF THE DOCUMENT ARE AUTOMATICALLY DETERMINED WHEN ADDING CONTENT. FOR THE PURPOSES OF EXAMS, IT IS ASSUMED THAT ON ONE PAGE THERE CAN BE 30 SIGNES INCLUDING SPACES
	examPRIII.AddContent("SKIPPING COMPLETE AND/OR SOME PARTS OF DESTRUCTORS THAT CAUSE RUNTIME ERROR WILL BE MARKED AS RE");
	cout << "Number of pages -> " << examPRIII.GetNumberOfPages() << endl;
	cout << examPRIII << endl;
	Document ispitBaze(DOC, "ispitBaze.doc");
	ispitBaze.AddContent("PROGRAM CODE CAN BE FOUND IN FILE CODE.TXT");
	cout << ispitBaze << endl;//PRINTING ALL AVAILABLE DATA ABOUT DOCUMENT

	Printer hp3200("HP", "3200");
	//PRINTER NEĆE DOZVOLITI PRINTANJE DOKUMENATA U ČIJEM SADRŽAJU SE NALAZI NEKA OD ZABRANJENIH RIJEČI
	//PRINTER WON'T ALLOW PRITING DOCUMENTS THAT CONTAIN FORBIDDEN WORDS
	hp3200.AddForbiddenWord("RE");// :)
	hp3200.AddForbiddenWord("RAT");
	hp3200.AddForbiddenWord("UBITI");
	hp3200.AddForbiddenWord("RE");//DISABLE REPETITION OF FORBIDDEN WORDS, USE ITERATORS
	hp3200.AddForbiddenWord("MRZITI");
	try {

		// IMPLEMENT PRINTING IN PRIVATE THREADS USING sleep_for FUNCTIONS
		hp3200.PrintD(today, ispitBaze);
		hp3200.PrintD(today, examPRIII);

	}
	catch (exception& err) {
		cout << err.what() << endl;
	}

	//WHEN COMPARING IT IS NOT NEEDED TO COMPARE MINUTES, ONLY DATES AND HOURS
	cout << "Average number of printed pages for " << today << " is -> " << hp3200.GetAverageNumberOfPagesPerDate(today) << crt;
	cout << "Most used forbidden word is -> " << hp3200.GetTopForbiddenWord() << crt;
	cout << hp3200 << crt;
	Printer hp4000(hp3200);
	cout << "Average number of printed pages for " << today << " is -> " << hp4000.GetAverageNumberOfPagesPerDate(today) << crt;
	cout << "Most used forbidden word is -> " << hp4000.GetTopForbiddenWord() << crt;
	hp4000.RemoveDocuments();//REMOVES ALL DOCUMENTS THAT DIDN'T FULFILL ALL THE REQUIREMENTS FOR PRINTING
	cout << hp4000 << crt;

#pragma endregion
	getchar();
}