#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <filesystem>

using namespace boost::program_options;

/**
 * The Visitor Interface declares a set of visiting methods that correspond to
 * component classes. The signature of a visiting method allows the visitor to
 * identify the exact class of the component that it's dealing with.
 */
class Smiley_Component;
class Top_Ten_Component;

class Visitor {
 public:
  virtual void output_start_pos_smileys(const Smiley_Component *element) const = 0;
  virtual void output_top_ten_words(const Top_Ten_Component *element) const = 0;
};

/**
 * The Component interface declares an `accept` method that should take the base
 * visitor interface as an argument.
 */

class Component {
 public:
  virtual ~Component() {}
  virtual void compute(Visitor *visitor) const = 0;
};

/**
 * Each Concrete Component must implement the `Accept` method in such a way that
 * it calls the visitor's method corresponding to the component's class.
 */
class Smiley_Component : public Component {
 public:
  void compute(Visitor *visitor) const override {
    visitor->output_start_pos_smileys(this);
  }
  std::string compute_start_pos_smileys() const {
    return "smileys";
  }
};

class Top_Ten_Component : public Component {
 public:
  void compute(Visitor *visitor) const override {
    visitor->output_top_ten_words(this);
  }
  std::string compute_top_ten_words() const {
    return "top ten";
  }
};

class StandarOutput : public Visitor {
 public:
  void output_start_pos_smileys(const Smiley_Component *element) const override {
    std::cout << element->compute_start_pos_smileys() << " + Standard Output\n";
  }

  void output_top_ten_words(const Top_Ten_Component *element) const override {
    std::cout << element->compute_top_ten_words() << " + Standard Output\n";
  }
};

class SimpleOutput : public Visitor {
 public:
  void output_start_pos_smileys(const Smiley_Component *element) const override {
    std::cout << element->compute_start_pos_smileys() << " + Simple Output\n";
  }
  void output_top_ten_words(const Top_Ten_Component *element) const override {
    std::cout << element->compute_top_ten_words() << " + Simple Output\n";
  }
};

class XmlOutput : public Visitor {
 public:
  void output_start_pos_smileys(const Smiley_Component *element) const override {
    std::cout << element->compute_start_pos_smileys() << " + XML Output\n";
  }
  void output_top_ten_words(const Top_Ten_Component *element) const override {
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
private:
  bool m_console_output{false};
  bool m_simple_output{false};
  bool m_xml_output{false};

  std::filesystem::path m_path_simple;
  std::filesystem::path m_path_xml;
};

/**
 * The client code can run visitor operations over any set of elements without
 * figuring out their concrete classes. The accept operation directs a call to
 * the appropriate operation in the visitor object.
 */
void ClientCode(std::array<const Component *, 2> components, Visitor *visitor) {
  for (const Component *comp : components) {
    comp->compute(visitor);
  }
}

int main(int argc, const char* argv[])
{
  std::array<const Component *, 2> components = {new Smiley_Component, new Top_Ten_Component};
  std::cout << "The client code works with all visitors via the base Visitor interface:\n";
  StandarOutput *visitor1 = new StandarOutput;
  ClientCode(components, visitor1);
  std::cout << "\n";
  std::cout << "It allows the same client code to work with different types of visitors:\n";
  SimpleOutput *visitor2 = new SimpleOutput;
  ClientCode(components, visitor2);
  std::cout << "\n";
  std::cout << "It allows the same client code to work with different types of visitors:\n";
  XmlOutput *visitor3 = new XmlOutput;
  ClientCode(components, visitor3);

  for (const Component *comp : components) {
    delete comp;
  }
  delete visitor1;
  delete visitor2;
  delete visitor3;
  
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

	//step 2 apply algorithm

	//step 3 generate output

	Input_filter filter(vm["console"].as<bool>(),
			    std::filesystem::path(vm["simple"].as<std::string>()),
			    std::filesystem::path(vm["xml"].as<std::string>()));
    } catch (const error& ex) {
        std::cerr << ex.what() << '\n';
    }

    
    return 0;
}


