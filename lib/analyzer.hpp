#include <string>
#include <filesystem>
#include <vector>
#include <map>
#include <memory>

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
  Smiley_Component(const std::string& text);
  void compute(std::shared_ptr<Visitor> visitor) const override;  
  std::optional<std::vector<int>> compute_start_pos_smileys() const;
    };

class Top_Ten_Component : public Component {
 public:
  Top_Ten_Component(const std::string& text);
  void compute(std::shared_ptr<Visitor> visitor) const override;
  std::map<int, std::vector<std::string>, std::greater<int>> compute_top_ten_words() const;
};

class StandarOutput : public Visitor {
 public:
  void output_start_pos_smileys(const std::shared_ptr<Smiley_Component> element) const override;
  void output_top_ten_words(const std::shared_ptr<Top_Ten_Component> element) const override;
};

class SimpleOutput : public Visitor {
 public:
  void output_start_pos_smileys(const std::shared_ptr<Smiley_Component> element) const override;
  void output_top_ten_words(const std::shared_ptr<Top_Ten_Component> element) const override;
};

class XmlOutput : public Visitor {
 public:
  void output_start_pos_smileys(const std::shared_ptr<Smiley_Component> element) const override;
  void output_top_ten_words(const std::shared_ptr<Top_Ten_Component> element) const override;
};

class Input_filter{
public:
  Input_filter(bool console, const std::filesystem::path& path_simple, const std::filesystem::path& path_xml);
//private:
  bool m_console_output{false};
  bool m_simple_output{false};
  bool m_xml_output{false};

  std::filesystem::path m_path_simple;
  std::filesystem::path m_path_xml;
};

void ClientCode(const std::vector<std::shared_ptr<Component>>& components,
		std::shared_ptr<Visitor> visitor);
