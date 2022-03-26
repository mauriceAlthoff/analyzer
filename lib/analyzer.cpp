#include "analyzer.hpp"
#include <boost/algorithm/string.hpp>
#include <regex>
#include <cctype>
#include <iostream>
#include <optional>

using namespace std;

Component::Component(const string& text, regex rex)
  :m_text(text), m_rex(rex)
{
}

Smiley_Component::Smiley_Component(const string& text, regex rex)
  :Component(text, rex)
{
}

void Smiley_Component::compute(shared_ptr<Visitor> visitor) const {
  visitor->output_start_pos_smileys(make_shared<Smiley_Component> (m_text, m_rex));
}
  
optional<vector<int>> Smiley_Component::compute_start_pos_smileys() const {
  vector<int> index_matches;
  for(auto it = sregex_iterator(m_text.begin(), m_text.end(), m_rex);
      it != sregex_iterator();
      ++it)
    {
      index_matches.push_back(it->position());
    }

  if(index_matches.empty())
    return  nullopt;
  return optional<vector<int>>(index_matches);
}

Top_Ten_Component::Top_Ten_Component(const string& text, regex rex)
  :Component(text, rex)
{
}

void Top_Ten_Component::compute(shared_ptr<Visitor> visitor) const {
  visitor->output_top_ten_words(make_shared<Top_Ten_Component> (m_text, m_rex));
}
  
optional<map<int, vector<string>, greater<int>>>
Top_Ten_Component::compute_top_ten_words() const {
  string no_punct_text;
  remove_copy_if(m_text.begin(), m_text.end(),            
                        back_inserter(no_punct_text), //Store output           
                        ptr_fun<int, int>(&ispunct)  
                       );
  regex_replace(no_punct_text, m_rex, " ");
  vector<string> word_list;
  boost::split(word_list, no_punct_text, boost::is_any_of("\t \n"), boost::token_compress_on);
  // for c++ 20
  // auto splitText = m_text | view::split(' ') | ranges::to<vector<string>>();
  sort(begin(word_list),end(word_list));

  map<string, int> duplicate;
  for(string word : word_list)
    ++duplicate[word];
    
  map<int, vector<string>, greater<int>> score_list;
  for(auto [key, value] : duplicate)
    score_list[value].push_back(key);

  if(score_list.empty())
    return  nullopt;
  return optional<map<int, vector<string>, greater<int>>>(score_list);
}

void Standard_Output::output_start_pos_smileys(const shared_ptr<Smiley_Component> element) const {
  auto pos_list = element->compute_start_pos_smileys();
  if(pos_list.has_value())
    m_out << "Standard smiley count: " << pos_list->size() << endl;
}

void Standard_Output::output_top_ten_words(const shared_ptr<Top_Ten_Component> element) const {
  auto top_ten_list = element->compute_top_ten_words();
  if(top_ten_list.has_value()){
    for(auto [count, words]: top_ten_list.value()){
      m_out << count << " times: ";
      for (auto i: words) {m_out << i << " ";}
      m_out << "\n";
    }
  }
  m_out << "Standard Output\n";
}


void Simple_Output::output_start_pos_smileys(const shared_ptr<Smiley_Component> element) const {
  auto pos_list = element->compute_start_pos_smileys();
  if(pos_list.has_value())
    m_out << "Simple smiley count: " << pos_list->size() << endl;
}

void Simple_Output::output_top_ten_words(const shared_ptr<Top_Ten_Component> element) const {
  auto top_ten_list = element->compute_top_ten_words();
  if(top_ten_list.has_value()){
    for(auto [count, words]: top_ten_list.value()){
      m_out << count << " times: ";
      for (auto i: words) {m_out << i << " ";}
      m_out << "\n";
    }
  }
  m_out << "Simple Output\n";
}

void Xml_Output::output_start_pos_smileys(const shared_ptr<Smiley_Component> element) const {
  auto pos_list = element->compute_start_pos_smileys();
  if(pos_list.has_value()){
    m_out << "<smiley_list>" << endl;  
    for (auto pos: pos_list.value()){
      m_out << "<pos>" << pos << "</pos>"<< endl;
    }
    m_out << "</smiley_list>"<< endl;
  }
}

void Xml_Output::output_top_ten_words(const shared_ptr<Top_Ten_Component> element) const {
  auto top_ten_list = element->compute_top_ten_words();
  if(top_ten_list.has_value()){
    m_out << "<top_ten>" << endl;  
    for(auto [count, words]: top_ten_list.value()){
      m_out << "<item value=" << count << ">";
      for (auto i: words) {m_out << "<word>" << i << "</word>";}
      m_out << "</item>\n";
    }
    m_out << "</top_ten>"<< endl;
  }
}

Options::Options(bool console,
		 const filesystem::path& path_simple,
		 const filesystem::path& path_xml)
  : m_console_output(console),
    m_path_simple(path_simple),
    m_path_xml(path_xml)
{
  if(!m_path_simple.empty()){
    m_simple_output = true;
  }
  if(!m_path_xml.empty()){
    m_xml_output = true;
  }
}

void compute_visitor(const vector<shared_ptr<Component>>& components,
		     shared_ptr<Visitor> visitor) {
  for_each (components.begin(),
		 components.end(),
		 [visitor](auto com){com->compute(visitor);});
}
