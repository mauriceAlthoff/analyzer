#include "analyzer.hpp"
#include <boost/program_options.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

using namespace boost::program_options;
using namespace std;

int main(int argc, const char* argv[])
{
    options_description desc { "Options" };
    variables_map vm;
    try {
        //step 0 parse arguments
        desc.add_options()("help,h", "Help screen")("console", value<bool>()->default_value(false), "console output")("simple", value<string>()->default_value(""), "simple text output")("xml", value<string>()->default_value(""), "xml file Output");

	// !linksto req 3 parse input parameters
        store(parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            cout << desc << '\n';
            return 0;
        }
    } catch (const error& ex) {
        cerr << ex.what() << '\n';
    }

    //step 1 read input
    cout << "please enter text: \n";
    string input;
    getline(cin, input);
    cout << "input:" << input << " \n";

    // regular expression to detect smileys
    regex rex("[:;][-]?[\\/\\[\\]\\\\{}\\(\\)]");
    //step 2 apply algorithms
    // !linksto req 1 compute start position of smileys
    // !linksto req 2 compute top 10 used words
    vector<shared_ptr<Component>> components =
      { make_shared<Smiley_Component>(input, rex),
        make_shared<Top_Ten_Component>(input, rex) };

    // store options
    Options options(vm["console"].as<bool>(),
        filesystem::path(vm["simple"].as<string>()),
        filesystem::path(vm["xml"].as<string>()));

    //step 3 generate output
    // !linksto req 3.1 console ouptput
    if (options.with_console_output()) {
        compute_visitor(components,
            make_shared<Simple_Output>(cout));
    }
    // !linksto req 3.2 storing to file
    if (options.with_simple_output()) {
        ofstream out(options.get_simple_path(),
            ios::out | ios::trunc);
        compute_visitor(components,
            make_shared<Simple_Output>(out));
    }
    // !linksto req 3.3 storing to xml formatted file
    if (options.with_xml_output()) {
        ofstream out(options.get_xml_path(),
            ios::out | ios::trunc);
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        compute_visitor(components,
            make_shared<Xml_Output>(out));
    }
    return 0;
}
