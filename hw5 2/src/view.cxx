#include "view.hxx"

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 36;

View::View(const Model& model)
        : model_(model),
          dark_sprite(15, Color::black()),
          light_sprite(15, Color::white()),
          board_sprite({grid_size, grid_size}, Color::black()),
          board_sprite_g({grid_size -1, grid_size -1}, Color::medium_green()),
          red_tiles({grid_size -1, grid_size -1}, Color::medium_red()),
          blue_tiles({grid_size -1, grid_size -1}, Color::medium_blue()),
          grey_sprite(grid_size / 2, Color::from_rgba(108, 122, 136))
{

}

void View::draw(Sprite_set& set, Position mouse_posn, Position_set position) {
    for (Position pos: model_.all_positions()) {
        int x = pos.x * grid_size;
        int y = pos.y * grid_size;
        board_to_screen(pos);
        set.add_sprite(board_sprite, {x, y}, 0);
        set.add_sprite(board_sprite_g, board_to_screen(pos), 1);
    }
        //if (is_game_over_)
        //check if the game is over and add the losign sprites positionas the grey sprite
        //const Move *move = model_.find_move(mouse_posn);
        for (Position pos : model_.all_positions()) {
            if (model_[pos] == Player::dark) {
                add_players(set, model_[pos], board_to_screen(pos));
            } if (model_[pos] == Player::light) {
                add_players(set, model_[pos], board_to_screen(pos));
            }
            if (model_.is_game_over() && model_.winner() != Player::neither) {
                if (model_[pos] != model_.winner()) {
                    set.add_sprite(grey_sprite, board_to_screen(pos), 4);
                }
            }
            if (model_.is_game_over() && model_.winner() == Player::neither) {
                set.add_sprite(blue_tiles, board_to_screen(pos), 4);
            }
        }
        if (model_.find_move(screen_to_board(mouse_posn)) != nullptr) {
            set.add_sprite(red_tiles, board_to_screen(screen_to_board(mouse_posn)), 2);
            if (!model_.is_game_over()){
                for (auto pos : model_.find_move(screen_to_board(mouse_posn))->second) {
                    set.add_sprite(red_tiles, board_to_screen(pos), 2);
                }
            }
            if (model_.turn() == Player::dark) {
                set.add_sprite(dark_sprite, mouse_posn.left_by(grid_size / 2).up_by (grid_size / 2), 10);
            }
            else if (model_.turn() == Player::light) {
                set.add_sprite(light_sprite, mouse_posn.left_by(grid_size / 2).up_by (grid_size / 2), 10);
            }

//            if (model_.turn() == Player::dark) {
//                //std::cout << "player turn" << model_.turn() << "\n";
//                add_players(set, Player::dark, board_to_screen(pos), 6);
//               // model_.advance_turn_();
//                //printf("hello");
//            }
//            if (model_.turn() == Player::light) {
//                //printf("hello");
//                add_players(set, Player::light, board_to_screen(pos), 6);
//            }
//        } else if (move == nullptr) {
//            set.add_sprite(red_tiles, board_to_screen(mouse_posn), 2);
//        }
        }
    //add_players(set, model_.turn(), board_to_screen(mouse_posn), 6);
}
    //Position mouse = screen_to_board(mouse_posn);
    //const Move *move = model_.find_move(mouse);
    //if (move == nullptr) {
        //int x = board_to_screen(mouse).x + grid_size / 2;
        //int y = board_to_screen(mouse).y + grid_size / 2;
        //Position center = {x, y};

//        //if (model_.turn() == Player::dark) {
//            set.add_sprite(small_player1_sprite, center, 1);
//        } else if (model_.turn() == Player::light) {
//            set.add_sprite(small_player2_sprite, center, 1);
//        }
////    } else {
////       set.add_sprite(red_tiles, board_to_screen(mouse), 2);
////   }
//
//}
//    if (!model_.is_game_over()) {
//        Position mouse = screen_to_board(mouse_posn);
//        const Move *move = model_.find_move(mouse);
//        if (move != nullptr) {
//            set.add_sprite(red_tiles, board_to_screen(mouse), 3);
//            for (Position pos : model_.all_positions()) {
//                set.add_sprite(board_sprite_g, board_to_screen(pos), 2);
//            }
//        }
//        Player current_player = model_.turn();
//        if (current_player != Player::neither) {
//            Position center = {mouse_posn.x - grid_size / 2, mouse_posn.y - grid_size / 2};
//            if (current_player == Player::dark) {
//                set.add_sprite(small_player1_sprite, center, 2);
//            } else if (current_player == Player::light) {
//                set.add_sprite(small_player2_sprite, center, 2);
//            }
//        }
//    }
//    for (Position pos: model_.all_positions()) {
//        if (model_[pos] == Player::dark) {
//            add_players(set, Player::dark, board_to_screen(pos), 3);
//        } else if (model_[pos] == Player::light) {
//            add_players(set, Player::light, board_to_screen(pos), 3);
//        }
//        if (model_.is_game_over() && model_.winner() != Player::neither) {
////          if (model_[pos] != model_.winner()) {
////                set.add_sprite(grey_sprite, board_to_screen(pos), 4);
////            }
////        }
//    }
//}

View::Dimensions
View::initial_window_dimensions() const
{
    // You can change this if you want:
    return grid_size * model_.all_positions().dimensions();
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Reversi";
}

View::Position View::board_to_screen(Position logical) const
{
    static int grid_size(36);
    int x = logical.x * grid_size ;
    int y = logical.y * grid_size;
    return {x, y};
}
View::Position View::screen_to_board(Position physical) const
{
    static int grid_size(36);
    int x = physical.x / grid_size;
    int y = physical.y / grid_size;
    return {x, y};
}

void View::add_players(Sprite_set& set, Player player, Model::Position pos) const {
//    if (model_.turn() == Player::neither) {
//        if (model_.winner() == Player::neither) {
//            for (Model::Position temp : model_.all_positions()) {
//                if ((model_[temp] == Player::dark) || (model_[temp] == Player::light)) {
//                    set.add_sprite(grey_sprite, board_to_screen(temp), 2);
//                }
//            }

    if (player == Player::dark) {
        set.add_sprite(dark_sprite, pos, 3);
    } else if (player == Player::light) {
        set.add_sprite(light_sprite,pos, 3);
    }
}

//           for (Position temp : model_.all_positions()) {
//                if (model_[pos] == Player::light) {
//                    set.add_sprite(grey_sprite, board_to_screen(temp), 1);
//                } else if (model_[pos] == Player::dark) {
//                    set.add_sprite(dark_sprite, board_to_screen(temp), 2);
//                }
//            }
//        } else if (model_.turn() == Player::light) {
//            for (Position temp : model_.all_positions()) {
//                if (model_[temp] == Player::dark) {
//                    set.add_sprite(grey_sprite, board_to_screen(temp), 2);
//                } else if (model_[temp] == Player::light) {
//                    set.add_sprite(light_sprite, board_to_screen(temp), 2);
//                }
//            }
//        }
//    } else {
//        if (player == Player::dark) {
//            set.add_sprite(dark_sprite, pos, 3);
//        } else {
//            set.add_sprite(light_sprite, pos, 3);
//        }
//    }
//}


