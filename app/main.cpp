#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include "analyzer.hpp"
#include <filesystem>
#include <vector>
#include <memory>
#include <regex>
#include <iostream>

using namespace boost::program_options;

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

    if (vm.count("help")){
      std::cout << desc << '\n';
      return 0;
    }
    //step 1 read input
    // skipped to test faster

    //step 2 apply algorithm
    std::vector<std::shared_ptr<Component>> components =
      {std::make_shared<Smiley_Component>(":\\hello:-] :{World:-[ :/ :) :-\\hello:] :-{World:[ :-/ :-)"),
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
