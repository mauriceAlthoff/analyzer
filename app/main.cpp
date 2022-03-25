#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <memory>
#include <regex>

using namespace boost::program_options;

class Smiley_Component;
class Top_Ten_Component;

class Visitor {
 public:
  virtual void output_start_pos_smileys(const std::shared_ptr<Smiley_Component> element) const = 0;
  virtual void output_top_ten_words(const std::shared_ptr<Top_Ten_Component> element) const = 0;
};

class Component {
 public:
  Component(const std::string& text):m_text(text){}
  virtual ~Component() {}
  virtual void compute(const std::shared_ptr<Visitor> visitor) const = 0;
  const std::string& get_text() const{return m_text;}
protected:
  std::string m_text;  
};

class Smiley_Component : public Component {
 public:
  Smiley_Component(const std::string& text):Component(text){}
  void compute(std::shared_ptr<Visitor> visitor) const override {
    visitor->output_start_pos_smileys(std::make_shared<Smiley_Component> (m_text));
  }
  
  std::optional<std::vector<int>> compute_start_pos_smileys() const {
    std::regex rx("[:][-]?[\\/\\[\\]\\\\{}\\(\\)]");    
    std::vector<int> index_matches;

    for(auto it = std::sregex_iterator(m_text.begin(), m_text.end(), rx);
	it != std::sregex_iterator();
	++it)
      {
	index_matches.push_back(it->position());
	std::cout << " " << std::to_string(it->position());
      }
    std::cout << "\n";

    if(index_matches.empty())
      return  std::nullopt;
    return std::optional<std::vector<int>>(index_matches);
  }
};

class Top_Ten_Component : public Component {
 public:
  Top_Ten_Component(const std::string& text):Component(text){}
  void compute(std::shared_ptr<Visitor> visitor) const override {
    visitor->output_top_ten_words(std::make_shared<Top_Ten_Component> (m_text));
  }
  std::string compute_top_ten_words() const {
    return "top ten " + m_text;
  }
};

class StandarOutput : public Visitor {
 public:
  void output_start_pos_smileys(const std::shared_ptr<Smiley_Component> element) const override {
    auto result = element->compute_start_pos_smileys();
    if(result.has_value())
      std::cout << result->size()  << " + Standard Output\n";
  }

  void output_top_ten_words(const std::shared_ptr<Top_Ten_Component> element) const override {
    std::cout << element->compute_top_ten_words() << " + Standard Output\n";
  }
};

class SimpleOutput : public Visitor {
 public:
  void output_start_pos_smileys(const std::shared_ptr<Smiley_Component> element) const override {
    auto result = element->compute_start_pos_smileys();
    if(result.has_value())
      std::cout << result->size()  << " + Simple Output\n";
  }
  void output_top_ten_words(const std::shared_ptr<Top_Ten_Component> element) const override {
    std::cout << element->compute_top_ten_words() << " + Simple Output\n";
  }
};

class XmlOutput : public Visitor {
 public:
  void output_start_pos_smileys(const std::shared_ptr<Smiley_Component> element) const override {
    auto result = element->compute_start_pos_smileys();
    if(result.has_value())
      std::cout << result->size()  << " + XML Output\n";
  }
  void output_top_ten_words(const std::shared_ptr<Top_Ten_Component> element) const override {
    std::cout << element->compute_top_ten_words() << " + XML Output\n";
  }
};

class Input_filter{
public:
  Input_filter(bool console, const std::filesystem::path& path_simple, const std::filesystem::path& path_xml)
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
  //private:
  bool m_console_output{false};
  bool m_simple_output{false};
  bool m_xml_output{false};

  std::filesystem::path m_path_simple;
  std::filesystem::path m_path_xml;
};

void ClientCode(const std::vector<std::shared_ptr<Component>>& components,
		std::shared_ptr<Visitor> visitor) {
  std::for_each (components.begin(),
		 components.end(),
		 [visitor](auto com){com->compute(visitor);});
}

int main(int argc, const char* argv[])
{  
  try {
    //step 0 parse arguments
    options_description desc { "Options" };
    desc.add_options()
      ("help,h", "Help screen")
      ("console", value<bool>()->default_value(false), "console output")
      ("simple", value<std::string>()->default_value(""), "simple text output")
      ("xml", value<std::string>()->default_value(""), "xml file Output");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help"))
      std::cout << desc << '\n';
    if (vm.count("console"))
      std::cout << "console: " << vm["console"].as<bool>() << '\n';
    if (vm.count("simple"))
      std::cout << "simple: " << vm["simple"].as<std::string>() << '\n';
    if (vm.count("xml"))
      std::cout << "xml: " << vm["xml"].as<std::string>() << '\n';
    //step 1 read input
    // skipped to test faster

    //step 2 apply algorithm
    std::vector<std::shared_ptr<Component>> components =
      {std::make_shared<Smiley_Component>(":\\hello:-] :{World:-[ :/ :)"),
       std::make_shared<Top_Ten_Component>("There are thousands of five-letter words in the English dictionary, but it only takes one to win Wordle. Whether it’s your first time playing, or you’re a seasoned Wordler who plays at midnight when a new word drops, these tips will help you build a strategy or improve upon one you’ve already created. Let’s get started.")};

    Input_filter filter(vm["console"].as<bool>(),
			std::filesystem::path(vm["simple"].as<std::string>()),
			std::filesystem::path(vm["xml"].as<std::string>()));

    //step 3 generate output
    if (filter.m_console_output){
      auto visitor1 = std::make_shared<StandarOutput>();
      ClientCode(components, visitor1);
    }
    if (filter.m_simple_output){
      auto visitor2 = std::make_shared<SimpleOutput>();
      ClientCode(components, visitor2);
    }
    if (filter.m_xml_output){
      auto visitor3 = std::make_shared<XmlOutput>();
      ClientCode(components, visitor3);
    }
  } catch (const error& ex) {
    std::cerr << ex.what() << '\n';
  }
    
  return 0;
}
