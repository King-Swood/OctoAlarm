#include "HAL_Sim.h"
#include <termox/termox.hpp>

extern void setup();
extern void loop();

using namespace ox;

// struct App : layout::Vertical<> {
//     // Label &title = this->make_child<Label>(U"OctoAlarm Sim");
//     HCheckbox_label &foo = this->make_child<HCheckbox_label>({U"Foo"});
//     HCheckbox_label &bar = this->make_child<HCheckbox_label>({U"Bar"});
//     // Thin_button &baz = this->make_child<Thin_button>(U"Baz");
//     Confirm_button &reset = this->make_child<Confirm_button>(U"Reset");

//     App() { *this | pipe::fixed_width(16); }

//     protected:
//     void timer_event() override{

//     }
// };#include <map>
#include <string>

#include <termox/termox.hpp>

using namespace ox;

class Pinbox : public Widget {
  public:
    // Emitted when a new pin is inserted.
    sl::Signal<void(Point)> pin_inserted;

    // Emitted when an existing pin is removed.
    sl::Signal<void(Point)> pin_removed;

  public:
    Pinbox()
    {
        using namespace ox::pipe;
        *this | on_mouse_press([&](auto const &m) { this->handle_mouse(m); }) |
            on_mouse_move([&](auto const &m) { this->handle_mouse(m); }) |
            on_paint([&](Painter &p) {
                for (auto [xy, color] : points_)
                    p.put(U'•' | fg(color), xy);
            });
    }

  public:
    // Set the Color for newly inserted pins.
    void set_foreground(Color c) { foreground_ = c; }

    // Remove all pins from the screen.
    void clear_screen()
    {
        points_.clear();
        this->update();
    }

  private:
    std::map<Point, Color> points_;
    Color foreground_ = Color::Light_blue;

  private:
    // Inserts pin at Point p, if p is empty; emits pin_inserted Signal.
    void insert_pin(Point p)
    {
        auto const [_, inserted] = points_.insert({p, foreground_});
        if (inserted) {
            pin_inserted.emit(p);
            this->update(); // Post a Paint Event
        }
    }

    // Removes pin at Point p, if it exists; emits pin_removed Signal.
    void remove_pin(Point p)
    {
        auto const count = points_.erase(p);
        if (count != 0) {
            pin_removed.emit(p);
            this->update(); // Post a Paint Event
        }
    }

    void handle_mouse(Mouse const &m)
    {
        switch (m.button) {
        case Mouse::Button::Left:
            this->insert_pin(m.at);
            break;
        case Mouse::Button::Right:
            this->remove_pin(m.at);
            break;
        default:
            break;
        }
    }

    auto timer_event() -> bool override
    {
        loop();
        return Widget::timer_event();
    }
};

int main()
{
    setup();
    // while (true) {
    //     loop();
    // }
    return ox::System{}.run<Pinbox>();
}