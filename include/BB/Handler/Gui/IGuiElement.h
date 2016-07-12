#ifndef I_GUI_ELEMENT_H
#define I_GUI_ELEMENT_H

namespace bb {
  class IGuiElement {
    friend class GuiHandler;
  public:
    IGuiElement() {
    }
    virtual ~IGuiElement() {
    }
  protected:
    std::string m_type;
  };
}

#endif