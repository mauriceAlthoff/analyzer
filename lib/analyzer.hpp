#include <string>
#include <filesystem>
#include <vector>
#include <map>
#include <memory>
#include <regex>

/**
 * smiley component class.
 *
 * These class implements a component to find the position of
 * smileys in a text.
 */
class Smiley_Component;
/**
 * Top ten component class.
 *
 * These class implements a top ten word list generator  and a smiley filter
 */
class Top_Ten_Component;

/**
 * Visitor class.
 *
 * visitor base class defines two methods to call components to compute top ten list
 * and find the position of smileys in a text. It also uses dependency injection
 * for writing the results to an ostream for outputting the results.
 */
class Visitor {
 public:
  Visitor(std::ostream& out):m_out(out){}
  virtual void output_start_pos_smileys(const std::shared_ptr<Smiley_Component> element) const = 0;
  virtual void output_top_ten_words(const std::shared_ptr<Top_Ten_Component> element) const = 0;
protected:
  std::ostream& m_out;
};

/**
 * Component class.
 *
 * Component class interface implements a compute function that uses a visitor interface
 * as an argument.
 */
class Component {
 public:
  Component(const std::string& text, std::regex rex);
  virtual ~Component() {}
  virtual void compute(const std::shared_ptr<Visitor> visitor) const = 0;
  const std::string& get_text() const{return m_text;}
protected:
  std::string m_text;
  std::regex m_rex;
};

/**
 * Implenetation of an Component class for finding smiley positions.
 *
 * Component class interface implements a compute function that uses a visitor interface
 * as an argument.
 */
class Smiley_Component : public Component {
 public:
  Smiley_Component(const std::string& text, std::regex m_rex);
  void compute(std::shared_ptr<Visitor> visitor) const override;  
  std::optional<std::vector<int>> compute_start_pos_smileys() const;
    };

/**
 * Implenetation of an Component class for calculating top ten word list.
 *
 * Component class interface implements a compute function that uses a visitor interface
 * as an argument.
 */
class Top_Ten_Component : public Component {
 public:
  Top_Ten_Component(const std::string& text, std::regex m_rex);
  void compute(std::shared_ptr<Visitor> visitor) const override;
  std::optional<std::map<int, std::vector<std::string>, std::greater<int>>> compute_top_ten_words() const;
};

/**
 * implementation of an Visitors, it implements an output to standard output, which
 * works with all component classes.
 */
class Standard_Output : public Visitor {
 public:
  Standard_Output(std::ostream& out):Visitor(out){}
  void output_start_pos_smileys(const std::shared_ptr<Smiley_Component> element) const override;
  void output_top_ten_words(const std::shared_ptr<Top_Ten_Component> element) const override;
};

/**
 * implementation of an Visitors, it implements an output to an file, which works 
 * with all component classes.
 */
class Simple_Output : public Visitor {
 public:
  Simple_Output(std::ostream& out):Visitor(out){}
  void output_start_pos_smileys(const std::shared_ptr<Smiley_Component> element) const override;
  void output_top_ten_words(const std::shared_ptr<Top_Ten_Component> element) const override;
};

/**
 * implementation of an Visitors, it implements an output to an xml file, which works 
 * with all component classes.
 */
class Xml_Output : public Visitor {
 public:
  Xml_Output(std::ostream& out):Visitor(out){}
  void output_start_pos_smileys(const std::shared_ptr<Smiley_Component> element) const override;
  void output_top_ten_words(const std::shared_ptr<Top_Ten_Component> element) const override;
};

/**
 *  class to store application options
 */
class Options{
public:
  Options(bool console,
	  const std::filesystem::path& path_simple,
	  const std::filesystem::path& path_xml);
  bool with_console_output(){return m_console_output;}
  bool with_simple_output(){return m_simple_output;}
  bool with_xml_output(){return m_xml_output;}

  std::filesystem::path get_simple_path(){return m_path_simple;}
  std::filesystem::path get_xml_path(){return m_path_xml;}

private:
  bool m_console_output{false};
  bool m_simple_output{false};
  bool m_xml_output{false};

  std::filesystem::path m_path_simple;
  std::filesystem::path m_path_xml;
};

void client_code(const std::vector<std::shared_ptr<Component>>& components,
		 std::shared_ptr<Visitor> visitor);
