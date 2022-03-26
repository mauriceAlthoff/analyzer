#include <boost/program_options.hpp>
#include "analyzer.hpp"
#include <filesystem>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

using namespace boost::program_options;
using namespace std;

int main(int argc, const char* argv[])
{  
  try {
    //step 0 parse arguments
    options_description desc { "Options" };
    desc.add_options()
      ("help,h", "Help screen")
      ("console", value<bool>()->default_value(false), "console output")
      ("simple", value<string>()->default_value(""), "simple text output")
      ("xml", value<string>()->default_value(""), "xml file Output");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")){
      cout << desc << '\n';
      return 0;
    }
    //step 1 read input
    // skipped to test faster
    string test_text = ":\\hello:-] :{World:-[ :/ :) :-\\hello:] :-{World:[ :-/ :-) There are thousands of five-letter words in the English dictionary, but it only takes one to win Wordle. Whether it’s your first time playing, or you’re a seasoned Wordler who plays at midnight when a new word drops, these tips will help you build a strategy or improve upon one you’ve already created. Let’s get started.";

    // regular expression to detect smileys
    regex rex("[:;][-]?[\\/\\[\\]\\\\{}\\(\\)]");
    //step 2 apply algorithms
    vector<shared_ptr<Component>> components =
      {make_shared<Smiley_Component>(test_text,rex),
       make_shared<Top_Ten_Component>(test_text,rex)};

    Options options(vm["console"].as<bool>(),
		    filesystem::path(vm["simple"].as<string>()),
		    filesystem::path(vm["xml"].as<string>()));

    //step 3 generate output
    if (options.with_console_output()){
      compute_visitor(components,
		      make_shared<Standard_Output>(cout));
    }
    
    if (options.with_simple_output()){
      ofstream out(options.get_simple_path(),
		   ios::out | ios::trunc);
      compute_visitor(components,
		      make_shared<Simple_Output>(out));
    }
    
    if (options.with_xml_output()){
      ofstream out(options.get_xml_path(),
		   ios::out | ios::trunc);
      out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
      compute_visitor(components,
		      make_shared<Xml_Output>(out));
    }
  } catch (const error& ex) {
    cerr << ex.what() << '\n';
  }
    
  return 0;
}
