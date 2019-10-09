#include "const_iterator.h"
#include "iterator.h"
#include "GapBuffer.h"
#include <iostream>
#include <algorithm>

using namespace std;

//DEBUG
ostream& print_gapbuffer(ostream& out, GapBuffer& gp) {
	for (auto& i : gp)
		cout << i;

	cout << endl;
	return cout;
}

int main() {
	//Insertion Test
	GapBuffer gp;
	string s = "hello world";
	for (string::size_type i = 0; i < s.size(); ++i)
		gp.Insert(i, s[i]);
	print_gapbuffer(cout, gp);

	//Check common iterator settings
	//gp.setNewData("1234***567", 4, 7);
	//auto beg = gp.begin();
	//auto end = gp.end();
	//while (beg != end) {
	//	cout << *beg;
	//	beg++;
	//}

	//Check operator-
	//auto a = find(std::begin(gp), std::end(gp), '2');
	//auto b = find(std::begin(gp), std::end(gp), '6');
	//auto dist = b - a;
	//cout << dist << "\n";

	//Check begin method
	//gp.setNewData("***1234", 0, 3);
	//auto new_beg = begin(gp);
	//cout << *new_beg << "\n";

	//Check operator+, operator-
	//gp.setNewData("12**34", 2, 4);
	//auto pre = find(std::begin(gp), std::end(gp), '2');
	//auto post = find(std::begin(gp), std::end(gp), '4');
	//auto b2 = pre + 2;
	//auto b3 = post - 2;
	//cout << *b2 << " " << *b3;

	//Check insert by iterator
	//gp.setNewData("124**6", 3, 5);
	//auto three = find(std::cbegin(gp), std::cend(gp), '4');
	//gp.Insert(three, '3');
	//auto six = find(std::cbegin(gp), std::cend(gp), '6');
	//gp.Insert(six, '5');
	//print_gapbuffer(cout, gp);

	//Check Erase by iterator
	//gp.setNewData("123a**456", 4, 6);
	//auto to_del = find(std::cbegin(gp), std::cend(gp), 'a');
	//auto next = gp.Erase(to_del);
	//gp.Erase(next);
	//print_gapbuffer(cout, gp);

	//Check Erase by range of iterators
	//gp.setNewData("1234ab**cd56", 6, 8);
	//auto a_del = find(std::cbegin(gp), std::cend(gp), 'a');
	//auto b_del = find(std::cbegin(gp), std::cend(gp), 'd');
	//gp.Erase(a_del, b_del);
	//print_gapbuffer(cout, gp);

	//Check Clear function
	//gp.Clear();

	//Check Size functions
	//gp.setNewData("1234****5", 4, 8);
	//cout << (gp.StorageSize() == 16 ? "Correct" : "Incorrect") << "\n";
	//cout << (gp.GapSize() == 4 ? "Correct" : "Incorrect") << "\n";
	//cout << (gp.Size() == 12 ? "Correct" : "Incorrect") << "\n";

	//Check operator=
	//GapBuffer gp_second;
	//gp_second = gp;
	//print_gapbuffer(cout, gp);
	//print_gapbuffer(cout, gp_second);

	return 0;
}