#include "catch2/catch.hpp"

#include "analyzer.hpp"
#include <string>
#include <iostream>
#include <array>
#include <sstream>

using namespace std;

unsigned int g_called{0};
regex g_rex("[:;][-]?[\\/\\[\\]\\\\{}\\(\\)]");
string g_test_text = "one two two three three three four four four four five five five five five six six six six six six seven seven seven seven seven seven seven eight eight eight eight eight eight eight eight nine nine nine nine nine nine nine nine nine ten ten ten ten ten ten ten ten ten ten";
string g_test_smiles = ":-] :{ :-[ :/ :) :-\\ :] :-{ :[ :-/ :-) :\\ ;-] ;{ ;-[ ;/ ;) ;-\\ ;] ;-{ ;[ ;-/ ;-) ;\\ ";
class Mock_Visitor;
class Mock_Component : public Component {
public:
  Mock_Component(const string& text, std::regex rex)
    :Component(text, rex){}
  void compute(shared_ptr<Visitor> visitor) const override{
    g_called++;
  }
  unsigned int m_called{0};
};

class Mock_Visitor : public Visitor {
public:
    Mock_Visitor(std::ostream& out)
        : Visitor(out)
  {
  }
  void output_mock(const shared_ptr<Mock_Component> element)
  {
    m_called++;
    element->m_called++;
  }
  void output_start_pos_smileys(const shared_ptr<Smiley_Component> element) const override
  {

  }
  void output_top_ten_words(const shared_ptr<Top_Ten_Component> element) const override
  {

  }
  unsigned int m_called{0};
};

int calc_chksm(const string& buffer){
  int sum=0;
  for(unsigned int i=0;i<buffer.size();i++)
    sum += buffer[i];
  return sum;
}

TEST_CASE("Simple output", "[visitor]")
{
  ostringstream store_output;
  string test(":)test");
  unsigned int chksm = 3429;
  vector<shared_ptr<Component>> components =
    { make_shared<Smiley_Component>(test, g_rex),
      make_shared<Top_Ten_Component>(test, g_rex) };

  compute_visitor(components,
		  make_shared<Simple_Output>(store_output));

  unsigned int sum = calc_chksm(store_output.str());
  REQUIRE(chksm == sum);
}

TEST_CASE("xml output", "[visitor]")
{
  string correct_output("");
  ostringstream store_output;
  string test(":)test");
  unsigned int chksm = 9038;
  // regular expression to detect smileys
  vector<shared_ptr<Component>> components =
    { make_shared<Smiley_Component>(test, g_rex),
      make_shared<Top_Ten_Component>(test, g_rex) };

  compute_visitor(components,
		  make_shared<Xml_Output>(store_output));

  unsigned int sum = calc_chksm(store_output.str());
  REQUIRE(chksm == sum);
}

TEST_CASE("Test smileys", "[component]")
{
  array<unsigned int, 24> result_pos = {0,4,7,11,14,17,21,24,28,31,35,39,42,46,49,53,56,59,63,66,70,73,77,81};
  Smiley_Component component(g_test_smiles, g_rex);
  auto result = component.compute_start_pos_smileys();
  REQUIRE(result.has_value());
  REQUIRE(result.value().size() == 24);
  for (unsigned int pos = 0;pos<result_pos.size();pos++ ){
    REQUIRE(result.value()[pos] == result_pos[pos]);
  }
}

TEST_CASE("Test top ten", "[component]")
{
  string test_text = "one two two three three three four four four four five five five five five six six six six six six seven seven seven seven seven seven seven eight eight eight eight eight eight eight eight nine nine nine nine nine nine nine nine nine ten ten ten ten ten ten ten ten ten ten";
  Top_Ten_Component component(test_text, g_rex);
  auto result = component.compute_top_ten_words();
  REQUIRE(result.has_value());
  REQUIRE(result.value().size() == 10);
  unsigned int pos = 10;
  for(auto [count, words]: result.value()){
    REQUIRE(count == pos);
    pos--;
  }
  //debug print
  for(auto [count, words]: result.value()){
    cout << count << " times: ";
    for (auto i: words) {
      cout << i << " ";
    }
    cout << "\n";
  }
}

TEST_CASE("compute_visitor test")
{
  g_called = 0;
  REQUIRE(g_called == 0);
  // regular expression to detect smileys
  regex rex("[1]");
  auto mock_component = make_shared<Mock_Component>(" ", rex);
  vector<shared_ptr<Component>> components = { mock_component };

  compute_visitor(components,
		  make_shared<Xml_Output>(std::cout));
  REQUIRE(g_called == 1);
}
