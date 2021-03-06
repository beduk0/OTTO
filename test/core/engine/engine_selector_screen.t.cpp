#include "core/engine/engine_selector_screen.hpp"
#include "graphics.t.hpp"
#include "testing.t.hpp"


namespace otto {

  using namespace core::engine;
  using namespace core::input;
  using namespace core::props;

  constexpr std::array<util::string_ref, 4> engine_names = {
    "OTTO.FM",
    "Goss",
    "Potion",
    "Subtraction",
  };

  TEST_CASE ("[graphics] EngineSelectorScreen" * doctest::skip()) {
    using Sender = itc::DirectActionSender<EngineSelectorScreen>;

    EngineDispatcherReturnChannel ret_ch = {itc::DirectActionSender()};
    EngineSelectorScreen screen = {ret_ch};
    Sender sender = {screen};
    //sender.push(PublishEngineNames::action::data(engine_names));

    struct Props : InputHandler {
      Sender sender;

      SelectedEngine::Prop<Sender> selected_engine_idx = {sender, 0, limits(0, engine_names.size() - 1)};
      SelectedPreset::Prop<Sender> selected_preset_idx = {sender, 0, limits(0, 12)};

      Props(const Sender& sender) : sender(sender)
      {
        selected_engine_idx.on_change().connect([&] { selected_preset_idx = 0; });
      }

    } props{sender};

    test::show_gui([&](nvg::Canvas& ctx) { screen.draw(ctx); }, &props);
  }

  TEST_CASE ("WriterUI") {
    WriterUI ws;

    SUBCASE ("Starts out with empty string") {
      REQUIRE(ws.to_string() == "");
    }

    SUBCASE ("Changing first letter to ?") {
      ws.cycle_char(-1);
      REQUIRE(ws.to_string() == "?");
    }

    SUBCASE ("Changing first letter to A") {
      ws.cycle_char(1);
      REQUIRE(ws.to_string() == "A");
    }

    SUBCASE ("Changing first letter to B") {
      ws.cycle_char(2);
      REQUIRE(ws.to_string() == "B");
    }

    SUBCASE ("Jumping two groups and trim start") {
      ws.step_idx(3);
      ws.cycle_char(-12);
      ws.step_idx(2);
      ws.cycle_char(27);
      REQUIRE(ws.to_string() == "= a");
    }

    SUBCASE ("Two letters") {
      ws.cycle_char(1);
      ws.step_idx(1);
      ws.cycle_char(2);
      REQUIRE(ws.to_string() == "AB");
    }

    SUBCASE ("No trim preserves all spaces at beginning, and two at the end") {
      ws.step_idx(2);
      ws.cycle_char(1);
      ws.step_idx(1);
      ws.cycle_char(2);
      REQUIRE(ws.to_string(false) == " AB  ");
    }
  }

} // namespace otto
