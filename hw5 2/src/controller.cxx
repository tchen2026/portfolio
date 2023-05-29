#include "controller.hxx"

Controller::Controller(int size)
        : Controller(size, size)
{
}

Controller::Controller(int width, int height)
        : model_(width, height),
          view_(model_),
          mouse_posn_(0, 0)
{ }

void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites, mouse_posn_, position);
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

void Controller::on_mouse_down(ge211::Mouse_button button, ge211::Posn<int> pos) {
    if (!model_.is_game_over()) {
        //if (model_.turn() != Player::neither && model_.find_move(view_.screen_to_board(pos))) {
        if (model_.find_move(view_.screen_to_board(pos)) != nullptr) {
            model_.play_move(view_.screen_to_board(pos));
//            mouse_posn_ = view_.screen_to_board(pos);
//            position [mouse_posn_] = true;
        }
    }
  }

    //if (!model_.is_game_over()) {
        //return;
    //}
    //if (button == ge211::Mouse_button::left) {
        //ge211::Posn<int> board_pos = view_.screen_to_board(pos);
        //model_.play_move(board_pos);
   // }
    //ge211::Sprite_set sprite_set;
        //view_.draw(sprite_set, pos);
//}

void Controller::on_mouse_move(ge211::Posn<int>pos)
{
    mouse_posn_ = pos;
}


