#include "src/model/shape_visitor.h"
#include "src/ui/canvas/canvas.h"

namespace micro_svg_editor {
namespace {

// Prompts for and applies new text properties; a no-op for non-text shapes.
class TextEditor : public ShapeVisitor {
 public:
  explicit TextEditor(QWidget* parent) : parent_(parent) {}
  bool edited() const { return edited_; }

  void Visit(Text& text) override {
    bool ok[3] = {false, false, false};
    QString new_text = QInputDialog::getText(
        parent_, "Edit Text", "Enter new text:", QLineEdit::Normal,
        QString::fromStdString(text.content()), &ok[0]);
    int new_font_size = QInputDialog::getInt(
        parent_, "Edit Text", "Enter new Font Size:", text.font_size(), 1, 100,
        1, &ok[1]);
    QString new_font_fam = QInputDialog::getText(
        parent_, "Edit Text", "Enter new Font Family:", QLineEdit::Normal,
        QString::fromStdString(text.font_family()), &ok[2]);
    if (ok[0] && ok[1] && ok[2] && !new_text.isEmpty()) {
      text.set_content(new_text.toStdString());
      text.set_font_size(new_font_size);
      text.set_font_family(new_font_fam.toStdString());
      edited_ = true;
    }
  }

  void Visit(Rect&) override {}
  void Visit(Circle&) override {}
  void Visit(Line&) override {}
  void Visit(Polyline&) override {}
  void Visit(Path&) override {}
  void Visit(Hexagon&) override {}

 private:
  QWidget* parent_;
  bool edited_ = false;
};

}  // namespace

void Canvas::TextEdit(bool& edited) {
  TextEditor editor(this);
  selected_shape_->Accept(editor);
  if (editor.edited()) edited = true;
  curr_tool_ = "";
}

}  // namespace micro_svg_editor
