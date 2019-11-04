#include "pch.h"
#include "../GapBuffer/GapBuffer.h"
#include "../GapBuffer/iterator.h"
#include "../GapBuffer/const_iterator.h"
#include <string>
#include <vector>
#include <numeric>
#include <string_view>
#include <type_traits>

using namespace std;

//Test fixture class for GapBuffer
class GapBufferTest : public ::testing::Test {
protected:
	void SetUp() override {
		gp_first.setNewData("abcd**gh", 4, 6);
		gp_second.setNewData("**3456789", 0, 2);
		gp_third.setNewData("1*******9", 1, 8);

		string s(25'000, '+');
		for (string::size_type i = 10'000; i < 19'500; ++i)
			s[i] = '*';
		gp_fourth.setNewData(s, 10'000, 19'500);
	}

	//void TearDown() override {}

	GapBuffer gp_first;
	GapBuffer gp_second;
	GapBuffer gp_third;
	GapBuffer gp_fourth;
};

bool IsGapPairEqual(const pair<size_t, size_t>& lhs, const pair<size_t, size_t>& rhs) {
	return lhs == rhs;
}


//It's not rational to create GapBufferTest object every time we're calling
//TEST_F function but it's the only way to reduce code ih these tests.

TEST_F(GapBufferTest, Constructing) {
	//Check for independency of copy
	GapBuffer gp_cpy(gp_first.begin(), gp_first.end());
	gp_first.Erase(gp_first.begin());
	EXPECT_NE(*begin(gp_first), *begin(gp_cpy));
	EXPECT_EQ(*begin(gp_cpy), 'a');
	//Check for correct construct by iterators
	EXPECT_EQ("abcdgh", string(gp_cpy.begin(), gp_cpy.end()));
	EXPECT_EQ("19", string(gp_third.begin(), gp_third.end()));

	auto gp_data = gp_third.getGapData();
	EXPECT_EQ("1*******9", string(gp_data.begin(), find(gp_data.begin(), gp_data.end(), '\0')));
}
TEST_F(GapBufferTest, Concepts) {
	EXPECT_TRUE(is_copy_constructible<decltype(gp_first)>::value);
	EXPECT_TRUE(is_copy_assignable<decltype(gp_first)>::value);
	EXPECT_TRUE(is_destructible<decltype(gp_first)>::value);
	EXPECT_TRUE(is_default_constructible<decltype(gp_first)>::value);
	EXPECT_TRUE(is_swappable<decltype(gp_first)>::value);
	bool is_asgn = is_assignable<decltype(gp_first), decltype(gp_first)>::value;
	EXPECT_TRUE(is_asgn);
}
TEST_F(GapBufferTest, 1_InsertByIndex) {
	gp_first.Insert(4, 'e');
	gp_first.Insert(5, 'f');
	vector<char>& data = gp_first.getGapData();
	string to_compare = "abcdefgh";
	EXPECT_TRUE(equal(begin(data), end(data), begin(to_compare))) << "Insert function mistake.e";
	EXPECT_TRUE(IsGapPairEqual(gp_first.getGapPos(), make_pair(6, 6))) << "Gap Buffer isn't in a right position after the insertion.";
}

TEST_F(GapBufferTest, 2_InsertByIndex) {
	gp_second.Insert(0, '1');
	gp_second.Insert(1, '2');
	vector<char>& data = gp_second.getGapData();
	auto end_data = find(begin(data), end(data), '\0');
	string to_compare = "123456789";
	EXPECT_TRUE(equal(begin(data), end_data, begin(to_compare))) << "Insert function mistake.";
	EXPECT_TRUE(IsGapPairEqual(gp_second.getGapPos(), make_pair(2, 2))) << "Gap Buffer isn't in a right position after the insertion.";
}

TEST_F(GapBufferTest, 3_InsertByIndex) {
	string to_compare = "123456789";
	for (string::size_type i = 1; i < to_compare.size() - 1; ++i)
		gp_third.Insert(i, to_compare[i]);

	vector<char>& data = gp_third.getGapData();
	auto end_data = find(begin(data), end(data), '\0');
	EXPECT_TRUE(equal(begin(data), end_data, begin(to_compare))) << "Insert function mistake.";
	EXPECT_TRUE(IsGapPairEqual(gp_third.getGapPos(), make_pair(8, 8))) << "Gap Buffer isn't in a right position after the insertion.";
}

TEST_F(GapBufferTest, 4_InsertByIndex) {
	string to_compare(25'000, '+');

	for (string::size_type i = 10'000; i < 19'500; ++i)
	    gp_fourth.Insert(i, to_compare[i]);

	vector<char>& data = gp_fourth.getGapData();
	auto end_data = find(begin(data), end(data), '\0');
	EXPECT_TRUE(equal(begin(data), end_data, begin(to_compare))) << "Insert function mistake.";
	EXPECT_TRUE(IsGapPairEqual(gp_fourth.getGapPos(), make_pair(19'500, 19'500))) << "Gap Buffer isn't in a right position after the insertion.";
}

TEST_F(GapBufferTest, 1_InsertByIterator) {
	string to_compare = "abcdefgh";
	GapBuffer::const_iterator iter = cbegin(gp_first) + 4;
	gp_first.Insert(iter, 'e');
	gp_first.Insert(iter, 'f');
	vector<char>& data = gp_first.getGapData();
	EXPECT_TRUE(equal(begin(data), end(data), begin(to_compare))) << "Insert function mistake";
	EXPECT_TRUE(IsGapPairEqual(gp_first.getGapPos(), make_pair(6, 6))) << "Gap Buffer isn't in a right position after the insertion.";;
}

TEST_F(GapBufferTest, 2_InsertByIterator) {
	GapBuffer::const_iterator iter = cbegin(gp_second);
	gp_second.Insert(iter, '1');
	gp_second.Insert(iter, '2');
	vector<char>& data = gp_second.getGapData();
	auto end_data = find(begin(data), end(data), '\0');
	string to_compare = "123456789";
	EXPECT_TRUE(equal(begin(data), end_data, begin(to_compare))) << "Insert function mistake.";
	EXPECT_TRUE(IsGapPairEqual(gp_second.getGapPos(), make_pair(2, 2))) << "Gap Buffer isn't in a right position after the insertion.";
}

TEST_F(GapBufferTest, 3_InsertByIterator) {
	string to_compare = "123456789";
	string_view sv(to_compare);
	sv.remove_prefix(1);
	sv.remove_suffix(1);
	GapBuffer::const_iterator iter = cend(gp_third) - 1;
	for (auto ch : sv)
		gp_third.Insert(iter, ch);

	vector<char>& data = gp_third.getGapData();
	auto end_data = find(begin(data), end(data), '\0');
	EXPECT_TRUE(equal(begin(data), end_data, begin(to_compare))) << "Insert function mistake.";
	EXPECT_TRUE(IsGapPairEqual(gp_third.getGapPos(), make_pair(8, 8))) << "Gap Buffer isn't in a right position after the insertion.";
}

TEST_F(GapBufferTest, 4_InsertByIterator) {
	string to_compare(25'000, '+');
	string_view cmp(to_compare);
	//Кудесничество
	cmp.remove_prefix(10'000);
	constexpr size_t dst = 25'000 - 19'500;
	cmp.remove_suffix(dst);
	GapBuffer::const_iterator iter = cbegin(gp_fourth) + 10'000;

	for (auto ch : cmp)
		gp_fourth.Insert(iter, ch);

	vector<char>& data = gp_fourth.getGapData();
	auto end_data = find(begin(data), end(data), '\0');
	EXPECT_TRUE(equal(begin(data), end_data, begin(to_compare))) << "Insert function mistake.";
	EXPECT_TRUE(IsGapPairEqual(gp_fourth.getGapPos(), make_pair(19'500, 19'500))) << "Gap Buffer isn't in a right position after the insertion.";
}
TEST_F(GapBufferTest, 1_Erase) {
	auto cbeg = cbegin(gp_first);
	GapBuffer::iterator next = gp_first.Erase(cbeg + 3);
	gp_first.Erase(next);
	string to_compare = "abch";
	EXPECT_TRUE(equal(begin(gp_first), end(gp_first), begin(to_compare))) << "Erase function mistake.";
	EXPECT_TRUE(IsGapPairEqual(gp_first.getGapPos(), make_pair(3, 7))) << "Gap Buffer isn't in a right position after the removal.";
}

TEST_F(GapBufferTest, 2_Erase) {
	auto nxt_iter = gp_second.Erase(cbegin(gp_second), cbegin(gp_second) + 4);
	EXPECT_EQ(*nxt_iter, '7') << "Erase const iters returns isn't correct iterator";
	string to_compare = "789";
	EXPECT_TRUE(equal(begin(gp_second), end(gp_second), begin(to_compare))) << "Erase function mistake";
	EXPECT_TRUE(IsGapPairEqual(gp_second.getGapPos(), make_pair(0, 6))) << "Gap Buffer isn't in a right position after the removal.";
}

TEST_F(GapBufferTest, 3_Erase) {
	auto nxt_iter = gp_third.Erase(begin(gp_third), begin(gp_third) + 2);
	EXPECT_EQ(nxt_iter, end(gp_third)) << "Erase iters returns isn't correct iterator";
	EXPECT_EQ(gp_third.begin(), gp_third.end()) << "Container should be empty";
}

TEST_F(GapBufferTest, 4_Erase) {
	gp_fourth.Erase(cbegin(gp_fourth), cend(gp_fourth));
	auto dist2 = gp_fourth.GapSize();
	EXPECT_EQ(gp_fourth.GapSize(), 25'000) << "Container should be empty";
	EXPECT_EQ(cbegin(gp_fourth), cend(gp_fourth)) << "Erase function mistake";
}

TEST_F(GapBufferTest, Clear) {
	gp_first.Clear();
	gp_second.Clear();
	gp_third.Clear();
	gp_fourth.Clear();
	bool first_ok = (gp_first.StorageSize() == 1 && gp_first.getGapPos() == make_pair<size_t, size_t>(0, 1));
	bool second_ok = (gp_second.StorageSize() == 1 && gp_second.getGapPos() == make_pair<size_t, size_t>(0, 1));
    bool third_ok = (gp_third.StorageSize() == 1 && gp_third.getGapPos() == make_pair<size_t, size_t>(0, 1));
    bool fourth_ok = (gp_fourth.StorageSize() == 1 && gp_fourth.getGapPos() == make_pair<size_t, size_t>(0, 1));
	EXPECT_TRUE(first_ok);
	EXPECT_TRUE(second_ok);
	EXPECT_TRUE(third_ok);
	EXPECT_TRUE(fourth_ok);
}

TEST_F(GapBufferTest, StorageSize) {
	EXPECT_EQ(gp_first.getGapData().size(), gp_first.StorageSize());
    EXPECT_EQ(gp_second.getGapData().size(), gp_second.StorageSize());
    EXPECT_EQ(gp_third.getGapData().size(), gp_third.StorageSize());
    EXPECT_EQ(gp_fourth.getGapData().size(), gp_fourth.StorageSize());
}

class ConstIteratorTest : public GapBufferTest {
protected:
	void SetUp() override {
		GapBufferTest::SetUp();
		beg_first = cbegin(GapBufferTest::gp_first);
		end_first = cend(GapBufferTest::gp_first);
		beg_second = cbegin(GapBufferTest::gp_second);
		end_second = cend(GapBufferTest::gp_second);
		beg_third = cbegin(GapBufferTest::gp_third);
		end_third = cend(GapBufferTest::gp_third);
		beg_fourth = cbegin(GapBufferTest::gp_fourth);
		end_fourth = cend(GapBufferTest::gp_fourth);
	}

//	void TearDown() override {}

	GapBuffer::const_iterator beg_first, end_first;
	GapBuffer::const_iterator beg_second, end_second;
	GapBuffer::const_iterator beg_third, end_third;
	GapBuffer::const_iterator beg_fourth, end_fourth;
};
TEST_F(ConstIteratorTest, ConceptsIter) {
	EXPECT_TRUE(is_copy_constructible<decltype(beg_first)>::value);
	EXPECT_TRUE(is_copy_assignable<decltype(beg_second)>::value);
	EXPECT_TRUE(is_destructible<decltype(beg_third)>::value);
	EXPECT_TRUE(is_default_constructible<decltype(beg_fourth)>::value);
	EXPECT_TRUE(is_swappable<decltype(end_first)>::value);
}

TEST_F(ConstIteratorTest, Construct) {
	EXPECT_EQ(*beg_second, *(gp_second.getGapData().cbegin() + 2)) << "Iterator points to the wrong place.";
    
	GapBuffer::const_iterator fourth_cpy(beg_fourth);
	EXPECT_EQ(fourth_cpy, beg_fourth) << "Copy constructor doesn't work properly.";
}

TEST_F(ConstIteratorTest, Increment) {
    EXPECT_EQ(*(++beg_first), 'b');
	EXPECT_EQ(*(beg_first++), 'b');
	EXPECT_EQ(*beg_first, 'c');

	EXPECT_EQ(*(++beg_second), '4');
	EXPECT_THROW(++end_third, out_of_range);
}

TEST_F(ConstIteratorTest, Decrement) {
	EXPECT_THROW(--beg_second, out_of_range);
	EXPECT_EQ(--end_third, (beg_third + 1));
	EXPECT_EQ(--end_third, beg_third);
	EXPECT_EQ(*(--end_first), 'h');
	EXPECT_EQ(*beg_first, 'a');
	EXPECT_THROW(beg_first-2, out_of_range);

}

TEST_F(ConstIteratorTest, SubtractIter) {
	EXPECT_EQ(end_first-beg_first, 6);
	EXPECT_EQ(end_second-beg_second, 7);
	EXPECT_EQ(end_third-beg_third, 2);
	EXPECT_EQ(end_fourth-beg_fourth, 15'500);
}

TEST_F(ConstIteratorTest, SubtractNumber) {
	EXPECT_EQ(*(end_first - 3), 'd');
	ASSERT_THROW(end_second - 8, out_of_range);
	EXPECT_EQ(*(end_second - 2), '8');
	EXPECT_EQ(*(end_first - 5), 'b');
	EXPECT_EQ(*(end_third - 2), '1');
	EXPECT_EQ(*(end_fourth-15'000), '+');
}

TEST_F(ConstIteratorTest, ShiftPositive) {
	EXPECT_EQ(*(beg_first + 4), 'g');
	ASSERT_EQ(beg_first + 6, end_first);
	EXPECT_THROW(beg_second + 8, out_of_range);
	EXPECT_EQ(*(beg_third + 1), '9');
	EXPECT_EQ(*(beg_fourth + 15'000), '+');
	
	GapBuffer test_gap;
	test_gap.setNewData("****", 0, 4);
	EXPECT_EQ(cbegin(test_gap), cend(test_gap))
;
	EXPECT_THROW(cbegin(test_gap) + 1, out_of_range) << "Data contains only gap doesn't work correctly with iterators";
}

TEST_F(ConstIteratorTest, CompoundOperators) {
	EXPECT_EQ(*(beg_first += 5), 'h');
	EXPECT_EQ(*(end_second -= 7), '3');
	EXPECT_EQ(*(beg_third += 1), '9');
	EXPECT_THROW(*(beg_third += 2), out_of_range);
	EXPECT_EQ(*(end_fourth -= 10'001), '+');
}

TEST_F(ConstIteratorTest, AccessOperators) {
	EXPECT_EQ(beg_first[4], 'g');
	EXPECT_EQ(beg_fourth[10'500], '+');
	
	GapBuffer::const_iterator it_test = beg_third;
	EXPECT_EQ(*it_test, '1');
	EXPECT_EQ(it_test[1], '9');
	ASSERT_EQ(*beg_third, '1');
}

TEST_F(ConstIteratorTest, ComparisonOperators) {
	GapBuffer::const_iterator beg_f_cpy = beg_first;
	EXPECT_EQ(beg_f_cpy, beg_first);
	EXPECT_GE(beg_f_cpy + 5, end_first - 3);
	EXPECT_LE(end_third - 2, beg_third);
	EXPECT_NE(end_fourth - 5'000, beg_fourth + 5'000);
}


class IteratorTest : public GapBufferTest {
protected:
	void SetUp() override {
		GapBufferTest::SetUp();
		beg_first = begin(GapBufferTest::gp_first);
		end_first = end(GapBufferTest::gp_first);
		beg_second = begin(GapBufferTest::gp_second);
		end_second = end(GapBufferTest::gp_second);
		beg_third = begin(GapBufferTest::gp_third);
		end_third = end(GapBufferTest::gp_third);
		beg_fourth = begin(GapBufferTest::gp_fourth);
		end_fourth = end(GapBufferTest::gp_fourth);
	}

	//	void TearDown() override {}

	GapBuffer::iterator beg_first, end_first;
	GapBuffer::iterator beg_second, end_second;
	GapBuffer::iterator beg_third, end_third;
	GapBuffer::iterator beg_fourth, end_fourth;
};

TEST_F(IteratorTest, ConceptsIter) {
	EXPECT_TRUE(is_copy_constructible<decltype(beg_first)>::value);
	EXPECT_TRUE(is_copy_assignable<decltype(beg_second)>::value);
	EXPECT_TRUE(is_destructible<decltype(beg_third)>::value);
	EXPECT_TRUE(is_default_constructible<decltype(beg_fourth)>::value);
	EXPECT_TRUE(is_swappable<decltype(end_first)>::value);
}

TEST_F(IteratorTest, Construct) {
	EXPECT_EQ(*beg_second, *(gp_second.getGapData().begin() + 2)) << "Iterator points to the wrong place.";

	GapBuffer::iterator fourth_cpy(beg_fourth);
	EXPECT_EQ(fourth_cpy, beg_fourth) << "Copy constructor doesn't work properly.";
}

TEST_F(IteratorTest, Increment) {
	EXPECT_EQ(*(++beg_first), 'b');
	EXPECT_EQ(*(beg_first++), 'b');
	EXPECT_EQ(*beg_first, 'c');

	EXPECT_EQ(*(++beg_second), '4');
	EXPECT_THROW(++end_third, out_of_range);
}

TEST_F(IteratorTest, Decrement) {
	EXPECT_THROW(--beg_second, out_of_range);
	EXPECT_EQ(--end_third, (beg_third + 1));
	EXPECT_EQ(--end_third, beg_third);
	EXPECT_EQ(*(--end_first), 'h');
	EXPECT_EQ(*beg_first, 'a');
	EXPECT_THROW(beg_first - 2, out_of_range);

}

TEST_F(IteratorTest, SubtractIter) {
	EXPECT_EQ(end_first - beg_first, 6);
	EXPECT_EQ(end_second - beg_second, 7);
	EXPECT_EQ(end_third - beg_third, 2);
	EXPECT_EQ(end_fourth - beg_fourth, 15'500);
}

TEST_F(IteratorTest, SubtractNumber) {
	EXPECT_EQ(*(end_first - 3), 'd');
	ASSERT_THROW(end_second - 8, out_of_range);
	EXPECT_EQ(*(end_second - 2), '8');
	EXPECT_EQ(*(end_first - 5), 'b');
	EXPECT_EQ(*(end_third - 2), '1');
	EXPECT_EQ(*(end_fourth - 15'000), '+');
}

TEST_F(IteratorTest, ShiftPositive) {
	EXPECT_EQ(*(beg_first + 4), 'g');
	ASSERT_EQ(beg_first + 6, end_first);
	EXPECT_THROW(beg_second + 8, out_of_range);
	EXPECT_EQ(*(beg_third + 1), '9');
	EXPECT_EQ(*(beg_fourth + 15'000), '+');

	GapBuffer test_gap;
	test_gap.setNewData("****", 0, 4);
	EXPECT_EQ(begin(test_gap), end(test_gap))
		;
	EXPECT_THROW(begin(test_gap) + 1, out_of_range) << "Data contains only gap doesn't work correctly with iterators";
}

TEST_F(IteratorTest, CompoundOperators) {
	EXPECT_EQ(*(beg_first += 5), 'h');
	EXPECT_EQ(*(end_second -= 7), '3');
	EXPECT_EQ(*(beg_third += 1), '9');
	EXPECT_THROW(*(beg_third += 2), out_of_range);
	EXPECT_EQ(*(end_fourth -= 10'001), '+');
}

TEST_F(IteratorTest, AccessOperators) {
	EXPECT_EQ(beg_first[4], 'g');
	EXPECT_EQ(beg_fourth[10'500], '+');

	GapBuffer::iterator it_test = beg_third;
	EXPECT_EQ(*it_test, '1');
	EXPECT_EQ(it_test[1], '9');
	ASSERT_EQ(*beg_third, '1');
}

TEST_F(IteratorTest, ComparisonOperators) {
	GapBuffer::iterator beg_f_cpy = beg_first;
	EXPECT_EQ(beg_f_cpy, beg_first);
	EXPECT_GE(beg_f_cpy + 5, end_first - 3);
	EXPECT_LE(end_third - 2, beg_third);
	EXPECT_NE(end_fourth - 5'000, beg_fourth + 5'000);
}