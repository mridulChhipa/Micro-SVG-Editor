#include "src/ui/tools/tool_registry.h"

#include <algorithm>
#include <memory>
#include <utility>

#include <QInputDialog>
#include <QLineEdit>
#include <QString>

#include "src/model/graphics_object.h"
#include "src/model/rect.h"
#include "src/model/shape_visitor.h"
#include "src/model/svg.h"
#include "src/model/text.h"

namespace micro_svg_editor {
namespace {

// Shared opacity prompt used by the fill/stroke/object opacity actions.
bool EditOpacityDialog(QWidget* parent, const QString& title, float& prop) {
  bool ok = false;
  double new_opacity = QInputDialog::getDouble(
      parent, title, QString::fromStdString("Enter (decimal fraction)"), prop,
      0, 1, 1, &ok, Qt::WindowFlags(), 0.01);
  if (ok) {
    prop = new_opacity;
    return true;
  }
  return false;
}

class DeleteAction : public ToolAction {
 public:
  bool Apply(ToolContext& ctx) override {
    if (ctx.selected == nullptr) return false;
    auto& objects = ctx.svg.objects();
    auto it = std::find_if(objects.begin(), objects.end(),
                           [&](const GraphicsObjectPtr& obj) {
                             return obj.get() == ctx.selected;
                           });
    if (it == objects.end()) return false;
    objects.erase(it);
    ctx.selected = nullptr;
    return true;
  }
};

class FillColorAction : public ToolAction {
 public:
  bool Apply(ToolContext& ctx) override {
    if (ctx.selected == nullptr) return false;
    bool ok;
    QString new_color = QInputDialog::getText(
        ctx.parent, "Fill Color",
        "Enter fill color (e.g. 'red' or '#ff0000'):", QLineEdit::Normal,
        QString::fromStdString(ctx.selected->fill()), &ok);
    if (ok && !new_color.isEmpty()) {
      ctx.selected->set_fill(new_color.toStdString());
      return true;
    }
    return false;
  }
};

class StrokeAction : public ToolAction {
 public:
  bool Apply(ToolContext& ctx) override {
    if (ctx.selected == nullptr) return false;
    bool w_enter;
    int new_width =
        QInputDialog::getInt(ctx.parent, "Stroke Width", "Enter stroke width:",
                             ctx.selected->stroke_width(), 1, 100, 1, &w_enter);
    if (!w_enter) return false;

    bool col_enter;
    QString new_color = QInputDialog::getText(
        ctx.parent, "Stroke Color",
        "Enter stroke color (e.g. 'black' or '#000000'):", QLineEdit::Normal,
        QString::fromStdString(ctx.selected->stroke()), &col_enter);
    if (col_enter && !new_color.isEmpty()) {
      ctx.selected->set_stroke_width(new_width);
      ctx.selected->set_stroke(new_color.toStdString());
      return true;
    }
    return false;
  }
};

// Border radius applies only to rectangles; a no-op for other shapes.
class BorderRadiusVisitor : public DefaultShapeVisitor {
 public:
  explicit BorderRadiusVisitor(QWidget* parent) : parent_(parent) {}
  bool edited() const { return edited_; }

  void Visit(Rect& rect) override {
    bool ok;
    int new_radius = QInputDialog::getInt(
        parent_, "Border Radius", "Enter border radius:", 0, 0, 100, 1, &ok);
    if (ok) {
      rect.set_rx(new_radius);
      rect.set_ry(new_radius);
      edited_ = true;
    }
  }

 private:
  QWidget* parent_;
  bool edited_ = false;
};

class BorderRadiusAction : public ToolAction {
 public:
  bool Apply(ToolContext& ctx) override {
    if (ctx.selected == nullptr) return false;
    BorderRadiusVisitor visitor(ctx.parent);
    ctx.selected->Accept(visitor);
    return visitor.edited();
  }
};

class CanvasDimensionsAction : public ToolAction {
 public:
  bool Apply(ToolContext& ctx) override {
    bool w_enter, ok_height;
    int new_width = QInputDialog::getInt(
        ctx.parent, "Canvas Width",
        "Enter canvas width:", static_cast<int>(ctx.svg.width()), 1, 5000, 1,
        &w_enter);
    int new_height = QInputDialog::getInt(
        ctx.parent, "Canvas Height",
        "Enter canvas height:", static_cast<int>(ctx.svg.height()), 1, 5000, 1,
        &ok_height);
    if (w_enter && ok_height) {
      ctx.svg.set_width(new_width);
      ctx.svg.set_height(new_height);
      return true;
    }
    return false;
  }
};

class FillOpacityAction : public ToolAction {
 public:
  bool Apply(ToolContext& ctx) override {
    if (ctx.selected == nullptr) return false;
    float value = ctx.selected->fill_opacity();
    if (!EditOpacityDialog(ctx.parent, "Fill Opacity", value)) return false;
    ctx.selected->set_fill_opacity(value);
    return true;
  }
};

class StrokeOpacityAction : public ToolAction {
 public:
  bool Apply(ToolContext& ctx) override {
    if (ctx.selected == nullptr) return false;
    float value = ctx.selected->stroke_opacity();
    if (!EditOpacityDialog(ctx.parent, "Stroke Opacity", value)) return false;
    ctx.selected->set_stroke_opacity(value);
    return true;
  }
};

class OpacityAction : public ToolAction {
 public:
  bool Apply(ToolContext& ctx) override {
    if (ctx.selected == nullptr) return false;
    float value = ctx.selected->opacity();
    if (!EditOpacityDialog(ctx.parent, "Opacity", value)) return false;
    ctx.selected->set_opacity(value);
    return true;
  }
};

// Prompts for and applies new text properties; a no-op for non-text shapes.
class TextEditor : public DefaultShapeVisitor {
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

 private:
  QWidget* parent_;
  bool edited_ = false;
};

class TextEditAction : public ToolAction {
 public:
  bool Apply(ToolContext& ctx) override {
    if (ctx.selected == nullptr) return false;
    TextEditor editor(ctx.parent);
    ctx.selected->Accept(editor);
    return editor.edited();
  }
};

class SelectAction : public ToolAction {
 public:
  bool Apply(ToolContext& ctx) override {
    ctx.selected = nullptr;
    return false;  // Deselecting does not modify the document.
  }
};

}  // namespace

ToolRegistry::ToolRegistry() {
  actions_["Delete"] = std::make_unique<DeleteAction>();
  actions_["Fill Color"] = std::make_unique<FillColorAction>();
  actions_["Stroke"] = std::make_unique<StrokeAction>();
  actions_["Border Radius"] = std::make_unique<BorderRadiusAction>();
  actions_["Canvas Dimensions"] = std::make_unique<CanvasDimensionsAction>();
  actions_["Fill Opacity"] = std::make_unique<FillOpacityAction>();
  actions_["Stroke Opacity"] = std::make_unique<StrokeOpacityAction>();
  actions_["Opacity"] = std::make_unique<OpacityAction>();
  actions_["Text Edit"] = std::make_unique<TextEditAction>();
  actions_["Select"] = std::make_unique<SelectAction>();
}

ToolAction* ToolRegistry::Find(const QString& tool_name) const {
  auto it = actions_.find(tool_name.toStdString());
  return it == actions_.end() ? nullptr : it->second.get();
}

}  // namespace micro_svg_editor
