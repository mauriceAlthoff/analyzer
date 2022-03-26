#include "analyzer.hpp"
#include <boost/algorithm/string.hpp>
#include <regex>
#include <cctype>
#include <iostream>
#include <optional>

Smiley_Component::Smiley_Component(const std::string& text)
  :Component(text)
{
}

void Smiley_Component::compute(std::shared_ptr<Visitor> visitor) const {
  visitor->output_start_pos_smileys(std::make_shared<Smiley_Component> (m_text));
}
  
std::optional<std::vector<int>> Smiley_Component::compute_start_pos_smileys() const {
  std::regex rx("[:;][-]?[\\/\\[\\]\\\\{}\\(\\)]");
  std::vector<int> index_matches;

  for(auto it = std::sregex_iterator(m_text.begin(), m_text.end(), rx);
      it != std::sregex_iterator();
      ++it)
    {
      index_matches.push_back(it->position());
    }

  if(index_matches.empty())
    return  std::nullopt;
  return std::optional<std::vector<int>>(index_matches);
}

Top_Ten_Component::Top_Ten_Component(const std::string& text)
  :Component(text)
{
}

void Top_Ten_Component::compute(std::shared_ptr<Visitor> visitor) const {
  visitor->output_top_ten_words(std::make_shared<Top_Ten_Component> (m_text));
}
  
std::map<int, std::vector<std::string>, std::greater<int>> Top_Ten_Component::compute_top_ten_words() const {
  std::map<std::string, int> duplicate;
  std::map<int, std::vector<std::string>, std::greater<int>> score_list;
  std::vector<std::string> word_list;
  std::string no_punct_text;
  std::remove_copy_if(m_text.begin(), m_text.end(),            
                        std::back_inserter(no_punct_text), //Store output           
                        std::ptr_fun<int, int>(&std::ispunct)  
                       );
  std::regex_replace(no_punct_text, std::regex("[:;][-]?[\\/\\[\\]\\\\{}\\(\\)]"), "");
  
  boost::split(word_list, no_punct_text, boost::is_any_of("\t \n"));
  // for c++ 20
  // auto splitText = m_text | view::split(' ') | ranges::to<std::vector<std::string>>();
  std::sort(std::begin(word_list),std::end(word_list));

  for(std::string word : word_list)
    ++duplicate[word];
    
  for(auto [key, value] : duplicate)
    score_list[value].push_back(key);

  return score_list;
}

void StandarOutput::output_start_pos_smileys(const std::shared_ptr<Smiley_Component> element) const {
  auto result = element->compute_start_pos_smileys();
  if(result.has_value())
    std::cout << "Standard smiley count: " << result->size() << std::endl;
}

void StandarOutput::output_top_ten_words(const std::shared_ptr<Top_Ten_Component> element) const {
  auto top_ten_list = element->compute_top_ten_words();
  for(auto [count, words]: top_ten_list){
    std::cout << count << " times: ";
    for (auto i: words) {std::cout << i << " ";}
    std::cout << "\n";
  }

  std::cout << "Standard Output\n";
}


void SimpleOutput::output_start_pos_smileys(const std::shared_ptr<Smiley_Component> element) const {
  auto result = element->compute_start_pos_smileys();
  if(result.has_value())
    std::cout << "Simple smiley count: " << result->size() << std::endl;
}

void SimpleOutput::output_top_ten_words(const std::shared_ptr<Top_Ten_Component> element) const {
  auto top_ten_list = element->compute_top_ten_words();
  for(auto [count, words]: top_ten_list){
    std::cout << count << " times: ";
    for (auto i: words) {std::cout << i << " ";}
    std::cout << "\n";
  }

  std::cout << "Simple Output\n";
}

void XmlOutput::output_start_pos_smileys(const std::shared_ptr<Smiley_Component> element) const {
  auto result = element->compute_start_pos_smileys();
  std::cout << "<smiley_list>" << std::endl;  
  if(result.has_value()){
    for (auto i: result.value()){
      std::cout << "<pos>" <<i << "</pos>"<< std::endl;
    }
  }
  std::cout << "</smiley_list>"<< std::endl;
}

void XmlOutput::output_top_ten_words(const std::shared_ptr<Top_Ten_Component> element) const {
  auto top_ten_list = element->compute_top_ten_words();
  std::cout << "<top_ten>" << std::endl;  
  for(auto [count, words]: top_ten_list){
    std::cout << "<item value=" << count << ">";
    for (auto i: words) {std::cout << "<word>" << i << "</word>";}
    std::cout << "</item>\n";
  }
  std::cout << "</top_ten>"<< std::endl;
}

Input_filter::Input_filter(bool console, const std::filesystem::path& path_simple, const std::filesystem::path& path_xml)
  : m_console_output(console),m_path_simple(path_simple), m_path_xml(path_xml)
{
  if(!m_path_simple.empty()){
    m_simple_output = true;
  }
  if(!m_path_xml.empty()){
    m_xml_output = true;
  }

  std::cout << "If: " << m_console_output << "," << m_simple_output << "," << m_xml_output<<'\n';
}

void ClientCode(const std::vector<std::shared_ptr<Component>>& components,
		std::shared_ptr<Visitor> visitor) {
  std::for_each (components.begin(),
		 components.end(),
		 [visitor](auto com){com->compute(visitor);});
}
